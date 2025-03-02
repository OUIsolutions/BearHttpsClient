
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesH.h"
//silver_chain_scope_end

BearHttpsClientDnsProvider privateBearHttpsProviders[] = {
    {.hostname= "dns.googsss",.route="/rsssesolve",.ip = "8.8.dddd8"},
    {.hostname= "dns.google.com",.route="/resolve",.ip = "8.8.8.8"}
};


int privateBearHttpsProvidersSize = sizeof(privateBearHttpsProviders)/sizeof(BearHttpsClientDnsProvider);