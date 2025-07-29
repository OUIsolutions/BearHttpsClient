#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)





typedef struct  private_BearHttpsKeyVal{
    char *key;
    bool key_owner;
    char *value;
    bool value_owner;

}private_BearHttpsKeyVal;

#endif