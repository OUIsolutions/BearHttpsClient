//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_define.h"
//silver_chain_scope_end


static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len)
{
    const int MAX_SEQUENTIAL_ERRORS = 200; // Maximum number of sequential errors before giving up
	for(int i = 0; i < MAX_SEQUENTIAL_ERRORS; i++){ 
		ssize_t read_len = Universal_recv(*(int*)ctx, buf, len, 0);
        printf("read_len: %d %ld\n",i, read_len);
        if(read_len >=0){
            return (int)read_len;
        }
        if (errno == EINTR) {
            continue;
        }
        // Handle non-blocking socket
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Socket would block, use select to wait until data is available
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(*(int*)ctx, &read_fds);
            
            // Wait for up to 10ms (adjust timeout as needed)
            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 10000; // Increase timeout with each iteration
            
            select(*(int*)ctx + 1, &read_fds, NULL, NULL, &timeout);
        
        }
              
	}
    return -1; // Too many errors, return -1
}


static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len)
{  
    const int MAX_SEQUENTIAL_ERRORS = 1000; // Maximum number of sequential errors before giving up
    int total_sequential_error = 0;
	while (true){
		ssize_t write_len;
		write_len = Universal_send(*(int *)ctx, buf, len, 0);
        printf("write_len: %ld\n", write_len);
        if(write_len > 0) {
            return (int)write_len;
        }
        
        total_sequential_error++;
        if (total_sequential_error >= MAX_SEQUENTIAL_ERRORS) {
            // Too many errors, return -1
            return -1;
        }

        if (errno == EINTR) {
            continue;
        }

        // Handle non-blocking socket
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Socket would block, use select to wait until it's writable
            fd_set write_fds;
            FD_ZERO(&write_fds);
            FD_SET(*(int*)ctx, &write_fds);
            
            // Wait for up to 100ms (adjust timeout as needed)
            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 10000;
            select(*(int*)ctx + 1, NULL, &write_fds, NULL, &timeout);
        }    
	}
}
