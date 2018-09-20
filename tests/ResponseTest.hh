<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use namespace HH\Lib\{Str, Vec};
use function Usox\HackMock\{mock, prospect};

class ResponseTest extends HackTest {

	public function testGetStatusCodeReturnsCode(): void {
		$code = 666;

		$response = new Response(
			$code
		);

		expect(
			$response->getStatusCode()
		)
		->toBeSame($code);
	}

	public function testGetReasonPhraseReturnsSetReason(): void {
		$code = 666;
		$phrase = 'some-reason';

		$response = new Response(
			$code,
			dict[],
			null,
			'1.1',
			$phrase
		);

		expect(
			$response->getReasonPhrase()
		)
		->toBeSame($phrase);
	}

	public function testGetReasonPhraseReturnsDefaultPhraseForCode(): void {
		$response = new Response(418);

		expect(
			$response->getReasonPhrase()
		)
		->toBeSame('I\'m a teapot');
	}

	public function testWithStatusReturnsInstanceWithStatus(): void {
		$code = 200;

		expect(
			(new Response())->withStatus($code)->getStatusCode()
		)
		->toBeSame($code);
	}

	public function testGetHeadersReturnsSetHeaders(): void {
		$header = 'some-header';
		$header_value = vec['some-header-value'];

		$response = new Response(
			200,
			dict[$header => $header_value]
		);

		expect(
			$response->getHeaders()
		)
		->toBeSame(dict[
			$header => $header_value
		]);
	}

	public function testGetBodyReturnsSetStream(): void {
		$stream = mock(Message\StreamInterface::class);

		expect(
			(new Response(200, dict[], $stream))->getBody()
		)
		->toBeSame(
			$stream
		);
	}
}