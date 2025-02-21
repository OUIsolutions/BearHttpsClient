
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesC.h"
//silver_chain_scope_end


typedef struct BearHttpsResponse{
    unsigned char *content;
    long body_readded;
    long body_size;
    private_BearHttpsHeadders *headders;

    unsigned char *body;
    bool error;
    int error_code;
    char *error_msg;
    int status_code;

}BearHttpsResponse ;
