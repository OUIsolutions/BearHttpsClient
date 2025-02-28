
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end



BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self){

    BearHttpsResponse *response = NULL;

    int uni_start = Universal_start_all();
    if (uni_start != 0) {
        BearHttpsResponse_set_error_msg(response, "failt to start unisocket");
        return response;
    }

    //we start at -1 , because the first conection its not consider a redirection
    for(int i = -1; i < self->max_redirections;i++){
        response =  private_newBearHttpsResponse();
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
        
        if(self->body_type ==PRIVATE_BEARSSL_BODY_RAW){
            char content_length[100];
            sprintf(content_length,"Content-Length: %ld\r\n",self->body_raw.size);
            private_BearHttpsResponse_write(response,(unsigned char*)content_length,strlen(content_length));
        }
        private_BearHttpsResponse_write(response, (unsigned char*)"\r\n", 2);
        
        if(self->body_type == PRIVATE_BEARSSL_BODY_RAW){

            private_BearHttpsResponse_write(response,self->body_raw.value,self->body_raw.size);
        }

         if(requisition_props->type == BEAR_HTTPS_HTTPS_REQUISITION_TYPE){
              br_sslio_flush(&response->ssl_io);
         }
        


         private_BearHttpsResponse_read_til_end_of_headders_or_reach_limit(response,self->headder_chunk_read_size,self->headder_chunk_read_size);
        printf("chegou aqui\n");

         if(BearHttpsResponse_error(response)){
             printf("error: %s\n",BearHttpsResponse_get_error_msg(response));
             private_BearHttpsRequisitionProps_free(requisition_props);
             return response;
         }
         private_BearHttpsRequisitionProps_free(requisition_props);
         const int REDIRECT_CODE = 300;
        const int REDIRECT_CODE2 = 301;

         if((response->status_code == REDIRECT_CODE || response->status_code == REDIRECT_CODE2)  && i < self->max_redirections -1){
            char *location = BearHttpsResponse_get_headder_value_by_sanitized_key(response,"location");
            if(location == NULL){
                return response;
            }
            BearHttpsRequest_set_url_with_ownership_config(self,location,BEARSSL_HTTPS_COPY );
            BearHttpsResponse_free(response);
            response = NULL;
            continue;
         }
         // ends the loop and returns the last response
         break;
    }

    return response;
}
