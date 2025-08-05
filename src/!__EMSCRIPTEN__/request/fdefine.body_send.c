//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)


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



#endif