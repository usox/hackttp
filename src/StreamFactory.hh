<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;

final class StreamFactory implements Message\StreamFactoryInterface {

    public function createStream(
        string $content = ''
    ): Message\StreamInterface {
        $resource = \fopen('php://temp', 'wb+');
        \fwrite($resource, $content);
        return new Stream($resource);
    }

    public function createStreamFromFile(
        string $filename,
        string $mode = 'r'
    ): Message\StreamInterface {
        return new Stream(
            \fopen($filename, $mode)
        );
    }

    public function createStreamFromResource(
        resource $resource
    ): Message\StreamInterface {
        return new Stream($resource);
    }
}