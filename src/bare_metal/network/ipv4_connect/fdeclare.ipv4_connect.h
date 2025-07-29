//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end
#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)


static int private_BearHttpsRequest_connect_ipv4(BearHttpsResponse *self, const char *ipv4_ip, int port,long connection_timeout);


static int private_BearHttpsRequest_connect_ipv4_no_error_raise( const char *ipv4_ip, int port,long connection_timeout);
#endif