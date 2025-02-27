
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end



void BearHttpsRequest_clear_body(BearHttpsRequest *self){
    if(self->body_typ == PRIVATE_BEARSSL_BODY_RAW){
        free(self->body.raw);
    }
}

unsigned char *BearHttpsRequest_send_any_with_ownership_control(BearHttpsResponse *self,unsigned char *content, long max_size,short ownership_mode){
    return NULL;
}
