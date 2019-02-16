/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP\Marshaler;

use namespace Facebook\Experimental\Http\Message;

interface UploadedFileMarshalerInterface {

	public static function batchMarshalPhpFilesArray(): dict<string, Message\UploadedFileInterface>;

	public function marshal(dict<string, mixed> $files): dict<string, Message\UploadedFileInterface>;
}
