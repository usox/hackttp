<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;

final class ServerRequestFactory implements Message\ServerRequestFactoryInterface {

    public function createServerRequest(
        string $method,
        Message\UriInterface $uri,
        dict<string, string> $serverParams = dict[]
    ): Message\ServerRequestInterface {
        return new ServerRequest(
            $method,
            $uri,
            dict[]
        );
    }
}