<?hh // strict

namespace Usox\HackTTP;

use function Facebook\FBExpect\expect;

class UriTest extends \Facebook\HackTest\HackTest {

  public function testParsesProvidedUri(): void {
    $orig_uri = 'https://user:pass@example.com:8080/path/123?q=abc#test';

    $uri = new Uri($orig_uri);

    expect($uri->getScheme())->toBeSame('https');
    expect($uri->getAuthority())->toBeSame('user:pass@example.com:8080');
    expect($uri->getUserInfo())->toBePHPEqual(
      ['user' => 'user', 'pass' => 'pass'],
    );
    expect($uri->getHost())->toBeSame('example.com');
    expect($uri->getPort())->toBeSame(8080);
    expect($uri->getPath())->toBeSame('/path/123');
    expect($uri->getRawQuery())->toBeSame('q=abc');
    expect($uri->getFragment())->toBeSame('test');
    expect((string)$uri)->toBeSame($orig_uri);
  }

  public function testCanTransformAndRetrievePartsIndividually(): void {
    $uri = (new Uri())
      ->withScheme('https')
      ->withUserInfo('user', 'pass')
      ->withHost('example.com')
      ->withPort(8080)
      ->withPath('/path/123')
      ->withRawQuery('q=abc')
      ->withFragment('test');

    expect($uri->getScheme())->toBeSame('https');
    expect($uri->getAuthority())->toBeSame('user:pass@example.com:8080');
    expect($uri->getUserInfo())->toBePHPEqual(
      ['user' => 'user', 'pass' => 'pass'],
    );
    expect($uri->getHost())->toBeSame('example.com');
    expect($uri->getPort())->toBeSame(8080);
    expect($uri->getPath())->toBeSame('/path/123');
    expect($uri->getRawQuery())->toBeSame('q=abc');
    expect($uri->getFragment())->toBeSame('test');
    expect((string)$uri)->toBeSame(
      'https://user:pass@example.com:8080/path/123?q=abc#test',
    );
  }

  public function testPortMustBeValid(): void {
    expect(() ==> (new Uri())->withPort(100000))->toThrow(
      \InvalidArgumentException::class,
      'Invalid port: 100000. Must be between 1 and 65535',
    );
  }

  public function testWithPortCannotBeZero(): void {
    expect(() ==> (new Uri())->withPort(0))->toThrow(
      \InvalidArgumentException::class,
      'Invalid port: 0. Must be between 1 and 65535',
    );
  }

  public function testParseUriPortCannotBeZero(): void {
    expect(() ==> new Uri('//example.com:0'))->toThrow(
      \InvalidArgumentException::class,
      'Unable to parse URI',
    );
  }

  public function testCanParseFalseyUriParts(): void {
    $orig_uri = '0://0:0@0/0?0#0';

    $uri = new Uri($orig_uri);

    expect($uri->getScheme())->toBeSame('0');
    expect($uri->getAuthority())->toBeSame('0:0@0');
    expect($uri->getUserInfo())->toBePHPEqual(['user' => '0', 'pass' => '0']);
    expect($uri->getHost())->toBeSame('0');
    expect($uri->getPath())->toBeSame('/0');
    expect($uri->getRawQuery())->toBeSame('0');
    expect($uri->getFragment())->toBeSame('0');
    expect((string)$uri)->toBeSame($orig_uri);
  }

  public function testCanConstructFalseyUriParts(): void {
    $uri = (new Uri())
      ->withScheme('0')
      ->withUserInfo('0', '0')
      ->withHost('0')
      ->withPath('/0')
      ->withRawQuery('0')
      ->withFragment('0');

    expect($uri->getScheme())->toBeSame('0');
    expect($uri->getAuthority())->toBeSame('0:0@0');
    expect($uri->getUserInfo())->toBePHPEqual(['user' => '0', 'pass' => '0']);
    expect($uri->getHost())->toBeSame('0');
    expect($uri->getPath())->toBeSame('/0');
    expect($uri->getRawQuery())->toBeSame('0');
    expect($uri->getFragment())->toBeSame('0');
    expect((string)$uri)->toBeSame('0://0:0@0/0?0#0');
  }

  public function testSchemeIsNormalizedToLowercase(): void {
    $uri = new Uri('HTTP://example.com');

    expect($uri->getScheme())->toBeSame('http');
    expect((string)$uri)->toBeSame('http://example.com');

    $uri = (new Uri('//example.com'))->withScheme('HTTP');

    expect($uri->getScheme())->toBeSame('http');
    expect((string)$uri)->toBeSame('http://example.com');
  }

  public function testHostIsNormalizedToLowercase(): void {
    $uri = new Uri('//eXaMpLe.CoM');

    expect($uri->getHost())->toBeSame('example.com');
    expect((string)$uri)->toBeSame('//example.com');

    $uri = (new Uri())->withHost('eXaMpLe.CoM');

    expect($uri->getHost())->toBeSame('example.com');
    expect((string)$uri)->toBeSame('//example.com');
  }

  public function testPortIsNullIfStandardPortForScheme(): void {
    // HTTPS standard port
    $uri = new Uri('https://example.com:443');

    expect($uri->getPort())->toBeNull();
    expect($uri->getAuthority())->toBeSame('example.com');

    $uri = (new Uri('https://example.com'))->withPort(443);

    expect($uri->getPort())->toBeNull();
    expect($uri->getAuthority())->toBeSame('example.com');

    // HTTP standard port
    $uri = new Uri('http://example.com:80');

    expect($uri->getPort())->toBeNull();
    expect($uri->getAuthority())->toBeSame('example.com');

    $uri = (new Uri('http://example.com'))->withPort(80);

    expect($uri->getPort())->toBeNull();
    expect($uri->getAuthority())->toBeSame('example.com');
  }

  public function testPortIsReturnedIfSchemeUnknown(): void {
    $uri = (new Uri('//example.com'))->withPort(80);

    expect($uri->getPort())->toBeSame(80);
    expect($uri->getAuthority())->toBeSame('example.com:80');
  }

  public function testStandardPortIsNullIfSchemeChanges(): void {
    $uri = new Uri('http://example.com:443');

    expect($uri->getScheme())->toBeSame('http');
    expect($uri->getPort())->toBeSame(443);

    $uri = $uri->withScheme('https');

    expect($uri->getPort())->toBeNull();
  }

  public function testPortCanBeRemoved(): void {
    $uri = (new Uri('http://example.com:8080'))->withPort(null);

    expect($uri->getPort())->toBeNull();
    expect((string)$uri)->toBeSame('http://example.com');
  }

  /**
   * In RFC 8986 the host is optional and the authority can only
   * consist of the user info and port.
   */
  public function testAuthorityWithUserInfoOrPortButWithoutHost(): void {
    $uri = (new Uri())->withUserInfo('user', 'pass');

    expect($uri->getUserInfo())->toBePHPEqual(
      ['user' => 'user', 'pass' => 'pass'],
    );
    expect($uri->getAuthority())->toBeSame('user:pass@');

    $uri = $uri->withPort(8080);

    expect($uri->getPort())->toBeSame(8080);
    expect($uri->getAuthority())->toBeSame('user:pass@:8080');
    expect((string)$uri)->toBeSame('//user:pass@:8080');

    $uri = $uri->withUserInfo('', '');

    expect($uri->getAuthority())->toBeSame(':8080');
  }

  public function testHostInHttpUriDefaultsToLocalhost(): void {
    $uri = (new Uri())->withScheme('http');

    expect($uri->getHost())->toBeSame('localhost');
    expect($uri->getAuthority())->toBeSame('localhost');
    expect((string)$uri)->toBeSame('http://localhost');
  }

  public function testHostInHttpsUriDefaultsToLocalhost(): void {
    $uri = (new Uri())->withScheme('https');

    expect($uri->getHost())->toBeSame('localhost');
    expect($uri->getAuthority())->toBeSame('localhost');
    expect((string)$uri)->toBeSame('https://localhost');
  }

  public function testFileSchemeWithEmptyHostReconstruction(): void {
    $orig_uri = 'file:///tmp/filename.ext';

    $uri = new Uri($orig_uri);

    expect($uri->getHost())->toBeSame('');
    expect($uri->getAuthority())->toBeSame('');
    expect((string)$uri)->toBeSame($orig_uri);
  }

  public function testWithPathEncodesProperly(): void {
    $encoded_path = '/baz%3F%23%E2%82%AC/b%61r';

    $uri = (new Uri())->withPath('/baz?#€/b%61r');

    // Query and fragment delimiters and multibyte chars are encoded.
    expect($uri->getPath())->toBeSame($encoded_path);
    expect((string)$uri)->toBeSame($encoded_path);
  }

  public function testWithRawQueryEncodesProperly(): void {
    $encoded_query = '?=%23&%E2%82%AC=/&b%61r';

    $uri = (new Uri())->withRawQuery('?=#&€=/&b%61r');

    // A query starting with a "?" is valid and must not be magically removed. Otherwise it would be impossible to
    // construct such an URI. Also the "?" and "/" does not need to be encoded in the query.
    expect($uri->getRawQuery())->toBeSame($encoded_query);
    expect((string)$uri)->toBeSame('?'.$encoded_query);
  }

  public function testWithFragmentEncodesProperly(): void {
    $encoded_fragment = '%23%E2%82%AC?/b%61r';

    $uri = (new Uri())->withFragment('#€?/b%61r');

    // A fragment starting with a "#" is valid and must not be magically removed. Otherwise it would be impossible to
    // construct such an URI. Also the "?" and "/" does not need to be encoded in the fragment.
    expect($uri->getFragment())->toBeSame($encoded_fragment);
    expect((string)$uri)->toBeSame('#'.$encoded_fragment);
  }

  public function testAllowsForRelativeUri(): void {
    $uri = (new Uri())->withPath('relative-foobar');

    expect($uri->getPath())->toBeSame('relative-foobar');
    expect((string)$uri)->toBeSame('relative-foobar');

  }

  public function testRelativePathAndAuhorityIsThrowsException(): void {
    expect(() ==> (new Uri())->withPath('foo')->withHost('example.com'))
      ->toThrow(\InvalidArgumentException::class);
  }

  public function testPathStartingWithTwoSlashesAndNoAuthorityIsInvalid(
  ): void {
    // URI "//foo" would be interpreted as network reference and thus change the original path to the host
    expect(() ==> (new Uri())->withPath('//foo'))->toThrow(
      \InvalidArgumentException::class,
      'The path of a URI without an authority must not start with two slashes "//"',
    );
  }

  public function testPathStartingWithTwoSlashes(): void {
    $uri = new Uri('http://example.org//path-not-host.com');

    expect($uri->getPath())->toBeSame('//path-not-host.com');

    $uri = $uri->withScheme('');

    expect((string)$uri)->toBeSame(
      '//example.org//path-not-host.com',
    ); // This is still valid

    expect(
      () ==> $uri->withHost(''), // Now it becomes invalid
    )->toThrow(\InvalidArgumentException::class);
  }

  public function testRelativeUriWithPathBeginngWithColonSegmentIsInvalid(
  ): void {
    expect(() ==> (new Uri())->withPath('mailto:foo'))->toThrow(
      \InvalidArgumentException::class,
      'A relative URI must not have a path beginning with a segment containing a colon',
    );
  }

  public function testRelativeUriWithPathHavingColonSegment(): void {
    $uri = (new Uri('urn:/mailto:foo'))->withScheme('');

    expect($uri->getPath())->toBeSame('/mailto:foo');

    expect(() ==> (new Uri('urn:mailto:foo'))->withScheme(''))->toThrow(
      \InvalidArgumentException::class,
    );
  }

  public function testDefaultReturnValuesOfGetters(): void {
    $uri = new Uri();

    expect($uri->getScheme())->toBeSame('');
    expect($uri->getAuthority())->toBeSame('');
    expect($uri->getHost())->toBeSame('');
    expect($uri->getPort())->toBeNull();
    expect($uri->getPath())->toBeSame('');
    expect($uri->getRawQuery())->toBeSame('');
    expect($uri->getFragment())->toBeSame('');
    expect($uri->getUserInfo())->toBePHPEqual(['user' => '', 'pass' => '']);
  }

  public function testImmutability(): void {
    $uri = new Uri('http://some-thing/');

    expect($uri->withScheme('https'))->toNotBeSame($uri);
    expect($uri->withUserInfo('user', 'pass'))->toNotBeSame($uri);
    expect($uri->withHost('example.com'))->toNotBeSame($uri);
    expect($uri->withPort(666))->toNotBeSame($uri);
    expect($uri->withPath('/some/where'))->toNotBeSame($uri);
    expect($uri->withRawQuery('q=a'))->toNotBeSame($uri);
    expect($uri->withFragment('test'))->toNotBeSame($uri);
  }

  public function testWithQueryEncodesProperly(): void {
    $query = dict[
      'foo' => 'bar',
      'f%=?oo' => 'bär',
    ];

    $uri = (new Uri())->withQuery($query);

    expect($uri->getQuery())->toBeSame($query);
    expect((string)$uri)->toBeSame('?foo=bar&f%25%3D%3Foo=b%C3%A4r');
  }
}
