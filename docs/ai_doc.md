# Installation 
single file mode 
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c -o BearHttpsClientOne.c
```
# Basic Usage
```c 
#include "BearHttpsClientOne.c"
// DO NOT include any other headers - they are already bundled in BearHttpsClient

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com/");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        printf("Response Status Code: %d\n", BearHttpsResponse_get_status_code(response));
        printf("Response Body: %s\n", BearHttpsResponse_read_body_str(response));
    }
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```
## Pre-Compiling the lib
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.c -o BearHttpsClient.c
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.h -o BearHttpsClient.h
gcc -c BearHttpsClient.c -o BearHttpsClient.o 
gcc main.c BearHttpsClient.o -o main.out
```
### include on Precompiled mode
```c
#include "BearHttpsClient.h"
// DO NOT include any other headers - they are already bundled in BearHttpsClient
```

### MinGW Cross-Compiler
```bash
i686-w64-mingw32-gcc main.c -o main.exe -lws2_32
./main.exe
```

# Public API

## IMPORTANT: All dependencies are already bundled - DO NOT include these headers manually:
```
❌ DO NOT INCLUDE:
- cJSON.h
- string.h  
- stdio.h
- stdint.h
- stdlib.h
- ctype.h
- arpa/inet.h
- netinet/in.h
- sys/socket.h
- sys/types.h
- netdb.h
- unistd.h
- errno.h
- winsock2.h
- ws2tcpip.h
- windows.h
```

**These are automatically included when you include BearHttpsClient.c, BearHttpsClient.h or BearHttpsClientOne.c**

### Ownership Modes

```c
BEARSSL_HTTPS_REFERENCE  = 0 // keeps the reference of the item passed to the function
BEARSSL_HTTPS_GET_OWNERSHIP = 1 // takes the ownership of the item passed to the function and free it when not needed anymore
BEARSSL_HTTPS_COPY  = 2 // makes a copy of the item passed to the function
BEARSSL_DEFAULT_STRATEGY = BEARSSL_HTTPS_COPY // default strategy for string parameters
```

### Dns Providers sample
```c
    BearHttpsClientDnsProvider providers[] = {
        {
            .hostname = "dns.google.com",
            .route = "/resolve", 
            .ip = "8.8.8.8",
            .port = 443
        },
        {
            .hostname = "dns.nextdns.io",
            .route = "/dns-query",
            .ip = "217.146.9.93", 
            .port = 443
        }
    };
```

###  Known IPs sample
```c 
const char *known_ips[] = {
    "93.184.216.34",    // Primary IP
    "93.184.216.35"     // Backup IP
};
int known_ips_count = sizeof(known_ips) / sizeof(const char*);
```

### Request Struct
```c 
typedef struct BearHttpsRequest BearHttpsRequest;
BearHttpsRequest * newBearHttpsRequest(const char *url);
// Headers
void BearHttpsRequest_add_header(BearHttpsRequest *self ,const char *key,const char *value);
void BearHttpsRequest_add_header_fmt(BearHttpsRequest *self ,const char *key,const char *format,...);

BearHttpsRequest * newBearHttpsRequest_with_url_ownership_config(char *url,short url_ownership_mode);
void BearHttpsRequest_set_known_ips(BearHttpsRequest *self , const char *known_ips[],int known_ips_size);
void BearHttpsRequest_set_max_redirections(BearHttpsRequest *self ,int max_redirections);
void BearHttpsRequest_set_dns_providers(BearHttpsRequest *self ,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers);
void BearHttpsRequest_set_chunk_header_read_props(BearHttpsRequest *self ,int chunk_size,int max_chunk_size);
void BearHttpsRequest_set_trusted_anchors(BearHttpsRequest *self ,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size);
// Body Send 

void BearHttpsRequest_send_any_with_ownership_control(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode);
void BearHttpsRequest_send_any(BearHttpsRequest *self,unsigned char *content, long size);
void BearHttpsRequest_send_body_str_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode);
void BearHttpsRequest_send_body_str(BearHttpsRequest *self, char *content);
//memory of request

void BearHttpsRequest_send_cJSON_with_ownership_control(BearHttpsRequest *self,cJSON *json,short ownership_mode);
void BearHttpsRequest_send_cJSON(BearHttpsRequest *self,cJSON *json);
const cJSON * BearHttpsRequest_create_cJSONPayloadObject(BearHttpsRequest *self);
const cJSON * BearHttpsRequest_create_cJSONPayloadArray(BearHttpsRequest *self);
BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self);

void BearHttpsRequest_free(BearHttpsRequest *self);
```

### Response Struct
```c
typedef struct BearHttpsResponse BearHttpsResponse;
BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self);
int BearHttpsResponse_get_status_code(BearHttpsResponse*self);
int BearHttpsResponse_get_body_size(BearHttpsResponse*self);
bool BearHttpsResponse_error(BearHttpsResponse*self);
const char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self);
int BearHttpsResponse_get_error_code(BearHttpsResponse*self);
int BearHttpsResponse_get_headers_size(BearHttpsResponse*self);
const char* BearHttpsResponse_get_header_value_by_index(BearHttpsResponse*self,int index);
const char* BearHttpsResponse_get_header_key_by_index(BearHttpsResponse*self,int index);
const char* BearHttpsResponse_get_header_value_by_key(BearHttpsResponse*self,const char *key);
const char* BearHttpsResponse_get_header_value_by_sanitized_key(BearHttpsResponse*self,const char *key);
const char* BearHttpsResponse_read_body_str(BearHttpsResponse *self);
const cJSON * BearHttpsResponse_read_body_json(BearHttpsResponse *self);
```