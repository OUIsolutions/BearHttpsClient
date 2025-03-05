
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end

BearHttpsRequest * newBearHttpsRequest_with_url_ownership_config(char *url,short url_ownership_mode){
    BearHttpsRequest *self = (BearHttpsRequest *)malloc(sizeof(BearHttpsRequest));
    *self = (BearHttpsRequest){0};
    self->max_redirections = BEARSSL_MAX_REDIRECTIONS;
    BearHttpsRequest_set_url_with_ownership_config(self,url,url_ownership_mode);
    self->headers = private_newBearHttpsHeaders();
    self->body_type =PRIVATE_BEARSSL_NO_BODY;
    self->header_chunk_read_size = BEARSSL_HEADER_CHUNK;
    self->header_chunk_reallocator_factor = BEARSSL_HEADER_REALLOC_FACTOR;
    private_BearsslHttps_strcpy(self->method,"GET");
    return self;
}

BearHttpsRequest * newBearHttpsRequest(const char *url){
    return newBearHttpsRequest_with_url_ownership_config((char*)url,BEARSSL_DEFAULT_STRATEGY);
}

BearHttpsRequest * newBearHttpsRequest_fmt(const char *url,...){
    va_list args;
    va_start(args,url);

    char *formmated = private_BearHttps_format_vaarg(url,args);
    va_end(args);
    if(formmated == NULL){
        return NULL;
    }
    return newBearHttpsRequest_with_url_ownership_config(formmated,BEARSSL_HTTPS_GET_OWNERSHIP);
}

void BearHttpsRequest_set_url_with_ownership_config(BearHttpsRequest *self , char *url,short url_ownership_mode){
    private_BearsslHttps_set_str_considering_ownership(&self->url,url,&self->route_owner,url_ownership_mode);

}

void BearHttpsRequest_set_url(BearHttpsRequest *self ,const char *url){
    BearHttpsRequest_set_url_with_ownership_config(self,(char*)url,BEARSSL_DEFAULT_STRATEGY);
}


void BearHttpsRequest_add_header_with_ownership_config(BearHttpsRequest *self ,char *key,short key_ownership_mode,char *value,short value_owner){
    //verify if the key already exists
    for(int i = 0; i < self->headers->size;i++){
        private_BearHttpsKeyVal *key_val = self->headers->keyvals[i];
        if(private_BearsslHttp_strcmp(key_val->key,key) == 0){
            private_BearHttpsKeyVal_set_value(key_val,value,value_owner);
            return;
        }
    }

    private_BearHttpsKeyVal * key_obj = private_newBearHttpsKeyVal();
    private_BearHttpsKeyVal_set_key(key_obj,key,key_ownership_mode);
    private_BearHttpsKeyVal_set_value(key_obj,value,value_owner);
    private_BearHttpsHeaders_add_keyval(self->headers,key_obj);
}

void BearHttpsRequest_add_header(BearHttpsRequest *self ,char *key,char *value){
    BearHttpsRequest_add_header_with_ownership_config(self,key,BEARSSL_DEFAULT_STRATEGY,value,BEARSSL_DEFAULT_STRATEGY);
}

void BearHttpsRequest_add_header_fmt(BearHttpsRequest *self ,char *key,char *format,...){
    va_list args;
    va_start(args,format);
    char *formmated = private_BearHttps_format_vaarg(format,args);
    va_end(args);
    if(formmated == NULL){
        return;
    }
    BearHttpsRequest_add_header_with_ownership_config(self,key,BEARSSL_DEFAULT_STRATEGY,formmated,BEARSSL_HTTPS_GET_OWNERSHIP);
}

void BearHttpsRequest_set_method(BearHttpsRequest *self ,const char *method){
    private_BearsslHttps_strcpy(self->method,self->method);
}

void BearHttpsRequest_represent(BearHttpsRequest *self){
    printf("Route: %s\n",self->url);
    printf("Method: %s\n",self->method);
    printf("Headers:\n");
    for(int i = 0; i < self->headers->size;i++){
        private_BearHttpsKeyVal *key_val = self->headers->keyvals[i];
        printf("\t%s: %s\n",key_val->key,key_val->value);
    }
    if(self->body_type == PRIVATE_BEARSSL_BODY_RAW){
        printf("Body: %s\n",self->body_raw.value);
    }
    if(self->body_type == PRIVATE_BEARSSL_BODY_FILE){
        printf("Body-File: %s\n",self->body_file.path);
    }
}


void BearHttpsRequest_set_max_redirections(BearHttpsRequest *self ,int max_redirections){
    self->max_redirections = max_redirections;
}

void BearHttpsRequest_set_dns_providers(BearHttpsRequest *self ,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers){
    self->dns_providers= dns_providers;
    self->total_dns_providers = total_dns_proviers;
}

void BearHttpsRequest_set_chunk_header_read_props(BearHttpsRequest *self ,int chunk_size,int max_chunk_size){
    self->header_chunk_read_size = chunk_size;
    self->header_chunk_reallocator_factor = max_chunk_size;
}

void BearHttpsRequest_set_trusted_anchors(BearHttpsRequest *self ,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size){
    self->trust_anchors = trust_anchors;
    self->trusted_anchors_size = trusted_anchors_size;
}

void BearHttpsRequest_free(BearHttpsRequest *self){
    private_BearHttpsRequest_free_body(self);
    private_BearHttpsHeaders_free(self->headers);
    private_BearsslHttps_free_considering_ownership((void **)&self->url,&self->route_owner);
    free(self);
}
