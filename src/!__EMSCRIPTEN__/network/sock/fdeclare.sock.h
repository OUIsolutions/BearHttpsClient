//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)

static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len);

static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len);

// Socket configuration functions for different OS
static int private_BearHttps_socket_set_nonblocking(int sockfd);
static int private_BearHttps_socket_set_blocking(int sockfd);
static int private_BearHttps_socket_check_connect_error(int sockfd);
static int private_BearHttps_socket_check_connect_in_progress(int ret);

#endif