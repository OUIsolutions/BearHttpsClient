
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.typesD.h"
//silver_chain_scope_end
typedef struct BearHttpsResponseNamespace{


    int (*read_body_chunck)(BearHttpsResponse *self,unsigned char *bufer,long size);
    unsigned char *(*read_body)(BearHttpsResponse *self);
    const  char *(*read_body_str)(BearHttpsResponse *self);


    int (*get_status_code)(BearHttpsResponse*self);
    int (*get_body_size)(BearHttpsResponse*self);

    int (*get_headers_size)(BearHttpsResponse*self);

    char* (*get_header_value_by_index)(BearHttpsResponse*self,int index);

    char* (*get_header_value_by_key)(BearHttpsResponse*self,const char *key);

    char* (*get_header_key_by_index)(BearHttpsResponse*self,int index);

    char* (*get_header_value_by_sanitized_key)(BearHttpsResponse*self,const char *key);

    void (*set_max_body_size)(BearHttpsResponse*self,long size);
    void (*set_body_read_props)(BearHttpsResponse*self,int chunk_size,double realloc_factor);
    

    bool (*error)(BearHttpsResponse*self);

    char* (*get_error_msg)(BearHttpsResponse*self);

    int (*get_error_code)(BearHttpsResponse*self);

    void (*free)(BearHttpsResponse *self);


    #ifndef BEARSSL_HTTPS_MOCK_CJSON
    cJSON * (*read_body_json)(BearHttpsResponse *self);
    #endif
} BearHttpsResponseNamespace;
