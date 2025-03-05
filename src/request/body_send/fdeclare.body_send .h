
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.consts.h"
//silver_chain_scope_end

void private_BearHttpsRequest_free_body(BearHttpsRequest *self);

void BearHttpsRequest_send_any_with_ownership_control(BearHttpsRequest *self,unsigned char *content, long size,short ownership_mode);

void BearHttpsRequest_send_any(BearHttpsRequest *self,unsigned char *content, long size);


void BearHttpsRequest_send_body_str_with_ownership_control(BearHttpsRequest *self, char *content,short ownership_mode);


void BearHttpsRequest_send_body_str(BearHttpsRequest *self, char *content);

void BearHttpsRequest_send_file_with_ownership_control(BearHttpsRequest *self, char *path,short ownership_mode,const char *content_type);

void BearHttpsRequest_send_file(BearHttpsRequest *self,const  char *path,const char *content_type);

void BearHttpsRequest_send_file_auto_detect_content_type(BearHttpsRequest *self, const char *path);


#ifndef BEARSSL_HTTPS_MOCK_CJSON

void BearHttpsRequest_send_cJSON_with_ownership_control(BearHttpsRequest *self,cJSON *json,short ownership_mode);

void BearHttpsRequest_send_cJSON(BearHttpsRequest *self,cJSON *json);

cJSON * BearHttpsRequest_create_cJSONPayloadObject(BearHttpsRequest *self);

cJSON * BearHttpsRequest_create_cJSONPayloadArray(BearHttpsRequest *self);


#endif