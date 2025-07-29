




BearHttpsResponse *private_newBearHttpsResponse();


void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self, const char *hostname,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size);


int BearHttpsResponse_get_status_code(BearHttpsResponse*self);


int BearHttpsResponse_get_body_size(BearHttpsResponse*self);


bool BearHttpsResponse_error(BearHttpsResponse*self);

void BearHttpsResponse_set_error(BearHttpsResponse*self,const char *msg,int error_code);


char* BearHttpsResponse_get_error_msg(BearHttpsResponse*self);


int BearHttpsResponse_get_error_code(BearHttpsResponse*self);


void BearHttpsResponse_free(BearHttpsResponse *self);

int BearHttpsResponse_get_headers_size(BearHttpsResponse*self);

char* BearHttpsResponse_get_header_value_by_index(BearHttpsResponse*self,int index);

char* BearHttpsResponse_get_header_key_by_index(BearHttpsResponse*self,int index);

char* BearHttpsResponse_get_header_value_by_key(BearHttpsResponse*self,const char *key);

void BearHttpsResponse_set_max_body_size(BearHttpsResponse*self,long size);


void BearHttpsResponse_set_body_read_props(BearHttpsResponse*self,int chunk_size,double realloc_factor);

char* BearHttpsResponse_get_header_value_by_sanitized_key(BearHttpsResponse*self,const char *key);