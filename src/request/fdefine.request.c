
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

BearHttpsRequest * newBearHttpsRequest_with_ownership_config(char *route,short route_ownernership_mode){
    BearHttpsRequest *self = (BearHttpsRequest *)malloc(sizeof(BearHttpsRequest));
    self->route = route;
    self->route_ownernership_mode = route_ownernership_mode;
    self->headders = newBearHttpsKeyVal();
    self->method = "GET";
    self->method_owner = false;

    return self;
}

BearHttpsRequest * newBearHttpsRequest(char *route){
    return newBearHttpsRequest_with_ownership_config(route,BEARSSL_HTTPS_GET_OWNERSHIP);
}

void BearHttpsRequest_set_headder_with_ownership_config(BearHttpsRequest *self ,char *key,short key_ownership_mode,char *value,short value_owner);

void BearHttpsRequest_set_headder(BearHttpsRequest *self ,char *key,char *value);

void BearHttpsRequest_set_body_with_onwership_config(BearHttpsRequest *self ,unsigned char *body,long size,short body_ownership_mode);

void BearHttpsRequest_set_body(BearHttpsRequest *self ,unsigned char *body,long size);

void BearHttpsRequest_set_body_file_stream(BearHttpsRequest *self ,const char *path);

void BearHttpsRequest_set_method(BearHttpsRequest *self ,const char *method);

void BearHttpsRequest_represent(BearHttpsRequest *self);

void BearHttpsRequest_free(BearHttpsRequest *self);