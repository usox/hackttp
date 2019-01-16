<?hh // strict

namespace Usox\HackTTP\Response;

use type Facebook\Experimental\Http\Message\ResponseInterface;

interface EmitterInterface {

	public function emitAsync(ResponseInterface $response): Awaitable<void>;
}