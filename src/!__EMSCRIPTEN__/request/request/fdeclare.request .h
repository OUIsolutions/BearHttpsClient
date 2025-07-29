//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)


void BearHttpsRequest_set_http_protocol(BearHttpsRequest *self ,short protocol);


void BearHttpsRequest_set_max_redirections(BearHttpsRequest *self ,int max_redirections);

void BearHttpsRequest_set_dns_providers(BearHttpsRequest *self ,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers);

void BearHttpsRequest_set_known_ips(BearHttpsRequest *self , const char *known_ips[],int known_ips_size);

void BearHttpsRequest_set_chunk_header_read_props(BearHttpsRequest *self ,int chunk_size,int max_chunk_size);


void BearHttpsRequest_set_trusted_anchors(BearHttpsRequest *self ,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size);






#endif