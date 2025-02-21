
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


BearHttpsResponse *private_newBearHttpsResponse(){
    BearHttpsResponse *self = (BearHttpsResponse *)malloc(sizeof(BearHttpsResponse));
    self->status_code = 0;
    self->headders = private_newBearHttpsHeadders();
    self->body = NULL;
    return self;
}

int BearHttpsResponse_get_status_code(BearHttpsResponse*self){
    return self->status_code;
}

bool BearHttpsResponse_error(BearHttpsResponse*self){
    return self->error_msg != NULL;
}

void BearHttpsResponse_set_error_msg(BearHttpsResponse*self,const char *msg){
    self->error_msg = private_BearsslHttps_strdup(msg);
}

char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self){
    return self->error_msg;
}
