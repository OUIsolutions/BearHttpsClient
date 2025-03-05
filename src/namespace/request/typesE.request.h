
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.typesD.h"
//silver_chain_scope_end
typedef struct BearHttpsRequestNamespace{

    void (*send_any_with_ownership_control)(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode);
    void (*send_any)(BearHttpsRequest *self,unsigned char *content, long size);
    void (*send_body_str_with_ownership_control)(BearHttpsRequest *self, char *content,short ownership_mode);
    void (*send_body_str)(BearHttpsRequest *self, char *content);
    void (*send_file_with_ownership_control)(BearHttpsRequest *self, char *path,short ownership_mode,const char *content_type);
    void (*send_file)(BearHttpsRequest *self,const  char *path,const char *content_type);
    void (*send_file_auto_detect_content_type)(BearHttpsRequest *self, const char *path);

    BearHttpsResponse * (*fetch)(BearHttpsRequest *self);

    #ifndef BEARSSL_HTTPS_MOCK_CJSON
        void (*send_cJSON_with_ownership_control)(BearHttpsRequest *self,cJSON *json,short ownership_mode);
        void (*send_cJSON)(BearHttpsRequest *self,cJSON *json);
        cJSON * (*create_cJSONPayloadObject)(BearHttpsRequest *self);
        cJSON * (*create_cJSONPayloadArray)(BearHttpsRequest *self);
    #endif

    BearHttpsRequest * (*newBearHttpsRequest_with_url_ownership_config)(char *url,short url_ownership_mode);
    BearHttpsRequest * (*newBearHttpsRequest)(const char *url);
    BearHttpsRequest * (*newBearHttpsRequest_fmt)(const char *url,...);
    void (*set_url_with_ownership_config)(BearHttpsRequest *self , char *url,short url_ownership_mode);
    void (*set_url)(BearHttpsRequest *self ,const char *url);
    void (*add_header_with_ownership_config)(BearHttpsRequest *self ,char *key,short key_ownership_mode,char *value,short value_owner);
    void (*add_header)(BearHttpsRequest *self ,char *key,char *value);
    void (*set_method)(BearHttpsRequest *self ,const char *method);
    void (*set_max_redirections)(BearHttpsRequest *self ,int max_redirections);
void (*set_dns_providers)(BearHttpsRequest *self ,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers);
    void (*set_chunk_header_read_props)(BearHttpsRequest *self ,int chunk_size,int max_chunk_size);
    void (*set_trusted_anchors)(BearHttpsRequest *self ,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size);
    void (*represent)(BearHttpsRequest *self);
    void (*free)(BearHttpsRequest *self);


} BearHttpsRequestNamespace;
