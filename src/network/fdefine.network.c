//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_define.h"
//silver_chain_scope_end





static int private_BearHttpsRequest_connect_ipv4(BearHttpsResponse *self, const char *ipv4_ip, int port) {
   
    int sockfd = Universal_socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        BearHttpsResponse_set_error(self,"ERROR: failed to create socket",BEARSSL_HTTPS_FAILT_TO_CREATE_SOCKET);
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

    if (Universal_connect(sockfd, (Universal_sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        BearHttpsResponse_set_error(self,"ERROR: failed to connect",BEARSSL_HTTPS_FAILT_TO_CONNECT);
        Universal_close(sockfd); 
        return -1;
    }

    return sockfd;
}


static int private_BearHttpsRequest_connect_ipv4_no_error_raise( const char *ipv4_ip, int port) {
    
    printf("testing ipv4 %s:%d\n",ipv4_ip,port);
    int sockfd = Universal_socket(UNI_AF_INET, UNI_SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }

    // Set socket to non-blocking mode
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    Universal_sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = UNI_AF_INET;
    server_addr.sin_port = Universal_htons(port); 

    if (Universal_inet_pton(UNI_AF_INET, ipv4_ip, &server_addr.sin_addr) <= 0) {
        Universal_close(sockfd);
        return -1;
    }
    int ret = Universal_connect(sockfd, (Universal_sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0 && errno != EINPROGRESS) {
        Universal_close(sockfd); 
        return -1;
    }

    // Use select to wait for connection with timeout
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(sockfd, &write_fds);
    
    struct timeval timeout;
    timeout.tv_sec = 1;  // 5 second timeout
    timeout.tv_usec = 0;
    
    ret = select(sockfd + 1, NULL, &write_fds, NULL, &timeout);
    if (ret <= 0) {
        Universal_close(sockfd);
        return -1;
    }

    // Check if connection succeeded
    int error = 0;
    socklen_t len = sizeof(error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
        Universal_close(sockfd);
        return -1;
    }

    // Set socket back to blocking mode
    fcntl(sockfd, F_SETFL, flags);

    return sockfd;
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
        if (Universal_connect(found_socket, current_addr->ai_addr, current_addr->ai_addrlen) < 0) {
            Universal_close(found_socket);
            continue;
        }
        break;
    }

    if (found_socket < 0) {
        BearHttpsResponse_set_error(response, "ERROR: failed to connect\n",BEARSSL_HTTPS_FAILT_TO_CONNECT);
        return -1;;
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
        printf("value of i %d\n",i);
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
            //printf("leu o body json: %s\n",cJSON_Print(body));
            if(BearHttpsResponse_error(dns_response)){
                char *message = BearHttpsResponse_get_error_msg(dns_response);
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
