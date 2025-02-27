
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

BearHttpsRequest * newBearHttpsRequest_with_ownership_config(char *url,short route_ownernership_mode){
    BearHttpsRequest *self = (BearHttpsRequest *)malloc(sizeof(BearHttpsRequest));
    *self = (BearHttpsRequest){0};
    self->max_redirections = BEARSSL_MAX_REDIRECTIONS;
    BearHttpsRequest_set_url_with_ownership_config(self,url,route_ownernership_mode);
    self->headders = private_newBearHttpsHeadders();
    self->body_type =PRIVATE_BEARSSL_HTTPS_BODY_UNDEFINED;
    self->headder_chunk_read_size = BEARSSL_HEADDER_CHUNK;
    self->headder_chunk_reallocator_facctor = BEARSSL_HEADDER_REALLOC_FACTOR;
    private_BearsslHttps_strcpy(self->method,"GET");
    return self;
}

BearHttpsRequest * newBearHttpsRequest(const char *url){
    return newBearHttpsRequest_with_ownership_config((char*)url,BEARSSL_DEFAULT_STRATEGY);
}

void BearHttpsRequest_set_url_with_ownership_config(BearHttpsRequest *self , char *url,short url_ownership_mode){
    private_BearsslHttps_set_str_considering_ownership(&self->url,url,&self->route_owner,url_ownership_mode);

}

void BearHttpsRequest_set_url(BearHttpsRequest *self ,const char *url){
    BearHttpsRequest_set_url_with_ownership_config(self,(char*)url,BEARSSL_DEFAULT_STRATEGY);
}


void BearHttpsRequest_add_headder_with_ownership_config(BearHttpsRequest *self ,char *key,short key_ownership_mode,char *value,short value_owner){
    private_BearHttpsKeyVal * key_obj = private_newBearHttpsKeyVal();
    private_BearHttpsKeyVal_set_key(key_obj,key,key_ownership_mode);
    private_BearHttpsKeyVal_set_value(key_obj,value,value_owner);
    private_BearHttpsHeadders_add_keyval(self->headders,key_obj);
}

void BearHttpsRequest_add_headder(BearHttpsRequest *self ,char *key,char *value){
    BearHttpsRequest_add_headder_with_ownership_config(self,key,BEARSSL_DEFAULT_STRATEGY,value,BEARSSL_DEFAULT_STRATEGY);
}

void BearHttpsRequest_set_body_with_onwership_config(BearHttpsRequest *self ,unsigned char *body,long size,short body_ownership_mode){

    BearHttpsRequest_free_body(self);
    self->body_type = PRIVATE_BEARSSL_HTTPS_BODY_RAW;
    private_BearHttpsBodyRawRequest *body_raw_obj = &self->body_raw;
    if(body_ownership_mode == BEARSSL_HTTPS_COPY){
        body_raw_obj->value =(unsigned char*)BearsslHttps_allocate(size);;
        for(int i = 0; i < size;i++){
            body_raw_obj->value[i] = body[i];
        }
        body_raw_obj->onwer = true;
        return;
    }
     body_raw_obj->value = body;
     body_raw_obj->onwer = false;
     if(body_ownership_mode == BEARSSL_HTTPS_GET_OWNERSHIP){
         body_raw_obj->onwer = true;

     }
}


void BearHttpsRequest_set_body(BearHttpsRequest *self ,unsigned char *body,long size){
BearHttpsRequest_set_body_with_onwership_config(self,body,size,BEARSSL_DEFAULT_STRATEGY);
}

void BearHttpsRequest_set_body_file_stream_with_ownershio_config(BearHttpsRequest *self , char *path,short path_ownership_mode){
       BearHttpsRequest_free_body(self);
    private_BearHttpsBodyRequestFile *body_file_obj = &self->body_file;
    private_BearsslHttps_set_str_considering_ownership(&body_file_obj->path, path, &body_file_obj->onwer,path_ownership_mode);
}

void BearHttpsRequest_set_body_file_stream(BearHttpsRequest *self ,const char *path){
    BearHttpsRequest_set_body_file_stream_with_ownershio_config(self,(char*)path,BEARSSL_DEFAULT_STRATEGY);
}



void BearHttpsRequest_set_method(BearHttpsRequest *self ,const char *method){
    private_BearsslHttps_strcpy(self->method,self->method);
}

void BearHttpsRequest_represent(BearHttpsRequest *self){
    printf("Route: %s\n",self->url);
    printf("Method: %s\n",self->method);
    printf("Headders:\n");
    for(int i = 0; i < self->headders->size;i++){
        private_BearHttpsKeyVal *key_val = self->headders->keyvals[i];
        printf("\t%s: %s\n",key_val->key,key_val->value);
    }
    if(self->body_type == PRIVATE_BEARSSL_HTTPS_BODY_RAW){
        printf("Body: %s\n",self->body_raw.value);
    }
    if(self->body_type == PRIVATE_BEARSSL_HTTPS_BODY_FILE){
        printf("Body-File: %s\n",self->body_file.path);
    }
}

void BearHttpsRequest_free_body(BearHttpsRequest *self){
    if(self->body_type == PRIVATE_BEARSSL_HTTPS_BODY_RAW){
       private_BearsslHttps_free_considering_ownership((void **)&self->body_raw.value,&self->body_raw.onwer);
    }
    if(self->body_type == PRIVATE_BEARSSL_HTTPS_BODY_FILE){
        private_BearsslHttps_free_considering_ownership((void **)&self->body_file.path,&self->body_file.onwer);
    }
}

void BearHttpsRequest_free(BearHttpsRequest *self){
    BearHttpsRequest_free_body(self);
    private_BearHttpsHeadders_free(self->headders);
    private_BearsslHttps_free_considering_ownership((void **)&self->url,&self->route_owner);
    free(self);
}
