/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP\Response;

use namespace HH\Lib\{File, IO, Str};
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

		// Lambda is used to force $path to be string, not TAny (`_`).
		$path = (): string ==> {
			/*HH_FIXME[4053] getPath is not part of the WriteHandle interface.*/
			$path_maybe_object = $body->getPath();
			// Path is going to be removed from the hsl.
			// We just used `->toString()` anyway.
			// This hacky expression is needed, because `$path_maybe_object`
			// is of the TAny type (`_`), so the typechecker won't error when
			// `->getPath()` starts returning a string.
			// This expression works for both Path and string.
			return $path_maybe_object |> $$ is string ? $$ : $$->toString();
		}();

		$this->writeStatusLine($response);
		$this->writeHeaders($response);

		if ($body is IO\CloseableHandle) {
			$body->close();
		}

		$read_handle = File\open_read_only($path);
		using ($read_handle->closeWhenDisposed()) {
			$out = IO\request_output();
			$content = await $read_handle->readAllAsync();
			await $out->writeAllAsync($content);
		};
		\unlink($path);
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
