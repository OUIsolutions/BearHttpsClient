### Function: BearHttpsRequest_set_dns_providers
```c
void BearHttpsRequest_set_dns_providers(BearHttpsRequest *self, BearHttpsClientDnsProvider *dns_providers, int total_dns_providers);
```
### Description
Sets custom DNS providers for resolving hostnames. This allows the client to use DNS-over-HTTPS providers instead of the system's default DNS resolver.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `dns_providers`: Array of `BearHttpsClientDnsProvider` structures containing DNS provider configurations
- `total_dns_providers`: Number of DNS providers in the array

### Returns
This function does not return a value.
