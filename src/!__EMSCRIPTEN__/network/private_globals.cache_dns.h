#if !defined(__EMSCRIPTEN__)

privateBearHttpsDnsCache privateBearHttpsDnsCache_itens[BEARSSL_DNS_CACHE_SIZE] ={0};

int privateBearHttpsDnsCache_last_free_point = 0;
#endif