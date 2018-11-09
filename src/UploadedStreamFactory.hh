<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Usox\HackHttpFactory\UploadedFileFactoryInterface;

final class UploadedStreamFactory implements UploadedFileFactoryInterface {

  public function createUploadedFile(
    Message\StreamInterface $stream,
    int $size = 0,
    ?Message\UploadedFileError $error = null,
    string $client_filename = '',
    string $client_media_type = '',
  ): Message\UploadedFileInterface {
    return new UploadedStream(
      $stream,
      $size,
      $error,
      $client_filename,
      $client_media_type,
    );
  }
}
