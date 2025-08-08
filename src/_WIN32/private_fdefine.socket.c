#if defined(_WIN32) 

static int private_BearHttps_socket_set_nonblocking(int sockfd) {
    u_long mode = 1;
    return ioctlsocket(sockfd, FIONBIO, &mode);
}

static int private_BearHttps_socket_set_blocking(int sockfd) {
    u_long mode = 0;
    return ioctlsocket(sockfd, FIONBIO, &mode);
}

static int private_BearHttps_socket_check_connect_error(int sockfd) {
    int error = 0;
    socklen_t len = sizeof(error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&error, &len) < 0 || error != 0) {
        return -1;
    }
    return 0;
}

static int private_BearHttps_socket_check_connect_in_progress(int ret) {
    if (ret < 0 && WSAGetLastError() != WSAEWOULDBLOCK) {
        return -1;
    }
    return 0;
}

#endif 