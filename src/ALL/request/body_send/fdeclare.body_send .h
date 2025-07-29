//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



void private_BearHttpsRequest_free_body(BearHttpsRequest *self);

void BearHttpsRequest_send_any_with_ownership_control(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode);

void BearHttpsRequest_send_any(BearHttpsRequest *self,unsigned char *content, long size);


void BearHttpsRequest_send_body_str_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode);


void BearHttpsRequest_send_body_str(BearHttpsRequest *self, char *content);





#ifndef BEARSSL_HTTPS_MOCK_CJSON

void BearHttpsRequest_send_cJSON_with_ownership_control(BearHttpsRequest *self,cJSON *json,short ownership_mode);

void BearHttpsRequest_send_cJSON(BearHttpsRequest *self,cJSON *json);

cJSON * BearHttpsRequest_create_cJSONPayloadObject(BearHttpsRequest *self);

cJSON * BearHttpsRequest_create_cJSONPayloadArray(BearHttpsRequest *self);


#endif
