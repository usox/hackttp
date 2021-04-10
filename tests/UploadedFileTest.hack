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
use namespace HH\Lib\{File, IO, OS};
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use function Usox\HackMock\mock;

class UploadedFileTest extends HackTest {

  public function testGetSizeReturnsSize(): void {
    $size = 666;

    $file = new UploadedFile(
      mock(IO\ReadHandle::class),
      $size
    );

    expect($file->getSize())
      ->toBeSame($size);
  }

  public function testGetErrorReturnsUploadStatus(): void {
    $error = Message\UploadedFileError::ERROR_INCOMPLETE;

    $file = new UploadedFile(
      mock(IO\ReadHandle::class),
      666,
      $error
    );

    expect($file->getError())
      ->toBeSame($error);
  }

  public function testGetClientFileNameReturnsName(): void {
    $name = 'some-name';

    $file = new UploadedFile(
      mock(IO\ReadHandle::class),
      666,
      null,
      $name
    );

    expect($file->getClientFilename())
      ->toBeSame($name);
  }

  public function testGetClientMediaTypeReturnsType(): void {
    $type = 'some/thing';

    $file = new UploadedFile(
      mock(IO\ReadHandle::class),
      666,
      null,
      '',
      $type
    );

    expect($file->getClientMediaType())
      ->toBeSame($type);
  }

  public function testMoveToThrowsErrorIfUploadErrorIsPresent(): void {
    $file = new UploadedFile(
      mock(IO\ReadHandle::class),
      666,
      Message\UploadedFileError::ERROR_CANCELED_BY_EXTENSION
    );

    expect(async () ==> await $file->moveToAsync('/dev/null'))
      ->toThrow(
        \RuntimeException::class,
        'Cannot retrieve stream due to upload error',
      );
  }

  public async function testMoveToMovesFile(): Awaitable<void> {
    $filename = \sys_get_temp_dir().'/'.\bin2hex(\random_bytes(16));
    $content = 'some-content';
    list($read_handle, $write_handle) = IO\pipe();
    await $write_handle->writeAllAsync($content);
    $write_handle->close();

    $file = new UploadedFile(
      $read_handle,
      666
    );

    await $file->moveToAsync($filename);
    $ex = expect(() ==> $read_handle->close())->toThrow(
      OS\ErrnoException::class,
    );
    expect($ex->getErrno())->toEqual(OS\Errno::EBADF);

    $target_file = File\open_read_only($filename);

    expect($target_file->read())
      ->toBeSame($content);

    // after moving the file, the readhandle becomes invalid

    expect(async () ==> await $file->moveToAsync('/dev/null'))
      ->toThrow(
        \RuntimeException::class,
        'Cannot retrieve stream after it has already been moved',
      );
  }
}
