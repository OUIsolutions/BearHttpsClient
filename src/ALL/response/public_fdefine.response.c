
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

void BearHttpsResponse_set_error(BearHttpsResponse*self,const char *msg,int error_code){
    self->error_msg = private_BearsslHttps_strdup(msg);
    self->error_code = error_code;
}

char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self){
    return self->error_msg;
}

int BearHttpsResponse_get_error_code(BearHttpsResponse*self){
    return self->error_code;
}

int BearHttpsResponse_get_headers_size(BearHttpsResponse*self){
    return self->headers->size;
}

char* BearHttpsResponse_get_header_value_by_index(BearHttpsResponse*self,int index){
    private_BearHttpsKeyVal * key_vall = private_BearHttpsHeaders_get_key_val_by_index(self->headers,index);
    if(key_vall == NULL){
        return NULL;
    }
    return key_vall->value;
}
char* BearHttpsResponse_get_header_key_by_index(BearHttpsResponse*self,int index){
    private_BearHttpsKeyVal * key_vall = private_BearHttpsHeaders_get_key_val_by_index(self->headers,index);
    if(key_vall == NULL){
        return NULL;
    }
    return key_vall->key;
}
char* BearHttpsResponse_get_header_value_by_key(BearHttpsResponse*self,const char *key){
    for(int i = 0; i < self->headers->size;i++){
        private_BearHttpsKeyVal * current_key_val = self->headers->keyvals[i];
        if(private_BearsslHttp_strcmp(current_key_val->key,key) == 0){
            return current_key_val->value;
        }
    }
    return NULL;
}

char* BearHttpsResponse_get_header_value_by_sanitized_key(BearHttpsResponse*self,const char *key){
    long key_size = private_BearsslHttps_strlen(key);
    for(int i = 0; i < self->headers->size;i++){
        private_BearHttpsKeyVal * current_key_val = self->headers->keyvals[i];
        if(private_BearHttps_is_sanitize_key(current_key_val->key,key,key_size)){
            return current_key_val->value;
        }
    }
    return NULL;
}
