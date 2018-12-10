<?hh // strict

namespace Usox\HackTTP;

use namespace HH\Lib\Experimental\IO;
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

  /**
   * We still have to rely on good old php's super globals, so provide a
   * nice convenience method for all the users out there
   */
  public static function createFromGlobals(): Message\ServerRequestInterface {
    /* HH_FIXME[2050] */
    $server_params = dict($_SERVER);
    /* HH_FIXME[2050] */
    $post_vars = dict($_POST);
    /* HH_FIXME[2050] */
    $get_vars = dict($_GET);
    /* HH_FIXME[2050] */
    $cookies = dict($_COOKIE);

    $request = new ServerRequest(
      Message\HTTPMethod::assert($server_params['REQUEST_METHOD'] ?? Message\HTTPMethod::GET),
      new Uri($server_params['REQUEST_URI'] ?? null),
      $server_params,
      IO\request_input()
    );

    return $request->withParsedBody($post_vars)
      ->withCookieParams($cookies)
      ->withQueryParams($get_vars)
      ->withUploadedFiles(Marshaler\UploadedFileMarshaler::batchMarshalPhpFilesArray());
  }
}
