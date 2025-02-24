

#ifndef BEARSSL_HTTPS_CLIENT_NOT_LOAD_LIB
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#endif
#ifndef BearsslHttps_allocate
#define BearsslHttps_allocate malloc
#endif

#define BR_ENABLE_INTRINSICS   1

#include "../../dependencies/BearSSLSingleUnit.h"
#include "../../dependencies/UniversalSocket.h"

#include "../../dependencies/BearSSLTrustAnchors.h"
