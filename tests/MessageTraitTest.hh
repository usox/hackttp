<?hh // strict

namespace Usox\HackTTP;

use type Facebook\Experimental\Http\Message\StreamInterface;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use namespace HH\Lib\{Str, Vec};
use function Usox\HackMock\mock;

class MessageTraitTest extends HackTest {

	public function testGetProtocolReturnsDefaultProtocolVersion(): void {
		expect(
			$this->createImplementation()->getProtocolVersion()
		)
		->toBeSame('1.1');
	}

	public function testWithProtocolVersionReturnsSetVersion(): void {
		$version = '6.66';

		expect(
			$this->createImplementation()->withProtocolVersion($version)->getProtocolVersion()
		)
		->toBeSame($version);
	}

	public function testGetHeadersReturnsEmptyDict(): void {
		expect(
			$this->createImplementation()->getHeaders()
		)
		->toBeSame(dict[]);
	}

	public function testGetHeaderReturnsEmptyHeader(): void {
		expect(
			$this->createImplementation()->getHeader('some-header')
		)
		->toBeSame(vec[]);
	}

	public function testGetHeaderLineReturnsEmptyStringOnMissingHeader(): void {
		expect(
			$this->createImplementation()->getHeaderLine('some-header')
		)
		->toBeSame('');
	}

	public function testWithHeaderSetsHeaders(): void {
		$header_values = vec['header-value1', 'header-value2'];
		$header_name = 'some-header';

		expect(
			$this->createImplementation()->withHeader($header_name, $header_values)->getHeaders()
		)
		->toBeSame(
			dict[$header_name => $header_values]
		);
	}

	public function testWithHeaderLineSetsHeader(): void {
		$header_values = vec['header-value1', 'header-value2'];
		$header_name = 'some-header';

		expect(
			$this->createImplementation()
				->withHeaderLine($header_name, Str\join($header_values, ','))
				->getHeaders()
		)
		->toBeSame(
			dict[$header_name => $header_values]
		);
	}

	public function testWithAddedHeaderAppendsHeader(): void {
		$existing_header_name = 'some-existing-header';
		$existing_header_value = vec['some-existing-header-value'];

		$header_values = vec['header-value1', 'header-value2'];
		$header_name = 'some-header';

		$impl = $this->createImplementation()->withHeader($existing_header_name, $existing_header_value);

		expect(
			$impl->withAddedHeader($header_name, $header_values)->getHeaders()
		)
		->toBeSame(
			dict[
				$existing_header_name => $existing_header_value,
				$header_name => $header_values
			]
		);
	}

	public function testWithAddedHeaderAppendsValuesToExistingHeader(): void {
		$existing_header_name = 'some-existing-header';
		$existing_header_value = vec['some-existing-header-value'];

		$header_values = vec['header-value1', 'header-value2'];

		$impl = $this->createImplementation()->withHeader($existing_header_name, $existing_header_value);

		expect(
			$impl->withAddedHeader($existing_header_name, $header_values)->getHeaders()
		)
		->toBeSame(
			dict[
				$existing_header_name => Vec\concat($existing_header_value, $header_values)
			]
		);
	}

	public function testWithoutHeaderDropsExistingHeaders(): void {
		$existing_header_name = 'some-existing-header';
		$existing_header_value = vec['some-existing-header-value'];

		expect(
			$this->createImplementation()
				->withHeader($existing_header_name, $existing_header_value)
				->withoutHeader($existing_header_name)
				->getHeaders()
		)
		->toBeSame(
			dict[]
		);
	}

	public function testGetBodyThrowsExceptionOnMissingBody(): void {
		expect(
			() ==> $this->createImplementation()->getBody()
		)
		->toThrow(
			\InvalidArgumentException::class,
			'No body available'
		);
	}

	public function testWithBodyReturnsSetBody(): void {
		$body = mock(StreamInterface::class);

		expect(
			$this->createImplementation()->withBody($body)->getBody()
		)
		->toBeSame($body);
	}

	private function createImplementation(): MessageTraitImplementation {
		return new MessageTraitImplementation();
	}
}