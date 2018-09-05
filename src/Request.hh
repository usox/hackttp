<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use namespace HH\Lib\{C, Dict, Str};

class Request implements Message\RequestInterface {

	use MessageTrait;

	private Message\HTTPMethod $method;

	private string $request_target = '';

    public function __construct(
        string $method,
        private Message\UriInterface $uri,
        private dict<string, vec<string>> $headers = dict[],
        private ?Message\StreamInterface $body = null,
        private string $version = '1.1'
    ) {
        $this->method = Message\HTTPMethod::assert(Str\lowercase($method));
        //$this->setHeaders($headers);
        $this->protocol = $version;
        if (!$this->hasHeader('Host')) {
            $this->updateHostFromUri();
        }
		$this->stream = $body;
    }

    public function getRequestTarget(): string {
        if ($this->request_target !== null) {
            return $this->request_target;
        }
        $target = $this->uri->getPath();
        if ($target === '' || $target === null) {
            $target = '/';
        }
        if ($this->uri->getQuery() != '') {
            $target .= '?' . $this->uri->getRawQuery();
        }
        return $target;
    }

    public function withRequestTarget(string $request_target): this {
        if (\preg_match('#\s#', $request_target)) {
            throw new \InvalidArgumentException(
                'Invalid request target provided; must not contain whitespace'
            );
        }
        $new = clone $this;
        $new->request_target = $request_target;

        return $new;
    }

    public function getMethod(): Message\HTTPMethod {
        return $this->method;
    }

    public function withMethod(Message\HTTPMethod $method): this {
        $new = clone $this;
        $new->method = $method;

        return $new;
    }

    public function getUri(): Message\UriInterface {
        return $this->uri;
    }

    public function withUri(
		Message\UriInterface $uri,
		Message\RequestURIOptions $options = shape('preserveHost' => false)
	): this {
        if ($uri === $this->uri) {
            return $this;
        }
        $new = clone $this;
        $new->uri = $uri;
		if ($options['preserveHost'] === false) {
            $new->updateHostFromUri();
        }
        return $new;
    }

    private function updateHostFromUri(): void{
        $host = $this->uri->getHost();
        if ($host == '' || $host === null) {
            return;
        }
        if (($port = $this->uri->getPort()) !== null) {
            $host .= ':' . $port;
        }

		if (C\contains_key($this->header_names, 'host')) {
            $header = $this->header_names['host'];
        } else {
            $header = 'Host';
            $this->header_names['host'] = 'Host';
        }

        // Ensure Host is the first header.
        // See: http://tools.ietf.org/html/rfc7230#section-5.4
		$this->headers = Dict\merge(
			dict[$header => vec[$host]],
			$this->headers
		);
    }
}