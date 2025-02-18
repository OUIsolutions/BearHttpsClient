
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

private_BearHttpsKeyVal  *private_newBearHttpsKeyVal(){
    private_BearHttpsKeyVal *self = (private_BearHttpsKeyVal *)BearsslHttps_malloc(sizeof(private_BearHttpsKeyVal));
    self->key = NULL;
    self->value = NULL;
    self->key_owner =false;
    self->value_owner = false;
    return self;
}

void private_BearHttpsKeyVal_set_key(private_BearHttpsKeyVal *self, const char *key,short key_onwership_mode){
    private_BearsslHttps_free_considering_ownership((void **)&self->key,&self->key_owner);
    self->key = (char *)key;
    self->key_owner = key_onwership_mode;
}

void private_BearHttpsKeyVal_set_value(private_BearHttpsKeyVal *self, const char *value,short value_onwership_mode){
    private_BearsslHttps_free_considering_ownership((void **)&self->value,&self->value_owner);
    self->value = (char *)value;
    self->value_owner = value_onwership_mode;
}  

void  private_BearHttpsKeyVal_free(private_BearHttpsKeyVal *self){
    private_BearsslHttps_free_considering_ownership((void **)&self->key,&self->key_owner);
    private_BearsslHttps_free_considering_ownership((void **)&self->value,&self->value_owner);
    free(self);
}
