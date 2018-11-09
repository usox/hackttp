<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect};

class RequestFactoryTest extends HackTest {

  public function testCreateRequestReturnsInstance(): void {
    $factory = new RequestFactory();

    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    expect($factory->createRequest(Message\HTTPMethod::GET, $uri))
      ->toBeInstanceOf(Message\RequestInterface::class);
  }
}
