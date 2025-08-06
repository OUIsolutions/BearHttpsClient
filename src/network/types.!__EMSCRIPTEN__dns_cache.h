//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.macros.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)

typedef struct privateBearHttpsDnsCache{

    char host[BEARSSL_DNS_CACHE_HOST_SIZE];
    char ip[BEARSSL_DNS_CACHE_IP_SIZE];
}privateBearHttpsDnsCache;
#endif