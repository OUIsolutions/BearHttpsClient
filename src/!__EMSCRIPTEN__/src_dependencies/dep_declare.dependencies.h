
#if !defined(__EMSCRIPTEN__)




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