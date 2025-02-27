
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.typesD.h"
//silver_chain_scope_end



void private_BearHttpsRequest_free_body(BearHttpsRequest *self);

void BearHttpsRequest_send_any_with_ownership_control(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode);

void BearHttpsRequest_send_str_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode);
