<?hh // strict

namespace Usox\HackTTP;

use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect};
use namespace Facebook\Experimental\Http\Message;

class ServerRequestTest extends HackTest {

  public function testGetServerParamsReturnsData(): void {
    $server_params = dict['foo' => 'bar'];

    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      $server_params
    );

    expect($server_request->getServerParams())
      ->toBeSame($server_params);
  }

  public function testWithServerParamsReturnsNewInstanceWithSetParams(): void {
    $server_params = dict['foo' => 'bar'];
    $server_params_new = dict['foo' => 'baz'];

    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = (new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      $server_params
    ))->withServerParams($server_params_new);

    expect($server_request->getServerParams())
      ->toBeSame($server_params_new);
  }

  public function testGetCookieParamsReturnsEmptyData(): void {
    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      dict[]
    );

    expect($server_request->getCookieParams())
      ->toBeSame(dict[]);
  }

  public function testWithCookieParamsSetDict(): void {
    $uri = mock(Message\UriInterface::class);

    $cookie_params = dict['some' => 'cookie'];

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = (new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      dict[]
    ))->withCookieParams($cookie_params);

    expect($server_request->getCookieParams())
      ->toBeSame($cookie_params);
  }

  public function testGetQueryParamsReturnsEmptyData(): void {
    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      dict[]
    );

    expect($server_request->getQueryParams())
      ->toBeSame(dict[]);
  }

  public function testWithQueryParamsSetDict(): void {
    $uri = mock(Message\UriInterface::class);

    $query_params = dict['some' => 'query'];

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = (new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      dict[]
    ))->withQueryParams($query_params);

    expect($server_request->getQueryParams())
      ->toBeSame($query_params);
  }

  public function testGetUploadedFilesReturnsEmptyData(): void {
    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      dict[]
    );

    expect($server_request->getUploadedFiles())
      ->toBeSame(dict[]);
  }

  public function testWithUploadedFilesSetDict(): void {
    $uri = mock(Message\UriInterface::class);

    $uploaded_files = dict['some' => mock(Message\UploadedFileInterface::class)];

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = (new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      dict[]
    ))->withUploadedFiles($uploaded_files);

    expect($server_request->getUploadedFiles())
      ->toBeSame($uploaded_files);
  }

  public function testGetParsedBodyDoesntReturnBodyIfMethodIsGet(): void {
    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = (new ServerRequest(
      Message\HTTPMethod::GET,
      $uri,
      dict[]
    ))->withParsedBody(dict['some' => 'body']);

    expect($server_request->getParsedBody())
      ->toBeSame(dict[]);
  }

  public function testGetParsedBodyDoesntReturnBodyIfMethodIsPostAndHeaderRequirementsNotMet(): void {
    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = (new ServerRequest(
      Message\HTTPMethod::POST,
      $uri,
      dict[]
    ))->withParsedBody(dict['some' => 'body']);

    expect($server_request->getParsedBody())
      ->toBeSame(dict[]);
  }

  public function testGetParsedBodyReturnsBodyWithFormUrlEncoded(): void {
    $uri = mock(Message\UriInterface::class);

    $body = dict['some' => 'body'];

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = (new ServerRequest(
      Message\HTTPMethod::POST,
      $uri,
      dict['CONTENT_TYPE' => 'multipart/form-data'],
    ))->withParsedBody($body);

    expect($server_request->getParsedBody())
      ->toBeSame($body);
  }

  public function testGetParsedBodyReturnsBodyWithMultipartFormData(): void {
    $uri = mock(Message\UriInterface::class);

    $body = dict['some' => 'body'];

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    $server_request = (new ServerRequest(
      Message\HTTPMethod::POST,
      $uri,
      dict['CONTENT_TYPE' => 'application/x-www-form-urlencoded'],
    ))->withParsedBody($body);

    expect($server_request->getParsedBody())
      ->toBeSame($body);
  }

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