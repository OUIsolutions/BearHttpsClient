
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesH.h"
//silver_chain_scope_end

BearHttpsClientDnsProvider privateBearHttpsProviders[] = {
    //{.hostname= "dns.quad9.net",.route="/dns-query",.ip = "9.9.9.9"},
    {.hostname= "dns.google.com",.route="/resolve",.ip = "8.8.8.8",.port=443}
};


int privateBearHttpsProvidersSize = sizeof(privateBearHttpsProviders)/sizeof(BearHttpsClientDnsProvider);