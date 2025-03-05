
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end



BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self){

    BearHttpsResponse *response = NULL;

    int uni_start = Universal_start_all();
    if (uni_start != 0) {
        BearHttpsResponse_set_error(response, "failt to start unisocket",BEARSSL_HTTPS_FAILT_TO_START_UNISOCKET);
        return response;
    }

    //we start at -1 , because the first conection its not consider a redirection
    for(int i = -1; i < self->max_redirections;i++){


        response =  private_newBearHttpsResponse();
         private_BearHttpsRequisitionProps *requisition_props = private_new_private_BearHttpsRequisitionProps(
             self->url,
             self->port
         );
        if(requisition_props == NULL){
            BearHttpsResponse_set_error(response, "invalid url",BEARSSL_HTTPS_INVALID_URL);
            return response;
        }

        if(requisition_props->is_ipv4){
                response->connection_file_descriptor = private_BearHttpsRequest_connect_ipv4(
                    response,
                    requisition_props->hostname,
                    requisition_props->port
                );
        }
        //these function its used on the private_BearHttpsRequest_connect_host and if returns these
        //means that the dns its wrong formatted
        if(requisition_props->is_ipv4 == false && self->must_be_ipv4){
            BearHttpsResponse_set_error(response, "must be ipv4",BEARSSL_HTTPS_MUST_BE_IPV4);
            private_BearHttpsRequisitionProps_free(requisition_props);
            return response;
        }

        if(requisition_props->is_ipv4 == false){
            response->connection_file_descriptor =private_BearHttpsRequest_connect_host(
                response,
                requisition_props->hostname,
                requisition_props->port,
                self->dns_providers,
                self->total_dns_providers
            );
        }

         if(response->connection_file_descriptor < 0){
             private_BearHttpsRequisitionProps_free(requisition_props);
             return response;
         }

         if(requisition_props->is_https){

           const char *chosen_host = self->custom_bear_dns ? self->custom_bear_dns : requisition_props->hostname;
           private_BearHttpsResponse_start_bearssl_props(response,chosen_host,self->trust_anchors,self->trusted_anchors_size);
         }
         private_BearHttpsResponse_write(response, (unsigned char*)self->method, private_BearsslHttps_strlen(self->method));
         private_BearHttpsResponse_write(response, (unsigned char*)" ", 1);
         private_BearHttpsResponse_write(response, (unsigned char*)requisition_props->route, private_BearsslHttps_strlen(requisition_props->route));
         private_BearHttpsResponse_write(response, (unsigned char*)" HTTP/1.0\r\nHost: ", private_BearsslHttps_strlen(" HTTP/1.0\r\nHost: "));
         private_BearHttpsResponse_write(response, (unsigned char*)requisition_props->hostname, private_BearsslHttps_strlen(requisition_props->hostname));
         private_BearHttpsResponse_write(response, (unsigned char*)"\r\n", 2);
        if(BearHttpsResponse_error(response)){
                    private_BearHttpsRequisitionProps_free(requisition_props);
                    return response;
        }

         for (int i = 0; i < self->headers->size; i++) {
             private_BearHttpsKeyVal *keyval = self->headers->keyvals[i];
             private_BearHttpsResponse_write(response, (unsigned char*)keyval->key, private_BearsslHttps_strlen(keyval->key));
             private_BearHttpsResponse_write(response, (unsigned char*)": ", 2);
             private_BearHttpsResponse_write(response, (unsigned char*)keyval->value, private_BearsslHttps_strlen(keyval->value));
             private_BearHttpsResponse_write(response, (unsigned char*)"\r\n", 2);
         }
         if(BearHttpsResponse_error(response)){
            private_BearHttpsRequisitionProps_free(requisition_props);
            return response;
        }


        if(self->body_type ==PRIVATE_BEARSSL_BODY_RAW){
            char content_length[100];
            snprintf(content_length,sizeof(content_length)-1,"Content-Length: %ld\r\n\r\n",self->body_raw.size);
            private_BearHttpsResponse_write(response,(unsigned char*)content_length,strlen(content_length));
            private_BearHttpsResponse_write(response,self->body_raw.value,self->body_raw.size);
        }
        if(BearHttpsResponse_error(response)){
            private_BearHttpsRequisitionProps_free(requisition_props);
            return response;
        }

        if(self->body_type == PRIVATE_BEARSSL_BODY_FILE){

             FILE  *file = fopen(self->body_file.path,"rb");

            if(file == NULL){
                BearHttpsResponse_set_error(response, "impssible to open file",BEARSSL_HTTPS_IMPOSSIBLE_TO_OPEN_FILE);
                private_BearHttpsRequisitionProps_free(requisition_props);
                return response;
            }
            char content_type[200];
            snprintf(content_type,sizeof(content_type)-1, "Content-Type: %s\r\n",self->body_file.content_type);
            private_BearHttpsResponse_write(response,(unsigned char*)content_type,strlen(content_type));

            fseek(file, 0, SEEK_END);
            long size = ftell(file);
            fseek(file, 0, SEEK_SET);
            char content_length[100];
            sprintf(content_length,"Content-Length: %ld\r\n\r\n",size);
            private_BearHttpsResponse_write(response,(unsigned char*)content_length,strlen(content_length));

            unsigned char send_buff[BEARSSL_BODY_CHUNK_SIZE];
            while (1) {
                size_t read_size = fread(send_buff, 1, BEARSSL_BODY_CHUNK_SIZE, file);
                if (read_size == 0) {
                    break;
                }
                private_BearHttpsResponse_write(response, send_buff, read_size);
            }
            fclose(file);

        }

        #ifndef BEARSSL_HTTPS_MOCK_CJSON
            if(self->body_type == PRIVATE_BEARSSL_BODY_JSON){
                const char *content_type = "Content-Type: application/json\r\n";
                private_BearHttpsResponse_write(response,(unsigned char*)content_type,strlen(content_type));
                char * dumped = cJSON_Print(self->body_json.json);
                long dumped_size = private_BearsslHttps_strlen(dumped);
                char content_length[100];
                sprintf(content_length,"Content-Length: %ld\r\n\r\n",dumped_size);
                private_BearHttpsResponse_write(response,(unsigned char*)content_length,strlen(content_length));
                private_BearHttpsResponse_write(response,(unsigned char*)dumped,dumped_size);
                BearsslHttps_free(dumped);
            }

         #endif
        if(self->body_type == PRIVATE_BEARSSL_NO_BODY){
             private_BearHttpsResponse_write(response, (unsigned char*)"\r\n", 2);
        }


        if(requisition_props->is_https){
              br_sslio_flush(&response->ssl_io);
         }

         private_BearHttpsResponse_read_til_end_of_headers_or_reach_limit(response,self->header_chunk_read_size,self->header_chunk_reallocator_factor);

         if(BearHttpsResponse_error(response)){
             private_BearHttpsRequisitionProps_free(requisition_props);
             return response;
         }
         private_BearHttpsRequisitionProps_free(requisition_props);
         const int REDIRECT_CODE = 300;
         const int REDIRECT_CODE2 = 301;

         if((response->status_code == REDIRECT_CODE || response->status_code == REDIRECT_CODE2)  && i < self->max_redirections -1){
            char *location = BearHttpsResponse_get_header_value_by_sanitized_key(response,"location");
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
