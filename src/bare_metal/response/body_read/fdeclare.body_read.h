#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)





unsigned char *BearHttpsResponse_read_body(BearHttpsResponse *self);


const  char *BearHttpsResponse_read_body_str(BearHttpsResponse *self);

#ifndef BEARSSL_HTTPS_MOCK_CJSON
cJSON * BearHttpsResponse_read_body_json(BearHttpsResponse *self);
#endif
#endif