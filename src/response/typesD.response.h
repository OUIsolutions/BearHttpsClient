
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesC.h"
//silver_chain_scope_end


typedef struct BearHttpsResponse{
    int connection_file_descriptor;
    bool is_https;
    br_sslio_context ssl_io;
    br_ssl_client_context ssl_client;
    unsigned char bear_buffer[BR_SSL_BUFSIZE_BIDI];
    br_x509_minimal_context certification_context;


    unsigned char *raw_content;
    long content_allocated;

    private_BearHttpsHeadders *headders;
    long user_content_length;
    long body_start_index;
    unsigned char *body;


    long body_size;
    long body_readded;
    long extra_body_remaning_to_send;


    bool error;
    int error_code;
    char *error_msg;
    int status_code;

}BearHttpsResponse ;
