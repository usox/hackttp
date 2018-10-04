<?hh // strict

namespace Usox\HackTTP;

use namespace HH\Lib\Experimental\Filesystem;
use type Facebook\Experimental\Http\Message\StreamInterface;
use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect};
use namespace HH\Lib\PseudoRandom;

class UploadedStreamTest extends \Facebook\HackTest\HackTest {

	private string $tmp_file_path = '';

	private int $size = 666;

	public async function beforeEachTestAsync(): Awaitable<void> {
		$this->tmp_file_path = '/tmp/'.(string) PseudoRandom\int();
	}

	public async function afterEachTestAsync(): Awaitable<void> {
		Filesystem\remove_file($this->tmp_file_path);
	}

	public function testGetSizeReturnsSize(): void {
		$uploaded_stream = new UploadedStream(
			mock(StreamInterface::class),
			$this->size,
			\UPLOAD_ERR_OK
		);
		expect(
			$uploaded_stream->getSize()
		)
		->toBeSame($this->size);
	}

	public function testGetStreamErrorsIfUploadIsNok(): void {
		$uploaded_stream = new UploadedStream(
			mock(StreamInterface::class),
			$this->size,
			\UPLOAD_ERR_INI_SIZE
		);
		expect(
			() ==> $uploaded_stream->getStream()
		)
		->toThrow(
			\RuntimeException::class,
			'Cannot retrieve stream due to upload error'
		);
	}

	public function testGetErrorReturnsErrorCode(): void {
		$uploaded_stream = new UploadedStream(
			mock(StreamInterface::class),
			$this->size,
			\UPLOAD_ERR_OK
		);
		expect(
			$uploaded_stream->getError()
		)
		->toBeSame(\UPLOAD_ERR_OK);
	}

	public function testGetClientMediaTypeReturnsTrpe(): void {
		$media_type = 'beer/hefeweizen';

		$uploaded_stream = new UploadedStream(
			mock(StreamInterface::class),
			$this->size,
			\UPLOAD_ERR_OK,
			null,
			$media_type
		);
		expect(
			$uploaded_stream->getClientMediaType()
		)
		->toBeSame($media_type);
	}

	public function testGetClientFileNameReturnsName(): void {
		$filename = 'some-filename';

		$uploaded_stream = new UploadedStream(
			mock(StreamInterface::class),
			$this->size,
			\UPLOAD_ERR_OK,
			$filename
		);
		expect(
			$uploaded_stream->getClientFilename()
		)
		->toBeSame($filename);
	}

	public function testMoveToMoves(): void {
		$stream = mock(StreamInterface::class);
		$uploaded_stream = new UploadedStream(
			$stream,
			$this->size,
			\UPLOAD_ERR_OK
		);
		$buffer = 'some-buffer';

		prospect($stream, 'eof')
			->times(1)
			->andReturn(false);
		prospect($stream, 'eof')
			->times(1)
			->andReturn(true);
		prospect($stream, 'read')
			->with(8192)
			->once()
			->andReturn($buffer);

		$uploaded_stream->moveTo($this->tmp_file_path);

		expect(
			\file_get_contents($this->tmp_file_path)
		)
		->toBeSame($buffer);

		expect(
			() ==> $uploaded_stream->getStream()
		)
		->toThrow(
			\RuntimeException::class,
			'Cannot retrieve stream after it has already been moved'
		);
	}
}
