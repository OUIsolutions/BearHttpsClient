//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.typesB.h"
//silver_chain_scope_end
#if defined(__EMSCRIPTEN__)


typedef struct BearHttpsRequest{
    char *url;
    bool route_owner;
    char method[30];
    private_BearHttpsHeaders *headers;
    short body_type;
    union{
        private_BearHttpsBodyRawRequest body_raw;
        private_BearHttpsBodyRequestFile body_file;
        #ifndef BEARSSL_HTTPS_MOCK_CJSON
                private_BearHttpsBodyJsonRequest body_json;
        #endif
    };

}BearHttpsRequest ;

#endif