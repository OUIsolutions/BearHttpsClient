

#ifndef BEARSSL_HTTPS_CLIENT_NOT_LOAD_LIB
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

#endif
#ifndef BearsslHttps_allocate
#define BearsslHttps_allocate malloc
#endif

#ifndef BearsslHttps_reallocate
#define BearsslHttps_reallocate realloc
#endif

#define BR_ENABLE_INTRINSICS   1

#include "../../dependencies/BearSSLSingleUnit.h"
#include "../../dependencies/UniversalSocket.h"

#include "../../dependencies/BearSSLTrustAnchors.h"
