
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


void private_BearsslHttps_free_considering_ownership(void **value,bool *owner){
    if(*owner){
        BearsslHttps_free(*value);
    }
    *owner = false;
    *value = NULL;
}


void private_BearsslHttps_set_str_considering_ownership(
    char **dest,
     char *value,
    bool *owner,
    short ownership_mode
    ){
    private_BearsslHttps_free_considering_ownership((void**)dest,owner);

    if(ownership_mode == BEARSSL_HTTPS_COPY){
        *dest = private_BearsslHttps_strdup(value);
        *owner = true;
    }

    if(ownership_mode == BEARSSL_HTTPS_GET_OWNERSHIP){
        *dest = value;
        *owner = true;
    }

    if(ownership_mode == BEARSSL_HTTPS_REFERENCE){
        *dest = value;
        *owner = false;
    }
}
