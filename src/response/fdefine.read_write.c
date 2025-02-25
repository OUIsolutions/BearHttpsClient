
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


int private_BearHttpsResponse_write(BearHttpsResponse *self,unsigned char *bufer,long size){
    //printf("%s",bufer);
    if(self->is_https){
      return br_sslio_write_all(&self->ssl_io, bufer, size);
    }
    return write(self->connection_file_descriptor, bufer, size);
}

int private_BearHttpsResponse_read_chunck_raw(BearHttpsResponse *self,unsigned char *buffer,long size){
    if(self->is_https){
      return br_sslio_read(&self->ssl_io, buffer, size);
    }
    return read(self->connection_file_descriptor, buffer, size);

}


int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *buffer,long size){

    if(self->body_readded == self->user_content_length){
        return 0;
    }

    long total_prev_sended = 0;
    while (self->extra_body_remaning_to_send > 0) {
        if(total_prev_sended >= size){
            return total_prev_sended;
        }
        buffer[total_prev_sended] = self->body[total_prev_sended];
        self->extra_body_remaning_to_send-=1;
        total_prev_sended+=1;
    }


    long readded =  private_BearHttpsResponse_read_chunck_raw(self,buffer+total_prev_sended,size-total_prev_sended);
    if(readded == -1){
        printf("error on read\n");
    }
    if(readded> 0){
        self->body_readded+=readded;
    }
    return readded + total_prev_sended;
}
unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self,long max_size,long *size){
    if(!self->user_content_length){
        *size = NULL;
        return NULL;
    }

    long new_size = self->body_start_index + self->user_content_length + 2;
    self->raw_content = (unsigned char *)BearsslHttps_reallocate(self->raw_content,new_size);
    self->body = self->raw_content + self->body_start_index;
    
    long size_to_read =self->user_content_length - self->body_readded;
    if(size_to_read > max_size){
        size_to_read = max_size;
    }

    unsigned char *buffer = self->raw_content + self->body_start_index + self->body_readded;

    while(true){
        long readded = BearHttpsResponse_read_body_chunck(self,buffer,size_to_read);
        if(readded == 0){
            break;
        }
        self->body_readded += readded;
        self->body_size += readded;
        if(self->body_readded == self->user_content_length){
            break;
        }
        size_to_read -= readded;
        buffer += readded;
    }

    self->body[self->body_size] = '\0';
    *size = self->body_size;
    return self->body;
    
}