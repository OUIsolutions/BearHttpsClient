
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesC.h"
//silver_chain_scope_end


typedef struct BearHttpsResponse{

    int connection_file_descriptor;
    bool is_https;
    br_sslio_context ssl_io;

    unsigned char *content;
    long content_readed;

    private_BearHttpsHeadders *headders;
    long body_size;
    unsigned char *body;
    bool error;
    int error_code;
    char *error_msg;
    int status_code;

}BearHttpsResponse ;
