


static int private_BearHttpsRequest_connect_ipv4(BearHttpsResponse *self, const char *ipv4_ip, int port);

static int private_BearHttpsRequest_connect_ipv4_no_error_raise( const char *ipv4_ip, int port);

static int private_BearHttpsRequest_connect_host(BearHttpsResponse *response, const char *host, int port,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers);


static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len);


static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len);

