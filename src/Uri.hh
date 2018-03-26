<?hh // strict
namespace Usox\HackTTP;

use Facebook\Experimental\Http\Message\UriInterface;
use HH\Lib\C;

final class Uri implements UriInterface {

	const HTTP_DEFAULT_HOST = 'localhost';

	private static dict<string, int> $default_ports = dict[
		'http'  => 80,
		'https' => 443,
		'ftp' => 21,
		'gopher' => 70,
		'nntp' => 119,
		'news' => 119,
		'telnet' => 23,
		'tn3270' => 23,
		'imap' => 143,
		'pop' => 110,
		'ldap' => 389,
	];

	private static string $char_unreserved = 'a-zA-Z0-9_\-\.~';

	private static string $char_sub_delimeters = '!\$&\'\(\)\*\+,;=';

	private string $scheme = '';

	private string $user_name = '';

	private ?string $user_password = '';

	private string $host = '';

	private ?int $port;

	private string $path = '';

	private string $query = '';

	private string $fragment = '';

	public function __construct(string $uri): void {
		if ($uri != '') {
			$this->loadFromUri($uri);
		}
	}

	public function __toString(): string {
		$uri = $this->scheme.':';

		$authority = $this->getAuthority();

		if ($authority != '' || $this->scheme === 'file') {
			$uri .= '//' . $authority;
		}

		$uri .= $this->path;

		if ($this->query != '') {
			$uri .= '?'.$this->query;
		}

		if ($this->fragment != '') {
			$uri .= '#'.$this->fragment;
		}

		return $uri;
	}

	public function getScheme(): string {
		return $this->scheme;
	}

	public function getAuthority(): string {
		$authority = $this->host;

		$user_info = '';
		if ($this->user_name !== '') {
			$user_info = $this->user_name;

			if ((string) $this->user_password !== '') {
				$user_info .= sprintf(':%s', $this->user_password);
			}

			$authority = sprintf(
				'%s@%s',
				$user_info,
				$authority
			);
		}

		if ($this->port !== null) {
			$authority .= sprintf(':%d', $this->port);
		}

		return $authority;
	}

	public function getUserInfo(): ?shape('user' => string, 'pass' => ?string) {
		if ($this->user_name === '') {
			return null;
		}
		return shape(
			'user' => $this->user_name,
			'pass' => $this->user_password
		);
	}

	public function getHost(): string {
		return $this->host;
	}

	public function getPort(): ?int	{
		return $this->port;
	}

	public function getPath(): string{
		return $this->path;
	}

	public function getQuery(): string {
		return $this->query;
	}

	public function getFragment(): string {
		return $this->fragment;
	}

	public function withScheme(string $scheme): this {
		$scheme = $this->filterScheme($scheme);

		if ($this->scheme === $scheme) {
			return $this;
		}

		$new = clone $this;
		$new->scheme = $scheme;
		$new->removeDefaultPort();
		$new->validateState();

		return $new;
	}

	public function withUserInfo(string $user, ?string $password = null): this {
		if ($this->user_name === $user && $this->user_password === $password) {
			return $this;
		}

		$new = clone $this;
		$new->user_name = $user;
		$new->user_password = $password;
		$new->validateState();

		return $new;
	}

	public function withHost(string $host): this {
		$host = $this->filterHost($host);

		if ($this->host === $host) {
			return $this;
		}

		$new = clone $this;
		$new->host = $host;
		$new->validateState();

		return $new;
	}

	public function withPort(?int $port): this {
		$port = $this->filterPort($port);

		if ($this->port === $port) {
			return $this;
		}

		$new = clone $this;
		$new->port = $port;
		$new->removeDefaultPort();
		$new->validateState();

		return $new;
	}

	public function withPath(string $path): this {
		$path = $this->filterPath($path);

		if ($this->path === $path) {
			return $this;
		}

		$new = clone $this;
		$new->path = $path;
		$new->validateState();

		return $new;
	}

	public function withQuery(string  $query): this {
		$query = $this->filterQueryAndFragment($query);

		if ($this->query === $query) {
			return $this;
		}

		$new = clone $this;
		$new->query = $query;

		return $new;
	}

	public function withFragment(string $fragment): this {
		$fragment = $this->filterQueryAndFragment($fragment);

		if ($this->fragment === $fragment) {
			return $this;
		}

		$new = clone $this;
		$new->fragment = $fragment;

		return $new;
	}

	private function loadFromUri(string $uri): void {
		$parts = parse_url($uri);
		if ($parts === false) {
			throw new \InvalidArgumentException("Unable to parse URI: $uri");
		}
		if (C\contains($parts, 'scheme')) {
			$this->scheme = $this->filterScheme((string) $parts['scheme']);
		}
		if (C\contains($parts, 'host')) {
			$this->host = $this->filterHost((string) $parts['host']);
		}
		if (C\contains($parts, 'port') && $parts['port'] !== null) {
			$this->port = $this->filterPort((int) $parts['port']);
		}
		if (C\contains($parts, 'path')) {
			$this->path = $this->filterPath((string) $parts['path']);
		}
		if (C\contains($parts, 'query')) {
			$this->query = $this->filterQueryAndFragment((string) $parts['query']);
		}
		if (C\contains($parts, 'fragment')) {
			$this->fragment = $this->filterQueryAndFragment((string) $parts['fragment']);
		}
		if (C\contains($parts, 'user')) {
			$this->user_name = (string) $parts['user'];
		}
		if (C\contains($parts, 'pass')) {
			$this->user_password = (string) $parts['pass'];
		}

		$this->removeDefaultPort();
	}

	private function filterScheme(string $scheme): string {
		return strtolower($scheme);
	}

	private function filterHost(string $host): string {
		if (!is_string($host)) {
			throw new \InvalidArgumentException('Host must be a string');
		}

		return strtolower($host);
	}

	private function filterPort(?int $port): ?int {
		if ($port === null) {
			return null;
		}

		$max_port = 65535;

		if (1 > $port || $max_port < $port) {
			throw new \InvalidArgumentException(
				sprintf('Invalid port: %d. Must be between 1 and %d', $port, $max_port)
			);
		}
		return $port;
	}

	private function removeDefaultPort(): void {
		if ($this->port !== null && C\contains(static::$default_ports, $this->scheme) && $this->port === static::$default_ports[$this->scheme]) {
			$this->port = null;
		}
	}

	private function filterPath(string $path): string {
		return preg_replace_callback(
			'/(?:[^' . static::$char_unreserved . static::$char_sub_delimeters . '%:@\/]++|%(?![A-Fa-f0-9]{2}))/',
			function ($match) {
				return rawurlencode($match[0]);
			},
			$path
		);
	}

	private function filterQueryAndFragment(string $str): string {
		return preg_replace_callback(
			'/(?:[^' . static::$char_unreserved . static::$char_sub_delimeters . '%:@\/\?]++|%(?![A-Fa-f0-9]{2}))/',
			function ($match) {
				return rawurlencode($match[0]);
			},
			$str
		);
	}

	private function validateState(): void {
		if ($this->host === '' && ($this->scheme === 'http' || $this->scheme === 'https')) {
			$this->host = static::HTTP_DEFAULT_HOST;
		}

		if ($this->getAuthority() === '') {
			if (0 === strpos($this->path, '//')) {
				throw new \InvalidArgumentException('The path of a URI without an authority must not start with two slashes "//"');
			}
			if ($this->scheme === '' && false !== strpos(explode('/', $this->path, 2)[0], ':')) {
				throw new \InvalidArgumentException('A relative URI must not have a path beginning with a segment containing a colon');
			}
		} elseif (strpos($this->path, '/') !== 0) {
			throw new \InvalidArgumentException('The path of a URI with an authority must start with a slash "/" or be empty');
		}
	}
}
