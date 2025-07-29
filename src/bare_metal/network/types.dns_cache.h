#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)

typedef struct privateBearHttpsDnsCache{

    char host[BEARSSL_DNS_CACHE_HOST_SIZE];
    char ip[BEARSSL_DNS_CACHE_IP_SIZE];
}privateBearHttpsDnsCache;
#endif