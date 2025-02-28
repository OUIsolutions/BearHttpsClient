
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end



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


}

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
            self->body_raw.onwer = false;
            self->body_raw.size = size;
    }
    if(ownership_mode == BEARSSL_HTTPS_REFERENCE){
        self->body_raw.value = content;
        self->body_raw.onwer = true;
        self->body_raw.size = size;
    }

}
void BearHttpsRequest_send_any(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode){
    BearHttpsRequest_send_any_with_ownership_control(self,content,size,BEARSSL_DEFAULT_STRATEGY);
}

void BearHttpsRequest_send_str_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode){
    BearHttpsRequest_send_any_with_ownership_control(self,(unsigned char *)content,strlen(content),ownership_mode);
}


void BearHttpsRequest_send_str(BearHttpsRequest *self, char *content,short ownership_mode){
    BearHttpsRequest_send_any_with_ownership_control(self,(unsigned char *)content,strlen(content),BEARSSL_DEFAULT_STRATEGY);
}

void BearHttpsRequest_send_file_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode){
   private_BearHttpsRequest_free_body(self);
    private_BearsslHttps_set_str_considering_ownership(
        &self->body_file.path,
        content,
        &self->body_file.onwer,
        ownership_mode);
    self->body_type = PRIVATE_BEARSSL_BODY_FILE;
}

void BearHttpsRequest_send_file(BearHttpsRequest *self, char *content,short ownership_mode){
    BearHttpsRequest_send_file_with_ownership_control(self,content,BEARSSL_DEFAULT_STRATEGY);
}