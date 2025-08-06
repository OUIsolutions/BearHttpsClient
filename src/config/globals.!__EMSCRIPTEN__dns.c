//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)



BearHttpsClientDnsProvider privateBearHttpsProviders[] = {
    {.hostname= "dns.google.com",.route="/resolve",.ip = "8.8.8.8",.port=443},
    {.hostname= "dns.nextdns.io",.route="/dns-query",.ip = "217.146.9.93",.port=443},

   
};


int privateBearHttpsProvidersSize = sizeof(privateBearHttpsProviders)/sizeof(BearHttpsClientDnsProvider);


#endif