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
use namespace HH\Lib\Experimental\IO;
use function Usox\HackMock\mock;

class ResponseTest extends HackTest {

  public function testGetStatusCodeReturnsCode(): void {
    $code = 666;

    $response = new Response(
      mock(IO\WriteHandle::class),
      $code
    );

    expect($response->getStatusCode())
      ->toBeSame($code);
  }

  public function testGetReasonPhraseReturnsSetReason(): void {
    $code = 666;
    $phrase = 'some-reason';

    $response = new Response(
      mock(IO\WriteHandle::class),
      $code,
      $phrase
    );

    expect($response->getReasonPhrase())
      ->toBeSame($phrase);
  }

  public function testGetReasonPhraseReturnsDefaultPhraseForCode(): void {
    $response = new Response(
      mock(IO\WriteHandle::class),
      418
    );

    expect($response->getReasonPhrase())
      ->toBeSame('I\'m a teapot');
  }

  public function testWithStatusReturnsInstanceWithStatus(): void {
    $code = 200;

    expect((new Response(mock(IO\WriteHandle::class)))->withStatus($code)->getStatusCode())
      ->toBeSame($code);
  }

  public function testGetHeadersReturnsSetHeaders(): void {
    $header = 'some-header';
    $header_value = vec['some-header-value'];

    $response = new Response(
      mock(IO\WriteHandle::class),
      200,
      'some-readson',
      dict[$header => $header_value]
    );

    expect($response->getHeaders())
      ->toBeSame(dict[
        $header => $header_value,
      ]);
  }

  public function testGetBodyReturnsSetBody(): void {
    $body = mock(IO\WriteHandle::class);

    expect((new Response($body, 200, 'some-reason', dict[]))->getBody())
      ->toBeSame($body);
  }
}
