//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.public_fdefine.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)


static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len)
{
	for (;;) {
		ssize_t read_len;

		read_len = Universal_recv(*(int*)ctx, buf, len,0);
		if (read_len <= 0) {
			if (read_len < 0 && errno == EINTR) {
				continue;
			}
			return -1;
		}
		return (int)read_len;
	}
}


static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len)
{
	for (;;) {
		ssize_t write_len;
		write_len = Universal_send(*(int *)ctx, buf, len,0);
		if (write_len <= 0) {
			if (write_len < 0 && errno == EINTR) {
				continue;
			}
			return -1;
		}
		return (int)write_len;
	}
}

#endif