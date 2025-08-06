//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)



unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self) {
    if (BearHttpsResponse_error(self)) {
        return NULL;
    }

    if (self->body_completed_read) {
        return self->body;
    }

    long body_allocated = self->body_size > 0 ? self->body_size : self->body_chunk_size;
    if (self->body_read_mode == PRIVATE_BEARSSL_BY_CONTENT_LENGTH) {
        body_allocated = self->respnse_content_lenght + 2;
    }
    self->body = (unsigned char *)BearsslHttps_reallocate(self->body, body_allocated);
    if (self->body == NULL) {
        BearHttpsResponse_set_error(self, "error allocating memory", BEARSSL_HTTPS_ALOCATION_FAILED);
        return NULL;
    }

    
    unsigned char *buffer = self->body;
    long total_readded = 0;
    while (true) {
      

        
        if ((total_readded + self->body_chunk_size + 2) > body_allocated) {
            
            while(body_allocated < (total_readded+ self->body_chunk_size + 2)) {
                body_allocated *= self->body_realloc_factor;
            }

            if (self->max_body_size != -1 && body_allocated > self->max_body_size) {
                BearHttpsResponse_set_error(self, "body size is bigger than max body size", BEARSSL_HTTPS_BODY_SIZE_ITS_BIGGER_THAN_LIMIT);
                BearsslHttps_free(self->body);
                self->body = NULL;
                return NULL;
            }
            self->body = (unsigned char *)BearsslHttps_reallocate(self->body, body_allocated);
            if (self->body == NULL) {
                BearHttpsResponse_set_error(self, "error allocating memory", BEARSSL_HTTPS_ALOCATION_FAILED);
                return NULL;
            }
            buffer = self->body + total_readded;
        }
    
        // Lê usando a função chunck
        int readded  = BearHttpsResponse_read_body_chunck(self, buffer, self->body_chunk_size);
        if(readded > 0){
            buffer += readded;
            total_readded += readded;
            continue;
        }        
        break;
        
    }

    self->body_size = self->body_readded_size;
    self->body_completed_read = true;
    return self->body;
}


#endif