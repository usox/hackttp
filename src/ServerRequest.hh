<?hh // strict

namespace Usox\HackTTP;

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
		private dict<string, string> $server_params
    ) {
		parent::__construct($method, $uri);
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
		// TBD
		return $this->query_params;
	}

	public function withQueryParams(dict<string, string> $query): this {
		$server_request = clone $this;
		$server_request->query_params = $query;

		return $server_request;
	}

	public function getUploadedFiles(): dict<string, Message\UploadedFileInterface> {
		return $this->uploaded_files;
	}

	public function withUploadedFiles(dict<string, Message\UploadedFileInterface> $uploaded_files): this {
		$server_request = clone $this;
		$server_request->uploaded_files = $uploaded_files;

		return $server_request;
	}

	public function getParsedBody(): dict<string, string> {
		$method = $this->server_params['REQUEST_METHOD'] ?? null;
		$content_type = $this->server_params['CONTENT_TYPE'] ?? null;
		if (
			$method === Message\HTTPMethod::POST && 
			(
				$content_type === 'application/x-www-form-urlencoded' ||
				$content_type === 'multipart/form-data'
			)
		) {
			return $this->post_body;
		}
		return dict[];
	}

	public function withParsedBody(dict<string, string> $data): this {
		$server_request = clone $this;
		$server_request->post_body = $data;

		return $server_request;
	}
}
