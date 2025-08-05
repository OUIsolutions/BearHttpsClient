//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end





BearHttpsResponse *private_newBearHttpsResponse();



int BearHttpsResponse_get_status_code(BearHttpsResponse*self);


int BearHttpsResponse_get_body_size(BearHttpsResponse*self);


bool BearHttpsResponse_error(BearHttpsResponse*self);


char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self);


int BearHttpsResponse_get_error_code(BearHttpsResponse*self);


void BearHttpsResponse_free(BearHttpsResponse *self);

int BearHttpsResponse_get_headers_size(BearHttpsResponse*self);

char* BearHttpsResponse_get_header_value_by_index(BearHttpsResponse*self,int index);

char* BearHttpsResponse_get_header_key_by_index(BearHttpsResponse*self,int index);

char* BearHttpsResponse_get_header_value_by_key(BearHttpsResponse*self,const char *key);


char* BearHttpsResponse_get_header_value_by_sanitized_key(BearHttpsResponse*self,const char *key);
