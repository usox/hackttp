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
use type Usox\HackTTP\UploadedFile;
use namespace HH\Lib\{C, File, Str};

type UploadedFileType = shape(
	'tmp_name' => string,
	'size' => ?int,
	'error' => int,
	?'type' => ?string,
	?'name' => ?string,
	...
);

final class UploadedFileMarshaler implements UploadedFileMarshalerInterface {

	/**
	 * Convenience method to use old style $_FILES global
	 */
	public static function batchMarshalPhpFilesArray(): dict<string, Message\UploadedFileInterface> {
		/* HH_FIXME[2050] */
		$files = dict($_FILES);

		$marshaler = new UploadedFileMarshaler();
		return $marshaler->marshal($files);
	}

	/**
	 * Creates an UploadedFiles instance for every file in the request. If some array-like magic is used,
	 * the key will be appended in the dict key, e.g. `somefile[]` will become `somefile[0]`
	 */
	public function marshal(dict<string, mixed> $files): dict<string, Message\UploadedFileInterface> {
		$result = dict[];
		foreach ($files as $index_name => $file_entry) {
			if (!$file_entry is KeyedContainer<_, _>) {
				continue;
			}

			if ($file_entry is UploadedFileType) {
				$result[$index_name] = $this->createUploadedFile($file_entry);
			} else {
				$file_count = C\count($file_entry['tmp_name'] as Container<_>);
				for ($i = 0; $i < $file_count; $i++) {
					$key = Str\format('%s[%d]', $index_name, $i);
					$result[$key] = $this->createUploadedFile(
						shape(
							'tmp_name' => $file_entry['tmp_name'] as KeyedContainer<_, _>[$i],
							'size' => $file_entry['size'] as KeyedContainer<_, _>[$i],
							'error' => $file_entry['error'] as KeyedContainer<_, _>[$i],
							'type' => $file_entry['type'] as KeyedContainer<_, _>[$i] ?? null,
							'name' => $file_entry['name'] as KeyedContainer<_, _>[$i] ?? null,
						) as UploadedFileType,
					);
				}
			}
		}

		return $result;
	}

	private function createUploadedFile(UploadedFileType $file): Message\UploadedFileInterface {
		$error = $file['error'] === 0 ? null : Message\UploadedFileError::assert($file['error']);

		return new UploadedFile(
			File\open_read_only($file['tmp_name']),
			$file['size'],
			$error,
			$file['name'] ?? '',
			$file['type'] ?? ''
		);
	}
}
