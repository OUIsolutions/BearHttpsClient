

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef BearsslHttps_allocate
#define BearsslHttps_allocate malloc
#endif

#ifndef BearsslHttps_reallocate
#define BearsslHttps_reallocate realloc
#endif

#ifndef BearsslHttps_free
#define BearsslHttps_free free
#endif

#include "../../dependencies/UniversalSocket.h"
#define BR_ENABLE_INTRINSICS   1
#include "../../dependencies/BearSSLSingleUnit.h"

#include "../../dependencies/BearSSLTrustAnchors.h"

#ifndef BEARSSL_HTTPS_MOCK_CJSON
#include "../../dependencies/cJSON.h"
#endif
