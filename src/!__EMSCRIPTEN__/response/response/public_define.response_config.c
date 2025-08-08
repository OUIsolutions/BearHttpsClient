
#if !defined(__EMSCRIPTEN__)




void BearHttpsResponse_set_max_body_size(BearHttpsResponse*self,long size){
    self->max_body_size = size;
}

void BearHttpsResponse_set_body_read_props(BearHttpsResponse*self,int chunk_size,double realloc_factor){
    self->body_chunk_size = chunk_size;
    self->body_realloc_factor = realloc_factor;
}


#endif