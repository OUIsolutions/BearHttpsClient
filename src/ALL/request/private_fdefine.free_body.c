

void private_BearHttpsRequest_free_body(BearHttpsRequest *self){
   
    if(self->body_type == PRIVATE_BEARSSL_BODY_RAW){
        private_BearsslHttps_free_considering_ownership(
            (void**)&self->body_raw.value,&self->body_raw.onwer);
    }
    if(self->body_type == PRIVATE_BEARSSL_BODY_FILE){
        private_BearsslHttps_free_considering_ownership(
            (void**)&self->body_file.path,
            &self->body_file.onwer
        );
    }
    #ifndef BEARSSL_HTTPS_MOCK_CJSON

    if(self->body_type == PRIVATE_BEARSSL_BODY_JSON){
        if(self->body_json.onwer){
            cJSON_Delete(self->body_json.json);
        }
    

    }
    #endif
    self->body_type = PRIVATE_BEARSSL_NO_BODY;

}