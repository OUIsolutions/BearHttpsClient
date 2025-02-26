
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
#include <string.h>
//silver_chain_scope_end


BearHttpsResponse * private_BearHttpsRequest_fetch_recursive(BearHttpsRequest *self,int total_follows){

   BearHttpsResponse *response =  private_newBearHttpsResponse();
    int uni_start = Universal_start_all();
    if (uni_start != 0) {
        BearHttpsResponse_set_error_msg(response, "failt to start unisocket");
        return response;
    }

    private_BearHttpsRequisitionProps *requisition_props = private_new_private_BearHttpsRequisitionProps(
        self->url,
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
    private_BearHttpsResponse_write(response, (unsigned char*)" ", 1);
    private_BearHttpsResponse_write(response, (unsigned char*)"/", 1);

    private_BearHttpsResponse_write(response, (unsigned char*)requisition_props->route, private_BearsslHttps_strlen(requisition_props->route));
    private_BearHttpsResponse_write(response, (unsigned char*)" HTTP/1.0\r\nHost: ", private_BearsslHttps_strlen(" HTTP/1.0\r\nHost: "));

    private_BearHttpsResponse_write(response, (unsigned char*)requisition_props->hostname, private_BearsslHttps_strlen(requisition_props->hostname));

    private_BearHttpsResponse_write(response, (unsigned char*)"\r\n", 2);

    for (int i = 0; i < self->headders->size; i++) {
        private_BearHttpsKeyVal *keyval = self->headders->keyvals[i];
        private_BearHttpsResponse_write(response, (unsigned char*)keyval->key, private_BearsslHttps_strlen(keyval->key));
        private_BearHttpsResponse_write(response, (unsigned char*)": ", 2);
        private_BearHttpsResponse_write(response, (unsigned char*)keyval->value, private_BearsslHttps_strlen(keyval->value));
        private_BearHttpsResponse_write(response, (unsigned char*)"\r\n", 2);
    }

    private_BearHttpsResponse_write(response, (unsigned char*)"\r\n", 2);

    if(requisition_props->type == BEAR_HTTPS_HTTPS_REQUISITION_TYPE){
         br_sslio_flush(&response->ssl_io);
    }

    private_BearHttpsResponse_read_til_end_of_headders_or_reach_limit(response,self->headder_chunk_read_size,self->headder_chunk_read_size);

    if(BearHttpsResponse_error(response)){
        printf("error: %s\n",BearHttpsResponse_get_error_msg(response));
        private_BearHttpsRequisitionProps_free(requisition_props);
        return 0;
    }

    private_BearHttpsRequisitionProps_free(requisition_props);
    if(total_follows <= self->max_redirections){
        const int REDIRECT_CODE = 301;
        if(response->status_code == REDIRECT_CODE){
            char *location = BearHttpsResponse_get_headder_value_by_sanitized_key(response,"location");
            printf("localização %s\n",location);

            if(location == NULL){
                return response;
            }
            BearHttpsRequest_set_url_with_ownership_config(self,location,BEARSSL_HTTPS_COPY );
            BearHttpsResponse *new_response = private_BearHttpsRequest_fetch_recursive(self,total_follows+1);
            BearHttpsResponse_free(response);
            return new_response;
        }
    }
 return response;

}

BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self){
return private_BearHttpsRequest_fetch_recursive(self,0);
}
