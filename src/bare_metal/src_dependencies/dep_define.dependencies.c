


#ifdef _WIN32
typedef __m128i __m128i_u;
#endif

#if !defined(BEARSSL_HTTPS_MOCK_UNIVERSAL_SOCKET_DEFINE) && defined(BEARSSL_HTTPS_UNIVERSAL_SOCKET_DECLARATED)
#include "../../dependencies/UniversalSocket.c"
#endif 

#if !defined(BEARSSL_HTTPS_MOCK_BEARSSL_DEFINE) && defined(BEARSSL_HTTPS_BEARSSL_DECLARATED)
#include "../../dependencies/BearSSLTrustAnchors.c"
#include "../../dependencies/BearSSLSingleUnit.c"
#endif 

#if !defined(BEARSSL_HTTPS_MOCK_CJSON_DEFINE) && defined(BEARSSL_HTTPS_CJSON_DECLARATED)
#include "../../dependencies/cJSON.c"
#endif
