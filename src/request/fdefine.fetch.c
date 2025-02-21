
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

    int main_file_descriptor =private_BearHttpsRequest_host_connect(response,self->route,self->port);


}
