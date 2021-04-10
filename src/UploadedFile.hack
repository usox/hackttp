/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use namespace HH\Lib\{File, IO};

final class UploadedFile implements Message\UploadedFileInterface {

  private ?string $file;

  private bool $moved = false;

  public function __construct(
    private IO\ReadHandle $stream,
    private ?int $size,
    private ?Message\UploadedFileError $upload_status = null,
    private string $client_filename = '',
    private string $client_media_type = '',
  ) {
  }

  private function validateActive(): void {
    if ($this->upload_status !== null) {
      throw new \RuntimeException('Cannot retrieve stream due to upload error');
    }
    if ($this->moved) {
      throw new \RuntimeException(
        'Cannot retrieve stream after it has already been moved',
      );
    }
  }

  /**
   * @deprecated HH\Asio\join() is bad for performance and could stall forever.
   */
  <<__Deprecated('Use moveToAsync instead')>>
  public function moveTo(string $target_path): void {
    \HH\Asio\join($this->moveToAsync($target_path));
  }

  public async function moveToAsync(string $target_path): Awaitable<void> {
    $this->validateActive();
    if ($target_path === '') {
      throw new \InvalidArgumentException(
        'Invalid path provided for move operation; must be a non-empty string',
      );
    }
    await $this->writeAsync($target_path);

    $this->moved = true;
  }

  private async function writeAsync(string $target_path): Awaitable<void> {
    $target = File\open_write_only($target_path);
    using $target->closeWhenDisposed();
    // Doing this a chunk at a time instead of using `readAllAsync()` to reduce
    // peak memory usage
    do {
      /* HHAST_IGNORE_ERROR[DontAwaitInALoop] */
      $chunk = await $this->stream->readAllowPartialSuccessAsync();
      if ($chunk === '') {
        break;
      }
      // ... but we need to write everything we read, so writeAllAsync
      /* HHAST_IGNORE_ERROR[DontAwaitInALoop] */
      await $target->writeAllAsync($chunk);
    } while (true);

    if ($this->stream is IO\CloseableHandle) {
      $this->stream->close();
    }
  }

  public function getSize(): ?int {
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

  public function getStream(): IO\ReadHandle {
    return $this->stream;
  }
}
