<?hh // strict

namespace Usox\HackTTP;

use function Facebook\FBExpect\expect;
use namespace HH\Lib\{Str};

class UriDataTest extends \Facebook\HackTest\HackTest {

	<<DataProvider('getValidUris')>>
	public function testValidUrisStayValid(string $input): void {
		$uri = new Uri($input);

		expect((string)$uri)->toBeSame($input);
	}

	<<DataProvider('getValidUris')>>
	public function testFromParts(string $input): void {
		$uri = new Uri($input);

		expect((string)$uri)->toBeSame($input);
	}

	public function getValidUris(): vec<vec<string>> {
		return vec[
			vec['urn:path-rootless'],
			vec['urn:path:with:colon'],
			vec['urn:/path-absolute'],
			vec['urn:/'],
			// only scheme with empty path
			vec['urn:'],
			// only path
			vec['/'],
			vec['relative/'],
			vec['0'],
			// same document reference
			vec[''],
			// network path without scheme
			vec['//example.org'],
			vec['//example.org/'],
			vec['//example.org?q#h'],
			// only query
			vec['?q'],
			vec['?q=abc&foo=bar'],
			// only fragment
			vec['#fragment'],
			// dot segments are not removed automatically
			vec['./foo/../bar'],
		];
	}

	<<DataProvider('getInvalidUris')>>
	public function testInvalidUrisThrowException(string $invalidUri): void {
		expect(() ==> new Uri($invalidUri))
			->toThrow(
				\InvalidArgumentException::class,
				'Unable to parse URI: '.$invalidUri,
			);
	}

	public function getInvalidUris(): vec<vec<string>> {
		return vec[
			// parse_url() requires the host component which makes sense for http(s)
			// but not when the scheme is not known or different. So '//' or '///' is
			// currently invalid as well but should not according to RFC 3986.
			vec['http://'],
			vec['urn://host:with:colon'], // host cannot contain ":"
		];
	}

	<<DataProvider('getPortTestCases')>>
	public function testIsDefaultPort(
		string $scheme,
		int $port,
		bool $is_default_port,
	): void {
		$uri = new Uri(Str\format('%s://some-host.de:%d', $scheme, $port));

		expect(Str\contains((string)$uri, (string)$port) !== false)
			->toBeSame($is_default_port === false);
	}

	public function getPortTestCases(): vec<vec<mixed>> {
		return vec[
			vec['http', 80, true],
			vec['http', 8080, false],
			vec['https', 443, true],
			vec['https', 444, false],
			vec['ftp', 21, true],
			vec['gopher', 70, true],
			vec['nntp', 119, true],
			vec['news', 119, true],
			vec['telnet', 23, true],
			vec['tn3270', 23, true],
			vec['imap', 143, true],
			vec['pop', 110, true],
			vec['ldap', 389, true],
		];
	}

	public function uriComponentsEncodingProvider(): vec<vec<string>> {
		$unreserved = 'a-zA-Z0-9.-_~!$&\'()*+,;=:@';

		return vec[
			// Percent encode spaces
			vec[
				'/pa th?q=va lue#frag ment',
				'/pa%20th',
				'q=va%20lue',
				'frag%20ment',
				'/pa%20th?q=va%20lue#frag%20ment',
			],
			// Percent encode multibyte
			vec[
				'/€?€#€',
				'/%E2%82%AC',
				'%E2%82%AC',
				'%E2%82%AC',
				'/%E2%82%AC?%E2%82%AC#%E2%82%AC',
			],
			// Don't encode something that's already encoded
			vec[
				'/pa%20th?q=va%20lue#frag%20ment',
				'/pa%20th',
				'q=va%20lue',
				'frag%20ment',
				'/pa%20th?q=va%20lue#frag%20ment',
			],
			// Percent encode invalid percent encodings
			vec[
				'/pa%2-th?q=va%2-lue#frag%2-ment',
				'/pa%252-th',
				'q=va%252-lue',
				'frag%252-ment',
				'/pa%252-th?q=va%252-lue#frag%252-ment',
			],
			// Don't encode path segments
			vec[
				'/pa/th//two?q=va/lue#frag/ment',
				'/pa/th//two',
				'q=va/lue',
				'frag/ment',
				'/pa/th//two?q=va/lue#frag/ment',
			],
			// Don't encode unreserved chars or sub-delimiters
			vec[
				"/$unreserved?$unreserved#$unreserved",
				"/$unreserved",
				$unreserved,
				$unreserved,
				"/$unreserved?$unreserved#$unreserved",
			],
			// Encoded unreserved chars are not decoded
			vec[
				'/p%61th?q=v%61lue#fr%61gment',
				'/p%61th',
				'q=v%61lue',
				'fr%61gment',
				'/p%61th?q=v%61lue#fr%61gment',
			],
		];
	}

	<<DataProvider('uriComponentsEncodingProvider')>>
	public function testUriComponentsGetEncodedProperly(
		string $input,
		string $path,
		string $query,
		string $fragment,
		string $output,
	): void {
		$uri = new Uri($input);

		expect($uri->getPath())->toBeSame($path);
		expect($uri->getRawQuery())->toBeSame($query);
		expect($uri->getFragment())->toBeSame($fragment);
		expect((string)$uri)->toBeSame($output);
	}
}
