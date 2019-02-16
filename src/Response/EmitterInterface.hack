/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP\Response;

use type Facebook\Experimental\Http\Message\ResponseInterface;

interface EmitterInterface {

	public function emitAsync(ResponseInterface $response): Awaitable<void>;
}
