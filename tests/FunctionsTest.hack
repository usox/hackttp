/*
 *  Copyright (c) 2019-present, Daniel Jakob
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Usox\HackTTP;

use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;

class FunctionsTest extends HackTest {

  public function testCreateFromGlobalsReturnsPreparedRequestObj(): void {
    $uri = 'up/and.at?them';

    // Define all those (not so) fancy php super globals
    $_POST = darray['foo' => 'bar'];
    $_GET = darray['foo' => 'baz'];
    $_SERVER = darray[
      'REQUEST_URI' => $uri,
      'REQUEST_METHOD' => 'POST',
      'CONTENT_TYPE' => 'multipart/form-data',
    ];
    $_COOKIE = darray['some' => 'cookie'];
    $_FILES = darray['some-file' => darray['tmp_name' => '/dev/null', 'size' => 666, 'error' => 0]];

    $server_request = create_server_request_from_globals();

    expect($server_request->getParsedBody())
      ->toBeSame(dict($_POST));

    expect($server_request->getCookieParams())
      ->toBeSame(dict($_COOKIE));

    expect($server_request->getQueryParams())
      ->toBeSame(dict($_GET));

    expect($server_request->getUri()->toString())
      ->toBeSame($uri);

    expect($server_request->getUploadedFiles())
      ->toContainKey('some-file');
  }

  public function testCreateUriReturnsUriObject(): void {
    $uri = 'https://come-and.get.some';

    expect((new Uri($uri))->toString())
      ->toBeSame($uri);
  }

  public function testCreateResponseReturnsResponseObject(): void {
    $code = 666;
    $reason = 'number of the beast';

    $respone = create_response($code, $reason);

    expect($respone->getStatusCode())
      ->toBeSame($code);

    expect($respone->getReasonPhrase())
      ->toBeSame($reason);
  }
}
