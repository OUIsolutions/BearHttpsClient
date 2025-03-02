
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.consts.h"
//silver_chain_scope_end

static int private_BearHttpsRequest_connect_ipv4(BearHttpsResponse *self, const char *ipv4_ip, int port);

static int private_BearHttpsRequest_connect_ipv4_no_error_raise( const char *ipv4_ip, int port);

static int private_BearHttpsRequest_connect_host(BearHttpsResponse *response, const char *host, int port,const char *dns_server_ip,const char *dns_server_hostname);


static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len);


static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len);

