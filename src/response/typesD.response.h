
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesC.h"
//silver_chain_scope_end


typedef struct BearHttpsResponse{
    unsigned char *content;
    bool error;
    int error_code;
    char *error_msg;
    int status_code;
    private_BearHttpsHeadders *headders;

}BearHttpsResponse ;
