
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

private_BearHttpsRequisitionProps * private_new_private_BearHttpsRequisitionProps(const char *route,int default_port){
    int url_size = private_BearsslHttps_strlen(route);
    const short HTTP_START_SIZE = sizeof("http://");
    const short HTTPS_START_SIZE = sizeof("https://");

    if(url_size < HTTP_START_SIZE+1){
        return NULL;
    }
    private_BearHttpsRequisitionProps *self = (private_BearHttpsRequisitionProps *)malloc(sizeof(private_BearHttpsRequisitionProps));
    *self = (private_BearHttpsRequisitionProps){0};
    short start_size;
    if(private_BearsslHttps_startswith(route,"http://")){
        self->port = default_port ?  default_port: 80;
        self->type = BEARSSL_HTTP_REQUISITION_TYPE;
        start_size = HTTP_START_SIZE;
    }

    else if(private_BearsslHttps_startswith(route,"https://")){
        self->port = default_port ?  default_port: 443;
        self->type = BEAR_HTTPS_HTTPS_REQUISITION_TYPE;
        start_size = HTTPS_START_SIZE;
    }
    else{
        private_BearHttpsRequisitionProps_free(self);
        return NULL;
    }
    //1000 //2000
    int end_host_name_and_start_of_route = private_BearsslHttps_indexof_from_point(route,'/',start_size-1);
    if(end_host_name_and_start_of_route == -1){
        end_host_name_and_start_of_route = url_size;
    }
    self->hostname = private_BearsslHttps_strndup((route+start_size-1),end_host_name_and_start_of_route-start_size+1);
    if(end_host_name_and_start_of_route == url_size){
        self->route = private_BearsslHttps_strdup("/");
        return self;
    }

    self->route = private_BearsslHttps_strndup((route+end_host_name_and_start_of_route),url_size-end_host_name_and_start_of_route+1);

    return self;
}

void private_BearHttpsRequisitionProps_free(private_BearHttpsRequisitionProps *self){
    free(self->hostname);
    free(self);
}
