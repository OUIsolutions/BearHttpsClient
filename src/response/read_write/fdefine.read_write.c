//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_define.h"
//silver_chain_scope_end




int private_BearHttpsResponse_write(BearHttpsResponse *self,unsigned char *bufer,long size){
    if(BearHttpsResponse_error(self)){
        return -1;
    }

    if(self->is_https){
      return br_sslio_write_all(&self->ssl_io, bufer, size);
    }
    long sended  = 0;
    while(sended < size){
        long sended_now = Universal_send(self->connection_file_descriptor, bufer+sended, size-sended,0);
        if(sended_now < 0){
            BearHttpsResponse_set_error(self,"error sending data",BEARSSL_HTTPS_IMPOSSIBLE_TO_SEND_DATA);
            return sended_now;
        }
        sended += sended_now;
    }
    return sended;
}

int private_BearHttpsResponse_read_chunck_raw(BearHttpsResponse *self,unsigned char *buffer,long size){
   
    if(BearHttpsResponse_error(self)){
        return -1;
    }
    if(self->is_https){
      return br_sslio_read(&self->ssl_io, buffer, size);
    }
    return Universal_recv(self->connection_file_descriptor, buffer, size,0);

}
