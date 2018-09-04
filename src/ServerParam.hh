<?hh // strict

namespace Usox\HackTTP;

final class ServerParam extends AbstractServerParam {

    public function __construct(
        protected dict<string, mixed> $params
    ): void {

    }
}