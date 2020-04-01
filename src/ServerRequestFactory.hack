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
use type Usox\HackHttpFactory\ServerRequestFactoryInterface;

final class ServerRequestFactory implements ServerRequestFactoryInterface {

  public function createServerRequest(
    Message\HTTPMethod $method,
    Message\UriInterface $uri,
    dict<string, string> $server_params = dict[],
  ): Message\ServerRequestInterface {
    return new ServerRequest(
      $method,
      $uri,
      $server_params,
      IO\request_input()
    );
  }
}
