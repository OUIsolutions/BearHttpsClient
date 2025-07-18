//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.typesH.h"
//silver_chain_scope_end



BearHttpsRequest * newBearHttpsRequest_with_url_ownership_config(char *url,short url_ownership_mode);

BearHttpsRequest * newBearHttpsRequest(const char *url);

BearHttpsRequest * newBearHttpsRequest_fmt(const char *url,...);

void BearHttpsRequest_set_url_with_ownership_config(BearHttpsRequest *self , char *url,short url_ownership_mode);

void BearHttpsRequest_set_url(BearHttpsRequest *self ,const char *url);


void BearHttpsRequest_add_header_with_ownership_config(BearHttpsRequest *self ,char *key,short key_ownership_mode,char *value,short value_owner);

void BearHttpsRequest_add_header(BearHttpsRequest *self ,const char *key,const char *value);

void BearHttpsRequest_add_header_fmt(BearHttpsRequest *self,const char *key,const char *format,...);


void BearHttpsRequest_set_http_protocol(BearHttpsRequest *self ,short protocol);


void BearHttpsRequest_set_max_redirections(BearHttpsRequest *self ,int max_redirections);

void BearHttpsRequest_set_dns_providers(BearHttpsRequest *self ,BearHttpsClientDnsProvider  *dns_providers,int total_dns_proviers);

void BearHttpsRequest_set_known_ips(BearHttpsRequest *self , const char *known_ips[],int known_ips_size);

void BearHttpsRequest_set_chunk_header_read_props(BearHttpsRequest *self ,int chunk_size,int max_chunk_size);


void BearHttpsRequest_set_trusted_anchors(BearHttpsRequest *self ,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size);


void BearHttpsRequest_set_method(BearHttpsRequest *self ,const char *method);

void BearHttpsRequest_represent(BearHttpsRequest *self);


void BearHttpsRequest_free(BearHttpsRequest *self);
