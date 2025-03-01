
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

static int private_BearHttpsRequest_connect_host(BearHttpsResponse *self, const char *host, int port) {

    BearHttpsRequest *dns_request = newBearHttpsRequest_fmt("8.8.8.8/resolve?name=%s&type=A", host); 
    printf("aaa\n");

    BearHttpsResponse *dns_response = BearHttpsRequest_fetch(dns_request);
   
    if(BearHttpsResponse_error(dns_response)){
        BearHttpsResponse_set_error_msg(self,"ERROR: failed to create dns request\n");
        BearHttpsRequest_free(dns_request);
        BearHttpsResponse_free(dns_response);
        return -1;
    }
    printf("aaa\n");
    const char *body = BearHttpsResponse_read_body_str(dns_response, 10000);
    printf("body: %s\n", body);
    return -1;

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
