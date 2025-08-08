#if !defined(__EMSCRIPTEN__)



typedef struct BearHttpsNamespace{
    short REFERENCE;
    short GET_OWNERSHIP;
    short COPY;
    BearHttpsRequestNamespace request;
    BearHttpsResponseNamespace response;
} BearHttpsNamespace;

#endif