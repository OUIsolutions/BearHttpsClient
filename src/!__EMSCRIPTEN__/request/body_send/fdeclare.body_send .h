//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)




void BearHttpsRequest_send_file_with_ownership_control(BearHttpsRequest *self, char *path,short ownership_mode,const char *content_type);

void BearHttpsRequest_send_file(BearHttpsRequest *self,const  char *path,const char *content_type);

void BearHttpsRequest_send_file_auto_detect_content_type(BearHttpsRequest *self, const char *path);


#endif