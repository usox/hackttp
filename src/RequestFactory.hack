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
use type Usox\HackHttpFactory\RequestFactoryInterface;

final class RequestFactory implements RequestFactoryInterface {

  public function createRequest(
    Message\HTTPMethod $method,
    Message\UriInterface $uri,
  ): Message\RequestInterface {
    return new Request(
      $method,
      $uri,
      IO\request_input(),
      dict[]
    );
  }
}
