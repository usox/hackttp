<?hh // strict

namespace Usox\HackTTP;

use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect};

class ServerRequestTest extends HackTest {

  public function testCreateFromGlobalsReturnsPreparedRequestObj(): void {
    $uri = 'up/and.at?them';

    // Define all those (not so) fancy php super globals
    /* HH_FIXME[2049] */
    /* HH_FIXME[4106] */
    global $_POST, $_GET, $_SERVER, $_COOKIE;
    $_POST = ['foo' => 'bar'];
    $_GET = ['foo' => 'baz'];
    $_SERVER = [
      'REQUEST_URI' => $uri,
      'REQUEST_METHOD' => 'POST',
      'CONTENT_TYPE' => 'multipart/form-data',
    ];
    $_COOKIE = ['some' => 'cookie'];

    $server_request = ServerRequest::createFromGlobals();

    expect($server_request->getParsedBody())
      ->toBeSame(dict($_POST));

    expect($server_request->getCookieParams())
      ->toBeSame(dict($_COOKIE));

    expect($server_request->getQueryParams())
      ->toBeSame(dict($_GET));

    expect($server_request->getUri()->toString())
      ->toBeSame($uri);
  }
}