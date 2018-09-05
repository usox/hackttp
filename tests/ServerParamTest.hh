<?hh // partial

namespace Usox\HackTTP;

use type Facebook\Experimental\Http\Message\HTTPMethod;

class ServerParamTest extends \PHPUnit_Framework_TestCase {

    private dict<string, mixed> $server_params = dict[
        'PHP_SELF' => 'php_self',
    ];

    /**
     * @dataProvider serverParamProvider
     */
    public function testServerParams(
        string $method_name,
        string $key,
        mixed $content,
    ): void {
        $instance = new ServerParam(dict[$key => $content]);

        $this->assertSame(
            $content,
            \call_user_func_array([$instance, $method_name], []),
        );
    }

    /**
     * @dataProvider serverParamProvider
     */
    public function testServerParamsWontFailCauseOfMissingKeys(
        string $method_name,
        string $key,
        mixed $content,
    ): void {
        $instance = new ServerParam(dict[]);

        \call_user_func_array([$instance, $method_name], []);
    }

    public function serverParamProvider() {
        return [
            ['getPhpSelf', 'PHP_SELF', 'php_self'],
            ['getArgC', 'argc', 666],
            [
                'getGatewayInterface',
                'GATEWAY_INTERFACE',
                'some-gateway-interface',
            ],
            ['getContentLength', 'CONTENT_LENGTH', 33],
            ['getContentType', 'CONTENT_TYPE', 'some-content-type'],
            ['getServerAddr', 'SERVER_ADDR', 'some-server-address'],
            ['getServerName', 'SERVER_NAME', 'some-sever-name'],
            ['getServerSoftware', 'SERVER_SOFTWARE', 'some-server-software'],
            ['getServerProtocol', 'SERVER_PROTOCOL', 'some-server-protocol'],
            ['getRequestTime', 'REQUEST_TIME', \time()],
            ['getRequestTimeFloat', 'REQUEST_TIME_FLOAT', \microtime(true)],
            ['getQueryString', 'QUERY_STRING', 'some-query-string'],
            ['getDocumentRoot', 'DOCUMENT_ROOT', 'some-document-root'],
            ['getHttpAccept', 'HTTP_ACCEPT', 'some-http-accept'],
            ['getHttpAcceptCharset', 'HTTP_ACCEPT_CHARSET', 'some-http-accept-charset'],
            ['getHttpAcceptEncoding', 'HTTP_ACCEPT_ENCODING', 'some-http-accept-enconding'],
            ['getHttpAcceptLanguage', 'HTTP_ACCEPT_LANGUAGE', 'some-http-accept-language'],
            ['getHttpConnection', 'HTTP_CONNECTION', 'some-http-connection'],
            ['getHttpHost', 'HTTP_HOST', 'some-http-host'],
            ['getHttpReferer', 'HTTP_REFERER', 'some-http-referer'],
            ['getHttpUserAgent', 'HTTP_USER_AGENT', 'some-http-user-agent'],
            ['getHttps', 'HTTPS', 'some-https'],
            ['getRemoteAddr', 'REMOTE_ADDR', 'some-remote-addr'],
            ['getRemoteHost', 'REMOTE_HOST', 'some-remote-host'],
            ['getRemotePort', 'REMOTE_PORT', 666],
            ['getRemoteUser', 'REMOTE_USER', 'some-remote-user'],
            ['getRedirectRemoteUser', 'REDIRECT_REMOTE_USER', 'some-redirect-remote-user'],
            ['getScriptFilename', 'SCRIPT_FILENAME', 'some-script-filename'],
            ['getServerAdmin', 'SERVER_ADMIN', 'some-server-admin'],
            ['getServerPort', 'SERVER_PORT', 42],
            ['getServerSignature', 'SERVER_SIGNATURE', 'some-server-signature'],
            ['getPathTranslated', 'PATH_TRANSLATED', 'some-path-translated'],
            ['getScriptName', 'SCRIPT_NAME', 'some-script-name'],
            ['getRequestUri', 'REQUEST_URI', 'some-request-uri'],
            ['getPhpAuthDigest', 'PHP_AUTH_DIGEST', 'some-php-auth-digest'],
            ['getPhpAuthUser', 'PHP_AUTH_USER', 'some-php-auth-user'],
            ['getPhpAuthPw', 'PHP_AUTH_PW', 'some-php-auth-pw'],
            ['getAuthType', 'AUTH_TYPE', 'some-auth-type'],
            ['getPathInfo', 'PATH_INFO', 'some-path-info'],
            ['getOrigPathInfo', 'ORIG_PATH_INFO', 'some-orig-path-info'],
        ];
    }

    public function testGetArgvReturnsVector() {
        $data = ['some-value'];

        $instance = new ServerParam(dict['argv' => $data]);

        $this->assertSame(
            vec($data),
            $instance->getArgV()
        );
    }

    public function testGetRequestMethodReturnsMethod() {
        $instance = new ServerParam(dict['REQUEST_METHOD' => HTTPMethod::GET]);

        $this->assertSame(
            HTTPMethod::GET,
            $instance->getRequestMethod()
        );
    }

    public function testGetRequestMethodFailsWithMissingKey() {
        $this->expectException(
            \UnexpectedValueException::class
        );
        $instance = new ServerParam(dict[]);

        $instance->getRequestMethod();
    }
}
