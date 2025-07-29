#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)


int private_BearHttpsResponse_write(BearHttpsResponse *self,unsigned char *bufer,long size);

int private_BearHttpsResponse_recv(BearHttpsResponse *self,unsigned char *buffer,long size);
#endif