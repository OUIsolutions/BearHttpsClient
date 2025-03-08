
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


private_BearHttpsHeaders *private_newBearHttpsHeaders(){
    private_BearHttpsHeaders *self = (private_BearHttpsHeaders *)BearsslHttps_allocate(sizeof(private_BearHttpsHeaders));
    *self  = (private_BearHttpsHeaders){0};
    self->keyvals = (private_BearHttpsKeyVal **)BearsslHttps_allocate(0);
    return self;
}

void private_BearHttpsHeaders_add_keyval(private_BearHttpsHeaders *self, private_BearHttpsKeyVal *keyval){
    self->keyvals = (private_BearHttpsKeyVal **)BearsslHttps_reallocate(
        self->keyvals,
        sizeof(private_BearHttpsKeyVal*) * (self->size + 1)

    );
    self->keyvals[self->size] = keyval;
    self->size++;
}

void private_BearHttpsHeaders_free(private_BearHttpsHeaders *self){
    for(int i = 0; i < self->size; i++){
        private_BearHttpsKeyVal_free(self->keyvals[i]);
    }
    free(self->keyvals);
    free(self);
}

private_BearHttpsKeyVal * private_BearHttpsHeaders_get_key_val_by_index(private_BearHttpsHeaders *self,int index){
    if(self->size <= index){
        return NULL;
    }
    return self->keyvals[index];
}
