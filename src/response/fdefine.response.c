
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


BearHttpsResponse *private_newBearHttpsResponse(){
    BearHttpsResponse *self = (BearHttpsResponse *)malloc(sizeof(BearHttpsResponse));
    self->status_code = 0;
    self->headders = private_newBearHttpsHeadders();
    self->body = NULL;
    return response;
}

int BearHttpsResponse_get_status_code(BearHttpsResponse*self){
    return self->status_code;
}
