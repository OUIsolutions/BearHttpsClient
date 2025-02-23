
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
    if(self->is_https){
      return br_sslio_write_all(&self->ssl_io, bufer, size);
    }
    return Universal_write(self->connection_file_descriptor, bufer, size);
}

int private_BearHttpsResponse_read(BearHttpsResponse *self,unsigned char *bufer,long size){
    if(self->is_https){
      return br_sslio_read(&self->ssl_io, bufer, size);
    }
    return Universal_read(self->connection_file_descriptor, bufer, size);
}


void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self, const char *hostname) {
    self->is_https = true;
    br_ssl_client_context ssl_client;
    br_x509_minimal_context certification_context;
    br_ssl_client_init_full(&ssl_client, &certification_context, TAs, TAs_NUM);
    br_ssl_engine_set_all_flags(&ssl_client.eng, BR_OPT_TOLERATE_NO_CLIENT_AUTH);
    
    unsigned char buffer[BR_SSL_BUFSIZE_BIDI];
    br_ssl_engine_set_buffer(&ssl_client.eng, buffer, sizeof buffer, 1);
    
    br_ssl_client_reset(&ssl_client, hostname, 0);
    br_sslio_init(&self->ssl_io, &ssl_client.eng, private_BearHttps_sock_read, 
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
