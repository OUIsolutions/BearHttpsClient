
//silver_chain_scope_start
//managed by silver chain
#include "../imports/imports.consts.h"
//silver_chain_scope_end


private_BearHttpsHeaders *private_newBearHttpsHeaders();

void private_BearHttpsHeaders_add_keyval(private_BearHttpsHeaders *self, private_BearHttpsKeyVal *keyval);

void private_BearHttpsHeaders_free(private_BearHttpsHeaders *self);

private_BearHttpsKeyVal * private_BearHttpsHeaders_get_key_val_by_index(private_BearHttpsHeaders *self,int index);
