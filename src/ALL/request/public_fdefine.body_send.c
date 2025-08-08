





void BearHttpsRequest_send_any_with_ownership_control(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode){
   private_BearHttpsRequest_free_body(self);
   self->body_type = PRIVATE_BEARSSL_BODY_RAW;
    if(ownership_mode == BEARSSL_HTTPS_COPY){
         self->body_raw.value = (unsigned char *)malloc(size+2);
         for(int i = 0; i < size; i++){
             self->body_raw.value[i] = content[i];
         }
         self->body_raw.value[size] = '\0';
         self->body_raw.onwer = true;
         self->body_raw.size = size;
    } 
    if(ownership_mode == BEARSSL_HTTPS_GET_OWNERSHIP){
            self->body_raw.value = content;
            self->body_raw.onwer = true;
            self->body_raw.size = size;
    }
    if(ownership_mode == BEARSSL_HTTPS_REFERENCE){
        self->body_raw.value = content;
        self->body_raw.onwer = false;
        self->body_raw.size = size;
    }

}
void BearHttpsRequest_send_any(BearHttpsRequest *self,unsigned char *content, long size){
    BearHttpsRequest_send_any_with_ownership_control(self,content,size,BEARSSL_DEFAULT_STRATEGY);
}

void BearHttpsRequest_send_body_str_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode){
    BearHttpsRequest_send_any_with_ownership_control(self,(unsigned char *)content,private_BearsslHttps_strlen(content),ownership_mode);
}


void BearHttpsRequest_send_body_str(BearHttpsRequest *self, char *content){
    BearHttpsRequest_send_any_with_ownership_control(self,(unsigned char *)content,private_BearsslHttps_strlen(content),BEARSSL_DEFAULT_STRATEGY);
}


#ifndef BEARSSL_HTTPS_MOCK_CJSON

void BearHttpsRequest_send_cJSON_with_ownership_control(BearHttpsRequest *self,cJSON *json,short ownership_mode){
    private_BearHttpsRequest_free_body(self);
    self->body_type = PRIVATE_BEARSSL_BODY_JSON;
    if(ownership_mode == BEARSSL_HTTPS_COPY){
        self->body_json.json = cJSON_Duplicate(json,true);
        self->body_json.onwer = true;

    }
    if(ownership_mode == BEARSSL_HTTPS_GET_OWNERSHIP){
        self->body_json.json = json;
        self->body_json.onwer = true;
    }
    if(ownership_mode == BEARSSL_HTTPS_REFERENCE){
        self->body_json.json = json;
        self->body_json.onwer = false;
    }
}
void BearHttpsRequest_send_cJSON(BearHttpsRequest *self,cJSON *json){
    BearHttpsRequest_send_cJSON_with_ownership_control(self,json,BEARSSL_DEFAULT_STRATEGY);
}

cJSON * BearHttpsRequest_create_cJSONPayloadObject(BearHttpsRequest *self){
    cJSON *json = cJSON_CreateObject();
    BearHttpsRequest_send_cJSON_with_ownership_control(self,json,BEARSSL_HTTPS_GET_OWNERSHIP);
    return json;
}


cJSON * BearHttpsRequest_create_cJSONPayloadArray(BearHttpsRequest *self){
    cJSON *json = cJSON_CreateArray();
    BearHttpsRequest_send_cJSON_with_ownership_control(self,json,BEARSSL_HTTPS_GET_OWNERSHIP);
    return json;
}


#endif
