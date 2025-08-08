#if defined(__EMSCRIPTEN__) 



typedef struct BearHttpsResponse{
    c2wasm_js_var response;
    private_BearHttpsHeaders *headers;
    int error_code;
    unsigned char *body;
    #ifndef BEARSSL_HTTPS_MOCK_CJSON
    cJSON *json_body;
    #endif
    long body_size;
    char *error_msg;
    int status_code;    
}BearHttpsResponse ;

#endif 