
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

bool private_BearsslHttps_set_str_considering_ownership(char  **value,short ownership_mode){
    if(ownership_mode == BEARSSL_HTTPS_COPY){
        *value = private_BearsslHttps_strdup(*value);
        return true;
    }
    *value = *value;
    
    if(ownership_mode == BEARSSL_HTTPS_GET_OWNERSHIP){
        return true;
    }
    return false;

}