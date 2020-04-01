/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP;

use namespace HH\Lib\IO;
use namespace Facebook\Experimental\Http\Message;

final class ServerRequest
  extends Request
  implements Message\ServerRequestInterface {

  private dict<string, string> $cookie_params = dict[];

  private dict<string, Message\UploadedFileInterface> $uploaded_files = dict[];

  private dict<string, string> $query_params = dict[];

  private dict<string, string> $post_body = dict[];

  public function __construct(
    Message\HTTPMethod $method,
    Message\UriInterface $uri,
    private dict<string, string> $server_params,
    IO\ReadHandle $body,
  ) {
    parent::__construct($method, $uri, $body);
  }

  public function getServerParams(): dict<string, string> {
    return $this->server_params;
  }

  public function withServerParams(dict<string, string> $server_params): this {
    $server_request = clone $this;
    $server_request->server_params = $server_params;

    return $server_request;
  }

  public function getCookieParams(): dict<string, string> {
    return $this->cookie_params;
  }

  public function withCookieParams(dict<string, string> $cookies): this {
    $server_request = clone $this;
    $server_request->cookie_params = $cookies;

    return $server_request;
  }

  public function getQueryParams(): dict<string, string> {
    return $this->query_params;
  }

  public function withQueryParams(dict<string, string> $query): this {
    $server_request = clone $this;
    $server_request->query_params = $query;

    return $server_request;
  }

  public function getUploadedFiles(
  ): dict<string, Message\UploadedFileInterface> {
    return $this->uploaded_files;
  }

  public function withUploadedFiles(
    dict<string, Message\UploadedFileInterface> $uploaded_files,
  ): this {
    $server_request = clone $this;
    $server_request->uploaded_files = $uploaded_files;

    return $server_request;
  }

  public function getParsedBody(): dict<string, string> {
      return $this->post_body;
  }

  public function withParsedBody(dict<string, string> $data): this {
    $server_request = clone $this;
    $server_request->post_body = $data;

    return $server_request;
  }
}
