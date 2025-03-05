
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

private_BearHttpsKeyVal  *private_newBearHttpsKeyVal(){
    private_BearHttpsKeyVal *self = (private_BearHttpsKeyVal *)BearsslHttps_allocate(sizeof(private_BearHttpsKeyVal));
    *self = (private_BearHttpsKeyVal){0};
    self->key = NULL;
    self->value = NULL;
    self->key_owner =false;
    self->value_owner = false;
    return self;
}

void private_BearHttpsKeyVal_set_key(private_BearHttpsKeyVal *self,  char *key,short key_onwership_mode){
    private_BearsslHttps_free_considering_ownership((void **)&self->key,&self->key_owner);
   private_BearsslHttps_set_str_considering_ownership(&self->key,key,&self->key_owner, key_onwership_mode);
}

void private_BearHttpsKeyVal_set_value(private_BearHttpsKeyVal *self,  char *value,short value_onwership_mode){
    private_BearsslHttps_free_considering_ownership((void **)&self->value,&self->value_owner);
    private_BearsslHttps_set_str_considering_ownership(&self->value,value,&self->value_owner,value_onwership_mode);
}

void  private_BearHttpsKeyVal_free(private_BearHttpsKeyVal *self){
    private_BearsslHttps_free_considering_ownership((void **)&self->key,&self->key_owner);
    private_BearsslHttps_free_considering_ownership((void **)&self->value,&self->value_owner);
    free(self);
}
