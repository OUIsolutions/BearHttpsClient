//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.public_fdefine.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)

static int private_BearHttpsRequest_connect_ipv4(BearHttpsResponse *self, const char *ipv4_ip, int port,long connection_timeout) {
    int sockfd = Universal_socket(UNI_AF_INET, UNI_SOCK_STREAM, 0);
    if (sockfd < 0) {
        BearHttpsResponse_set_error(self,"ERROR: failed to create socket",BEARSSL_HTTPS_FAILT_TO_CREATE_SOCKET);
        return -1; 
    }

    // Set socket to non-blocking mode
    if (private_BearHttps_socket_set_nonblocking(sockfd) < 0) {
        BearHttpsResponse_set_error(self,"ERROR: failed to set socket non-blocking",BEARSSL_HTTPS_FAILT_TO_CREATE_SOCKET);
        Universal_close(sockfd);
        return -1;
    }

    Universal_sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = UNI_AF_INET;
    server_addr.sin_port = Universal_htons(port); 

    if (Universal_inet_pton(UNI_AF_INET, ipv4_ip, &server_addr.sin_addr) <= 0) {
        BearHttpsResponse_set_error(self,"ERROR: invalid address",BEARSSL_HTTPS_INVALID_IPV4);
        Universal_close(sockfd);
        return -1;
    }

    int ret = Universal_connect(sockfd, (Universal_sockaddr *)&server_addr, sizeof(server_addr));
    if (private_BearHttps_socket_check_connect_in_progress(ret) < 0) {
        BearHttpsResponse_set_error(self,"ERROR: failed to connect",BEARSSL_HTTPS_FAILT_TO_CONNECT);
        Universal_close(sockfd); 
        return -1;
    }

    // Use select to wait for connection with timeout
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(sockfd, &write_fds);
    
    struct timeval timeout;
    timeout.tv_sec = connection_timeout / 1000;  
    timeout.tv_usec = (connection_timeout % 1000) * BEARSSL_MILISECONDS_MULTIPLIER; 
    
    ret = select(sockfd + 1, NULL, &write_fds, NULL, &timeout);
    if (ret <= 0) {
        BearHttpsResponse_set_error(self,"ERROR: connection timeout or failed",BEARSSL_HTTPS_FAILT_TO_CONNECT);
        Universal_close(sockfd);
        return -1;
    }

    // Check if connection succeeded
    if (private_BearHttps_socket_check_connect_error(sockfd) < 0) {
        BearHttpsResponse_set_error(self,"ERROR: failed to connect",BEARSSL_HTTPS_FAILT_TO_CONNECT);
        Universal_close(sockfd);
        return -1;
    }

    // Set socket back to blocking mode
    if (private_BearHttps_socket_set_blocking(sockfd) < 0) {
        BearHttpsResponse_set_error(self,"ERROR: failed to set socket blocking",BEARSSL_HTTPS_FAILT_TO_CREATE_SOCKET);
        Universal_close(sockfd);
        return -1;
    }

    return sockfd;
}


static int private_BearHttpsRequest_connect_ipv4_no_error_raise( const char *ipv4_ip, int port,long connection_timeout) {
    int sockfd = Universal_socket(UNI_AF_INET, UNI_SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }

    // Set socket to non-blocking mode
    if (private_BearHttps_socket_set_nonblocking(sockfd) < 0) {
        Universal_close(sockfd);
        return -1;
    }

    Universal_sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = UNI_AF_INET;
    server_addr.sin_port = Universal_htons(port); 

    if (Universal_inet_pton(UNI_AF_INET, ipv4_ip, &server_addr.sin_addr) <= 0) {
        Universal_close(sockfd);
        return -1;
    }
    int ret = Universal_connect(sockfd, (Universal_sockaddr *)&server_addr, sizeof(server_addr));
    if (private_BearHttps_socket_check_connect_in_progress(ret) < 0) {
        Universal_close(sockfd); 
        return -1;
    }

    // Use select to wait for connection with timeout
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(sockfd, &write_fds);
    
    struct timeval timeout;
    timeout.tv_sec = connection_timeout / 1000;
    timeout.tv_usec = (connection_timeout % 1000) * BEARSSL_MILISECONDS_MULTIPLIER; 
    
    ret = select(sockfd + 1, NULL, &write_fds, NULL, &timeout);
    if (ret <= 0) {
        Universal_close(sockfd);
        return -1;
    }

    // Check if connection succeeded
    if (private_BearHttps_socket_check_connect_error(sockfd) < 0) {
        Universal_close(sockfd);
        return -1;
    }

    // Set socket back to blocking mode
    if (private_BearHttps_socket_set_blocking(sockfd) < 0) {
        Universal_close(sockfd);
        return -1;
    }

    return sockfd;
}


#endif