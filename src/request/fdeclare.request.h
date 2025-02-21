
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesD.h"
//silver_chain_scope_end


BearHttpsRequest * newBearHttpsRequest_with_ownership_config(char *route,short route_ownernership_mode);

BearHttpsRequest * newBearHttpsRequest(char *route);

void BearHttpsRequest_add_headder_with_ownership_config(BearHttpsRequest *self ,char *key,short key_ownership_mode,char *value,short value_owner);

void BearHttpsRequest_add_headder(BearHttpsRequest *self ,char *key,char *value);

void BearHttpsRequest_set_body_with_onwership_config(BearHttpsRequest *self ,unsigned char *body,long size,short body_ownership_mode);

void BearHttpsRequest_set_body(BearHttpsRequest *self ,unsigned char *body,long size);

void BearHttpsRequest_set_body_file_stream(BearHttpsRequest *self ,const char *path);

void BearHttpsRequest_set_body_file_stream_with_ownershio_config(BearHttpsRequest *self , char *path,short path_ownership_mode);

void BearHttpsRequest_set_method(BearHttpsRequest *self ,const char *method);

void BearHttpsRequest_represent(BearHttpsRequest *self);


void BearHttpsRequest_free(BearHttpsRequest *self);

void BearHttpsRequest_free_body(BearHttpsRequest *self);
