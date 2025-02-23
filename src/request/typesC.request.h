
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesB.h"
//silver_chain_scope_end




typedef struct BearHttpsRequest{
    char *url;
    bool route_owner;

    private_BearHttpsHeadders *headders;
    char method[30];
    int port;

    int response_max_headders_size;


    short body_type;
    union{
        private_BearHttpsBodyRawRequest body_raw;
        private_BearHttpsBodyRequestFile body_file;
    };

}BearHttpsRequest ;
