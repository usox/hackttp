<?hh // strict

namespace Usox\HackTTP;

use namespace HH\Lib\Experimental\IO;
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
      $server_params,
      mock(IO\ReadHandle::class)
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
      $server_params,
      mock(IO\ReadHandle::class),
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
      dict[],
      mock(IO\ReadHandle::class),
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
      dict[],
      mock(IO\ReadHandle::class),
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
      dict[],
      mock(IO\ReadHandle::class),
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
      dict[],
      mock(IO\ReadHandle::class),
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
      dict[],
      mock(IO\ReadHandle::class),
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
      dict[],
      mock(IO\ReadHandle::class),
    ))->withUploadedFiles($uploaded_files);

    expect($server_request->getUploadedFiles())
      ->toBeSame($uploaded_files);
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
      mock(IO\ReadHandle::class),
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
      mock(IO\ReadHandle::class),
    ))->withParsedBody($body);

    expect($server_request->getParsedBody())
      ->toBeSame($body);
  }
}