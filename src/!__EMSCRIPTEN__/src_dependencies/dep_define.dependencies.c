//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.globals.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)




#if !defined(BEARSSL_HTTPS_MOCK_UNIVERSAL_SOCKET_DEFINE) && defined(BEARSSL_HTTPS_UNIVERSAL_SOCKET_DECLARATED)
#include "../../../dependencies/UniversalSocket.c"
#endif 

#if !defined(BEARSSL_HTTPS_MOCK_BEARSSL_DEFINE) && defined(BEARSSL_HTTPS_BEARSSL_DECLARATED)
#include "../../../dependencies/BearSSLTrustAnchors.c"
#include "../../../dependencies/BearSSLSingleUnit.c"
#endif 


#endif