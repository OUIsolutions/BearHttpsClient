
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


BearHttpsResponseNamespace newBearHttpsResponseNamespace(){
    BearHttpsResponseNamespace self = {0};
    self.read_body_chunck = BearHttpsResponse_read_body_chunck;
    self.read_body = BearHttpsResponse_read_body;
    self.read_body_str = BearHttpsResponse_read_body_str;
    self.get_status_code = BearHttpsResponse_get_status_code;
    self.get_body_size = BearHttpsResponse_get_body_size;
    self.get_headers_size = BearHttpsResponse_get_headers_size;
    self.get_header_value_by_index = BearHttpsResponse_get_header_value_by_index;
    self.get_header_value_by_key = BearHttpsResponse_get_header_value_by_key;
    self.get_header_key_by_index = BearHttpsResponse_get_header_key_by_index;
    self.get_header_value_by_sanitized_key = BearHttpsResponse_get_header_value_by_sanitized_key;
    self.set_max_body_size = BearHttpsResponse_set_max_body_size;
    self.set_body_read_props = BearHttpsResponse_set_body_read_props;
    self.error = BearHttpsResponse_error;
    self.get_error_msg = BearHttpsResponse_get_error_msg;
    self.get_error_code = BearHttpsResponse_get_error_code;
    self.free = BearHttpsResponse_free;
    #ifndef BEARSSL_HTTPS_MOCK_CJSON
    self.read_body_json = BearHttpsResponse_read_body_json;
    #endif
    return self;
    
}