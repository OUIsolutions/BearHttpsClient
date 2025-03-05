
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


int private_BearHttpsResponse_write(BearHttpsResponse *self,unsigned char *bufer,long size){
    if(BearHttpsResponse_error(self)){
        return -1;
    }

    if(self->is_https){
      return br_sslio_write_all(&self->ssl_io, bufer, size);
    }
    long sended  = 0;
    while(sended < size){
        long sended_now = Universal_send(self->connection_file_descriptor, bufer+sended, size-sended,0);
        if(sended_now < 0){
            BearHttpsResponse_set_error(self,"error sending data",BEARSSL_HTTPS_IMPOSSIBLE_TO_SEND_DATA);
            return sended_now;
        }
        sended += sended_now;
    }
    return sended;
}

int private_BearHttpsResponse_read_chunck_raw(BearHttpsResponse *self,unsigned char *buffer,long size){
   
    if(BearHttpsResponse_error(self)){
        return -1;
    }
    if(self->is_https){
      return br_sslio_read(&self->ssl_io, buffer, size);
    }
    return Universal_recv(self->connection_file_descriptor, buffer, size,0);

}


int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *buffer,long size){
    if(BearHttpsResponse_error(self)){
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
        BearHttpsResponse_set_error(self,"error reading body",BEARSSL_HTTPS_IMPOSSIBLE_TO_READ_BODY);
        return readded;
    }
    if(readded> 0){
        self->body_readded+=readded;
    }
    int total_readded = readded + total_prev_sended;
    buffer[total_readded] = 0;
    return total_readded;

}
unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self){
    if(BearHttpsResponse_error(self)){
        return NULL;
    }

    long body_allocated = self->body_size;
    long size_to_read ;

    if(self->body_readded == self->user_content_length){
        return self->body;
    }
    
    if(self->user_content_length){
        body_allocated = self->user_content_length+2;
        if(self->max_body_size != -1 && body_allocated > self->max_body_size){
            BearHttpsResponse_set_error(self,"body size is bigger than max body size",BEARSSL_HTTPS_BODY_SIZE_ITS_BIGGER_THAN_LIMIT);
            BearsslHttps_free(self->body);
            self->body = NULL;
            return NULL;
        }
        self->body = (unsigned char *)BearsslHttps_reallocate(self->body,body_allocated);
        if(self->body == NULL){
            BearHttpsResponse_set_error(self,"error allocating memory",BEARSSL_HTTPS_ALOCATION_FAILED);
            return NULL;
        }
        size_to_read = self->user_content_length - self->body_readded;
    }
    else{
        body_allocated = self->body_size + self->body_chunk_size+ 2;
        self->body = (unsigned char *)BearsslHttps_reallocate(self->body,body_allocated);
        size_to_read = self->body_chunk_size;
    }

    unsigned char *buffer = (unsigned char*)(self->body + self->body_readded);
    while(true){


        while(self->body_size + self->body_chunk_size + 2  > body_allocated){
            body_allocated = body_allocated * self->body_realloc_factor;
            if(self->max_body_size != -1 && body_allocated > self->max_body_size){
                BearHttpsResponse_set_error(self,"body size is bigger than max body size",BEARSSL_HTTPS_BODY_SIZE_ITS_BIGGER_THAN_LIMIT);
                BearsslHttps_free(self->body);
                self->body = NULL;
                return NULL;
            }
            self->body = (unsigned char *)BearsslHttps_reallocate(self->body,body_allocated);
            if(self->body == NULL){
                    BearHttpsResponse_set_error(self,"error allocating memory",BEARSSL_HTTPS_ALOCATION_FAILED);
                    return NULL;
                }

            buffer = (unsigned char*)(self->body + self->body_readded);
            //printf("reallocating body to %ld\n",body_allocated);
        }

        if(self->body_readded == self->user_content_length){
            break;
        }
        
        long readded = private_BearHttpsResponse_read_chunck_raw(self,buffer,size_to_read);
        if(readded <= 0 ){
            break;
        }

        if(self->user_content_length){
            size_to_read -= readded;
        }


        self->body_readded += readded;
        self->body_size += readded;
       
        buffer += readded;

    }
    self->body[self->body_size] = '\0';
    return self->body;

}

const  char *BearHttpsResponse_read_body_str(BearHttpsResponse *self){
    
    if(BearHttpsResponse_error(self)){
        return NULL;
    }
    unsigned char *body = BearHttpsResponse_read_body(self);
    if(body == NULL){
        return NULL;
    }

    //check if its a binary
    for(int i  = 0; i < self->body_size; i++){
        if(body[i] == '\0'){
            BearHttpsResponse_set_error(self,"body is binary",BEARSSL_HTTPS_BODY_ITS_BINARY);
            BearsslHttps_free(self->body);
            self->body = NULL;
           return NULL;
        }
    }
    return (const char *)body;
}
#ifndef BEARSSL_HTTPS_MOCK_CJSON
cJSON * BearHttpsResponse_read_body_json(BearHttpsResponse *self){
    if(BearHttpsResponse_error(self)){
        return NULL;
    }
    const char *body = BearHttpsResponse_read_body_str(self);
    if(body == NULL){
        return NULL;
    }
    
    self->json_body = cJSON_Parse(body);
    free((void *)body);
    self->body = NULL;

    if(self->json_body == NULL){
        BearHttpsResponse_set_error(self,"error parsing json",BEARSSL_HTTPS_BODY_ITS_NOT_A_VALID_JSON);
        return NULL;
    }
    return self->json_body;
}
#endif