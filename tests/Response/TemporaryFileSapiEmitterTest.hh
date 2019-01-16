<?hh // strict

namespace Usox\HackTTP\Response;

use namespace Facebook\Experimental\Http\Message;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use namespace HH\Lib\{Experimental\Filesystem, Experimental\IO, Str};
use function Usox\HackMock\{mock, prospect};

class TemporaryFileSapiEmitterTest extends HackTest {

	public function testEmitEmitsReponse(): void {
		$emitter = new TemporaryFileSapiEmitter();

		$status_code = 666;
		$protocol_version = '4.2';
		$resaon = 'I\'m in the honeypot';
		$header_key = 'some-header';
		$header_value = 'some-header-value';
		$body_value = '';
		$path = \sys_get_temp_dir().'/'.\bin2hex(\random_bytes(16));

		\file_put_contents($path, $body_value);

		$response = mock(Message\ResponseInterface::class);
		$handle = mock(Filesystem\FileHandle::class);
		$path_handle = mock(Filesystem\Path::class);

		prospect($handle, 'getPath')
			->once()
			->andReturn($path_handle);
		prospect($handle, 'closeAsync')
			->once();

		prospect($path_handle, 'toString')
			->once()
			->andReturn($path);

		prospect($response, 'getBody')
			->once()
			->andReturn($handle);
		prospect($response, 'getStatusCode')
			->times(2)
			->andReturn($status_code);
		prospect($response, 'getProtocolVersion')
			->once()
			->andReturn($protocol_version);
		prospect($response, 'getReasonPhrase')
			->once()
			->andReturn($resaon);
		prospect($response, 'getHeaders')
			->once()
			->andReturn(dict[$header_key => vec[$header_value]]);

		\HH\Asio\join($emitter->emitAsync($response));
	}
}