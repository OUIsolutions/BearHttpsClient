
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


static int private_BearHttpsRequest_host_connect(BearHttpsResponse *self, const char *host, int port) {

    Universal_addrinfo hints = {0};
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = UNI_SOCK_STREAM;
    char port_str[10];
    snprintf(port_str,sizeof(port_str)-1, "%d", port);

    Universal_addrinfo *addr_info;
    int status = Universal_getaddrinfo(host, port_str, &hints, &addr_info);
    if (status != 0) {
        BearHttpsResponse_set_error_msg(self, gai_strerror(status));
        return -1;
    }

    int found_socket = -1;
    for (Universal_addrinfo *current_addr = addr_info; current_addr != NULL; current_addr = current_addr->ai_next) {
        found_socket = Universal_socket(current_addr->ai_family, current_addr->ai_socktype, current_addr->ai_protocol);
        if (found_socket < 0) {
            continue;
        }
        if (Universal_connect(found_socket, current_addr->ai_addr, current_addr->ai_addrlen) < 0) {
            Universal_close(found_socket);
            continue;
        }
        break;
    }

    if (found_socket < 0) {
        BearHttpsResponse_set_error_msg(self, "ERROR: failed to connect\n");
    }
    Universal_freeaddrinfo(addr_info);
    return found_socket;
}


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
