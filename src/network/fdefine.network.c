//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_define.h"

//silver_chain_scope_end


 int private_bear_https_close (int fd){
  if(fd < 0){
        return -1;
    }
    return Universal_close(fd);
}

/**
 * Sets a socket to non-blocking mode
 * @param sockfd The socket file descriptor
 * @return 0 on success, -1 on failure
 */
static int private_BearHttps_set_nonblocking(int sockfd) {
    #ifdef _WIN32
        unsigned long mode = 1;
        return ioctlsocket(sockfd, FIONBIO, &mode);
    #else
        int flags = fcntl(sockfd, F_GETFL, 0);
        if (flags == -1) return -1;
        return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    #endif
}



static int private_BearHttpsRequest_connect_ipv4(BearHttpsResponse *self, const char *ipv4_ip, int port) {
    int sockfd = Universal_socket(UNI_AF_INET, UNI_SOCK_STREAM, 0);
    if (sockfd < 0) {
        BearHttpsResponse_set_error(self, "ERROR: failed to create socket", BEARSSL_HTTPS_FAILT_TO_CREATE_SOCKET);
        return -1;
    }

    // Set socket to non-blocking mode
    if (private_BearHttps_set_nonblocking(sockfd) < 0) {
        BearHttpsResponse_set_error(self, "ERROR: failed to set non-blocking socket", BEARSSL_HTTPS_FAILT_TO_CREATE_SOCKET);
        private_bear_https_close(sockfd);
        return -1;
    }

    Universal_sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = UNI_AF_INET;
    server_addr.sin_port = Universal_htons(port);

    if (Universal_inet_pton(UNI_AF_INET, ipv4_ip, &server_addr.sin_addr) <= 0) {
        BearHttpsResponse_set_error(self, "ERROR: invalid address", BEARSSL_HTTPS_INVALID_IPV4);
        private_bear_https_close(sockfd);
        return -1;
    }

    int connect_result = Universal_connect(sockfd, (Universal_sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_result == 0) {
        // Connected immediately
        return sockfd;
    }

    if (errno != EINPROGRESS && errno != EWOULDBLOCK) {
        BearHttpsResponse_set_error(self, "ERROR: failed to connect", BEARSSL_HTTPS_FAILT_TO_CONNECT);
        private_bear_https_close(sockfd);
        return -1;
    }

    const int MAX_CONNECT_ATTEMPTS = 1000;
    for (int attempt = 0; attempt < MAX_CONNECT_ATTEMPTS; attempt++) {
        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(sockfd, &write_fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000; // Increase timeout with each iteration

        int select_result = select(sockfd + 1, NULL, &write_fds, NULL, &timeout);
        if (select_result < 0) {
            BearHttpsResponse_set_error(self, "ERROR: select() failed", BEARSSL_HTTPS_FAILT_TO_CONNECT);
            private_bear_https_close(sockfd);
            return -1;
        }
        if (select_result == 0) {
            // Timeout, try again
            continue;
        }

        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (void *)&error, &len) < 0) {
            BearHttpsResponse_set_error(self, "ERROR: getsockopt() failed", BEARSSL_HTTPS_FAILT_TO_CONNECT);
            private_bear_https_close(sockfd);
            return -1;
        }
        if (error != 0) {
            BearHttpsResponse_set_error(self, "ERROR: connection error", BEARSSL_HTTPS_FAILT_TO_CONNECT);
            private_bear_https_close(sockfd);
         
            return -1;
        }
        // Connected successfully
        printf("Connected to %s:%d\n", ipv4_ip, port);
        return sockfd;
    }
    printf("estourou o timeout em  %s\n",ipv4_ip);
    BearHttpsResponse_set_error(self, "ERROR: connection timed out", BEARSSL_HTTPS_FAILT_TO_CONNECT);
    private_bear_https_close(sockfd);
    return -1;
}

static int private_BearHttpsRequest_connect_ipv4_no_error_raise( const char *ipv4_ip, int port) {
    int sockfd = Universal_socket(UNI_AF_INET, UNI_SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }
    
    // Set socket to non-blocking mode
    if (private_BearHttps_set_nonblocking(sockfd) < 0) {
        private_bear_https_close(sockfd);
        return -1;
    }

    Universal_sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = UNI_AF_INET;
    server_addr.sin_port = Universal_htons(port); 

    if (Universal_inet_pton(UNI_AF_INET, ipv4_ip, &server_addr.sin_addr) <= 0) {
        private_bear_https_close(sockfd);
        return -1;
    }

       int connect_result = Universal_connect(sockfd, (Universal_sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_result == 0) {
        // Connected immediately
        return sockfd;
    }

    if (errno != EINPROGRESS && errno != EWOULDBLOCK) {
        private_bear_https_close(sockfd);
        return -1;
    }

    const int MAX_CONNECT_ATTEMPTS = 10;
    for (int attempt = 0; attempt < MAX_CONNECT_ATTEMPTS; attempt++) {
        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(sockfd, &write_fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // Increase timeout with each iteration

        int select_result = select(sockfd + 1, NULL, &write_fds, NULL, &timeout);
        if (select_result < 0) {
            private_bear_https_close(sockfd);
            return -1;
        }
        if (select_result == 0) {
            // Timeout, try again
            continue;
        }

        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (void *)&error, &len) < 0) {
            private_bear_https_close(sockfd);
            return -1;
        }
        if (error != 0) {
            //printf("Connection error: %s\n", strerror(error));
            return -1;
        }
        // Connected successfully
        return sockfd;
    }

    private_bear_https_close(sockfd);
    return -1;
}

#if  defined(BEARSSL_USSE_GET_ADDRINFO) || defined(BEARSSL_HTTPS_MOCK_CJSON)
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
        if (private_BearHttps_set_nonblocking(found_socket) < 0) {
            private_bear_https_close(found_socket);
            continue;
        }
        
        // Connect will return immediately with EINPROGRESS since the socket is non-blocking
        int connect_result = Universal_connect(found_socket, current_addr->ai_addr, current_addr->ai_addrlen);
        if (connect_result < 0) {
            if (errno != EINPROGRESS && errno != EWOULDBLOCK) {
                private_bear_https_close(found_socket);
                continue;
            }
            
            // Wait for socket to be ready using select
            fd_set write_fds;
            FD_ZERO(&write_fds);
            FD_SET(found_socket, &write_fds);
            
            // Set timeout for connection attempt (3 seconds)
            struct timeval timeout;
            timeout.tv_sec = 3;
            timeout.tv_usec = 0;
            
            int select_result = select(found_socket + 1, NULL, &write_fds, NULL, &timeout);
            
            if (select_result <= 0) {
                // Timeout or error
                private_bear_https_close(found_socket);
                continue;
            }
            
            // Check if the connection was successful
            int error = 0;
            socklen_t len = sizeof(error);
            if (getsockopt(found_socket, SOL_SOCKET, SO_ERROR, (void*)&error, &len) < 0 || error != 0) {
                private_bear_https_close(found_socket);
                continue;
            }
        }
        break;
    }

    if (found_socket < 0) {
        BearHttpsResponse_set_error(response, "ERROR: failed to connect\n",BEARSSL_HTTPS_FAILT_TO_CONNECT);
        return -1;
    }
    Universal_freeaddrinfo(addr_info);
    return found_socket;
}

#else


static int private_BearHttps_connect_host(BearHttpsRequest *self, BearHttpsResponse *response, const char *host, int port){
    

    if(strcmp(host,"localhost")==0){
        return private_BearHttpsRequest_connect_ipv4(response,"127.0.0.1",port);
    }
    for(int i = 0; i < self->known_ips_size;i++){
        const char *ip = self->known_ips[i];
        int sockfd = private_BearHttpsRequest_connect_ipv4_no_error_raise(ip,port);
        if(sockfd < 0){
            continue;
        }

        return sockfd;
    }
    for(int i = 0; i < BEARSSL_DNS_CACHE_SIZE;i++){
        privateBearHttpsDnsCache *cache = &privateBearHttpsDnsCache_itens[i];

        if(private_BearsslHttp_strcmp(cache->host,host) == 0){
            int sockfd = private_BearHttpsRequest_connect_ipv4_no_error_raise(cache->ip,port);
            if(sockfd < 0){
                break;
            }
            return sockfd;
        }
    }

    BearHttpsClientDnsProvider *chosen_dns_providers  = self->dns_providers ?  self->dns_providers : privateBearHttpsProviders;
    int chosen_dns_providers_size = self->total_dns_providers ? self->total_dns_providers : privateBearHttpsProvidersSize;

    if(chosen_dns_providers_size == 0){
        BearHttpsResponse_set_error(response,"ERROR: no dns providers\n",BEARSSL_HTTPS_NO_DNS_PROVIDED);
        return -1;
    }

    for(int i = 0; i < chosen_dns_providers_size;i++){
            BearHttpsClientDnsProvider provider = chosen_dns_providers[i];
            BearHttpsRequest *dns_request = newBearHttpsRequest_fmt("https://%s:%d%s?name=%s&type=A",provider.ip,provider.port, provider.route, host); 
           //printf("used url %s\n",dns_request->url);
            dns_request->custom_bear_dns = provider.hostname;

            //these its require, otherwise can generate indirect recursion error if provider.ip its wrong
            dns_request->must_be_ipv4 = true;
            BearHttpsResponse *dns_response = BearHttpsRequest_fetch(dns_request);
            

            if(BearHttpsResponse_error(dns_response)){
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                continue;
            }       

            cJSON * body = BearHttpsResponse_read_body_json(dns_response);
            if(BearHttpsResponse_error(dns_response)){
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                continue;
            }
            cJSON * answer = cJSON_GetObjectItem(body, "Answer");
            if(answer == NULL){
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                continue;
            }
            long size = cJSON_GetArraySize(answer);
            if(size == 0){
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                continue;
            }

            for(int j = 0; j < size;j++){
                cJSON * item = cJSON_GetArrayItem(answer,j);
                cJSON * data = cJSON_GetObjectItem(item, "data");
                if(data == NULL){
                    continue;
                }
                const char * ipv4 = cJSON_GetStringValue(data);
                if(ipv4 == NULL){
                    continue;
                }
                int sockfd = private_BearHttpsRequest_connect_ipv4_no_error_raise(ipv4,port);
                if(sockfd < 0){
                    continue;
                }
                long host_size = private_BearsslHttps_strlen(host);
                long ip_size = private_BearsslHttps_strlen(ipv4);

                if(privateBearHttpsDnsCache_last_free_point >= BEARSSL_DNS_CACHE_SIZE){
                    privateBearHttpsDnsCache_last_free_point = 0;
                }

                if(host_size < BEARSSL_DNS_CACHE_HOST_SIZE && ip_size < BEARSSL_DNS_CACHE_IP_SIZE){
                    
                    
                    privateBearHttpsDnsCache* cache = &privateBearHttpsDnsCache_itens[privateBearHttpsDnsCache_last_free_point];
                    private_BearsslHttps_strcpy( cache->host,host);
                    private_BearsslHttps_strcpy( cache->ip,ipv4);
                    privateBearHttpsDnsCache_last_free_point++; 
                }

                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                return sockfd;
            }
            
    }

     BearHttpsResponse_set_error(response,"ERROR: failed to create dns request",BEARSSL_HTTPS_FAILT_TO_CREATE_DNS_REQUEST);
    return -1;
}


#endif

