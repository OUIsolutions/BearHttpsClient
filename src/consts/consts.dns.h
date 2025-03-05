
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.typesH.h"
//silver_chain_scope_end

BearHttpsClientDnsProvider privateBearHttpsProviders[] = {
    {.hostname= "dns.google.com",.route="/resolve",.ip = "8.8.8.8",.port=443},
    {.hostname= "dns.quad9.net",.route="/dns-query",.ip = "9.9.9.9",.port=5053},
    {.hostname= "dns.nextdns.io",.route="/dns-query",.ip = "217.146.9.93",.port=443},

   
};


int privateBearHttpsProvidersSize = sizeof(privateBearHttpsProviders)/sizeof(BearHttpsClientDnsProvider);