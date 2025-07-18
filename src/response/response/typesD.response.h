//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.typesC.h"
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
    long respnse_content_lenght;
    long body_start_index;
    unsigned char *body;
    #ifndef BEARSSL_HTTPS_MOCK_CJSON
    cJSON *json_body;
    #endif

    long body_size;
    long body_readded_size;
    short body_read_mode;
    long extra_body_remaning_to_send;
    bool body_completed_read;
    int body_chunk_size;
    double body_realloc_factor;

    char *error_msg;
    int status_code;
    //http1.1 vars 
    short http1_state;
    long http1_reaming_to_read;
    
}BearHttpsResponse ;
