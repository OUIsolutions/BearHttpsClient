#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)



typedef struct BearHttpsNamespace{
    short REFERENCE;
    short GET_OWNERSHIP;
    short COPY;
    BearHttpsRequestNamespace request;
    BearHttpsResponseNamespace response;
} BearHttpsNamespace;

#endif