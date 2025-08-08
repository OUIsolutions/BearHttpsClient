//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.public_fdefine.h"
//silver_chain_scope_end
#if defined(__unix__)  && !defined(__EMSCRIPTEN__)

static int private_BearHttps_socket_set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

static int private_BearHttps_socket_set_blocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    return fcntl(sockfd, F_SETFL, flags & ~O_NONBLOCK);
}

static int private_BearHttps_socket_check_connect_error(int sockfd) {
    int error = 0;
    socklen_t len = sizeof(error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
        return -1;
    }
    return 0;
}

static int private_BearHttps_socket_check_connect_in_progress(int ret) {
    if (ret < 0 && errno != EINPROGRESS) {
        return -1;
    }
    return 0;
}

#endif 