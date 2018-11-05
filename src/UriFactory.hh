<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;

final class UriFactory implements Message\UriFactoryInterface {

    public function createUri(?string $uri = null): Message\UriInterface {
        return new Uri($uri);
    }
}