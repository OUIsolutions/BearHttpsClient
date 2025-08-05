//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)



BearHttpsRequest * newBearHttpsRequest_with_url_ownership_config(char *url,short url_ownership_mode){
    BearHttpsRequest *self = (BearHttpsRequest *)malloc(sizeof(BearHttpsRequest));
    *self = (BearHttpsRequest){0};
    self->max_redirections = BEARSSL_MAX_REDIRECTIONS;
    BearHttpsRequest_set_url_with_ownership_config(self,url,url_ownership_mode);
    self->headers = private_newBearHttpsHeaders();
    self->body_type =PRIVATE_BEARSSL_NO_BODY;
    self->http_protocol = BEARSSL_HTTPS_HTTP1_0;
    self->header_chunk_read_size = BEARSSL_HEADER_CHUNK;
    self->header_chunk_reallocator_factor = BEARSSL_HEADER_REALLOC_FACTOR;
    self->connection_timeout = BEARSSL_TIMEOUT;
    private_BearsslHttps_strcpy(self->method,"GET");
    return self;
}

void BearHttpsRequest_set_known_ips(BearHttpsRequest *self , const char *known_ips[],int known_ips_size){
    self->known_ips = known_ips;
    self->known_ips_size = known_ips_size;
}


void BearHttpsRequest_set_max_redirections(BearHttpsRequest *self ,int max_redirections){
    self->max_redirections = max_redirections;
}

void BearHttpsRequest_set_dns_providers(BearHttpsRequest *self ,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers){
    self->dns_providers= dns_providers;
    self->total_dns_providers = total_dns_proviers;
}

void BearHttpsRequest_set_chunk_header_read_props(BearHttpsRequest *self ,int chunk_size,int max_chunk_size){
    self->header_chunk_read_size = chunk_size;
    self->header_chunk_reallocator_factor = max_chunk_size;
}

void BearHttpsRequest_set_trusted_anchors(BearHttpsRequest *self ,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size){
    self->trust_anchors = trust_anchors;
    self->trusted_anchors_size = trusted_anchors_size;
}


#endif