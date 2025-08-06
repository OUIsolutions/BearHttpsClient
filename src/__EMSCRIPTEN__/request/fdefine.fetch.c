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
    c2wasm_show_var_on_console(js_response);

    response->status_code = (int)c2wasm_get_object_prop_long(js_response, "status");
    c2wasm_js_var js_headers = c2wasm_get_object_prop_any(js_response, "headers");
    c2wasm_js_var entries = c2wasm_call_object_prop(js_headers, "entries", -1);
    c2wasm_js_var array = c2wasm_get_object_prop_any(c2wasm_window,"Array");
    c2wasm_js_var entries_array = c2wasm_call_object_prop(array,"from", entries);


    

    return response;
}
#endif 