/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use namespace HH\Lib\{File, IO};

/**
 * We still have to rely on good old php's super globals, so provide a
 * nice convenience method for all the users out there
 */
function create_server_request_from_globals(): Message\ServerRequestInterface {
	/* HH_FIXME[2050] */
	$server_params = dict($_SERVER);
	/* HH_FIXME[2050] */
	$post_vars = dict($_POST);
	/* HH_FIXME[2050] */
	$get_vars = dict($_GET);
	/* HH_FIXME[2050] */
	$cookies = dict($_COOKIE);

	return create_server_request(
		Message\HTTPMethod::assert(
			$server_params['REQUEST_METHOD'] ?? Message\HTTPMethod::GET,
		),
		create_uri($server_params['REQUEST_URI'] ?? null),
		$server_params,
		IO\request_input(),
	)
		->withParsedBody($post_vars)
		->withCookieParams($cookies)
		->withQueryParams($get_vars)
		->withUploadedFiles(
			Marshaler\UploadedFileMarshaler::batchMarshalPhpFilesArray(),
		);
}

function create_server_request(
	Message\HTTPMethod $method,
	Message\UriInterface $uri,
	dict<string, string> $server_params,
	IO\ReadHandle $request_input,
): Message\ServerRequestInterface {
	return new ServerRequest($method, $uri, $server_params, $request_input);
}

function create_uri(?string $uri = null): Message\UriInterface {
	return new Uri($uri);
}

function create_response(
	int $code = 200,
	string $reason = '',
): Message\ResponseInterface {
	$write_handle = File\open_write_only(
		\sys_get_temp_dir().'/'.\bin2hex(\random_bytes(16)),
		File\WriteMode::MUST_CREATE
	);
	return new Response($write_handle, $code, $reason);
}
