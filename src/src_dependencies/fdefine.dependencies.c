
//silver_chain_scope_start
//managed by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

#if !defined(BEARSSL_HTTPS_MOCK_UNIVERSAL_SOCKET_DEFINE)
#include "../../dependencies/UniversalSocket.c"
#endif 

#if !defined(BEARSSL_HTTPS_MOCK_BEARSSL_DEFINE)
#include "../../dependencies/BearSSLSingleUnit.c"
#endif 

#if !defined(BEARSSL_HTTPS_MOCK_CJSON) || !defined(BEARSSL_HTTPS_MOCK_CJSON_DEFINE)
#include "../../dependencies/cJSON.c"
#endif