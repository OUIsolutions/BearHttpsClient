//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.macros.h"
//silver_chain_scope_end
#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)





typedef struct  private_BearHttpsKeyVal{
    char *key;
    bool key_owner;
    char *value;
    bool value_owner;

}private_BearHttpsKeyVal;

#endif