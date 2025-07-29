
#if !defined(__EMSCRIPTEN__)


#if defined(_MSC_VER)
    #include <BaseTsd.h>
    #include <intrin.h>

    typedef SSIZE_T ssize_t;
    typedef __m128i __m128i_u;

    #define __src_inner_hbyteswap_ulong _byteswap_ulong
#endif


#if !defined(UniversalSocket_dep)
#include "../../../dependencies/UniversalSocket.h"
#define BEARSSL_HTTPS_UNIVERSAL_SOCKET_DECLARATED
#endif 

#if !defined(BR_BEARSSL_H__)
#define BR_ENABLE_INTRINSICS   1
#include "../../../dependencies/BearSSLSingleUnit.h"
#define BEARSSL_HTTPS_BEARSSL_DECLARATED
#endif


#endif