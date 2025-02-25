
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesD.h"
//silver_chain_scope_end

BearHttpsResponse *private_newBearHttpsResponse();

int private_BearHttpsResponse_read_chunck_raw(BearHttpsResponse *self,unsigned char *bufer,long size);

int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *bufer,long size);

int private_BearHttpsResponse_write(BearHttpsResponse *self,unsigned char *bufer,long size);

void private_BearHttpsResponse_read_til_end_of_headders_or_reach_limit(
    BearHttpsResponse *self,
    int chunk,
    double factor_headders_growth
);


void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self,const char *hostname);

int BearHttpsResponse_get_status_code(BearHttpsResponse*self);

bool BearHttpsResponse_error(BearHttpsResponse*self);

char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self);

void BearHttpsResponse_set_error_msg(BearHttpsResponse*self,const char *msg);

void BearHttpsResponse_free(BearHttpsResponse *self);
