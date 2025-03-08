
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end




BearHttpsRequestNamespace newBearHttpsRequestNamespace(){
    BearHttpsRequestNamespace self = {0};
    self.send_any_with_ownership_control = BearHttpsRequest_send_any_with_ownership_control;
    self.send_any = BearHttpsRequest_send_any;
    self.send_body_str_with_ownership_control = BearHttpsRequest_send_body_str_with_ownership_control;
    self.send_body_str = BearHttpsRequest_send_body_str;
    self.send_file_with_ownership_control = BearHttpsRequest_send_file_with_ownership_control;
    self.send_file = BearHttpsRequest_send_file;
    self.send_file_auto_detect_content_type = BearHttpsRequest_send_file_auto_detect_content_type;
    self.fetch = BearHttpsRequest_fetch;
    #ifndef BEARSSL_HTTPS_MOCK_CJSON
        self.send_cJSON_with_ownership_control = BearHttpsRequest_send_cJSON_with_ownership_control;
        self.send_cJSON = BearHttpsRequest_send_cJSON;
        self.create_cJSONPayloadObject = BearHttpsRequest_create_cJSONPayloadObject;
        self.create_cJSONPayloadArray = BearHttpsRequest_create_cJSONPayloadArray;
    #endif
    self.newBearHttpsRequest_with_url_ownership_config = newBearHttpsRequest_with_url_ownership_config;
    self.newBearHttpsRequest =newBearHttpsRequest;
    self.newBearHttpsRequest_fmt = newBearHttpsRequest_fmt;
    self.set_url_with_ownership_config = BearHttpsRequest_set_url_with_ownership_config;
    self.set_url = BearHttpsRequest_set_url;
    self.add_header_with_ownership_config = BearHttpsRequest_add_header_with_ownership_config;
    self.add_header = BearHttpsRequest_add_header;
    self.set_method = BearHttpsRequest_set_method;
    self.set_max_redirections = BearHttpsRequest_set_max_redirections;
    self.set_dns_providers = BearHttpsRequest_set_dns_providers;
    self.set_chunk_header_read_props = BearHttpsRequest_set_chunk_header_read_props;
    self.set_trusted_anchors = BearHttpsRequest_set_trusted_anchors;

    self.represent = BearHttpsRequest_represent;
    self.free = BearHttpsRequest_free;
    return self;

}