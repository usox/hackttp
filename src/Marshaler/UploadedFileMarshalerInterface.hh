<?hh // strict

namespace Usox\HackTTP\Marshaler;

use Facebook\Experimental\Http\Message;

interface UploadedFileMarshalerInterface {

	public static function batchMarshalPhpFilesArray(): dict<string, Message\UploadedFileInterface>;

	public function marshal(dict<string, mixed> $files): dict<string, Message\UploadedFileInterface>;
}