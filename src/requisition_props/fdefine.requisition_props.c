
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

private_BearHttpsRequisitionProps * private_new_private_BearHttpsRequisitionProps(const char *url,int default_port){
    int url_size = private_BearsslHttps_strlen(url);
    const short HTTP_START_SIZE = sizeof("http://")-1;
    const short HTTPS_START_SIZE = sizeof("https://")-1;


    private_BearHttpsRequisitionProps *self = (private_BearHttpsRequisitionProps *)malloc(sizeof(private_BearHttpsRequisitionProps));
    *self = (private_BearHttpsRequisitionProps){0};
    short start_size;

    //setuping defaults elements
    if(private_BearsslHttps_startswith(url,"http://")){
        self->port = default_port ?  default_port: 80;
        self->is_https = false;
        start_size = HTTP_START_SIZE;
    }

    else if(private_BearsslHttps_startswith(url,"https://")){
        self->port = default_port ?  default_port: 443;
        self->is_https = true;
        start_size = HTTPS_START_SIZE;
    }
    else{
        self->port = default_port ?  default_port: 80;
        self->is_https = false;
        start_size = 0;
    }

    //1000 //2000
    int end_host_name_and_start_of_route = private_BearsslHttps_indexof_from_point(url,'/',start_size);

    if(end_host_name_and_start_of_route == -1){
        end_host_name_and_start_of_route = url_size;
    }
    self->hostname = private_BearsslHttps_strndup((url+start_size),end_host_name_and_start_of_route-start_size);

    long host_name_size = private_BearsslHttps_strlen(self->hostname);

    if(end_host_name_and_start_of_route == url_size){
        self->route = private_BearsslHttps_strdup("/");    }
    else{
         self->route = private_BearsslHttps_strndup((url+end_host_name_and_start_of_route),url_size-end_host_name_and_start_of_route+1);
    }
    
    //iterate over hostname to determine if its ipv4
    short total_dots = 0;
    bool only_numbers = true;
    for(int i = 0; i < host_name_size; i++){
        if(self->hostname[i] == '.'){
            total_dots++;
            continue;
        }
        if(self->hostname[i] == ':'){
            break;
        }
        if(self->hostname[i] < '0' || self->hostname[i] > '9'){
            only_numbers = false;
            break;
        }
        
    }

    if(total_dots == 3 && only_numbers){
        self->is_ipv4 = true;
    }

    //detect if self.hostname its ipv4
    if(default_port == 0){
    int start_port = private_BearsslHttps_indexof_from_point(self->hostname,':',0);
        if(start_port  != -1){
    
            self->hostname[start_port] = '\0';
            self->port = atoi(self->hostname+start_port+1);
        }
    }

    return self;
}

void private_BearHttpsRequisitionProps_free(private_BearHttpsRequisitionProps *self){
    BearsslHttps_free(self->hostname);
    BearsslHttps_free(self->route);
    BearsslHttps_free(self);
}
