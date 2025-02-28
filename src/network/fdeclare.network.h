
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesH.h"
//silver_chain_scope_end
static int private_BearHttpsRequest_host_connect(BearHttpsResponse *self,const char *host,int port);


static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len);


static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len);

