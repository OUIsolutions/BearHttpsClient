
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesC.h"
//silver_chain_scope_end


BearHttpsKeyVal  *private_newBearHttpsKeyVal();


void private_BearHttpsKeyVal_set_key(BearHttpsKeyVal *self, const char *key,short key_onwership_mode);

void private_BearHttpsKeyVal_set_value(BearHttpsKeyVal *self, const char *value,short value_onwership_mode);

void  private_BearHttpsKeyVal_free(BearHttpsKeyVal *self);
