<?hh // partial
namespace Usox\HackTTP;

class UriDataTest extends \PHPUnit_Framework_TestCase {

	/**
	 * @dataProvider getValidUris
	 */
	public function testValidUrisStayValid($input) {
		$uri = new Uri($input);

		$this->assertSame($input, (string) $uri);
	}

	/**
	 * @dataProvider getValidUris
	 */
	public function testFromParts($input) {
		$uri = new Uri($input);

		$this->assertSame($input, (string) $uri);
	}

	public function getValidUris() {
		return [
			['urn:path-rootless'],
			['urn:path:with:colon'],
			['urn:/path-absolute'],
			['urn:/'],
			// only scheme with empty path
			['urn:'],
			// only path
			['/'],
			['relative/'],
			['0'],
			// same document reference
			[''],
			// network path without scheme
			['//example.org'],
			['//example.org/'],
			['//example.org?q#h'],
			// only query
			['?q'],
			['?q=abc&foo=bar'],
			// only fragment
			['#fragment'],
			// dot segments are not removed automatically
			['./foo/../bar'],
		];
	}

	/**
	 * @expectedException \InvalidArgumentException
	 * @expectedExceptionMessage Unable to parse URI
	 * @dataProvider getInvalidUris
	 */
	public function testInvalidUrisThrowException($invalidUri) {
		new Uri($invalidUri);
	}

	public function getInvalidUris() {
		return [
			// parse_url() requires the host component which makes sense for http(s)
			// but not when the scheme is not known or different. So '//' or '///' is
			// currently invalid as well but should not according to RFC 3986.
			['http://'],
			['urn://host:with:colon'], // host cannot contain ":"
		];
	}

	/**
	 * @dataProvider getPortTestCases
	 */
	public function testIsDefaultPort($scheme, $port, $is_default_port) {
		$uri = new Uri(
			sprintf('%s://some-host.de:%d', $scheme, $port)
		);

		$this->assertSame(
			$is_default_port === false,
			strpos((string) $uri, (string) $port) !== false
		);
	}

	public function getPortTestCases() {
		return [
			['http', 80, true],
			['http', 8080, false],
			['https', 443, true],
			['https', 444, false],
			['ftp', 21, true],
			['gopher', 70, true],
			['nntp', 119, true],
			['news', 119, true],
			['telnet', 23, true],
			['tn3270', 23, true],
			['imap', 143, true],
			['pop', 110, true],
			['ldap', 389, true],
		];
	}

	public function uriComponentsEncodingProvider() {
		$unreserved = 'a-zA-Z0-9.-_~!$&\'()*+,;=:@';

		return [
			// Percent encode spaces
			['/pa th?q=va lue#frag ment', '/pa%20th', 'q=va%20lue', 'frag%20ment', '/pa%20th?q=va%20lue#frag%20ment'],
			// Percent encode multibyte
			['/€?€#€', '/%E2%82%AC', '%E2%82%AC', '%E2%82%AC', '/%E2%82%AC?%E2%82%AC#%E2%82%AC'],
			// Don't encode something that's already encoded
			['/pa%20th?q=va%20lue#frag%20ment', '/pa%20th', 'q=va%20lue', 'frag%20ment', '/pa%20th?q=va%20lue#frag%20ment'],
			// Percent encode invalid percent encodings
			['/pa%2-th?q=va%2-lue#frag%2-ment', '/pa%252-th', 'q=va%252-lue', 'frag%252-ment', '/pa%252-th?q=va%252-lue#frag%252-ment'],
			// Don't encode path segments
			['/pa/th//two?q=va/lue#frag/ment', '/pa/th//two', 'q=va/lue', 'frag/ment', '/pa/th//two?q=va/lue#frag/ment'],
			// Don't encode unreserved chars or sub-delimiters
			["/$unreserved?$unreserved#$unreserved", "/$unreserved", $unreserved, $unreserved, "/$unreserved?$unreserved#$unreserved"],
			// Encoded unreserved chars are not decoded
			['/p%61th?q=v%61lue#fr%61gment', '/p%61th', 'q=v%61lue', 'fr%61gment', '/p%61th?q=v%61lue#fr%61gment'],
		];
	}

	/**
	 * @dataProvider uriComponentsEncodingProvider
	 */
	public function testUriComponentsGetEncodedProperly($input, $path, $query, $fragment, $output) {
		$uri = new Uri($input);
		$this->assertSame($path, $uri->getPath());
		$this->assertSame($query, $uri->getRawQuery());
		$this->assertSame($fragment, $uri->getFragment());
		$this->assertSame($output, (string) $uri);
	}
}
