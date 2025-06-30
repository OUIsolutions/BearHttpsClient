//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_define.h"
//silver_chain_scope_end

int BearHttpsResponse_read_body_chunck_http1(BearHttpsResponse *self,unsigned char *buffer,long size){

    if(self->http1_state == PRIVATE_BEARHTTPS_COLLECTING_NUMBER){
    }
}


int BearHttpsResponse_read_body_chunck_raw(BearHttpsResponse *self,unsigned char *buffer,long size){

    long total_prev_sended = 0;
    while (self->extra_body_remaning_to_send > 0) {
        if(total_prev_sended >= size){
            return total_prev_sended;
        }
        buffer[total_prev_sended] = self->body[total_prev_sended];
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