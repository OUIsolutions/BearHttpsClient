//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if (defined(__unix__) || defined(__APPLE__)) && !defined(__EMSCRIPTEN__)

void private_BearHttpsResponse_inject_entropy(br_ssl_client_context *ctx) {
    unsigned char entropy[32];
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, entropy, sizeof(entropy));
    close(fd);
    br_ssl_engine_inject_entropy(&ctx->eng, entropy, sizeof(entropy));
}

#endif
