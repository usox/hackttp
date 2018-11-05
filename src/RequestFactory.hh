<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;

final class RequestFactory implements Message\RequestFactoryInterface {

    public function createRequest(
        string $method,
        Message\UriInterface $uri
    ): Message\RequestInterface {
        return new Request(
            $method,
            $uri,
            dict[]
        );
    }
}