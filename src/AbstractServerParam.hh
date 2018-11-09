<?hh // strict

namespace Usox\HackTTP;

use namespace Facebook\TypeSpec;
use type Facebook\Experimental\Http\Message\HTTPMethod;
use type Facebook\Experimental\Http\Message\ServerParamInterface;

abstract class AbstractServerParam {

    protected dict<string, mixed> $params = dict[];

    public function getPhpSelf(): string {
        return (string)($this->params['PHP_SELF'] ?? '');
    }

    public function getArgC(): int {
        return (int)($this->params['argc'] ?? 0);
    }

    public function getArgV(): vec<string> {
        $params = $this->params['argv'] ?? [];
        invariant(\is_array($params), 'argv must be traversable');

        $spec = TypeSpec\vec(
            TypeSpec\string()
        );
        return $spec->assertType(vec($params));
    }

    public function getContentLength(): int {
        return (int)($this->params['CONTENT_LENGTH'] ?? 0);
    }

    public function getContentType(): string {
        return (string)($this->params['CONTENT_TYPE'] ?? '');
    }

    public function getGatewayInterface(): string {
        return (string)($this->params['GATEWAY_INTERFACE'] ?? '');
    }

    public function getServerAddr(): string {
        return (string)($this->params['SERVER_ADDR'] ?? '');
    }

    public function getServerName(): string {
        return (string)($this->params['SERVER_NAME'] ?? '');
    }

    public function getServerSoftware(): string {
        return (string)($this->params['SERVER_SOFTWARE'] ?? '');
    }

    public function getServerProtocol(): string {
        return (string)($this->params['SERVER_PROTOCOL'] ?? '');
    }

    public function getRequestMethod(): HTTPMethod {
        return HTTPMethod::assert($this->params['REQUEST_METHOD'] ?? '');
    }

    public function getRequestTime(): int {
        return (int)($this->params['REQUEST_TIME'] ?? 0);
    }

    public function getRequestTimeFloat(): float {
        return (float)($this->params['REQUEST_TIME_FLOAT'] ?? 0.0);
    }

    public function getQueryString(): string {
        return (string)($this->params['QUERY_STRING'] ?? '');
    }

    public function getDocumentRoot(): string {
        return (string)($this->params['DOCUMENT_ROOT'] ?? '');
    }

    public function getHttpAccept(): string {
        return (string)($this->params['HTTP_ACCEPT'] ?? '');
    }

    public function getHttpAcceptCharset(): string {
        return (string)($this->params['HTTP_ACCEPT_CHARSET'] ?? '');
    }

    public function getHttpAcceptEncoding(): string {
        return (string)($this->params['HTTP_ACCEPT_ENCODING'] ?? '');
    }

    public function getHttpAcceptLanguage(): string {
        return (string)($this->params['HTTP_ACCEPT_LANGUAGE'] ?? '');
    }

    public function getHttpConnection(): string {
        return (string)($this->params['HTTP_CONNECTION'] ?? '');
    }

    public function getHttpHost(): string {
        return (string)($this->params['HTTP_HOST'] ?? '');
    }

    public function getHttpReferer(): string {
        return (string)($this->params['HTTP_REFERER'] ?? '');
    }

    public function getHttpUserAgent(): string {
        return (string)($this->params['HTTP_USER_AGENT'] ?? '');
    }

    public function getHttps(): string {
        return (string)($this->params['HTTPS'] ?? '');
    }

    public function getRemoteAddr(): string {
        return (string)($this->params['REMOTE_ADDR'] ?? '');
    }

    public function getRemoteHost(): string {
        return (string)($this->params['REMOTE_HOST'] ?? '');
    }

    public function getRemotePort(): int {
        return (int)($this->params['REMOTE_PORT'] ?? 0);
    }

    public function getRemoteUser(): string {
        return (string)($this->params['REMOTE_USER'] ?? '');
    }

    public function getRedirectRemoteUser(): string {
        return (string)($this->params['REDIRECT_REMOTE_USER'] ?? '');
    }

    public function getScriptFilename(): string {
        return (string)($this->params['SCRIPT_FILENAME'] ?? '');
    }

    public function getServerAdmin(): string {
        return (string)($this->params['SERVER_ADMIN'] ?? '');
    }

    public function getServerPort(): int {
        return (int)($this->params['SERVER_PORT'] ?? 0);
    }

    public function getServerSignature(): string {
        return (string)($this->params['SERVER_SIGNATURE'] ?? '');
    }

    public function getPathTranslated(): string {
        return (string)($this->params['PATH_TRANSLATED'] ?? '');
    }

    public function getScriptName(): string {
        return (string)($this->params['SCRIPT_NAME'] ?? '');
    }

    public function getRequestUri(): string {
        return (string)($this->params['REQUEST_URI'] ?? '');
    }

    public function getPhpAuthDigest(): string {
        return (string)($this->params['PHP_AUTH_DIGEST'] ?? '');
    }

    public function getPhpAuthUser(): string {
        return (string)($this->params['PHP_AUTH_USER'] ?? '');
    }

    public function getPhpAuthPw(): string {
        return (string)($this->params['PHP_AUTH_PW'] ?? '');
    }

    public function getAuthType(): string {
        return (string)($this->params['AUTH_TYPE'] ?? '');
    }

    public function getPathInfo(): string {
        return (string)($this->params['PATH_INFO'] ?? '');
    }

    public function getOrigPathInfo(): string {
        return (string)($this->params['ORIG_PATH_INFO'] ?? '');
    }
}
