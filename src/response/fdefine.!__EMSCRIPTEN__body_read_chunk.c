//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)

int BearHttpsResponse_read_body_chunck_http1(BearHttpsResponse *self,unsigned char *buffer,long size){
    long remaning_to_read = size;
    while (true){
        if(self->http1_state == PRIVATE_BEARHTTPS_COLLECTING_NUMBER){
            char number_buffer[10] = {0};
            bool number_buffer_filled = false;
            for(int i =0; i < 10;i++){
                BearHttpsResponse_read_body_chunck_raw(self, (unsigned char*)number_buffer + i, 1);
                if(number_buffer[i] == '\r' && i > 0){
                    number_buffer[i] = 0;
                    self->http1_reaming_to_read = strtol((const char*)number_buffer, NULL, 16);

                    if(self->http1_reaming_to_read < 0){
                        BearHttpsResponse_set_error(self,"invalid http response",BEARSSL_HTTPS_INVALID_HTTP_RESPONSE);
                        return -1;
                    }
                    if(self->http1_reaming_to_read == 0){
                        self->http1_state = PRIVATE_BEARHTTPS_COLLECTING_NUMBER;
                        self->body_completed_read = true;
                        return size - remaning_to_read; //return the size of the body readed
                    }

                    self->http1_state = PRIVATE_BEARHTTPS_READING_CHUNK;
                    number_buffer_filled= true;
                    char trash[2] = {0};
                    BearHttpsResponse_read_body_chunck_raw(self, (unsigned char*)trash, 1);
                    break;
                }
            }

            if(!number_buffer_filled){
                BearHttpsResponse_set_error(self,"invalid http response",BEARSSL_HTTPS_INVALID_HTTP_RESPONSE);
                return -1;
            }        
        
        } 
        if(self->http1_state == PRIVATE_BEARHTTPS_READING_CHUNK){
            long read_size;
            if(remaning_to_read <= self->http1_reaming_to_read){
                read_size = remaning_to_read;
            }
            
            if(remaning_to_read > self->http1_reaming_to_read){
                read_size = self->http1_reaming_to_read;
            }

            long readded = BearHttpsResponse_read_body_chunck_raw(self, buffer, read_size);
            if(readded < 0){
                BearHttpsResponse_set_error(self,"error reading body chunk",BEARSSL_HTTPS_ERROR_READING_CHUNK);
                return -1;  
            }
            remaning_to_read -= readded;
            self->http1_reaming_to_read -= readded;
            if(self->http1_reaming_to_read == 0){
                self->http1_state = PRIVATE_BEARHTTPS_COLLECTING_NUMBER;
                char trash[3] = {0};
                BearHttpsResponse_read_body_chunck_raw(self, (unsigned char*)trash, 2); //read \r\n
            }

        }
       
    }

    return 0;
}


int BearHttpsResponse_read_body_chunck_raw(BearHttpsResponse *self,unsigned char *buffer,long size){

    long total_prev_sended = 0;
    while (self->extra_body_remaning_to_send > 0) {
        if(total_prev_sended >= size){
            return total_prev_sended;
        }
        buffer[total_prev_sended] = self->body[self->body_readded_size - self->extra_body_remaning_to_send];
        self->extra_body_remaning_to_send-=1;
        total_prev_sended+=1;
    }

    long readded = private_BearHttpsResponse_recv(self,buffer+total_prev_sended,size-total_prev_sended);
    if(readded < 0){
        return readded;
    }
    if(readded> 0){
        self->body_readded_size+=readded;
    }
    int total_readded = readded + total_prev_sended;
    buffer[total_readded] = 0;
    return total_readded;

}

int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *buffer,long size){
    if(BearHttpsResponse_error(self)){
        return -1;
    }

    if(self->body_readded_size == self->respnse_content_lenght && self->body_read_mode == PRIVATE_BEARSSL_BY_CONTENT_LENGTH ){
        return 0;
    }

    if(self->body_read_mode == PRIVATE_BEARSSL_BY_CHUNKED){
        return BearHttpsResponse_read_body_chunck_http1(self,buffer,size);
    }
 
    return BearHttpsResponse_read_body_chunck_raw(self,buffer,size);
}
#endif