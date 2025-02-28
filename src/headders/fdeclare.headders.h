
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesH.h"
//silver_chain_scope_end


private_BearHttpsHeadders *private_newBearHttpsHeadders();

void private_BearHttpsHeadders_add_keyval(private_BearHttpsHeadders *self, private_BearHttpsKeyVal *keyval);

void private_BearHttpsHeadders_free(private_BearHttpsHeadders *self);

private_BearHttpsKeyVal * private_BearHttpsHeadders_get_key_val_by_index(private_BearHttpsHeadders *self,int index);
