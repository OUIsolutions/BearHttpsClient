#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)




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
#endif