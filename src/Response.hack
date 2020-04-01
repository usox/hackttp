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
use namespace HH\Lib\{C, IO};

class Response implements Message\ResponseInterface {

  use MessageTrait;

  private static dict<int, string> $phrases = dict[
    100 => 'Continue',
    101 => 'Switching Protocols',
    102 => 'Processing',
    200 => 'OK',
    201 => 'Created',
    202 => 'Accepted',
    203 => 'Non-Authoritative Information',
    204 => 'No Content',
    205 => 'Reset Content',
    206 => 'Partial Content',
    207 => 'Multi-status',
    208 => 'Already Reported',
    300 => 'Multiple Choices',
    301 => 'Moved Permanently',
    302 => 'Found',
    303 => 'See Other',
    304 => 'Not Modified',
    305 => 'Use Proxy',
    306 => 'Switch Proxy',
    307 => 'Temporary Redirect',
    400 => 'Bad Request',
    401 => 'Unauthorized',
    402 => 'Payment Required',
    403 => 'Forbidden',
    404 => 'Not Found',
    405 => 'Method Not Allowed',
    406 => 'Not Acceptable',
    407 => 'Proxy Authentication Required',
    408 => 'Request Time-out',
    409 => 'Conflict',
    410 => 'Gone',
    411 => 'Length Required',
    412 => 'Precondition Failed',
    413 => 'Request Entity Too Large',
    414 => 'Request-URI Too Large',
    415 => 'Unsupported Media Type',
    416 => 'Requested range not satisfiable',
    417 => 'Expectation Failed',
    418 => 'I\'m a teapot',
    422 => 'Unprocessable Entity',
    423 => 'Locked',
    424 => 'Failed Dependency',
    425 => 'Unordered Collection',
    426 => 'Upgrade Required',
    428 => 'Precondition Required',
    429 => 'Too Many Requests',
    431 => 'Request Header Fields Too Large',
    451 => 'Unavailable For Legal Reasons',
    500 => 'Internal Server Error',
    501 => 'Not Implemented',
    502 => 'Bad Gateway',
    503 => 'Service Unavailable',
    504 => 'Gateway Time-out',
    505 => 'HTTP Version not supported',
    506 => 'Variant Also Negotiates',
    507 => 'Insufficient Storage',
    508 => 'Loop Detected',
    511 => 'Network Authentication Required',
  ];

  private string $reason_phrase = '';

  public function __construct(
    private IO\WriteHandle $body,
    private int $status_code = 200,
    string $reason = '',
    dict<string, vec<string>> $headers = dict[],
    private string $protocol_version = '1.1',
  ) {
    $this->setHeaders($headers);

    $this->reason_phrase = $this->getReason($status_code, $reason);
  }

  public function getStatusCode(): int {
    return $this->status_code;
  }

  public function getReasonPhrase(): string {
    return $this->reason_phrase;
  }

  public function withStatus(int $code, string $reason_phrase = ''): this {
    $new = clone $this;
    $new->status_code = $code;
    $new->reason_phrase = $this->getReason($code, $reason_phrase);

    return $new;
  }

  public function getBody(): IO\WriteHandle {
    return $this->body;
  }

  public function withBody(
    IO\WriteHandle $body
  ): this {
    if ($body === $this->body) {
      return $this;
    }
    $new = clone $this;
    $new->body = $body;

    return $new;
  }

  private function getReason(int $code, string $reason_phrase): string {
    if ($reason_phrase === '' && C\contains_key(self::$phrases, $code)) {
      $reason_phrase = self::$phrases[$code];
    }
    return $reason_phrase;
  }
}
