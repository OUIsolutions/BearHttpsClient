
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

void private_BearHttpsResponse_sand_raw_body(BearHttpsResponse *self,private_BearHttpsBodyRawRequest *raw_body){
    //parse the content length

    long size_to_send = raw_body->size;
    while(size_to_send > 0){
       long send = private_BearHttpsResponse_write(self,raw_body->value,size_to_send);
        printf("send: %ld\n",send);

        if(send <= 0){
            return;
        }
        size_to_send -= send;
    }
}


bool private_BearHttpsResponse_stream_body(BearHttpsResponse *self,BearHttpsRequest *request){
   
   if(request->body_type == PRIVATE_BEARSSL_BODY_RAW){
         private_BearHttpsResponse_sand_raw_body(self,&request->body_raw);
         return true;
   }
  
}