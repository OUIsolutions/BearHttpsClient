//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

privateBearHttpsDnsCache privateBearHttpsDnsCache_itens[BEARSSL_DNS_CACHE_SIZE] ={0};

int privateBearHttpsDnsCache_last_free_point = 0;