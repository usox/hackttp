/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP;

use type Facebook\Experimental\Http\Message\UriInterface;
use type Usox\HackHttpFactory\UriFactoryInterface;

final class UriFactory implements UriFactoryInterface {

  public function createUri(?string $uri = null): UriInterface {
    return new Uri($uri);
  }
}
