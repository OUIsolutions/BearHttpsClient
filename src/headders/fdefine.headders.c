
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

private_BearHttpsHeadders *private_newBearHttpsHeadders(){
    private_BearHttpsHeadders *self = (private_BearHttpsHeadders *)BearsslHttps_allocate(sizeof(private_BearHttpsHeadders));
    self->keyvals = (private_BearHttpsKeyVal **)BearsslHttps_allocate(0);
    return self;
}

void private_BearHttpsHeadders_add_keyval(private_BearHttpsHeadders *self, private_BearHttpsKeyVal *keyval){
    self->keyvals = (private_BearHttpsKeyVal **)realloc(self->keyvals, sizeof(private_BearHttpsKeyVal *) * (self->size + 1));
    self->keyvals[self->size] = keyval;
    self->size++;
}

void private_BearHttpsHeadders_free(private_BearHttpsHeadders *self){
    for(int i = 0; i < self->size; i++){
        private_BearHttpsKeyVal_free(self->keyvals[i]);
    }
    free(self->keyvals);
    free(self);
}
