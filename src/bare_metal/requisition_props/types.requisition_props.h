#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)





typedef struct private_BearHttpsRequisitionProps{
    char *hostname;
    char *route;
    bool is_ipv4;
    bool is_ipv6;
    bool is_https;
    int port;
}private_BearHttpsRequisitionProps ;

#endif