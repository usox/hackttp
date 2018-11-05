<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use namespace HH\Lib\{Str, Vec};
use function Usox\HackMock\{mock, prospect};

class ServerRequestFactoryTest extends HackTest {

	public function testCreateServerRequestReturnsInstance(): void {
		$factory = new ServerRequestFactory();

		$uri = mock(Message\UriInterface::class);

		prospect($uri, 'getHost')
			->once()
			->andReturn(null);

		expect(
			$factory->createServerRequest(
				Message\HTTPMethod::GET,
				$uri
			)
		)
		->toBeInstanceOf(Message\ServerRequestInterface::class);
	}
}