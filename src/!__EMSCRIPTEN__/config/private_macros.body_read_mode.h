#if !defined(__EMSCRIPTEN__)

#define PRIVATE_BEARSSL_BY_CONTENT_LENGTH 0
#define PRIVATE_BEARSSL_BY_CHUNKED 1
//when no content length or chunked encoding is present, the body is read until an error occurs or the end of the stream is reached.
#define PRIVATE_BEARSSL_BY_READ_ERROR 2 
#endif