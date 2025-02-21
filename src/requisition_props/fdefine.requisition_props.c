
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

private_BearHttpsRequisitionProps * private_new_private_BearHttpsRequisitionProps(const char *route,int default_port){
    int route_size = private_BearsslHttps_strlen(route);
    if(route_size <sizeof("http://")+1){
        return NULL;
    }
 
    private_BearHttpsRequisitionProps *self = (private_BearHttpsRequisitionProps *)malloc(sizeof(private_BearHttpsRequisitionProps));

    self->port = default_port;
    
    if(private_BearsslHttps_startswith(route,"http://")){
        if(default_port == 0){
            self->port = 80;
        }
        int end = private_BearsslHttps_indexof_from_point(route,'/',sizeof("http://")-1);
        self->hostname = private_BearsslHttps_strndup(route+sizeof("http://"),end-sizeof("http://"));
        self->type = BEARSSL_HTTP_REQUISITION_TYPE;
    }

    else if(private_BearsslHttps_startswith(route,"https://")){
        if(default_port == 0){
            self->port = 443;
        }
        int end = private_BearsslHttps_indexof_from_point(route,'/',sizeof("https://")-1);
        self->hostname = private_BearsslHttps_strndup(route+sizeof("https://"),end-sizeof("https://"));
        self->type = BEAR_HTTPS_HTTPS_REQUISITION_TYPE;
    }

    else{   
        private_BearHttpsRequisitionProps_free(self);
        return NULL;
    }



}

void private_BearHttpsRequisitionProps_free(private_BearHttpsRequisitionProps *self){
    free(self->hostname);
    free(self);
}
