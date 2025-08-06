//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.typesC.h"
//silver_chain_scope_end
#if defined(__EMSCRIPTEN__) 



typedef struct BearHttpsResponse{
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