<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;

class ResponseFactoryTest extends HackTest {

  public function testCreateRequestReturnsInstance(): void {
    $factory = new ResponseFactory();

    expect($factory->createResponse(200, 'some-response'))
      ->toBeInstanceOf(Message\ResponseInterface::class);
  }
}
