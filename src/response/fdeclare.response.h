
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesD.h"
//silver_chain_scope_end

BearHttpsResponse *private_newBearHttpsResponse();

int BearHttpsResponse_get_status_code(BearHttpsResponse*self);

bool BearHttpsResponse_error(BearHttpsResponse*self);

char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self);

bool BearHttpsResponse_is_body_str(BearHttpsResponse*self);

char * BearHttpsResponse_get_body_str(BearHttpsResponse*self);

void BearHttpsResponse_free(BearHttpsResponse *self);
