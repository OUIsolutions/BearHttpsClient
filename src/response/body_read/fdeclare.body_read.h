//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.typesH.h"
//silver_chain_scope_end




int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *buffer,long size);

unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self);


const  char *BearHttpsResponse_read_body_str(BearHttpsResponse *self);

#ifndef BEARSSL_HTTPS_MOCK_CJSON
cJSON * BearHttpsResponse_read_body_json(BearHttpsResponse *self);
#endif