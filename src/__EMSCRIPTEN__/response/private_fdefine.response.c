//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.public_fdefine.h"
//silver_chain_scope_end
#if defined(__EMSCRIPTEN__) 

BearHttpsResponse *private_newBearHttpsResponse(){
    BearHttpsResponse *self = (BearHttpsResponse *)malloc(sizeof(BearHttpsResponse));
    *self = (BearHttpsResponse){0};
    self->headers = private_newBearHttpsHeaders();

    return self;
}


void BearHttpsResponse_free(BearHttpsResponse *self){
    if(!self){
        return;
    }
    private_BearHttpsHeaders_free(self->headers);
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


#endif 