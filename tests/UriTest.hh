<?hh // partial
namespace Usox\HackTTP;

class UriTest extends \PHPUnit_Framework_TestCase {

	public function testParsesProvidedUri() {
		$uri = new Uri('https://user:pass@example.com:8080/path/123?q=abc#test');

		$this->assertSame('https', $uri->getScheme());
		$this->assertSame('user:pass@example.com:8080', $uri->getAuthority());
		$this->assertSame(['user' => 'user', 'pass' => 'pass'], $uri->getUserInfo());
		$this->assertSame('example.com', $uri->getHost());
		$this->assertSame(8080, $uri->getPort());
		$this->assertSame('/path/123', $uri->getPath());
		$this->assertSame('q=abc', $uri->getQuery());
		$this->assertSame('test', $uri->getFragment());
		$this->assertSame('https://user:pass@example.com:8080/path/123?q=abc#test', (string) $uri);
	}

	public function testCanTransformAndRetrievePartsIndividually() {
		$uri = (new Uri())
			->withScheme('https')
			->withUserInfo('user', 'pass')
			->withHost('example.com')
			->withPort(8080)
			->withPath('/path/123')
			->withQuery('q=abc')
			->withFragment('test');

		$this->assertSame('https', $uri->getScheme());
		$this->assertSame('user:pass@example.com:8080', $uri->getAuthority());
		$this->assertSame(['user' => 'user', 'pass' => 'pass'], $uri->getUserInfo());
		$this->assertSame('example.com', $uri->getHost());
		$this->assertSame(8080, $uri->getPort());
		$this->assertSame('/path/123', $uri->getPath());
		$this->assertSame('q=abc', $uri->getQuery());
		$this->assertSame('test', $uri->getFragment());
		$this->assertSame('https://user:pass@example.com:8080/path/123?q=abc#test', (string) $uri);
	}

	/**
	 * @expectedException \InvalidArgumentException
	 * @expectedExceptionMessage Invalid port: 100000. Must be between 1 and 65535
	 */
	public function testPortMustBeValid() {
		(new Uri())->withPort(100000);
	}

	/**
	 * @expectedException \InvalidArgumentException
	 * @expectedExceptionMessage Invalid port: 0. Must be between 1 and 65535
	 */
	public function testWithPortCannotBeZero() {
		(new Uri())->withPort(0);
	}

	/**
	 * @expectedException \InvalidArgumentException
	 * @expectedExceptionMessage Unable to parse URI
	 */
	public function testParseUriPortCannotBeZero() {
		new Uri('//example.com:0');
	}

	public function testCanParseFalseyUriParts() {
		$uri = new Uri('0://0:0@0/0?0#0');

		$this->assertSame('0', $uri->getScheme());
		$this->assertSame('0:0@0', $uri->getAuthority());
		$this->assertSame(['user' => '0', 'pass' => '0'], $uri->getUserInfo());
		$this->assertSame('0', $uri->getHost());
		$this->assertSame('/0', $uri->getPath());
		$this->assertSame('0', $uri->getQuery());
		$this->assertSame('0', $uri->getFragment());
		$this->assertSame('0://0:0@0/0?0#0', (string) $uri);
	}

	public function testCanConstructFalseyUriParts() {
		$uri = (new Uri())
			->withScheme('0')
			->withUserInfo('0', '0')
			->withHost('0')
			->withPath('/0')
			->withQuery('0')
			->withFragment('0');

		$this->assertSame('0', $uri->getScheme());
		$this->assertSame('0:0@0', $uri->getAuthority());
		$this->assertSame(['user' => '0', 'pass' => '0'], $uri->getUserInfo());
		$this->assertSame('0', $uri->getHost());
		$this->assertSame('/0', $uri->getPath());
		$this->assertSame('0', $uri->getQuery());
		$this->assertSame('0', $uri->getFragment());
		$this->assertSame('0://0:0@0/0?0#0', (string) $uri);
	}

	public function testSchemeIsNormalizedToLowercase() {
		$uri = new Uri('HTTP://example.com');

		$this->assertSame('http', $uri->getScheme());
		$this->assertSame('http://example.com', (string) $uri);

		$uri = (new Uri('//example.com'))->withScheme('HTTP');

		$this->assertSame('http', $uri->getScheme());
		$this->assertSame('http://example.com', (string) $uri);
	}

	public function testHostIsNormalizedToLowercase() {
		$uri = new Uri('//eXaMpLe.CoM');

		$this->assertSame('example.com', $uri->getHost());
		$this->assertSame('//example.com', (string) $uri);

		$uri = (new Uri())->withHost('eXaMpLe.CoM');

		$this->assertSame('example.com', $uri->getHost());
		$this->assertSame('//example.com', (string) $uri);
	}

	public function testPortIsNullIfStandardPortForScheme() {
		// HTTPS standard port
		$uri = new Uri('https://example.com:443');
		$this->assertNull($uri->getPort());
		$this->assertSame('example.com', $uri->getAuthority());

		$uri = (new Uri('https://example.com'))->withPort(443);
		$this->assertNull($uri->getPort());
		$this->assertSame('example.com', $uri->getAuthority());

		// HTTP standard port
		$uri = new Uri('http://example.com:80');
		$this->assertNull($uri->getPort());
		$this->assertSame('example.com', $uri->getAuthority());

		$uri = (new Uri('http://example.com'))->withPort(80);
		$this->assertNull($uri->getPort());
		$this->assertSame('example.com', $uri->getAuthority());
	}

	public function testPortIsReturnedIfSchemeUnknown() {
		$uri = (new Uri('//example.com'))->withPort(80);

		$this->assertSame(80, $uri->getPort());
		$this->assertSame('example.com:80', $uri->getAuthority());
	}

	public function testStandardPortIsNullIfSchemeChanges() {
		$uri = new Uri('http://example.com:443');
		$this->assertSame('http', $uri->getScheme());
		$this->assertSame(443, $uri->getPort());

		$uri = $uri->withScheme('https');
		$this->assertNull($uri->getPort());
	}

	public function testPortCanBeRemoved() {
		$uri = (new Uri('http://example.com:8080'))->withPort(null);

		$this->assertNull($uri->getPort());
		$this->assertSame('http://example.com', (string) $uri);
	}

	/**
	 * In RFC 8986 the host is optional and the authority can only
	 * consist of the user info and port.
	 */
	public function testAuthorityWithUserInfoOrPortButWithoutHost() {
		$uri = (new Uri())->withUserInfo('user', 'pass');

		$this->assertSame(['user' => 'user', 'pass' => 'pass'], $uri->getUserInfo());
		$this->assertSame('user:pass@', $uri->getAuthority());

		$uri = $uri->withPort(8080);
		$this->assertSame(8080, $uri->getPort());
		$this->assertSame('user:pass@:8080', $uri->getAuthority());
		$this->assertSame('//user:pass@:8080', (string) $uri);

		$uri = $uri->withUserInfo('', null);
		$this->assertSame(':8080', $uri->getAuthority());
	}

	public function testHostInHttpUriDefaultsToLocalhost() {
		$uri = (new Uri())->withScheme('http');

		$this->assertSame('localhost', $uri->getHost());
		$this->assertSame('localhost', $uri->getAuthority());
		$this->assertSame('http://localhost', (string) $uri);
	}

	public function testHostInHttpsUriDefaultsToLocalhost() {
		$uri = (new Uri())->withScheme('https');

		$this->assertSame('localhost', $uri->getHost());
		$this->assertSame('localhost', $uri->getAuthority());
		$this->assertSame('https://localhost', (string) $uri);
	}

	public function testFileSchemeWithEmptyHostReconstruction() {
		$uri = new Uri('file:///tmp/filename.ext');

		$this->assertSame(null, $uri->getHost());
		$this->assertSame('', $uri->getAuthority());
		$this->assertSame('file:///tmp/filename.ext', (string) $uri);
	}

	public function testWithPathEncodesProperly() {
		$uri = (new Uri())->withPath('/baz?#€/b%61r');
		// Query and fragment delimiters and multibyte chars are encoded.
		$this->assertSame('/baz%3F%23%E2%82%AC/b%61r', $uri->getPath());
		$this->assertSame('/baz%3F%23%E2%82%AC/b%61r', (string) $uri);
	}

	public function testWithQueryEncodesProperly() {
		$uri = (new Uri())->withQuery('?=#&€=/&b%61r');
		// A query starting with a "?" is valid and must not be magically removed. Otherwise it would be impossible to
		// construct such an URI. Also the "?" and "/" does not need to be encoded in the query.
		$this->assertSame('?=%23&%E2%82%AC=/&b%61r', $uri->getQuery());
		$this->assertSame('??=%23&%E2%82%AC=/&b%61r', (string) $uri);
	}

	public function testWithFragmentEncodesProperly() {
		$uri = (new Uri())->withFragment('#€?/b%61r');
		// A fragment starting with a "#" is valid and must not be magically removed. Otherwise it would be impossible to
		// construct such an URI. Also the "?" and "/" does not need to be encoded in the fragment.
		$this->assertSame('%23%E2%82%AC?/b%61r', $uri->getFragment());
		$this->assertSame('#%23%E2%82%AC?/b%61r', (string) $uri);
	}

	public function testAllowsForRelativeUri() {
		$uri = (new Uri())->withPath('foo');
		$this->assertSame('foo', $uri->getPath());
		$this->assertSame('foo', (string) $uri);
	}

	public function testRelativePathAndAuhorityIsThrowsException() {
		$this->expectException(\InvalidArgumentException::class);

		$uri = (new Uri())->withPath('foo')->withHost('example.com');
	}

	/**
	 * @expectedException \InvalidArgumentException
	 * @expectedExceptionMessage The path of a URI without an authority must not start with two slashes "//"
	 */
	public function testPathStartingWithTwoSlashesAndNoAuthorityIsInvalid() {
		// URI "//foo" would be interpreted as network reference and thus change the original path to the host
		(new Uri())->withPath('//foo');
	}

	public function testPathStartingWithTwoSlashes() {
		$uri = new Uri('http://example.org//path-not-host.com');
		$this->assertSame('//path-not-host.com', $uri->getPath());

		$uri = $uri->withScheme(null);
		$this->assertSame('//example.org//path-not-host.com', (string) $uri); // This is still valid
		$this->expectException(
			\InvalidArgumentException::class
		);
		$uri->withHost(null); // Now it becomes invalid
	}

	/**
	 * @expectedException \InvalidArgumentException
	 * @expectedExceptionMessage A relative URI must not have a path beginning with a segment containing a colon
	 */
	public function testRelativeUriWithPathBeginngWithColonSegmentIsInvalid() {
		(new Uri())->withPath('mailto:foo');
	}

	public function testRelativeUriWithPathHavingColonSegment() {
		$uri = (new Uri('urn:/mailto:foo'))->withScheme(null);
		$this->assertSame('/mailto:foo', $uri->getPath());

		$this->expectException(
			\InvalidArgumentException::class
		);
		(new Uri('urn:mailto:foo'))->withScheme(null);
	}

	public function testDefaultReturnValuesOfGetters() {
		$uri = new Uri();

		$this->assertSame(null, $uri->getScheme());
		$this->assertSame('', $uri->getAuthority());
		$this->assertSame(null, $uri->getHost());
		$this->assertNull($uri->getPort());
		$this->assertSame(null, $uri->getPath());
		$this->assertSame(null, $uri->getQuery());
		$this->assertSame(null, $uri->getFragment());
		$this->assertSame(
			[
				'user' => '',
				'pass' => null
			],
			$uri->getUserInfo()
		);
	}

	public function testImmutability() {
		$uri = new Uri('http://some-thing/');

		$this->assertNotSame($uri, $uri->withScheme('https'));
		$this->assertNotSame($uri, $uri->withUserInfo('user', 'pass'));
		$this->assertNotSame($uri, $uri->withHost('example.com'));
		$this->assertNotSame($uri, $uri->withPort(8080));
		$this->assertNotSame($uri, $uri->withPath('/path/123'));
		$this->assertNotSame($uri, $uri->withQuery('q=abc'));
		$this->assertNotSame($uri, $uri->withFragment('test'));
	}
}
