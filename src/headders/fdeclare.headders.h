
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesC.h"
//silver_chain_scope_end


BearHttpsHeadders *newBearHttpsHeadders();

void BearHttpsHeadders_add_keyval(BearHttpsHeadders *self, BearHttpsKeyVal *keyval);

void BearHttpsHeadders_free(BearHttpsHeadders *self);