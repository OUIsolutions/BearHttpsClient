# WARNING: THIS IS ALPHA SOFTWARE. USE AT YOUR OWN RISK. NO WARRANTY IS PROVIDED AND BUGS ARE EXPECTED.

# BearHttpsClient

BearHttpsClient is a simple and easy to use single-file HTTPS Client Library for C/C++ for Windows and Linux that uses [BearSSL](https://bearssl.org/) as its SSL/TLS backend. BearHttpsClient is projected to be a full out-of-the-box solution providing:

- HTTP Requisitions
- HTTPS Requisitions
- Single-file headers or multiple-file headers setting
- File Upload
- File Download
- JSON Body Parsing
- Streaming Body Reading


# Releases

| item          | What Is |
|-------        |-----------|
| [BearSSLSingleUnit.zip](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.002/BearHttpsClient.zip)| Full Folder Mode  |
| [BearSSLSingleUnitOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.002/BearHttpsClientOne.c)| Single File Version|
| [BearHttpsClient.h](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.002/BearHttpsClient.h)|Declaration |
| [BearHttpsClient.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.002/BearHttpsClient.c)|Definition |


# Simple Example

The simplest example of using BearHttpsClient -- with the single file include [BearSSLSingleUnitOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.002/BearHttpsClientOne.c) -- is this example that fetches a page from example.com and prints it to the console.

```c
#include "BearHttpsClientOne.c"

BearHttpsNamespace bear ;
int main(){
    bear = newBearHttpsNamespace();

    BearHttpsRequest *request = bear.request.newBearHttpsRequest("https://example.com");   
    BearHttpsResponse *response = bear.request.fetch(request);
    if(bear.response.error(response)){
        printf("Error: %s\n",bear.response.get_error_msg(response));
        bear.request.free(request);
        bear.response.free(response);
        return 1;
    }
    const char *body = bear.response.read_body_str(response);
    if(bear.response.error(response)){
            printf("Error: %s\n",bear.response.get_error_msg(response));
            bear.request.free(request);
            bear.response.free(response); 
                        return 1;

    }
    printf("Body: %s\n",body);
    bear.request.free(request);
    bear.response.free(response);
    return 0;
}
```

# Documentation

| item          | What Is |
|-------        |-----------|
| [Build](/docs/build_and_install.md)|Build and Install |
| [Request Usage](/docs/request.md)|Request Usage such as setting headers or uploading content|
| [Response Usage](/docs/response.md)|Response Headers and Body Read |
| [Network](/docs/network_configuration.md)|Lib network Configuration |
| [Memory](/docs/memoryy_and_limits.md)|Memory Usage and Limits|
| [Ownership](/docs/ownership_system.md)|OwnerShip Mode |
| [Dependencies](/docs/dependencies.md)|Dependencies |

# Build Toolchain

* [Silver Chain](https://github.com/OUIsolutions/SilverChain)
