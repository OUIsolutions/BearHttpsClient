//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_define.h"
//silver_chain_scope_end

int BearHttpsResponse_read_body_chunck_http1(BearHttpsResponse *self,unsigned char *buffer,long size){




    if(self->http1_state == PRIVATE_BEARHTTPS_COLLECTING_NUMBER){
        char number_buffer[10] = {0};
        bool number_buffer_filled = false;
        for(int i =0; i < 10;i++){
            BearHttpsResponse_read_body_chunck_raw(self, (unsigned char*)number_buffer + i, 1);
            if(number_buffer[i] == '\r' && i > 0){
                number_buffer[i] = 0;
                self->http1_current_chunk_size = strtol((const char*)number_buffer, NULL, 16);
                self->http1_state = PRIVATE_BEARHTTPS_READING_CHUNK;
                self->http1_current_chunk_readed = 0;
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
        printf("filled number buffer %d\n",number_buffer_filled);
        printf("buffer %s\n",number_buffer);
        
    } 
    char *bufff = malloc( self->http1_current_chunk_size + 1);
    int total_readed = BearHttpsResponse_read_body_chunck_raw(self, (unsigned char*)bufff,  1000);
    printf("expected chunk size %ld\n",self->http1_current_chunk_size);
    printf("total readed %d\n",total_readed);
    printf("readed chunk %s\n",bufff);
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