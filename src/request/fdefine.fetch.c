
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

    response->connection_file_descriptor =private_BearHttpsRequest_host_connect(
        response,
        requisition_props->hostname,
        requisition_props->port
    );


    if(response->connection_file_descriptor < 0){
        private_BearHttpsRequisitionProps_free(requisition_props);
        return response;
    }

    if(requisition_props->type == BEAR_HTTPS_HTTPS_REQUISITION_TYPE){
      private_BearHttpsResponse_start_bearssl_props(response, requisition_props->hostname);
    }

    private_BearHttpsResponse_write(response, (unsigned char*)self->method, private_BearsslHttps_strlen(self->method));
    private_BearHttpsResponse_write(response, (unsigned char*)self->route, private_BearsslHttps_strlen(self->route));
    const char *HOST_TEXT =" HTTP/1.0\r\nHost: ";
    private_BearHttpsResponse_write(response, (unsigned char*)HOST_TEXT, sizeof(HOST_TEXT)-1);
    private_BearHttpsResponse_write(response, (unsigned char*)requisition_props->hostname, private_BearsslHttps_strlen(requisition_props->hostname));
    for(int i = 0; i < self->headders->size; i++){
        private_BearHttpsKeyVal *keyval = self->headders->keyvals[i];
        private_BearHttpsResponse_write(response, (unsigned char*)keyval->key, private_BearsslHttps_strlen(keyval->key));
        private_BearHttpsResponse_write(response, (unsigned char*)": ", 2);
        private_BearHttpsResponse_write(response, (unsigned char*)keyval->value, private_BearsslHttps_strlen(keyval->value));
        private_BearHttpsResponse_write(response, (unsigned char*)"\r\n", 2);
    }
    private_BearHttpsResponse_write(response, (unsigned char*)"\r\n\r\n", 4);
    br_sslio_flush(&response->ssl_io);

    private_BearHttpsRequisitionProps_free(requisition_props);

 return response;

}
