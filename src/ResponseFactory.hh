<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\Experimental\Http\Message;

final class ResponseFactory implements Message\ResponseFactoryInterface {

    public function createResponse(
        int $code = 200,
        string $phrase = ''
    ): Message\ResponseInterface {
        return new Response(
            $code,
            dict[],
            null,
            '1.1',
            $phrase
        );
    }
}