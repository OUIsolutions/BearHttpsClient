
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesD.h"
//silver_chain_scope_end

BearHttpsResponse *private_newBearHttpsResponse();


void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self);

int BearHttpsResponse_get_status_code(BearHttpsResponse*self);

bool BearHttpsResponse_error(BearHttpsResponse*self);

char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self);

void BearHttpsResponse_set_error_msg(BearHttpsResponse*self,const char *msg);

void BearHttpsResponse_free(BearHttpsResponse *self);
