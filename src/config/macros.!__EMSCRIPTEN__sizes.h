//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_declareB.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)


#define BEARSSL_HEADER_CHUNK           200
#define BEARSSL_HEADER_REALLOC_FACTOR    3
#define BEARSSL_MAX_REDIRECTIONS        10
#define BEARSSL_BODY_CHUNK_SIZE       1024
#define BEARSSL_BODY_REALLOC_FACTOR      1.5
#define BEARSSL_DNS_CACHE_HOST_SIZE 1000
#define BEARSSL_DNS_CACHE_IP_SIZE 200
#define BEARSSL_DNS_CACHE_SIZE 100
#define BEARSSL_TIMEOUT 2000
#define BEARSSL_MILISECONDS_MULTIPLIER 1000
#endif