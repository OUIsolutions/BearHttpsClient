//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#ifdef _WIN32


void private_BearHttpsResponse_inject_entropy(br_ssl_client_context *ctx) {
    if (privateBearHttps_entropy_code_readed == 0) {
        HCRYPTPROV hCryptProv = 0;
        
        // Acquire a cryptographic provider context handle
        if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
            // Generate random data
            if (CryptGenRandom(hCryptProv, sizeof(privateBearHttps_entropy_code), privateBearHttps_entropy_code)) {
                privateBearHttps_entropy_code_readed = 1;
            }
            // Release the provider handle
            CryptReleaseContext(hCryptProv, 0);
        }
    }
    br_ssl_engine_inject_entropy(&ctx->eng, privateBearHttps_entropy_code, sizeof(privateBearHttps_entropy_code));
}

#endif