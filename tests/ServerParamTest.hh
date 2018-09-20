<?hh // strict

namespace Usox\HackTTP;

use type Facebook\Experimental\Http\Message\HTTPMethod;
use function Facebook\FBExpect\expect;

class ServerParamTest extends \Facebook\HackTest\HackTest {

    private dict<string, mixed> $server_params = dict[
        'PHP_SELF' => 'php_self',
    ];

    <<DataProvider('serverParamProvider')>>
    public function testServerParams(
        string $method_name,
        string $key,
        mixed $content,
    ): void {
        $instance = new ServerParam(dict[$key => $content]);

        expect(
            \call_user_func_array([$instance, $method_name], [])
        )->toBeSame($content);
    }

    <<DataProvider('serverParamProvider')>>
    public function testServerParamsWontFailCauseOfMissingKeys(
        string $method_name,
        string $key,
        mixed $content,
    ): void {
        $instance = new ServerParam(dict[]);

        \call_user_func_array([$instance, $method_name], []);
    }

    public function serverParamProvider(): vec<vec<mixed>> {
        return vec[
            vec['getPhpSelf', 'PHP_SELF', 'php_self'],
            vec['getArgC', 'argc', 666],
            vec[
                'getGatewayInterface',
                'GATEWAY_INTERFACE',
                'some-gateway-interface',
            ],
            vec['getContentLength', 'CONTENT_LENGTH', 33],
            vec['getContentType', 'CONTENT_TYPE', 'some-content-type'],
            vec['getServerAddr', 'SERVER_ADDR', 'some-server-address'],
            vec['getServerName', 'SERVER_NAME', 'some-sever-name'],
            vec['getServerSoftware', 'SERVER_SOFTWARE', 'some-server-software'],
            vec['getServerProtocol', 'SERVER_PROTOCOL', 'some-server-protocol'],
            vec['getRequestTime', 'REQUEST_TIME', \time()],
            vec['getRequestTimeFloat', 'REQUEST_TIME_FLOAT', \microtime(true)],
            vec['getQueryString', 'QUERY_STRING', 'some-query-string'],
            vec['getDocumentRoot', 'DOCUMENT_ROOT', 'some-document-root'],
            vec['getHttpAccept', 'HTTP_ACCEPT', 'some-http-accept'],
            vec['getHttpAcceptCharset', 'HTTP_ACCEPT_CHARSET', 'some-http-accept-charset'],
            vec['getHttpAcceptEncoding', 'HTTP_ACCEPT_ENCODING', 'some-http-accept-enconding'],
            vec['getHttpAcceptLanguage', 'HTTP_ACCEPT_LANGUAGE', 'some-http-accept-language'],
            vec['getHttpConnection', 'HTTP_CONNECTION', 'some-http-connection'],
            vec['getHttpHost', 'HTTP_HOST', 'some-http-host'],
            vec['getHttpReferer', 'HTTP_REFERER', 'some-http-referer'],
            vec['getHttpUserAgent', 'HTTP_USER_AGENT', 'some-http-user-agent'],
            vec['getHttps', 'HTTPS', 'some-https'],
            vec['getRemoteAddr', 'REMOTE_ADDR', 'some-remote-addr'],
            vec['getRemoteHost', 'REMOTE_HOST', 'some-remote-host'],
            vec['getRemotePort', 'REMOTE_PORT', 666],
            vec['getRemoteUser', 'REMOTE_USER', 'some-remote-user'],
            vec['getRedirectRemoteUser', 'REDIRECT_REMOTE_USER', 'some-redirect-remote-user'],
            vec['getScriptFilename', 'SCRIPT_FILENAME', 'some-script-filename'],
            vec['getServerAdmin', 'SERVER_ADMIN', 'some-server-admin'],
            vec['getServerPort', 'SERVER_PORT', 42],
            vec['getServerSignature', 'SERVER_SIGNATURE', 'some-server-signature'],
            vec['getPathTranslated', 'PATH_TRANSLATED', 'some-path-translated'],
            vec['getScriptName', 'SCRIPT_NAME', 'some-script-name'],
            vec['getRequestUri', 'REQUEST_URI', 'some-request-uri'],
            vec['getPhpAuthDigest', 'PHP_AUTH_DIGEST', 'some-php-auth-digest'],
            vec['getPhpAuthUser', 'PHP_AUTH_USER', 'some-php-auth-user'],
            vec['getPhpAuthPw', 'PHP_AUTH_PW', 'some-php-auth-pw'],
            vec['getAuthType', 'AUTH_TYPE', 'some-auth-type'],
            vec['getPathInfo', 'PATH_INFO', 'some-path-info'],
            vec['getOrigPathInfo', 'ORIG_PATH_INFO', 'some-orig-path-info'],
        ];
    }

    public function testGetArgvReturnsVector(): void {
        $data = ['some-value'];

        $instance = new ServerParam(dict['argv' => $data]);

        expect($instance->getArgV())->toBeSame(vec($data));
    }

    public function testGetRequestMethodReturnsMethod(): void {
        $instance = new ServerParam(dict['REQUEST_METHOD' => HTTPMethod::GET]);

        expect($instance->getRequestMethod())->toBeSame(HTTPMethod::GET);
    }

    public function testGetRequestMethodFailsWithMissingKey(): void {
        $instance = new ServerParam(dict[]);

        expect(
            () ==> $instance->getRequestMethod()
        )->toThrow(\UnexpectedValueException::class);
    }
}
