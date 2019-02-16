/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP;

use namespace HH\Lib\{C, Dict, Str, Vec};

trait MessageTrait {

  private dict<string, vec<string>> $headers = dict[];

  private dict<string, string> $header_names = dict[];

  private string $protocol_version = '1.1';

  public function getProtocolVersion(): string {
    return $this->protocol_version;
  }

  public function withProtocolVersion(string $version): this {
    if ($this->protocol_version === $version) {
      return $this;
    }
    $new = clone $this;
    $new->protocol_version = $version;

    return $new;
  }

  public function getHeaders(): dict<string, vec<string>> {
    return $this->headers;
  }

  public function hasHeader(string $header): bool {
    return C\contains_key($this->header_names, Str\lowercase($header));
  }

  public function getHeader(string $header): vec<string> {
    $header = Str\lowercase($header);
    if (!C\contains_key($this->header_names, $header)) {
      return vec[];
    }

    return $this->headers[$this->header_names[$header]];
  }

  public function getHeaderLine(string $header): string {
    return Str\join($this->getHeader($header), ', ');
  }

  public function withHeader(string $header, vec<string> $values): this {
    $normalized = Str\lowercase($header);
    $new = clone $this;

    if (C\contains_key($new->header_names, $normalized)) {
      $new->headers = Dict\filter(
        $new->headers,
        ($key) ==> {
          return $key !== $new->header_names[$normalized];
        },
      );
    }
    $new->header_names[$normalized] = $header;
    $new->headers[$header] =
      Vec\map($values, ($value) ==> Str\trim($value, " \t"));

    return $new;
  }

  public function withHeaderLine(string $name, string $value): this {
    return $this->withHeader($name, Str\split($value, ','));
  }

  public function withAddedHeader(string $header, vec<string> $values): this {
    $values = Vec\map($values, ($value) ==> Str\trim($value, " \t"));
    $normalized = Str\lowercase($header);
    $new = clone $this;

    if (C\contains_key($new->header_names, $normalized)) {
      $header = $this->header_names[$normalized];
      $new->headers[$header] = Vec\concat($this->headers[$header], $values);
    } else {
      $new->header_names[$normalized] = $header;
      $new->headers[$header] = $values;
    }

    return $new;
  }

  public function withAddedHeaderLine(string $name, string $value): this {
    return $this->withAddedHeader($name, Str\split($value, ','));
  }

  public function withoutHeader(string $header): this {
    $normalized = Str\lowercase($header);

    if (!C\contains_key($this->header_names, $normalized)) {
      return $this;
    }

    $header = $this->header_names[$normalized];
    $new = clone $this;
    $new->headers = dict[];
    $new->header_names = dict[];

    return $new;
  }

  private function setHeaders(dict<string, vec<string>> $headers): void {
    $this->header_names = dict[];
    $this->headers = dict[];

    foreach ($headers as $header => $value) {
      $normalized = Str\lowercase($header);

      $this->header_names[$normalized] = $header;
      $this->headers[$header] =
        Vec\map($value, ($header) ==> Str\trim($header, " \t"));
    }
  }
}
