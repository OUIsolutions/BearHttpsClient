
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