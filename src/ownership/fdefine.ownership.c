
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


void private_BearsslHttps_free_considering_ownership(void **value,bool *owner){
    if(*owner){
        free(*value);
    }
    *owner = false;
    *value = NULL;
}

void private_BearsslHttps_set_str_considering_ownership(
    char **dest,
    const  char *value, 
    bool *owner, 
    short ownership_mode
    ){
    if(ownership_mode == BEARSSL_HTTPS_COPY){
        *dest = private_BearsslHttps_strdup(value);
        *owner = true;
    }

    *dest = value;
    
    if(ownership_mode == BEARSSL_HTTPS_GET_OWNERSHIP){
        *owner = true;
        return ;
    }
    *owner = false;

}