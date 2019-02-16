/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP\Response;

use namespace HH\Lib\{Experimental\Filesystem, Experimental\IO, Math, Str};
use type Facebook\Experimental\Http\Message\ResponseInterface;
use type Usox\HackTTP\Exception\EmitterException;

final class TemporaryFileSapiEmitter implements EmitterInterface {

	/**
	 * Emits a response for a SAPI environment using a temporary file
	 *
	 * Due to the current limitations of the http-interface definitions, an insecure
	 * call to `getPath` on the WriteHandle is currently necessary. Also, a temporary
	 * file has to be used when creating the response.
	 *
	 * @see \Usox\HackTTP\createResponse
	 */
	public async function emitAsync(ResponseInterface $response): Awaitable<void> {
		if (\headers_sent()) {
			throw new EmitterException('Headers already sent');
		}
		if (\ob_get_level() > 0 && \ob_get_length() > 0) {
			throw new EmitterException('Output already sent');
		}

		$body = $response->getBody();

		/* HH_FIXME[4053] */
		$path = $body->getPath();

		$path as Filesystem\Path;

		$this->writeStatusLine($response);
		$this->writeHeaders($response);

		$temporary_file_path = $path->toString();

		await $body->closeAsync();
	  await using ($read_handle = Filesystem\open_read_only($temporary_file_path)) {
			$out = IO\request_output();
			$content = await $read_handle->readAsync(Math\INT64_MAX);
			await $out->writeAsync($content);
			await $out->flushAsync();
		};
		\unlink($temporary_file_path);
	}

	/**
	 * Emits the status line
	 */
	private function writeStatusLine(ResponseInterface $response): void {
		$code = $response->getStatusCode();
		\header(
			Str\format(
				'HTTP/%s %d%s',
				$response->getProtocolVersion(),
				$code,
				$response->getReasonPhrase(),
			),
			true,
			$code,
		);
	}

	/**
	 * Emits the response headers
	 */
	private function writeHeaders(ResponseInterface $response): void {
		$code = $response->getStatusCode();

		foreach ($response->getHeaders() as $header_name => $values) {
			$first = true;
			if (Str\lowercase($header_name) === 'set-cookie') {
				$first = false;
			}

			foreach ($values as $value) {
				\header(
					Str\format('%s: %s', $header_name, $value),
					$first,
					$code
				);

				$first = false;
			}
		}
	}
}
