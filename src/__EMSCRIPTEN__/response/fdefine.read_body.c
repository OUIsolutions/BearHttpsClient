//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if defined(__EMSCRIPTEN__)

unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self) {
    if (!self || !self->response) {
        return NULL;
    }

    if (self->body) {
        return self->body;
    }
    c2wasm_js_var array_buffer = await_c2wasm_call_object_prop(self->response, "arrayBuffer", -1);
    if (c2wasm_instance_of(array_buffer, c2wasm_error)) {
        BearHttpsResponse_set_error(self, "Error reading response body", 1);
        return NULL;
    }

    long body_size = c2wasm_get_object_prop_long(array_buffer, "byteLength");
    printf("body size: %ld\n", body_size);
    if (body_size <= 0) {
        // Corpo vazio
        self->body = (unsigned char*)malloc(1);
        self->body[0] = '\0';
        self->body_size = 0;
        return self->body;
    }

    // Aloca memória para o corpo
    self->body = (unsigned char*)malloc(body_size + 1);
    if (!self->body) {
        BearHttpsResponse_set_error(self, "Memory allocation failed", 1);
        return NULL;
    }
// Cria um Uint8Array para acessar os bytes
    c2wasm_js_var uint8_array = c2wasm_call_object_constructor(c2wasm_window, "Uint8Array", array_buffer);

    // Copia os bytes do JavaScript para C
    for (long i = 0; i < body_size; i++) {
        self->body[i] = (unsigned char)c2wasm_get_array_long_by_index(uint8_array, i);
    }
    
    // Adiciona terminador null para compatibilidade com strings
    self->body[body_size] = '\0';
    self->body_size = body_size;

    return self->body;
}

#endif