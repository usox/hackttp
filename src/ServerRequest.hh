<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;

final class ServerRequest extends Request implements Message\ServerRequestInterface {

	private dict<string, string> $cookie_params = dict[];

	private mixed $uploaded_files;

	private mixed $query_params;

	private dict<string, string> $post_body = dict[];

    public function __construct(
    ): void {
    }

	public function getServerParams(): Message\ServerParamInterface {
		/* HH_FIXME[2050] Access to super global */
		return new ServerParam($_SERVER);
	}

	public function getCookieParams(): dict<string, string> {
		// TBD
		return $this->cookie_params;
	}

	public function withCookieParams(dict<string, string> $cookies): this {
		$server_request = clone $this;
		$server_request->cookie_params = $cookies;

		return $server_request;
	}

	public function getQueryParams(): mixed {
		// TBD
		return $this->query_params;
	}

	public function withQueryParams(mixed $query): this {
		$server_request = clone $this;
		$server_request->query_params = $query;

		return $server_request;
	}

	public function getUploadedFiles(): mixed {
		// TBD
		return $this->uploaded_files;
	}

	public function withUploadedFiles(mixed $uploaded_files): this {
		$server_request = clone $this;
		$server_request->uploaded_files = $uploaded_files;

		return $server_request;
	}

	public function getPostBody(): dict<string, string> {
		$server_params = $this->getServerParams();
		if (
			$server_params->getRequestMethod() === Message\HTTPMethod::POST &&
			(
				$server_params->getContentType() === 'application/x-www-form-urlencoded' ||
				$server_params->getContentType() === 'multipart/form-data'
			)
		) {
			return $this->post_body;
		}
		return dict[];
	}

	public function withPostBody(dict<string, string> $data): this {
		$server_request = clone $this;
		$server_request->post_body = $data;

		return $server_request;
	}
}