//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.typesH.h"
//silver_chain_scope_end


int BearHttpsResponse_read_body_chunck_http1(BearHttpsResponse *self,unsigned char *buffer,long size);



int BearHttpsResponse_read_body_chunck_raw(BearHttpsResponse *self,unsigned char *buffer,long size);

int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *buffer,long size);