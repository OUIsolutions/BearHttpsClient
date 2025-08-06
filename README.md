# WARNING: THIS IS ALPHA SOFTWARE. USE AT YOUR OWN RISK. NO WARRANTY IS PROVIDED AND BUGS ARE EXPECTED.

# BearHttpsClient

BearHttpsClient is a simple and easy to use single-file HTTPS Client Library for C/C++ for Windows and Linux that uses [BearSSL](https://bearssl.org/) as its SSL/TLS backend. BearHttpsClient is projected to be a full out-of-the-box solution providing:

- HTTP Requisitions
- HTTPS Requisitions
- Single-file include or multiple-files header option
- File Upload
- File Download
- JSON Body Parsing
- Streaming Body Reading


# Releases

| item          | What Is |
|-------        |-----------|
| [BearSSLSingleUnit.zip](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.2.8/BearHttpsClient.zip)| Full Folder Mode  |
| [BearSSLSingleUnitOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.2.8/BearHttpsClientOne.c)| Single File Version|
| [BearHttpsClient.h](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.2.8/BearHttpsClient.h)|Declaration |
| [BearHttpsClient.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.2.8/BearHttpsClient.c)|Definition |


# Simple Example

The simplest example of using BearHttpsClient -- with the single file include [BearSSLSingleUnitOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.2.8/BearHttpsClientOne.c) -- is this example that fetches a page from example.com and prints it to the console.

```c
#include "BearHttpsClientOne.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    const char *body = BearHttpsResponse_read_body_str(response);
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    printf("Body: %s\n", body);
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

# Documentation

| Item                                      | What It Is                                                 |
|-------------------------------------------|------------------------------------------------------------|
| [Build](/docs/build_and_install.md)       | Build and Install                                          |
| [Request Usage](/docs/request.md)         | Request Usage such as setting headers or uploading content |
| [Response Usage](/docs/response.md)       | Response Headers and Body Read                             |
| [Network](/docs/network_configuration.md) | Lib network Configuration                                  |
| [WebAsm](/docs/web_asm.md)                | how to work with web assembly                              |
| [Memory](/docs/memory_and_limits.md)      | Memory Usage and Limits                                    |
| [Ownership](/docs/ownership_system.md)    | Ownership Mode                                             |
| [Dependencies](/docs/dependencies.md)     | Dependencies                                               |

# Build Toolchain

* [Darwin](https://github.com/OUIsolutions/Darwin)
* [Silver Chain](https://github.com/OUIsolutions/SilverChain)
* [CAmalgamator](https://github.com/OUIsolutions/CAmalgamator)
* [BearSSL](https://bearssl.org/)
