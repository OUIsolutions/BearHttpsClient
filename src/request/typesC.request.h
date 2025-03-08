
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.typesB.h"
//silver_chain_scope_end





typedef struct BearHttpsRequest{
    char *url;
    bool route_owner;
    int max_redirections;
    const char *custom_bear_dns;

   BearHttpsClientDnsProvider  *dns_providers;
    int total_dns_providers;
    bool must_be_ipv4;
    
    private_BearHttpsHeaders *headers;
    char method[30];
    int port;

    int header_chunk_read_size;
    int header_chunk_reallocator_factor;

    int response_max_headers_size;

    br_x509_trust_anchor *trust_anchors;
    size_t trusted_anchors_size;


    short body_type;
    union{
        private_BearHttpsBodyRawRequest body_raw;
        private_BearHttpsBodyRequestFile body_file;

        #ifndef BEARSSL_HTTPS_MOCK_CJSON
            
                private_BearHttpsBodyJsonRequest body_json;
        #endif
    };

}BearHttpsRequest ;
