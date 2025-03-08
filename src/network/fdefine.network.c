
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
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
    int sockfd = Universal_socket(UNI_AF_INET, UNI_SOCK_STREAM, 0);
    if (sockfd < 0) {
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

    if (Universal_connect(sockfd, (Universal_sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        Universal_close(sockfd); 
        return -1;
    }

    return sockfd;
}

#if  defined(BEARSSL_USSE_GET_ADDRINFO) || defined(BEARSSL_HTTPS_MOCK_CJSON)
static int private_BearHttpsRequest_connect_host(BearHttpsResponse *response, const char *host, int port,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers) {
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
static int private_BearHttpsRequest_connect_host(BearHttpsResponse *response, const char *host, int port,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers) {
    
    BearHttpsClientDnsProvider *chosen_dns_providers  = dns_providers ?  dns_providers : privateBearHttpsProviders;
    int chosen_dns_providers_size = total_dns_proviers ? total_dns_proviers : privateBearHttpsProvidersSize;

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
