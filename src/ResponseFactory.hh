<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Usox\HackHttpFactory\ResponseFactoryInterface;

final class ResponseFactory implements ResponseFactoryInterface {

  public function createResponse(
    int $code = 200,
    string $phrase = '',
  ): Message\ResponseInterface {
    return new Response($code, $phrase);
  }
}
