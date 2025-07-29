//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.os.h"
//silver_chain_scope_end
#if defined(PRIVATE_BEAR_HTTPS_BARE_METAL)

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#ifndef BearsslHttps_allocate
#define BearsslHttps_allocate malloc
#endif

#ifndef BearsslHttps_reallocate
#define BearsslHttps_reallocate realloc
#endif

#ifndef BearsslHttps_free
#define BearsslHttps_free free
#endif

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

#if !defined(cJSON__h) && !defined(BEARSSL_HTTPS_MOCK_CJSON) 
#include "../../../dependencies/cJSON.h"
#define BEARSSL_HTTPS_CJSON_DECLARATED
#endif

#endif