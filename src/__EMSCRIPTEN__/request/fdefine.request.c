//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if defined(__EMSCRIPTEN__)

BearHttpsRequest * newBearHttpsRequest_with_url_ownership_config(char *url,short url_ownership_mode){
    c2wasm_start();
    BearHttpsRequest *self = (BearHttpsRequest *)malloc(sizeof(BearHttpsRequest));
    *self = (BearHttpsRequest){0};
    BearHttpsRequest_set_url_with_ownership_config(self,url,url_ownership_mode);
    self->headers = private_newBearHttpsHeaders();
    self->body_type =PRIVATE_BEARSSL_NO_BODY;
    private_BearsslHttps_strcpy(self->method,"GET");
    return self;
}
#endif 