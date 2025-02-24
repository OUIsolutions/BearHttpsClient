
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
    return private_BearHttpsResponse_read_chunck_raw(self,buffer,size);
}

void private_BearHttpsResponse_read_til_end_of_headders_or_reach_limit(BearHttpsResponse *self,int headder){

    const int START_ALLOC_CONTENT = 100;
    const int CHUNK = START_ALLOC_CONTENT;
    const int FACTOR  = 2.5;
    self->content = malloc(START_ALLOC_CONTENT);
    self->content_allocated  = START_ALLOC_CONTENT;
    long content_size = 0;


    while(true){
        //apply the factor realloc
        while(content_size >= (self->content_allocated -1)){
            self->content_allocated = self->content_allocated * FACTOR;
            self->content = (unsigned char*)BearsslHttps_reallocate(self->content,self->content_allocated);
        }

        //we create a buff str , to allow 'append' into the content
        unsigned char *content_point = (self->content + content_size);
        int readded = private_BearHttpsResponse_read_chunck_raw(self,content_point, CHUNK);
        if(readded == 0){
            return;
        }
        if(readded <0){
            char error_buff[100] ={0};
            sprintf(error_buff,"invalid read code: %d",readded);
            BearHttpsResponse_set_error_msg(self,error_buff);
            return;
        }

        content_size+=readded;
        for(int i = 3; i < readded;i++){
            if(
                content_point[i-3] == '\r' &&
                content_point[i-2] == '\n' &&
                content_point[i-1] == '\r' &&
                content_point[i] == '\n' )
            {
                self->body = (content_point+i);
                self->body_size = readded;
                return;
            }
        }
    }

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
    if(self->content){
        free(self->content);
    }
    free(self);

}
