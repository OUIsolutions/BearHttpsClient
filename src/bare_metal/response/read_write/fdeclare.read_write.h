//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end
#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)


int private_BearHttpsResponse_write(BearHttpsResponse *self,unsigned char *bufer,long size);

int private_BearHttpsResponse_recv(BearHttpsResponse *self,unsigned char *buffer,long size);
#endif