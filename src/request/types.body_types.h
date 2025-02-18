
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.macros.h"
//silver_chain_scope_end


typedef struct private_BearHttpsBodyRawRequest{
    unsigned char *value;
    long size;
    bool onwer;
}private_BearHttpsBodyRawRequest;

typedef struct private_BearHttpsBodyRequestFile{
    char *path;
    bool onwer;
}private_BearHttpsBodyRequestFile;
