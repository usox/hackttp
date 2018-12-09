<?hh // strict

namespace Usox\HackTTP;

use HH\Lib\Experimental\IO;
use namespace Facebook\Experimental\Http\Message;
use type Usox\HackHttpFactory\ServerRequestFactoryInterface;

final class ServerRequestFactory implements ServerRequestFactoryInterface {

  public function createServerRequest(
    Message\HTTPMethod $method,
    Message\UriInterface $uri,
    dict<string, string> $server_params = dict[],
  ): Message\ServerRequestInterface {
    return new ServerRequest(
      $method,
      $uri,
      $server_params,
      IO\server_input()
    );
  }
}
