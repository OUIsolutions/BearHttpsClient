
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


BearHttpsResponse *private_newBearHttpsResponse(){
    BearHttpsResponse *self = (BearHttpsResponse *)malloc(sizeof(BearHttpsResponse));
    *self = (BearHttpsResponse){0};
    self->status_code = 0;
    self->headders = private_newBearHttpsHeadders();
    self->max_body_size = -1;
    self->body_chunk_size = BEARSSL_BODY_CHUNK_SIZE;
    self->body_realloc_factor = BEARSSL_BODY_REALLOC_FACTOR;
    self->body = NULL;
    return self;
}




void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self, const char *hostname,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size) {
    self->is_https = true;
    if(trust_anchors && trusted_anchors_size > 0){
       br_ssl_client_init_full(&self->ssl_client, &self->certification_context, trust_anchors, trusted_anchors_size);
    }
    else{
       br_ssl_client_init_full(&self->ssl_client, &self->certification_context, TAs, TAs_NUM);

    }
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
    if(BearHttpsResponse_error(self)){
        return -1;
    }
    return self->status_code;
}

int BearHttpsResponse_get_body_size(BearHttpsResponse*self){
    return self->body_size;
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
    Universal_close(self->connection_file_descriptor);
    if(self->is_https){
        br_ssl_client_zero(&self->ssl_client);
    }
    private_BearHttpsHeadders_free(self->headders);
    if(self->raw_content){
        free(self->raw_content);
    }
    #ifndef BEARSSL_HTTPS_MOCK_CJSON
    if(self->json_body){
        cJSON_Delete(self->json_body);
    }
    #endif
    if(self->body){
        free(self->body);
    }
    if(self->error_msg){
        free(self->error_msg);
    }

    free(self);

}
int BearHttpsResponse_get_headders_size(BearHttpsResponse*self){
    return self->headders->size;
}

char* BearHttpsResponse_get_headder_value_by_index(BearHttpsResponse*self,int index){
    private_BearHttpsKeyVal * key_vall = private_BearHttpsHeadders_get_key_val_by_index(self->headders,index);
    if(key_vall == NULL){
        return NULL;
    }
    return key_vall->value;
}
char* BearHttpsResponse_get_headder_key_by_index(BearHttpsResponse*self,int index){
    private_BearHttpsKeyVal * key_vall = private_BearHttpsHeadders_get_key_val_by_index(self->headders,index);
    if(key_vall == NULL){
        return NULL;
    }
    return key_vall->key;
}
char* BearHttpsResponse_get_headder_value_by_key(BearHttpsResponse*self,const char *key){
    for(int i = 0; i < self->headders->size;i++){
        private_BearHttpsKeyVal * current_key_val = self->headders->keyvals[i];
        if(private_BearsslHttp_strcmp(current_key_val->key,key) == 0){
            return current_key_val->value;
        }
    }
    return NULL;
}

void BearHttpsResponse_set_max_body_size(BearHttpsResponse*self,long size){
    self->max_body_size = size;
}

void BearHttpsResponse_set_body_read_props(BearHttpsResponse*self,int chunk_size,double realloc_factor){
    self->body_chunk_size = chunk_size;
    self->body_realloc_factor = realloc_factor;
}

char* BearHttpsResponse_get_headder_value_by_sanitized_key(BearHttpsResponse*self,const char *key){
    long key_size = private_BearsslHttps_strlen(key);
    for(int i = 0; i < self->headders->size;i++){
        private_BearHttpsKeyVal * current_key_val = self->headders->keyvals[i];
        if(private_BearHttps_is_sanitize_key(current_key_val->key,key,key_size)){
            return current_key_val->value;
        }
    }
    return NULL;
}
