


#ifdef _WIN32
typedef __m128i __m128i_u;
#endif

#if !defined(BEARSSL_HTTPS_MOCK_UNIVERSAL_SOCKET_DEFINE)
#include "../../dependencies/UniversalSocket.c"
#endif 

#if !defined(BEARSSL_HTTPS_MOCK_BEARSSL_DEFINE)
#include "../../dependencies/BearSSLSingleUnit.c"
#endif 

#if !defined(BEARSSL_HTTPS_MOCK_CJSON) || !defined(BEARSSL_HTTPS_MOCK_CJSON_DEFINE)
#include "../../dependencies/cJSON.c"
#endif
