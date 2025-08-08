#if !defined(__EMSCRIPTEN__)





typedef struct BearHttpsClientDnsProvider {
    const char *hostname;
    const  char *route;
    const char *ip;
    int port;
}BearHttpsClientDnsProvider;
#endif