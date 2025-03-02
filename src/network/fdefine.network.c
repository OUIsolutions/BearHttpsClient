
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


static int private_BearHttpsRequest_connect_ipv4(BearHttpsResponse *self, const char *ipv4_ip, int port) {
    int sockfd = Universal_socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        BearHttpsResponse_set_error_msg(self,"ERROR: failed to create socket\n");
        return -1; 
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); 

    if (inet_pton(AF_INET, ipv4_ip, &server_addr.sin_addr) <= 0) {
        BearHttpsResponse_set_error_msg(self,"ERROR: invalid address\n");
        Universal_close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        BearHttpsResponse_set_error_msg(self,"ERROR: failed to connect\n");
        Universal_close(sockfd); 
        return -1;
    }

    return sockfd;
}
static int private_BearHttpsRequest_connect_ipv4_no_error_raise( const char *ipv4_ip, int port) {
    int sockfd = Universal_socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1; 
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); 

    if (inet_pton(AF_INET, ipv4_ip, &server_addr.sin_addr) <= 0) {
        Universal_close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        Universal_close(sockfd); 
        return -1;
    }

    return sockfd;
}

#if  defined(BEARSSL_USSE_GET_ADDRINFO) || defined(BEARSSL_HTTPS_MOCK_CJSON)
static int private_BearHttpsRequest_connect_host(BearHttpsResponse *response, const char *host, int port,const char *dns_server_ip,const char *dns_server_hostname) {

    Universal_addrinfo hints = {0};
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = UNI_SOCK_STREAM;

    char port_str[10];
    snprintf(port_str,sizeof(port_str)-1, "%d", port);
    Universal_addrinfo *addr_info;
    int status = Universal_getaddrinfo(host, port_str, &hints, &addr_info);
    if (status != 0) {
        BearHttpsResponse_set_error_msg(response, gai_strerror(status));
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
        BearHttpsResponse_set_error_msg(response, "ERROR: failed to connect\n");
    }
    Universal_freeaddrinfo(addr_info);
    return found_socket;
}

#else 
static int private_BearHttpsRequest_connect_host(BearHttpsResponse *response, const char *host, int port,const char *dns_server_ip,const char *dns_server_hostname) {
   

    for(int i = 0; i < privateBearHttpsProvidersSize;i++){
            BearHttpsClientDnsProvider provider = privateBearHttpsProviders[i];
            BearHttpsRequest *dns_request = newBearHttpsRequest_fmt("https://%s/%s?name=%s&type=A",provider.ip,provider.route, host); 
            dns_request->custom_bear_dns = provider.hostname;
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
            for(int i = 0; i < size;i++){
                cJSON * item = cJSON_GetArrayItem(answer,i);
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
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                return sockfd;
            }

            continue;
    }

     BearHttpsResponse_set_error_msg(response,"ERROR: failed to create dns request\n");
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
