[![Build Status](https://travis-ci.org/usox/hackttp.svg?branch=master)](https://travis-ci.org/usox/hackttp)

# HackTTP #

This is a (currently unstable) implementation of hack-http-request-response-interfaces to allow type-safe access to http requests.

## Usage ##

Creating a server request object containing the request data:

```Hack
  $server_request = \Usox\HackTTP\createServerRequestFromGlobals();
```

Creating a response:

```Hack
  $response = \Usox\HackTTP\createResponse(200, 'Up and at them');
```

Creating an uri:

```Hack
  $uri = \Usox\HackTTP\createUri('https://some-uri.com');
```

HackTTP also provides a set of factories to create the various classes. Those factories implement the hack-http-factory interfaces.

### Inspired by guzzles psr7 implementation ###
