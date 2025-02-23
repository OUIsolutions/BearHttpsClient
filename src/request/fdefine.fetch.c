
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





    private_BearHttpsRequisitionProps_free(requisition_props);

 return response;

}
