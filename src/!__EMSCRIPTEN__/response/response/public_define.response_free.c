
//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)




void BearHttpsResponse_free(BearHttpsResponse *self){
    Universal_close(self->connection_file_descriptor);
    if(self->is_https){
        br_ssl_client_zero(&self->ssl_client);
    }
    private_BearHttpsHeaders_free(self->headers);
    if(self->raw_content){
        free(self->raw_content);
    }
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