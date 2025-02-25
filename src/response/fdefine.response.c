
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


BearHttpsResponse *private_newBearHttpsResponse(){
    BearHttpsResponse *self = (BearHttpsResponse *)malloc(sizeof(BearHttpsResponse));
    *self = (BearHttpsResponse){0};
    self->status_code = 0;
    self->headders = private_newBearHttpsHeadders();
    self->body = NULL;
    return self;
}

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
    self->body_readded+=readded;
    return readded + total_prev_sended;
}


void private_BearHttpsResponse_parse_headders(BearHttpsResponse *self,int headders_end){
    const short WAITING_FIRST_SPACE = 0;
    const short WAITING_STATUS_CODE = 1;
    const short WAITING_STATUS_CODE_TERMINATION = 2;
    const short WAITING_FIRST_LINE_TERMINATION  = 3;
    const short COLECTING_KEY = 4;
    const short COLECTING_VAL = 4;
    short state = WAITING_FIRST_SPACE;
    for(int i =0 ; i < headders_end;i++){
       // printf("%c",self->raw_content[i]);
    }
    ///printf("\n------------\n");
    //printf("%s",self->raw_content);
}
void private_BearHttpsResponse_read_til_end_of_headders_or_reach_limit(
    BearHttpsResponse *self,
    int chunk_size,
    double factor_headders_growth
){


    self->raw_content = BearsslHttps_allocate(chunk_size+2);
    self->content_allocated  = chunk_size;
    long content_size = 0;


    while(true){
        //apply the factor realloc
        while(content_size + chunk_size >= self->content_allocated -2 ){
            self->content_allocated = (long)(self->content_allocated * factor_headders_growth);
            self->raw_content = (unsigned char*)BearsslHttps_reallocate(self->raw_content,self->content_allocated);
        }
        //we create a buff str , to allow 'append' into the content
        unsigned char *content_point = (self->raw_content +content_size);

        int readded = private_BearHttpsResponse_read_chunck_raw(self,content_point, chunk_size);
        if(readded == 0){
            return;
        }
        if(readded < 0){
            char error_buff[100] ={0};
            sprintf(error_buff,"invalid read code: %d",readded);
            BearHttpsResponse_set_error_msg(self,error_buff);
            return;
        }


        for(int i = 3; i < readded;i++){
            if(
                content_point[i-3] == '\r' &&
                content_point[i-2] == '\n' &&
                content_point[i-1] == '\r' &&
                content_point[i] == '\n' )
            {
                self->body_start =content_size + i+1;
                self->body_size = ((content_size+readded) - self->body_start);
                self->extra_body_remaning_to_send = self->body_size;
                self->body_readded = self->body_size;
                self->body = (self->raw_content+ self->body_start);
                private_BearHttpsResponse_parse_headders(self,content_size + i-3);
                return;
            }
        }
        content_size+=readded;

    }


    BearHttpsResponse_set_error_msg(self,"invalid http response");


}

void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self, const char *hostname) {
    self->is_https = true;
    br_ssl_client_init_full(&self->ssl_client, &self->certification_context, TAs, TAs_NUM);
    br_ssl_engine_set_all_flags(& self->ssl_client.eng, BR_OPT_TOLERATE_NO_CLIENT_AUTH);


    br_ssl_engine_set_buffer(&self->ssl_client.eng, self->bear_buffer, sizeof(self->bear_buffer), 1);

    br_ssl_client_reset(&self->ssl_client, hostname, 0);
    br_sslio_init(&self->ssl_io, &self->ssl_client.eng, private_BearHttps_sock_read,
                  &self->connection_file_descriptor,
                  private_BearHttps_sock_write,
                  &self->connection_file_descriptor
    );
}

int BearHttpsResponse_get_status_code(BearHttpsResponse*self){
    return self->status_code;
}

bool BearHttpsResponse_error(BearHttpsResponse*self){
    return self->error_msg != NULL;
}

void BearHttpsResponse_set_error_msg(BearHttpsResponse*self,const char *msg){
    self->error_msg = private_BearsslHttps_strdup(msg);
}

char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self){
    return self->error_msg;
}
void BearHttpsResponse_free(BearHttpsResponse *self){
    private_BearHttpsHeadders_free(self->headders);
    if(self->raw_content){
        free(self->raw_content);
    }
    free(self);

}
