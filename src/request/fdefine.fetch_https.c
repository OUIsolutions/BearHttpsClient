
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


void BearHttpsRequest_fetch_https(BearHttpsRequest *self, BearHttpsResponse *response,const char *hostname,int connection_file_descriptor){
        br_sslio_context ioc;
        br_ssl_client_context sc;
        br_x509_minimal_context xc;
        br_ssl_client_init_full(&sc, &xc, TAs, TAs_NUM);
        br_ssl_engine_set_all_flags(&sc.eng, BR_OPT_TOLERATE_NO_CLIENT_AUTH);
        unsigned char iobuf[BR_SSL_BUFSIZE_BIDI];
	br_ssl_engine_set_buffer(&sc.eng, iobuf, sizeof iobuf, 1);
        br_ssl_client_reset(&sc,hostname, 0);
        br_sslio_init(&ioc, &sc.eng, private_BearHttps_sock_read, &connection_file_descriptor, private_BearHttps_sock_write, &connection_file_descriptor);
}