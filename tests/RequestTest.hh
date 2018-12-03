<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use namespace HH\Lib\{Experimental\IO, Str, Vec};
use function Usox\HackMock\{mock, prospect};

class RequestTest extends HackTest {

  public function testGetRequestTargetReturnsTargetFromUri(): void {
    $uri = mock(Message\UriInterface::class);

    $path = 'some-path';
    $raw_query = 'some-raw-query';
    $result = Str\format('%s?%s', $path, $raw_query);

    prospect($uri, 'getPath')
      ->times(1)
      ->andReturn($path);
    prospect($uri, 'getRawQuery')
      ->times(1)
      ->andReturn($raw_query);

    $request = new Request(
      Message\HTTPMethod::GET,
      $uri,
      mock(IO\ReadHandle::class),
      dict['host' => vec[]]
    );

    expect($request->getRequestTarget())
      ->toBeSame($result);
  }

  public function testWithRequestTargetReturnsInstanceWithTarget(): void {
    $request_target = 'some-request-target';

    $request = new Request(
      Message\HTTPMethod::GET,
      mock(Message\UriInterface::class),
      mock(IO\ReadHandle::class),
      dict['host' => vec[]],
    );

    expect($request->withRequestTarget($request_target)->getRequestTarget())
      ->toBeSame($request_target);
  }

  public function testGetMethodReturnsMethod(): void {
    $request = new Request(
      Message\HTTPMethod::GET,
      mock(Message\UriInterface::class),
      mock(IO\ReadHandle::class),
      dict['host' => vec[]],
    );

    expect($request->getMethod())
      ->toBeSame('GET');
  }

  public function testWithMethodSetsMethod(): void {
    $request = new Request(
      Message\HTTPMethod::GET,
      mock(Message\UriInterface::class),
      mock(IO\ReadHandle::class),
      dict['host' => vec[]],
    );

    expect($request->withMethod(Message\HTTPMethod::POST)->getMethod())
      ->toBeSame('POST');
  }

  public function testGetUriReturnsUri(): void {
    $uri = mock(Message\UriInterface::class);

    $request = new Request(
      Message\HTTPMethod::GET,
      $uri,
      mock(IO\ReadHandle::class),
      dict['host' => vec[]]
    );

    expect($request->getUri())
      ->toBeSame($uri);
  }

  public function testWithUriSetsUri(): void {
    $uri = mock(Message\UriInterface::class);

    $host = 'some-host';
    $port = 666;
    $sample_header = 'some-sample-header';
    $sample_header_values = vec['some-sample-header-value'];

    prospect($uri, 'getHost')
      ->times(1)
      ->andReturn($host);
    prospect($uri, 'getPort')
      ->times(1)
      ->andReturn($port);

    $request = new Request(
      Message\HTTPMethod::GET,
      mock(Message\UriInterface::class),
      mock(IO\ReadHandle::class),
      dict[
        'host' => vec[],
        $sample_header => $sample_header_values,
      ],
    );
    $new_request = $request->withUri($uri, shape('preserveHost' => false));

    expect($new_request->getUri())
      ->toBeSame($uri);

    expect($new_request->getHeaders())
      ->toBeSame(
        dict[
          'host' => vec[Str\format('%s:%d', $host, $port)],
          $sample_header => $sample_header_values,
        ],
      );
  }
}
