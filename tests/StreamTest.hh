<?hh // strict
namespace Usox\HackTTP;

use function Facebook\FBExpect\expect;
use namespace HH\Lib\Str;

class StreamTest extends \Facebook\HackTest\HackTestCase {
	public static bool $has_fread_error = false;

	public function testConstructorInitializesProperties(): void {
		$handle = \fopen('php://temp', 'r+');
		\fwrite($handle, 'data');
		$stream = new Stream($handle);

		$meta_data = $stream->getMetadata();

		expect($stream->isReadable())->toBeTrue();
		expect($stream->isWritable())->toBeTrue();
		expect($stream->isSeekable())->toBeTrue();
		expect($meta_data['uri'])->toBeSame('php://temp');
		expect($stream->getSize())->toBeSame(4);
		expect($stream->eof())->toBeFalse();

		$stream->close();
	}

	public function testConvertsToString(): void {
		$handle = \fopen('php://temp', 'w+');
		\fwrite($handle, 'data');
		$stream = new Stream($handle);

		expect((string) $stream)->toBeSame('i am deprecated');

		$stream->close();
	}

	public function testGetsFullContents(): void {
		$handle = \fopen('php://temp', 'w+');
		\fwrite($handle, 'data');
		$stream = new Stream($handle);

		expect($stream->getRemainingContents())->toBeSame('');

		$stream->seek(0);

		expect($stream->getRemainingContents())->toBeSame('data');
		expect($stream->getRemainingContents())->toBeSame('');

		$stream->close();
	}

	public function testGetRemainingContentsReturnsTheCompleteStream(): void {
		$handle = \fopen('php://temp', 'w+');
		\fwrite($handle, 'data');
		$stream = new Stream($handle);

		expect((string) $stream->getFullContents())->toBeSame('data');
		expect((string) $stream->getFullContents())->toBeSame('data');

		$stream->close();
	}

	public function testChecksEof(): void {
		$handle = \fopen('php://temp', 'w+');
		\fwrite($handle, 'data');
		$stream = new Stream($handle);

		expect($stream->tell())->toBeSame(4, 'Stream cursor already at the end');
		expect($stream->eof())->toBeFalse('Stream still not eof');
		expect($stream->read(1))->toBeSame('', 'Need to read one more byte to reach eof');
		expect($stream->eof())->toBeTrue();

		$stream->close();
	}

	public function testGetSize(): void{
		$size = \filesize(__FILE__);
		$handle = \fopen(__FILE__, 'r');
		$stream = new Stream($handle);

		expect($stream->getSize())->toBeSame($size);

		// Load from cache
		expect($stream->getSize())->toBeSame($size);

		$stream->close();
	}

	public function testEnsuresSizeIsConsistent(): void{
		$h = \fopen('php://temp', 'w+');

		expect(\fwrite($h, 'foo'))->toBeSame(3);

		$stream = new Stream($h);

		expect($stream->getSize())->toBeSame(3);
		expect($stream->write('test'))->toBeSame(4);
		expect($stream->getSize())->toBeSame(7);
		expect($stream->getSize())->toBeSame(7);

		$stream->close();
	}

	public function testProvidesStreamPosition(): void {
		$handle = \fopen('php://temp', 'w+');
		$stream = new Stream($handle);

		expect($stream->tell())->toBeSame(0);

		$stream->write('foo');

		expect($stream->tell())->toBeSame(3);

		$stream->seek(1);

		expect($stream->tell())->toBeSame(1);
		expect(\ftell($handle))->toBeSame($stream->tell());

		$stream->close();
	}

	public function testDetachStreamAndClearProperties(): void {
		$handle = \fopen('php://temp', 'r');
		$stream = new Stream($handle);

		expect($stream->detach())->toBeSame($handle);
		expect(\is_resource($handle))->toBeTrue('Stream is not closed');
		expect($stream->detach())->toBeNull();

		$this->assertStreamStateAfterClosedOrDetached($stream);

		$stream->close();
	}

	public function testCloseResourceAndClearProperties(): void {
		$handle = \fopen('php://temp', 'r');
		$stream = new Stream($handle);
		$stream->close();

		expect(\is_resource($handle))->toBeFalse();

		$this->assertStreamStateAfterClosedOrDetached($stream);
	}

	private function assertStreamStateAfterClosedOrDetached(Stream $stream): void {
		expect($stream->isReadable())->toBeFalse();
		expect($stream->isWritable())->toBeFalse();
		expect($stream->isSeekable())->toBeFalse();
		expect($stream->getSize())->toBeNull();
		expect($stream->getMetadata())->toBeSame(dict[]);

		$throws = function ($fn) {
			try {
				$fn();
			} catch (\Exception $e) {
				expect(Str\contains($e->getMessage(), 'Stream is detached'))->toBeTrue();

				return;
			}

			//$this->fail('Exception should be thrown after the stream is detached.');
		};

		$throws(function () use ($stream) { $stream->read(10); });
		$throws(function () use ($stream) { $stream->write('bar'); });
		$throws(function () use ($stream) { $stream->seek(10); });
		$throws(function () use ($stream) { $stream->tell(); });
		$throws(function () use ($stream) { $stream->eof(); });
		$throws(function () use ($stream) { $stream->getRemainingContents(); });
		$throws(function () use ($stream) { $stream->getFullContents(); });
	}

	public function testStreamReadingWithZeroLength(): void {
		$r = \fopen('php://temp', 'r');
		$stream = new Stream($r);

		expect($stream->read(0))->toBeSame('');

		$stream->close();
	}

	public function testStreamReadingWithNegativeLength(): void {
		$r = \fopen('php://temp', 'r');
		$stream = new Stream($r);

		expect(
			() ==> {
				try {
					$stream->read(-1);
				} catch (\Exception $e) {
					$stream->close();
					throw $e;
				}
			}
		)->toThrow(
			\RuntimeException::class,
			'Length parameter cannot be negative'
		);

		$stream->close();
	}

	public function testStreamReadingFreadError(): void {
		self::$has_fread_error = true;
		$r = \fopen('php://temp', 'r');
		$stream = new Stream($r);

		expect(
			() ==> {
				try {
					$stream->read(1);
				} catch (\Exception $e) {
					self::$has_fread_error = false;
					$stream->close();
					throw $e;
				}
			}
		)->toThrow(
			\RuntimeException::class,
			'Unable to read from stream'
		);

		self::$has_fread_error = false;
		$stream->close();
	}
}

function fread(?resource $handle, int $length): mixed {
	return StreamTest::$has_fread_error ? false : \fread($handle, $length);
}
