#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)





typedef struct BearHttpsClientDnsProvider {
    const char *hostname;
    const  char *route;
    const char *ip;
    int port;
}BearHttpsClientDnsProvider;
#endif