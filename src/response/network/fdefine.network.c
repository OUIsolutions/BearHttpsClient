
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end



//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


static int private_BearHttpsRequest_host_connect(BearHttpsResponse *self, const char *host, int port) {
    struct addrinfo hints = {0};
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    char port_str[10];
    sprintf(port_str, "%d", port);

    struct addrinfo *addr_info;
    int status = Universal_getaddrinfo(host, port_str, &hints, &addr_info);
    if (status != 0) {
        BearHttpsResponse_set_error_msg(self, gai_strerror(status));
        return -1;
    }
    int found_socket = -1;
    for (struct addrinfo *current_addr = addr_info; current_addr != NULL; current_addr = current_addr->ai_next) {
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
