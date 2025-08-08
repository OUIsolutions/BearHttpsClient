

# Public API

### OOwnership Modes

~~~c
#define BEARSSL_HTTPS_REFERENCE  0
#define BEARSSL_HTTPS_GET_OWNERSHIP 1
#define BEARSSL_HTTPS_COPY  2

#define BEARSSL_DEFAULT_STRATEGY BEARSSL_HTTPS_COPY
~~~

### Request Struct
~~~c 
typedef struct BearHttpsRequest BearHttpsRequest;
BearHttpsRequest * newBearHttpsRequest(const char *url);
// Headders
void BearHttpsRequest_add_header(BearHttpsRequest *self ,const char *key,const char *value);
void BearHttpsRequest_add_header_fmt(BearHttpsRequest *self ,const char *key,const char *format,...);
// Body Send 

void BearHttpsRequest_send_any_with_ownership_control(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode);
void BearHttpsRequest_send_any(BearHttpsRequest *self,unsigned char *content, long size);
void BearHttpsRequest_send_body_str_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode);
void BearHttpsRequest_send_body_str(BearHttpsRequest *self, char *content);
//memory of request

void BearHttpsRequest_send_cJSON_with_ownership_control(BearHttpsRequest *self,cJSON *json,short ownership_mode);
void BearHttpsRequest_send_cJSON(BearHttpsRequest *self,cJSON *json);

// dont use cJSON_Delete on the return of these function, since they are managed by the BearHttpsRequest struct
const cJSON * BearHttpsRequest_create_cJSONPayloadObject(BearHttpsRequest *self);

// dont use cJSON_Delete on the return of these functions, since they are managed by the BearHttpsRequest struct
const cJSON * BearHttpsRequest_create_cJSONPayloadArray(BearHttpsRequest *self);

BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self);


void BearHttpsRequest_free(BearHttpsRequest *self);
~~~

### Response Struct
~~~c
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
cJSON * BearHttpsResponse_read_body_json(BearHttpsResponse *self);
~~~