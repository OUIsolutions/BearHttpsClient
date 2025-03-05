
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.typesC.h"
//silver_chain_scope_end


typedef struct BearHttpsResponse{
    int connection_file_descriptor;
    bool is_https;
    br_sslio_context ssl_io;
    br_ssl_client_context ssl_client;
    unsigned char bear_buffer[BR_SSL_BUFSIZE_BIDI];
    br_x509_minimal_context certification_context;
    long max_body_size;
    unsigned char *raw_content;
    int error_code;
    private_BearHttpsHeaders *headers;
    long user_content_length;
    long body_start_index;
    unsigned char *body;
    #ifndef BEARSSL_HTTPS_MOCK_CJSON
    cJSON *json_body;
    #endif

    long body_size;
    long body_readded;
    long extra_body_remaning_to_send;

    int body_chunk_size;
    double body_realloc_factor;

    char *error_msg;
    int status_code;

}BearHttpsResponse ;
