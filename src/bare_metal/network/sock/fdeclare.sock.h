//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end
#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)


static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len);

static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len);
#endif