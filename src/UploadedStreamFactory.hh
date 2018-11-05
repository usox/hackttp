<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;

final class UploadedStreamFactory implements Message\UploadedFileFactoryInterface {

    public function createUploadedFile(
        Message\StreamInterface $stream,
        int $size = 0,
        int $error = \UPLOAD_ERR_OK,
        ?string $clientFilename = null,
        ?string $clientMediaType = null
    ): Message\UploadedFileInterface {
        return new UploadedStream(
            $stream,
            $size,
            $error,
            $clientFilename,
            $clientMediaType
        );
    }
}