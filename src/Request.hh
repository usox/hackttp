<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use namespace HH\Lib\{C, Dict};

class Request implements Message\RequestInterface {

  use MessageTrait;

  private ?string $request_target;

  public function __construct(
    private Message\HTTPMethod $method,
    private Message\UriInterface $uri,
    dict<string, vec<string>> $headers = dict[],
    ?string $body = null,
    string $version = '1.1',
  ) {
    $this->setHeaders($headers);
    $this->protocol_version = $version;
    if (!$this->hasHeader('Host')) {
      $this->updateHostFromUri();
    }

    $this->body = $body;
  }

  public function getRequestTarget(): string {
    if ($this->request_target !== null) {
      return $this->request_target;
    }
    $target = $this->uri->getPath();
    if ($target === '' || $target === null) {
      $target = '/';
    }

    $raw_query = $this->uri->getRawQuery();
    if ($raw_query !== '') {
      $target .= '?'.$raw_query;
    }
    return $target;
  }

  public function withRequestTarget(string $request_target): this {
    if (\preg_match('#\s#', $request_target)) {
      throw new \InvalidArgumentException(
        'Invalid request target provided; must not contain whitespace',
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
    Message\RequestURIOptions $options = shape('preserveHost' => false),
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

  /**
   * Updates the host header and ensure its position
   * @see http://tools.ietf.org/html/rfc7230#section-5.4
   */
  private function updateHostFromUri(): void {
    $host = $this->uri->getHost();
    if ($host === '' || $host === null) {
      return;
    }
    if (($port = $this->uri->getPort()) !== null) {
      $host .= ':'.$port;
    }

    if (C\contains_key($this->header_names, 'host')) {
      $header = $this->header_names['host'];
    } else {
      $header = 'Host';
      $this->header_names['host'] = 'Host';
    }

    $this->headers = Dict\merge(
      dict[$header => vec[$host]],
      Dict\filter_keys($this->headers, ($key) ==> $key !== 'host'),
    );
  }
}
