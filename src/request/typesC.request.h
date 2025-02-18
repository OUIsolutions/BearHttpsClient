
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
    bool route_ownernership_mode;
    private_BearHttpsHeadders *headders;
    union{
        private_BearHttpsBodyRequest *body;
        private_BearHttpsBodyRequestFile *body_file;
    }
    
}BearHttpsRequest ;
