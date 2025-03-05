
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
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
    #ifndef BEARSSL_HTTPS_MOCK_CJSON

    if(self->body_type == PRIVATE_BEARSSL_BODY_JSON){
        if(self->body_json.onwer){
            cJSON_Delete(self->body_json.json);
        }
    

    }
    #endif
    self->body_type = PRIVATE_BEARSSL_NO_BODY;

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

void BearHttpsRequest_send_file_with_ownership_control(BearHttpsRequest *self, char *path,short ownership_mode,const char *content_type){
   private_BearHttpsRequest_free_body(self);
    private_BearsslHttps_set_str_considering_ownership(
        &self->body_file.path,
        path,
        &self->body_file.onwer,
        ownership_mode);
    private_BearsslHttps_strcpy(self->body_file.content_type,content_type);
    self->body_type = PRIVATE_BEARSSL_BODY_FILE;
}

void BearHttpsRequest_send_file(BearHttpsRequest *self,const  char *path,const char *content_type){
    BearHttpsRequest_send_file_with_ownership_control(self,(char*)path,BEARSSL_DEFAULT_STRATEGY,content_type);
}

void BearHttpsRequest_send_file_auto_detect_content_type(BearHttpsRequest *self,const  char *path){

    char extension[100] = {0};
    bool found = false;
    long path_size = private_BearsslHttps_strlen(path);
    //iterate in negative from path, to count the extension
    for(int i = path_size-1; i >= 0; i--){
        if(path[i] == '.'){
            //verify if the extension is bigger than 100
            if(path_size - i > 100){
                break;
            }
            found = true;
            private_BearsslHttps_strcpy(extension,&path[i+1]);
            break;
        }
    }
    if(!found){
        BearHttpsRequest_send_file(self,path,"application/octet-stream");
        return;
    }
    const char *content_type = "application/octet-stream";
    if(private_BearsslHttp_strcmp(extension,"txt") == 0){
        content_type = "text/plain";
    }
    else if(private_BearsslHttp_strcmp(extension,"png") == 0){
        content_type = "image/png";
    }
    else if(private_BearsslHttp_strcmp(extension,"jpg") == 0){
        content_type = "image/jpg";
    }
    else if(private_BearsslHttp_strcmp(extension,"jpeg") == 0){
        content_type = "image/jpeg";
    }
    else if(private_BearsslHttp_strcmp(extension,"gif") == 0){
        content_type = "image/gif";
    }
    else if(private_BearsslHttp_strcmp(extension,"pdf") == 0){
        content_type = "application/pdf";
    }
    else if(private_BearsslHttp_strcmp(extension,"html") == 0){
        content_type = "text/html";
    }
    else if(private_BearsslHttp_strcmp(extension,"css") == 0){
        content_type = "text/css";
    }
    else if(private_BearsslHttp_strcmp(extension,"js") == 0){
        content_type = "application/javascript";
    }
    else if(private_BearsslHttp_strcmp(extension,"json") == 0){
        content_type = "application/json";
    }
    else if(private_BearsslHttp_strcmp(extension,"xml") == 0){
        content_type = "application/xml";
    }    

    BearHttpsRequest_send_file(self,path,content_type);

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