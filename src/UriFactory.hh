<?hh // strict

namespace Usox\HackTTP;

use type Facebook\Experimental\Http\Message\UriInterface;
use type Usox\HackHttpFactory\UriFactoryInterface;

final class UriFactory implements UriFactoryInterface {

    public function createUri(?string $uri = null): UriInterface {
        return new Uri($uri);
    }
}