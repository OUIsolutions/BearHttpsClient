### Function: BearHttpsRequest_set_known_ips
```c
void BearHttpsRequest_set_known_ips(BearHttpsRequest *self, const char *known_ips[], int known_ips_size);
```
### Description
Sets a list of known IP addresses for the request's hostname. This allows the client to bypass DNS resolution and connect directly to specific IP addresses, which can improve performance and reliability.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `known_ips`: Array of IP address strings (e.g., ["93.184.216.34", "93.184.216.35"])
- `known_ips_size`: Number of IP addresses in the array

### Returns
This function does not return a value.
