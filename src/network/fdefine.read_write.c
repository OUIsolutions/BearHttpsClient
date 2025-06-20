//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_define.h"
//silver_chain_scope_end


static int private_BearHttps_sock_read(void *ctx, unsigned char *buf, size_t len)
{
    const int MAX_SEQUENTIAL_ERRORS = 100; // Maximum number of sequential errors before giving up
	for(int i = 0; i < MAX_SEQUENTIAL_ERRORS; i++){ 
		ssize_t read_len = Universal_recv(*(int*)ctx, buf, len, 0);
        printf("read_lenxxxx:%d %d %ld\n",*(int*)ctx, i, read_len);
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
            timeout.tv_usec = 100000; // Increase timeout with each iteration
            
            select(*(int*)ctx, &read_fds, NULL, NULL, &timeout);
            
        }
              
	}
    return -1; // Too many errors, return -1
}


static int private_BearHttps_sock_read_all(void *ctx, unsigned char *buf, size_t len)
{
    //printf("private_BearHttps_sock_read_all called with len: %ld\n", len);
    size_t total_read = 0;  
    while (total_read < len) {
        ssize_t read = private_BearHttps_sock_read(ctx, buf + total_read, len - total_read);
        if (read == 0) {
            return total_read;
        }
        if(read < 0) {
            printf("Error reading from socket: %s\n", strerror(errno));
            return -1; // Error occurred
        }
        total_read += read;
    }
    
    //printf("total_read: %ld\n", total_read);
    return (int)total_read; // Return total bytes read
}

static int private_BearHttps_sock_write(void *ctx, const unsigned char *buf, size_t len)
{  



    const int MAX_SEQUENTIAL_ERRORS = 10; // Maximum number of sequential errors before giving up
	for(int i = 0; i < MAX_SEQUENTIAL_ERRORS; i++){
		ssize_t write_len = Universal_send(*(int *)ctx, buf, len, 0);
        //printf("write_lenxxx: %d %ld\n", i, write_len);
        if(write_len >= 0) {
            return (int)write_len;
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
            
            // Wait for up to 10ms (adjust timeout as needed)
            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 100000; // Increase timeout with each iteration
            
            select(*(int*)ctx, NULL, &write_fds, NULL, &timeout);
        }    
	}
    return -1; // Too many errors, return -1
}
static int private_BearHttps_sock_write_all(void *ctx, const unsigned char *buf, size_t len){
   // printf("private_BearHttps_sock_write_all called with len: %ld\n", len);
    size_t total_written = 0;
    while (total_written < len) {
        ssize_t written = private_BearHttps_sock_write(ctx, buf + total_written, len - total_written);
        if (written <= 0) {
            return total_written; // Error or no more data
        }
        total_written += written;
    }
    
   /// printf("total_written: %ld\n", total_written);
    return (int)total_written; // Return total bytes written
}
