//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if defined(__EMSCRIPTEN__) 
#include <unistd.h>
BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self){

    c2wasm_js_var headers = c2wasm_create_object();

    for(int i = 0; i < self->headers->size;i++){
        private_BearHttpsKeyVal *key_val = self->headers->keyvals[i];
        c2wasm_set_object_prop_string(headers, key_val->key, key_val->value);
    }
    c2wasm_js_var props = c2wasm_create_object();
    c2wasm_set_object_prop_string(props, "method", self->method);
    c2wasm_set_object_prop_any(props, "headers", headers);

    c2wasm_js_var args_to_cal = c2wasm_create_array();
    c2wasm_append_array_string(args_to_cal, self->url);
    c2wasm_append_array_any(args_to_cal, props);
    BearHttpsResponse *response = private_newBearHttpsResponse();

    c2wasm_js_var js_response = await_c2wasm_call_object_prop(c2wasm_window, "fetch", args_to_cal);
    if(c2wasm_instance_of(js_response,c2wasm_error)){
        BearHttpsResponse_set_error(response,"Error performing fetch",1);
        return response;
    }

    response->status_code = (int)c2wasm_get_object_prop_long(js_response, "status");
    c2wasm_js_var js_headers = c2wasm_get_object_prop_any(js_response, "headers");
    c2wasm_js_var entries = c2wasm_call_object_prop(js_headers, "entries", -1);
    c2wasm_js_var array = c2wasm_get_object_prop_any(c2wasm_window,"Array");
    c2wasm_js_var entries_array = c2wasm_call_object_prop(array,"from", entries);
    long size = c2wasm_get_array_size(entries_array);
    for (long i = 0; i < size; i++) {
        c2wasm_js_var entry = c2wasm_get_array_any_by_index(entries_array, i);
        private_BearHttpsKeyVal * key_obj = private_newBearHttpsKeyVal();
        int key_size = c2wasm_get_array_string_size_by_index(entry,0);
        char *key = (char *)malloc(key_size + 2);
        c2wasm_array_memcpy_string(entry,0,0,key,key_size);
        key[key_size] = '\0';


        int value_size = c2wasm_get_array_string_size_by_index(entry,1);
        char *value = (char *)malloc(value_size + 2);
        value[value_size] = '\0';
        c2wasm_array_memcpy_string(entry,1,0,value,value_size);
        private_BearHttpsKeyVal_set_key(key_obj,key,BEARSSL_HTTPS_GET_OWNERSHIP);
        private_BearHttpsKeyVal_set_value(key_obj,value,BEARSSL_HTTPS_GET_OWNERSHIP);
        private_BearHttpsHeaders_add_keyval(response->headers,key_obj);
    }
    
    
    return response;
}
#endif 