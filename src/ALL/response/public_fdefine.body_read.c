
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