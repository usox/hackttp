/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP;

use namespace HH\Lib\IO;
use namespace Facebook\Experimental\Http\Message;
use type Usox\HackHttpFactory\ResponseFactoryInterface;

final class ResponseFactory implements ResponseFactoryInterface {

  public function createResponse(
    int $code = 200,
    string $phrase = '',
  ): Message\ResponseInterface {
    return new Response(
      IO\request_output(),
      $code,
      $phrase
    );
  }
}
