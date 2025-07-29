//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end
#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)




void private_BearHttpsResponse_parse_headers(BearHttpsResponse *self,int headers_end);

void private_BearHttpsResponse_read_til_end_of_headers_or_reach_limit(
    BearHttpsResponse *self,
    int chunk_size,
    double factor_headers_growth
);

#endif