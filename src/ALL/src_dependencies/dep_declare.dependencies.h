

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


#if !defined(cJSON__h) && !defined(BEARSSL_HTTPS_MOCK_CJSON)
#include "../../../dependencies/cJSON.h"
#define BEARSSL_HTTPS_CJSON_DECLARATED
#endif
