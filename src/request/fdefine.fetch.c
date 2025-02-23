
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end



BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self){

   BearHttpsResponse *response =  private_newBearHttpsResponse();
    int uni_start = Universal_start_all();
    if (uni_start != 0) {
        BearHttpsResponse_set_error_msg(response, "failt to start unisocket");
        return response;
    }

    private_BearHttpsRequisitionProps *requisition_props = private_new_private_BearHttpsRequisitionProps(
        self->route,
        self->port
    );
    
    if (requisition_props == NULL) {
        BearHttpsResponse_set_error_msg(response, "failt to create requisition props");
        return response;
    }

    int conection_file_descriptor =private_BearHttpsRequest_host_connect(
        response,
        requisition_props->hostname,
        requisition_props->port
    );

    if(conection_file_descriptor < 0){
        return response;
    }

    if(requisition_props->type == BEAR_HTTPS_HTTPS_REQUISITION_TYPE){
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



    private_BearHttpsRequisitionProps_free(requisition_props);

 return response;

}
