
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
    if(self->error){
        return -1;
    }

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
    if(readded < 0){
        BearHttpsResponse_set_error_msg(self,"error reading body");
        return readded;
    }
    if(readded> 0){
        self->body_readded+=readded;
    }
    return readded + total_prev_sended;
}
unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self,long max_size){
    if(self->error){
        return NULL;
    }

    long body_allocated = self->body_size;
    long size_to_read = self->body_chunk_read;

    if(self->body_readded == self->user_content_length){
        return self->body;
    }
    
    if(self->user_content_length){
        self->body = (unsigned char *)BearsslHttps_reallocate(self->body,self->user_content_length+2);
        body_allocated = self->user_content_length+2;
        size_to_read =self->user_content_length - self->body_readded;
    }

    if(size_to_read > max_size){
        size_to_read = max_size;
    }

    unsigned char *buffer = (unsigned char*)(self->body + self->body_readded);
    while(true){


        if(body_allocated - self->body_size < size_to_read){
            self->body = (unsigned char *)BearsslHttps_reallocate(self->body,self->body_size + size_to_read + 2);
            body_allocated = self->body_size + size_to_read + 2;
            buffer = (unsigned char*)(self->body + self->body_readded);
        }

        if(self->body_readded >= max_size){
            break;
        }

        if(self->body_readded == self->user_content_length){
            break;
        }


        long readded = private_BearHttpsResponse_read_chunck_raw(self,buffer,size_to_read);
        printf("--readded: %ld\n",readded);
        if(readded == 0){
            break;
        }

        if(readded < 0){
            break;
        }

        self->body_readded += readded;
        self->body_size += readded;
        if(self->user_content_length){
            size_to_read -= readded;
        }
        buffer += readded;

    }

    self->body[self->body_size] = '\0';
    return self->body;

}

const  char *BearHttpsResponse_read_body_str(BearHttpsResponse *self,long max_size){
    unsigned char *body = BearHttpsResponse_read_body(self,max_size);
    if(body == NULL){
        return NULL;
    }

    //check if its a binary
    for(int i  = 0; i < self->body_size; i++){
        if(body[i] == '\0'){
           return NULL;
        }
    }
    return (const char *)body;
}
