<?hh // strict

namespace Usox\HackTTP;

use Facebook\Experimental\Http\Message\StreamInterface;

final class Stream implements StreamInterface {

	private ?int $size;

	private ?string $uri;

	private bool $readable = false;

	private bool $writable = false;

	private bool $seekable = false;

	private static Vector<string> $read_modes = Vector{
		'r', 'w+', 'r+', 'x+', 'c+', 'rb', 'w+b', 'r+b', 'x+b',
		'c+b', 'rt', 'w+t', 'r+t', 'x+t', 'c+t', 'a+'
	};

	private static Vector<string> $write_modes = Vector{
		'w', 'w+', 'rw', 'r+', 'x+', 'c+', 'wb', 'w+b', 'r+b',
		'x+b', 'c+b', 'w+t', 'r+t', 'x+t', 'c+t', 'a', 'a+'
	};

	public function __construct(private ?resource $stream) {
		if ($stream === null) {
			throw new \InvalidArgumentException('Stream must be a resource');
		}

		$meta = stream_get_meta_data($this->stream);
		$this->seekable = (bool) $meta['seekable'];
		$this->readable = static::$read_modes->linearSearch($meta['mode']) !== -1;
		$this->writable = static::$write_modes->linearSearch($meta['mode']) !== -1;
		$this->uri = (string) $this->getMetadata('uri');
	}

	public function __destruct(): void {
		$this->close();
	}

	public function getContents(): string {
		if ($this->stream === null) {
			throw new \RuntimeException('Stream is detached');
		}

		$contents = stream_get_contents($this->stream);

		if ($contents === false) {
			throw new \RuntimeException('Unable to read stream contents');
		}

		return $contents;
	}

	public function close(): void {
		if ($this->stream !== null) {
			fclose($this->stream);
			$this->detach();
		}
	}

	public function detach(): ?resource {
		if ($this->stream === null) {
			return null;
		}

		$result = $this->stream;
		$this->stream = $this->size = $this->uri = null;
		$this->readable = $this->writable = $this->seekable = false;

		return $result;
	}

	public function getSize(): ?int {
		if ($this->size !== null) {
			return $this->size;
		}

		if ($this->stream === null) {
			return null;
		}

		if ($this->uri !== null) {
			clearstatcache();
		}

		$stats = fstat($this->stream);

		if (array_key_exists('size', $stats)) {
			$this->size = $stats['size'];
			return $this->size;
		}

		return null;
	}

	public function isReadable(): bool {
		return $this->readable;
	}

	public function isWritable(): bool {
		return $this->writable;
	}

	public function isSeekable(): bool {
		return $this->seekable;
	}

	public function eof(): bool {
		if ($this->stream === null) {
			throw new \RuntimeException('Stream is detached');
		}

		return feof($this->stream);
	}

	public function tell(): int {
		if ($this->stream === null) {
			throw new \RuntimeException('Stream is detached');
		}

		$result = ftell($this->stream);

		if ($result === false) {
			throw new \RuntimeException('Unable to determine stream position');
		}

		return $result;
	}

	public function rewind(): void {
		$this->seek(0);
	}

	public function seek(int $offset, int $whence = \SEEK_SET): void {
		if ($this->stream === null) {
			throw new \RuntimeException('Stream is detached');
		}
		if ($this->isSeekable() === false) {
			throw new \RuntimeException('Stream is not seekable');
		}
		if (fseek($this->stream, $offset, $whence) === -1) {
			throw new \RuntimeException('Unable to seek to stream position '
				. $offset . ' with whence ' . var_export($whence, true));
		}
	}

	public function read(int $length): string {
		if ($this->stream === null) {
			throw new \RuntimeException('Stream is detached');
		}
		if ($this->isReadable() === false) {
			throw new \RuntimeException('Cannot read from non-readable stream');
		}
		if ($length < 0) {
			throw new \RuntimeException('Length parameter cannot be negative');
		}

		if ($length === 0) {
			return '';
		}

		$content = fread($this->stream, $length);
		if ($content === false) {
			throw new \RuntimeException('Unable to read from stream');
		}

		return $content;
	}

	public function write(string $string): int {
		if ($this->stream === null) {
			throw new \RuntimeException('Stream is detached');
		}
		if ($this->isWritable() === false) {
			throw new \RuntimeException('Cannot write to a non-writable stream');
		}

		$this->size = null;
		$bytes = fwrite($this->stream, $string);

		if ($bytes === false) {
			throw new \RuntimeException('Unable to write to stream');
		}

		return $bytes;
	}

	public function getMetadata(?string $key = null): mixed {
		if ($this->stream === null) {
			return $key !== null ? null : [];
		} elseif ($key === null) {
			return stream_get_meta_data($this->stream);
		}

		$meta = stream_get_meta_data($this->stream);

		return array_key_exists($key, $meta) ? $meta[$key] : null;
	}

	public function __toString(): string {
		return 'i am deprecated';
	}
}
