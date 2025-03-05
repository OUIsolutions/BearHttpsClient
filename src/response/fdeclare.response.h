
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.consts.h"
//silver_chain_scope_end

BearHttpsResponse *private_newBearHttpsResponse();

int private_BearHttpsResponse_read_chunck_raw(BearHttpsResponse *self,unsigned char *bufer,long size);

int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *bufer,long size);

unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self);

const  char *BearHttpsResponse_read_body_str(BearHttpsResponse *self);

int private_BearHttpsResponse_write(BearHttpsResponse *self,unsigned char *bufer,long size);

void private_BearHttpsResponse_read_til_end_of_headers_or_reach_limit(
    BearHttpsResponse *self,
    int chunk,
    double factor_headers_growth
);


void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self, const char *hostname,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size);

int BearHttpsResponse_get_status_code(BearHttpsResponse*self);

int BearHttpsResponse_get_body_size(BearHttpsResponse*self);

int BearHttpsResponse_get_headers_size(BearHttpsResponse*self);

char* BearHttpsResponse_get_header_value_by_index(BearHttpsResponse*self,int index);

char* BearHttpsResponse_get_header_value_by_key(BearHttpsResponse*self,const char *key);

char* BearHttpsResponse_get_header_key_by_index(BearHttpsResponse*self,int index);

char* BearHttpsResponse_get_header_value_by_sanitized_key(BearHttpsResponse*self,const char *key);


bool BearHttpsResponse_error(BearHttpsResponse*self);

char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self);

int BearHttpsResponse_get_error_code(BearHttpsResponse*self);

void BearHttpsResponse_set_error(BearHttpsResponse*self,const char *msg,int error_code);

void BearHttpsResponse_set_max_body_size(BearHttpsResponse*self,long size);

void BearHttpsResponse_set_body_read_props(BearHttpsResponse*self,int chunk_size,double realloc_factor);

void BearHttpsResponse_free(BearHttpsResponse *self);




#ifndef BEARSSL_HTTPS_MOCK_CJSON
cJSON * BearHttpsResponse_read_body_json(BearHttpsResponse *self);
#endif 