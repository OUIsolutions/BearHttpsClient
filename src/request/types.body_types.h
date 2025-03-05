
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_declare.h"
//silver_chain_scope_end


typedef struct private_BearHttpsBodyRawRequest{
    unsigned char *value;
    long size;
    bool onwer;
}private_BearHttpsBodyRawRequest;


typedef struct private_BearHttpsBodyRequestFile{
    char *path;
    char content_type[100];
    bool onwer;
}private_BearHttpsBodyRequestFile;



#ifndef BEARSSL_HTTPS_MOCK_CJSON

typedef struct private_BearHttpsBodyJsonRequest{
    cJSON *json;
    bool onwer;
}private_BearHttpsBodyJsonRequest;
#endif