//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if defined(__EMSCRIPTEN__) 

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

    c2wasm_js_var js_response = async_c2wasm_call_object_prop(c2wasm_window, "fetch", args_to_cal);
    BearHttpsResponse *response = private_newBearHttpsResponse();
    if(c2wasm_instance_of(js_response,c2wasm_error)){
        printf("its a error\n");
        return response;
    }
    printf("its not a error\n");
    c2wasm_show_var_on_console(js_response);
    return NULL;
}
#endif 