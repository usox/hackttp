<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use namespace HH\Lib\{Experimental\IO, Str, Vec};
use function Usox\HackMock\{mock, prospect};

class ResponseTest extends HackTest {

  public function testGetStatusCodeReturnsCode(): void {
    $code = 666;

    $response = new Response(
      mock(IO\WriteHandle::class),
      $code
    );

    expect($response->getStatusCode())
      ->toBeSame($code);
  }

  public function testGetReasonPhraseReturnsSetReason(): void {
    $code = 666;
    $phrase = 'some-reason';

    $response = new Response(
      mock(IO\WriteHandle::class),
      $code,
      $phrase
    );

    expect($response->getReasonPhrase())
      ->toBeSame($phrase);
  }

  public function testGetReasonPhraseReturnsDefaultPhraseForCode(): void {
    $response = new Response(
      mock(IO\WriteHandle::class),
      418
    );

    expect($response->getReasonPhrase())
      ->toBeSame('I\'m a teapot');
  }

  public function testWithStatusReturnsInstanceWithStatus(): void {
    $code = 200;

    expect((new Response(mock(IO\WriteHandle::class)))->withStatus($code)->getStatusCode())
      ->toBeSame($code);
  }

  public function testGetHeadersReturnsSetHeaders(): void {
    $header = 'some-header';
    $header_value = vec['some-header-value'];

    $response = new Response(
      mock(IO\WriteHandle::class),
      200,
      'some-readson',
      dict[$header => $header_value]
    );

    expect($response->getHeaders())
      ->toBeSame(dict[
        $header => $header_value,
      ]);
  }

  public function testGetBodyReturnsSetBody(): void {
    $body = mock(IO\WriteHandle::class);

    expect((new Response($body, 200, 'some-reason', dict[]))->getBody())
      ->toBeSame($body);
  }
}
