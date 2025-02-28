
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

BearHttpsNamespace newBearHttpsNamespace(){
    BearHttpsNamespace self = {0};
    self.response = newBearHttpsResponseNamespace();
    self.request= newBearHttpsRequestNamespace();
    return self;
}
