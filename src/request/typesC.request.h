
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesB.h"
#include <stdbool.h>
//silver_chain_scope_end

typedef struct BearHttpsRequest{
    char *route;
    bool route_owner;
    BearHttpsHeadders *headders;

}BearHttpsRequest ;


BearHttpsRequest * newBearHttpsRequest_with_ownership_config(char *route,bool route_owner);

BearHttpsRequest * newBearHttpsRequest_route_by_copy(char *route);

void BearHttpsRequest_set_headder_with_ownership_config(BearHttpsRequest *self ,char *key,bool key_owner,char *value,bool value_owner);


void BearHttpsRequest_set_headder(BearHttpsRequest *self ,char *key,char *value);

void BearHttpsRequest_set_body_with_onwership_config(BearHttpsRequest *self ,unsigned char *body,long size,bool body_owner);

void BearHttpsRequest_set_body(BearHttpsRequest *self ,unsigned char *body,long size);

void BearHttpsRequest_set_body_file_stream(BearHttpsRequest *self ,const char *path);
