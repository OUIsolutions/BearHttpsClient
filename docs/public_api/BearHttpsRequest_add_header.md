### Function: BearHttpsRequest_add_header
```c
void BearHttpsRequest_add_header(BearHttpsRequest *self, const char *key, const char *value);
```
### Description
Adds a header to the HTTP request. The header key and value are copied internally, so the original strings can be freed after this call.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `key`: The header key/name (e.g., "Content-Type", "Authorization")
- `value`: The header value (e.g., "application/json", "Bearer token123")

### Returns
This function does not return a value.
