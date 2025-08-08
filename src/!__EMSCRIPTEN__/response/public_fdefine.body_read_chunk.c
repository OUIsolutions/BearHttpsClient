#if !defined(__EMSCRIPTEN__)


int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *buffer,long size){
    if(BearHttpsResponse_error(self)){
        return -1;
    }

    if(self->body_readded_size == self->respnse_content_lenght && self->body_read_mode == PRIVATE_BEARSSL_BY_CONTENT_LENGTH ){
        return 0;
    }

    if(self->body_read_mode == PRIVATE_BEARSSL_BY_CHUNKED){
        return BearHttpsResponse_read_body_chunck_http1(self,buffer,size);
    }
 
    return BearHttpsResponse_read_body_chunck_raw(self,buffer,size);
}
#endif