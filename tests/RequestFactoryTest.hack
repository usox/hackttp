/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect};

class RequestFactoryTest extends HackTest {

  public function testCreateRequestReturnsInstance(): void {
    $factory = new RequestFactory();

    $uri = mock(Message\UriInterface::class);

    prospect($uri, 'getHost')
      ->once()
      ->andReturn(null);

    expect($factory->createRequest(Message\HTTPMethod::GET, $uri))
      ->toBeInstanceOf(Message\RequestInterface::class);
  }
}
