//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "imports/imports.typesH.h"
//silver_chain_scope_end
//ALL/fdefine.ownership.c




void private_BearsslHttps_free_considering_ownership(void **value,bool *owner);


void private_BearsslHttps_set_str_considering_ownership(
    char **dest,
     char *value,
    bool *owner,
    short ownership_mode
    );

//ALL/fdefine.extra.c


bool private_BearHttps_is_sanitize_key(const char *key,const char *sanitized,int sanitized_size);

char * private_BearHttps_format_vaarg(const char *expresion, va_list args);
//ALL/fdefine.str.c



long private_BearsslHttps_strlen(const char *str);
int private_BearsslHttp_strcmp(const char *str1,const char *str2);


bool private_BearsslHttps_startswith(const char *str,const char *prefix);
char * private_BearsslHttps_strdup(const char *str);

char * private_BearsslHttps_strcpy( char *dest,const char *str);
char * private_BearsslHttps_strndup(const char *str,int size);

int private_BearsslHttps_indexof_from_point(const char *str,char c,int start);

char  private_BearsslHttps_parse_char_to_lower(char c);
//_WIN32/fdefine.socket.c
#if defined(_WIN32) 

static int private_BearHttps_socket_set_nonblocking(int sockfd) ;

static int private_BearHttps_socket_set_blocking(int sockfd) ;

static int private_BearHttps_socket_check_connect_error(int sockfd) ;

static int private_BearHttps_socket_check_connect_in_progress(int ret) ;

#endif //__unix__&&!__EMSCRIPTEN__/network/fdefine.connect_host.c
#if defined(__unix__) && !defined(__EMSCRIPTEN__)


#if  (defined(BEARSSL_USSE_GET_ADDRINFO) || defined(BEARSSL_HTTPS_MOCK_CJSON))
static int private_BearHttps_connect_host(BearHttpsRequest *self, BearHttpsResponse *response, const char *host, int port);

#endif

#endif//__unix__&&!__EMSCRIPTEN__/network/fdefine.socket.c
#if defined(__unix__)  && !defined(__EMSCRIPTEN__)

static int private_BearHttps_socket_set_nonblocking(int sockfd) ;

static int private_BearHttps_socket_set_blocking(int sockfd) ;

static int private_BearHttps_socket_check_connect_error(int sockfd) ;

static int private_BearHttps_socket_check_connect_in_progress(int ret) ;

#endif //__EMSCRIPTEN__/request/fdefine.fetch.c
#if defined(__EMSCRIPTEN__) 
#include <unistd.h>
BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self);
#endif//__EMSCRIPTEN__/request/fdefine.request.c
#if defined(__EMSCRIPTEN__) 

BearHttpsRequest * newBearHttpsRequest_with_url_ownership_config(char *url,short url_ownership_mode);
#endif //__EMSCRIPTEN__/response/fdefine.response.c
#if defined(__EMSCRIPTEN__) 

BearHttpsResponse *private_newBearHttpsResponse();


void BearHttpsResponse_free(BearHttpsResponse *self);


#endif //__EMSCRIPTEN__/response/fdefine.read_body.c
#if defined(__EMSCRIPTEN__)

unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self) ;

#endif//ALL/request/fdefine.body_send.c





void private_BearHttpsRequest_free_body(BearHttpsRequest *self);

void BearHttpsRequest_send_any_with_ownership_control(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode);
void BearHttpsRequest_send_any(BearHttpsRequest *self,unsigned char *content, long size);

void BearHttpsRequest_send_body_str_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode);


void BearHttpsRequest_send_body_str(BearHttpsRequest *self, char *content);


#ifndef BEARSSL_HTTPS_MOCK_CJSON

void BearHttpsRequest_send_cJSON_with_ownership_control(BearHttpsRequest *self,cJSON *json,short ownership_mode);
void BearHttpsRequest_send_cJSON(BearHttpsRequest *self,cJSON *json);

cJSON * BearHttpsRequest_create_cJSONPayloadObject(BearHttpsRequest *self);


cJSON * BearHttpsRequest_create_cJSONPayloadArray(BearHttpsRequest *self);


#endif
//ALL/headers/fdefine.headers.c


private_BearHttpsHeaders *private_newBearHttpsHeaders();

void private_BearHttpsHeaders_add_keyval(private_BearHttpsHeaders *self, private_BearHttpsKeyVal *keyval);

void private_BearHttpsHeaders_free(private_BearHttpsHeaders *self);

private_BearHttpsKeyVal * private_BearHttpsHeaders_get_key_val_by_index(private_BearHttpsHeaders *self,int index);

//ALL/StringArray/fdefine.StringArray.c

struct privateBearHttpsStringArray * newprivateBearHttpsStringArray();

int privateBearHttpsStringArray_find_position( privateBearHttpsStringArray *self, const char *string);


void privateBearHttpsStringArray_set_value( privateBearHttpsStringArray *self, int index, const char *value);
void privateBearHttpsStringArray_append_getting_ownership( privateBearHttpsStringArray *self, char *string);

// Function prototypes
void privateBearHttpsStringArray_append( privateBearHttpsStringArray *self, const  char *string);

void privateBearHttpsStringArray_pop( privateBearHttpsStringArray *self, int position);

void privateBearHttpsStringArray_merge( privateBearHttpsStringArray *self,  privateBearHttpsStringArray *other);


void privateBearHttpsStringArray_represent( privateBearHttpsStringArray *self);




 privateBearHttpsStringArray * privateBearHttpsStringArray_clone(privateBearHttpsStringArray *self);

char * privateprivateBearHttpsStringArray_append_if_not_included(privateBearHttpsStringArray *self,char *value);
void privateBearHttpsStringArray_free(struct privateBearHttpsStringArray *self);
//ALL/response/fdefine.response.c

int BearHttpsResponse_get_status_code(BearHttpsResponse*self);

int BearHttpsResponse_get_body_size(BearHttpsResponse*self);

bool BearHttpsResponse_error(BearHttpsResponse*self);

void BearHttpsResponse_set_error(BearHttpsResponse*self,const char *msg,int error_code);

char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self);

int BearHttpsResponse_get_error_code(BearHttpsResponse*self);

int BearHttpsResponse_get_headers_size(BearHttpsResponse*self);

char* BearHttpsResponse_get_header_value_by_index(BearHttpsResponse*self,int index);
char* BearHttpsResponse_get_header_key_by_index(BearHttpsResponse*self,int index);
char* BearHttpsResponse_get_header_value_by_key(BearHttpsResponse*self,const char *key);

char* BearHttpsResponse_get_header_value_by_sanitized_key(BearHttpsResponse*self,const char *key);
//ALL/response/fdefine.body_read.c

const  char *BearHttpsResponse_read_body_str(BearHttpsResponse *self);
#ifndef BEARSSL_HTTPS_MOCK_CJSON
cJSON * BearHttpsResponse_read_body_json(BearHttpsResponse *self);
#endif//ALL/keyval/fdefine.keyval.c



private_BearHttpsKeyVal  *private_newBearHttpsKeyVal();

void private_BearHttpsKeyVal_set_key(private_BearHttpsKeyVal *self,  char *key,short key_onwership_mode);

void private_BearHttpsKeyVal_set_value(private_BearHttpsKeyVal *self,  char *value,short value_onwership_mode);

void  private_BearHttpsKeyVal_free(private_BearHttpsKeyVal *self);

//!__EMSCRIPTEN__/requisition_props/fdefine.requisition_props.c
#if !defined(__EMSCRIPTEN__)



private_BearHttpsRequisitionProps * private_new_private_BearHttpsRequisitionProps(const char *url,int default_port);

void private_BearHttpsRequisitionProps_free(private_BearHttpsRequisitionProps *self);

#endif//!__EMSCRIPTEN__/request/fdefine.fetch.c
#if !defined(__EMSCRIPTEN__)





BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self);

#endif//!__EMSCRIPTEN__/request/fdefine.body_send.c
#if !defined(__EMSCRIPTEN__)


void BearHttpsRequest_send_file_with_ownership_control(BearHttpsRequest *self, char *path,short ownership_mode,const char *content_type);

void BearHttpsRequest_send_file(BearHttpsRequest *self,const  char *path,const char *content_type);

void BearHttpsRequest_send_file_auto_detect_content_type(BearHttpsRequest *self,const  char *path);



#endif//!__EMSCRIPTEN__/request/fdefine.request.c
#if !defined(__EMSCRIPTEN__)



BearHttpsRequest * newBearHttpsRequest_with_url_ownership_config(char *url,short url_ownership_mode);

void BearHttpsRequest_set_known_ips(BearHttpsRequest *self , const char *known_ips[],int known_ips_size);


void BearHttpsRequest_set_max_redirections(BearHttpsRequest *self ,int max_redirections);

void BearHttpsRequest_set_dns_providers(BearHttpsRequest *self ,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers);

void BearHttpsRequest_set_chunk_header_read_props(BearHttpsRequest *self ,int chunk_size,int max_chunk_size);

void BearHttpsRequest_set_trusted_anchors(BearHttpsRequest *self ,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size);


#endif//!__EMSCRIPTEN__/namespace/fdefine.namespace.c
#if !defined(__EMSCRIPTEN__)



BearHttpsNamespace newBearHttpsNamespace();

#endif//!__EMSCRIPTEN__/response/fdefine.read_write.c
#if !defined(__EMSCRIPTEN__)




int private_BearHttpsResponse_write(BearHttpsResponse *self,unsigned char *bufer,long size);

int private_BearHttpsResponse_recv(BearHttpsResponse *self,unsigned char *buffer,long size);

#endif//!__EMSCRIPTEN__/response/fdefine.http_parser.c
#if !defined(__EMSCRIPTEN__)




void private_BearHttpsResponse_parse_headers(BearHttpsResponse *self,int headers_end);
void private_BearHttpsResponse_read_til_end_of_headers_or_reach_limit(
    BearHttpsResponse *self,
    int chunk_size,
    double factor_headers_growth
);

#endif//!__EMSCRIPTEN__/response/fdefine.body_read_chunk.c
#if !defined(__EMSCRIPTEN__)

int BearHttpsResponse_read_body_chunck_http1(BearHttpsResponse *self,unsigned char *buffer,long size);


int BearHttpsResponse_read_body_chunck_raw(BearHttpsResponse *self,unsigned char *buffer,long size);

int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *buffer,long size);
#endif//!__EMSCRIPTEN__/response/fdefine.body_read.c
#if !defined(__EMSCRIPTEN__)



unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self) ;


#endif//!__EMSCRIPTEN__/network/fdefine.sock.c
#if !defined(__EMSCRIPTEN__)


static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len)
;


static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len)
;

#endif//!__EMSCRIPTEN__/network/fdefine.connect_host.c
#if !defined(__EMSCRIPTEN__)


#if  (!defined(BEARSSL_USSE_GET_ADDRINFO) && !defined(BEARSSL_HTTPS_MOCK_CJSON))
static int private_BearHttps_connect_host(BearHttpsRequest *self, BearHttpsResponse *response, const char *host, int port);


#endif


#endif//!__EMSCRIPTEN__/network/fdefine.ipv4_connect.c
#if !defined(__EMSCRIPTEN__)

static int private_BearHttpsRequest_connect_ipv4(BearHttpsResponse *self, const char *ipv4_ip, int port,long connection_timeout) ;


static int private_BearHttpsRequest_connect_ipv4_no_error_raise( const char *ipv4_ip, int port,long connection_timeout) ;


#endif//ALL/request/request/fdefine.request.c



BearHttpsRequest * newBearHttpsRequest(const char *url);

BearHttpsRequest * newBearHttpsRequest_fmt(const char *url,...);

void BearHttpsRequest_set_url_with_ownership_config(BearHttpsRequest *self , char *url,short url_ownership_mode);

void BearHttpsRequest_set_url(BearHttpsRequest *self ,const char *url);


void BearHttpsRequest_add_header_with_ownership_config(BearHttpsRequest *self ,char *key,short key_ownership_mode,char *value,short value_owner);

void BearHttpsRequest_add_header(BearHttpsRequest *self ,const char *key,const char *value);

void BearHttpsRequest_add_header_fmt(BearHttpsRequest *self ,const char *key,const char *format,...);

void BearHttpsRequest_set_method(BearHttpsRequest *self ,const char *method);

void BearHttpsRequest_represent(BearHttpsRequest *self);



void BearHttpsRequest_free(BearHttpsRequest *self);
//!__EMSCRIPTEN__/namespace/request/fdefine.request.c
#if !defined(__EMSCRIPTEN__)





BearHttpsRequestNamespace newBearHttpsRequestNamespace();
#endif//!__EMSCRIPTEN__/namespace/response/fdefine.response.c
#if !defined(__EMSCRIPTEN__)



BearHttpsResponseNamespace newBearHttpsResponseNamespace();
#endif//!__EMSCRIPTEN__/response/response/fdefine.response.c
#if !defined(__EMSCRIPTEN__)




BearHttpsResponse *private_newBearHttpsResponse();




void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self, const char *hostname,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size) ;


void BearHttpsResponse_free(BearHttpsResponse *self);

void BearHttpsResponse_set_max_body_size(BearHttpsResponse*self,long size);

void BearHttpsResponse_set_body_read_props(BearHttpsResponse*self,int chunk_size,double realloc_factor);


#endif