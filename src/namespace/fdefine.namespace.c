
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


BearHttpsNamespace newBearHttpsNamespace(){
    BearHttpsNamespace self = {0};
    self.REFERENCE = BEARSSL_HTTPS_REFERENCE;
    self.GET_OWNERSHIP = BEARSSL_HTTPS_GET_OWNERSHIP;
    self.COPY = BEARSSL_HTTPS_COPY;
    self.response = newBearHttpsResponseNamespace();
    self.request= newBearHttpsRequestNamespace();
    return self;
}
