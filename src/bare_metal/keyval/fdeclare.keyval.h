#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)




private_BearHttpsKeyVal  *private_newBearHttpsKeyVal();


void private_BearHttpsKeyVal_set_key(private_BearHttpsKeyVal *self,  char *key,short key_onwership_mode);

void private_BearHttpsKeyVal_set_value(private_BearHttpsKeyVal *self,  char *value,short value_onwership_mode);

void  private_BearHttpsKeyVal_free(private_BearHttpsKeyVal *self);

#endif