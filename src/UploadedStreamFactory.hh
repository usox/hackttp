<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Usox\HackHttpFactory\UploadedFileFactoryInterface;

final class UploadedStreamFactory implements UploadedFileFactoryInterface {

    public function createUploadedFile(
        Message\StreamInterface $stream,
        int $size = 0,
        int $error = \UPLOAD_ERR_OK,
        ?string $client_filename = null,
        ?string $client_media_type = null
    ): Message\UploadedFileInterface {
        return new UploadedStream(
            $stream,
            $size,
            $error,
            $client_filename,
            $client_media_type
        );
    }
}