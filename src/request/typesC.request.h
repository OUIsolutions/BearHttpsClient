
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesB.h"
//silver_chain_scope_end


typedef struct private_BearHttpsBodyRequest{
    unsigned char *body;
    long size;
    bool onwer;
}private_BearHttpsBodyRequest;

typedef struct private_BearHttpsBodyRequestFile{
    char *path;
    bool onwer;
}private_BearHttpsBodyRequestFile;


typedef struct BearHttpsRequest{
    char *route;
    bool route_owner;
    private_BearHttpsHeadders *headders;
    char *method;
    bool method_owner;

    union{
        private_BearHttpsBodyRequest *body;
        private_BearHttpsBodyRequestFile *body_file;
    }
    
}BearHttpsRequest ;
