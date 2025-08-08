#if !defined(__EMSCRIPTEN__)




BearHttpsResponse *private_newBearHttpsResponse(){
    BearHttpsResponse *self = (BearHttpsResponse *)malloc(sizeof(BearHttpsResponse));
    *self = (BearHttpsResponse){0};
    self->status_code = 0;
    self->headers = private_newBearHttpsHeaders();
    self->max_body_size = -1;
    self->body_read_mode = PRIVATE_BEARSSL_BY_READ_ERROR;
    self->body_chunk_size = BEARSSL_BODY_CHUNK_SIZE;
    self->body_realloc_factor = BEARSSL_BODY_REALLOC_FACTOR;
    self->body = NULL;
    return self;
}




void private_BearHttpsResponse_start_bearssl_props(BearHttpsResponse *self, const char *hostname,br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size) {
    self->is_https = true;
    if(trust_anchors && trusted_anchors_size > 0){
       br_ssl_client_init_full(&self->ssl_client, &self->certification_context, trust_anchors, trusted_anchors_size);
    }
    else{
       br_ssl_client_init_full(&self->ssl_client, &self->certification_context, TAs, TAs_NUM);

    }
    br_ssl_engine_set_all_flags(& self->ssl_client.eng, BR_OPT_TOLERATE_NO_CLIENT_AUTH);
    br_ssl_engine_set_buffer(&self->ssl_client.eng, self->bear_buffer, sizeof(self->bear_buffer), 1);

    br_ssl_client_reset(&self->ssl_client, hostname, 0);
    br_sslio_init(&self->ssl_io, &self->ssl_client.eng, private_BearHttps_sock_read,
                  &self->connection_file_descriptor,
                  private_BearHttps_sock_write,
                  &self->connection_file_descriptor
    );
}



#endif