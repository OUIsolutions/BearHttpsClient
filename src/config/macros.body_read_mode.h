//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_declare.h"
//silver_chain_scope_end

#define PRIVATE_BEARSSL_BY_CONTENT_LENGTH 0
#define PRIVATE_BEARSSL_BY_CHUNKED 1
//when no content length or chunked encoding is present, the body is read until an error occurs or the end of the stream is reached.
#define PRIVATE_BEARSSL_BY_READ_ERROR 2 