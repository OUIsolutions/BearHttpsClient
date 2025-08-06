//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if defined(__EMSCRIPTEN__) 

BearHttpsResponse *private_newBearHttpsResponse(){
    BearHttpsResponse *self = (BearHttpsResponse *)malloc(sizeof(BearHttpsResponse));
    *self = (BearHttpsResponse){0};
    self->headers = private_newBearHttpsHeaders();

    return self;
}
#endif 