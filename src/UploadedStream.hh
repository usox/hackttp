<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use namespace HH\Lib\{C, Experimental\Filesystem, Str};

final class UploadedStream implements Message\UploadedFileInterface {

	private ?string $file;

	private bool $moved = false;

	public function __construct(
		private Message\StreamInterface $stream,
		private int $size,
		private ?Message\UploadedFileError $upload_status = null,
		private string $client_filename = '',
		private string $client_media_type = '',
	) {
	}

	private function validateActive(): void {
		if ($this->upload_status !== null) {
			throw new \RuntimeException(
				'Cannot retrieve stream due to upload error',
			);
		}
		if ($this->moved) {
			throw new \RuntimeException(
				'Cannot retrieve stream after it has already been moved',
			);
		}
	}

	public function getStream(): Message\StreamInterface {
		$this->validateActive();

		return $this->stream;
	}

	public function moveTo(string $target_path): void {
		$this->validateActive();
		if (Str\length($target_path) === 0) {
			throw new \InvalidArgumentException(
				'Invalid path provided for move operation; must be a non-empty string',
			);
		}
		using (
			$target = Filesystem\open_file(
				$target_path,
				Filesystem\FileMode::WRITE_TRUNCATE,
			)
		) {
			$buffer_size = 8192;
			while (!$this->stream->eof()) {
				$buf = $this->stream->read($buffer_size);
				$len = Str\length($buf);
				if ($len === 0) {
					break;
				}
				$target->write($buf);
			}
		}
		$this->moved = true;
	}

	public function getSize(): int {
		return $this->size;
	}

	public function getError(): ?Message\UploadedFileError {
		return $this->upload_status;
	}

	public function getClientFilename(): string {
		return $this->client_filename;
	}

	public function getClientMediaType(): string {
		return $this->client_media_type;
	}
}
