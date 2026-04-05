//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if (defined(__unix__) || defined(__APPLE__)) && !defined(__EMSCRIPTEN__)

void private_BearHttpsResponse_inject_entropy(br_ssl_client_context *ctx) {
    if (privateBearHttps_entropy_code_readed == 0){
        int fd = open("/dev/urandom", O_RDONLY);
        read(fd, privateBearHttps_entropy_code, sizeof(privateBearHttps_entropy_code));
        close(fd);
        privateBearHttps_entropy_code_readed = 1;
    }
    br_ssl_engine_inject_entropy(&ctx->eng, privateBearHttps_entropy_code, sizeof(privateBearHttps_entropy_code));
}

#endif
