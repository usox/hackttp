namespace Usox\HackTTP\Example;


<<__EntryPoint>>
async function main(): Awaitable<noreturn> {
  require_once '../vendor/hh_autoload.hack';
  \Facebook\AutoloadMap\initialize();
  $response = \Usox\HackTTP\create_response(200);

  $response = $response->withAddedHeaderLine('X-ZOMG', 'aggi');

  await $response->getBody()->writeAllAsync('foo');

  $emitter = new \Usox\HackTTP\Response\TemporaryFileSapiEmitter();
  await $emitter->emitAsync($response);
  exit();
}
