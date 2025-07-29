#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)



BearHttpsNamespace newBearHttpsNamespace(){
    BearHttpsNamespace self = {0};
    self.REFERENCE = BEARSSL_HTTPS_REFERENCE;
    self.GET_OWNERSHIP = BEARSSL_HTTPS_GET_OWNERSHIP;
    self.COPY = BEARSSL_HTTPS_COPY;
    self.response = newBearHttpsResponseNamespace();
    self.request= newBearHttpsRequestNamespace();
    return self;
}

#endif