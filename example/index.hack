require_once '../vendor/hh_autoload.hh';

<<__EntryPoint>>
async function main(): Awaitable<noreturn> {
	$response = \Usox\HackTTP\create_response(200);

	$response = $response->withAddedHeaderLine('X-ZOMG', 'aggi');

	await $response->getBody()->writeAsync('foo');

	$emitter = new \Usox\HackTTP\Response\TemporaryFileSapiEmitter();
	await $emitter->emitAsync($response);
	exit();
}