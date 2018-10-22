<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use namespace HH\Lib\{C, Experimental\Filesystem, Str};

final class UploadedStream implements Message\UploadedFileInterface {

	private static vec<int> $errors = vec[
		\UPLOAD_ERR_OK,
		\UPLOAD_ERR_INI_SIZE,
		\UPLOAD_ERR_FORM_SIZE,
		\UPLOAD_ERR_PARTIAL,
		\UPLOAD_ERR_NO_FILE,
		\UPLOAD_ERR_NO_TMP_DIR,
		\UPLOAD_ERR_CANT_WRITE,
		\UPLOAD_ERR_EXTENSION,
	];

	private int $error = \UPLOAD_ERR_OK;

	private ?string $file;

	private bool $moved = false;

	public function __construct(
		private Message\StreamInterface $stream,
		private int $size,
		int $error_status,
		private ?string $client_filename = null,
		private ?string $client_media_type = null,
	) {
		$this->setError($error_status);
	}

	private function setError(int $error): void {
		if (!C\contains(self::$errors, $error)) {
			throw new \InvalidArgumentException(
				'Invalid error status for UploadedFile',
			);
		}
		$this->error = $error;
	}

	private function validateActive(): void {
		if ($this->error !== \UPLOAD_ERR_OK) {
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

	public function getError(): int {
		return $this->error;
	}

	public function getClientFilename(): ?string {
		return $this->client_filename;
	}

	public function getClientMediaType(): ?string {
		return $this->client_media_type;
	}
}
