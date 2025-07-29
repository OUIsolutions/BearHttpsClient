#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)


int BearHttpsResponse_read_body_chunck_http1(BearHttpsResponse *self,unsigned char *buffer,long size);



int BearHttpsResponse_read_body_chunck_raw(BearHttpsResponse *self,unsigned char *buffer,long size);

int BearHttpsResponse_read_body_chunck(BearHttpsResponse *self,unsigned char *buffer,long size);
#endif