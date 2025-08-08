//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if defined(__unix__) && !defined(__EMSCRIPTEN__)


#if  (defined(BEARSSL_USSE_GET_ADDRINFO) || defined(BEARSSL_HTTPS_MOCK_CJSON))
static int private_BearHttps_connect_host(BearHttpsRequest *self, BearHttpsResponse *response, const char *host, int port){
    Universal_addrinfo hints = {0};
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = UNI_SOCK_STREAM;

    char port_str[10];
    snprintf(port_str,sizeof(port_str)-1, "%d", port);
    Universal_addrinfo *addr_info;
    int status = Universal_getaddrinfo(host, port_str, &hints, &addr_info);
    if (status != 0) {
        BearHttpsResponse_set_error(response, gai_strerror(status),BEARSSL_HTTPS_UNKNOW_ERROR);
        return -1;
    }

    int found_socket = -1;
    for (Universal_addrinfo *current_addr = addr_info; current_addr != NULL; current_addr = current_addr->ai_next) {
        found_socket = Universal_socket(current_addr->ai_family, current_addr->ai_socktype, current_addr->ai_protocol);
        
        if (found_socket < 0) {
            continue;
        }

        // Set socket to non-blocking mode
        int flags = fcntl(found_socket, F_GETFL, 0);
        fcntl(found_socket, F_SETFL, flags | O_NONBLOCK);

        int ret = Universal_connect(found_socket, current_addr->ai_addr, current_addr->ai_addrlen);
        if (ret < 0 && errno != EINPROGRESS) {
            Universal_close(found_socket);
            continue;
        }

        // Use select to wait for connection with timeout
        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(found_socket, &write_fds);
        
        struct timeval timeout;
        timeout.tv_sec = 0;  
        timeout.tv_usec = self->connection_timeout * BEARSSL_MILISECONDS_MULTIPLIER; 
        
        ret = select(found_socket + 1, NULL, &write_fds, NULL, &timeout);
        if (ret <= 0) {
            Universal_close(found_socket);
            continue;
        }

        // Check if connection succeeded
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(found_socket, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
            Universal_close(found_socket);
            continue;
        }

        // Set socket back to blocking mode
        fcntl(found_socket, F_SETFL, flags);
        break;
    }

    if (found_socket < 0) {
        BearHttpsResponse_set_error(response, "ERROR: failed to connect\n",BEARSSL_HTTPS_FAILT_TO_CONNECT);
        return -1;
    }
    Universal_freeaddrinfo(addr_info);
    return found_socket;
}

#endif

#endif