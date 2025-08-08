### Function: BearHttpsResponse_get_header_value_by_key
```c
const char* BearHttpsResponse_get_header_value_by_key(BearHttpsResponse *self, const char *key);
```
### Description
Gets the value of a response header by its key (name). Header key matching is case-sensitive. **Important: Always check for NULL before using the return value.**

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object
- `key`: The header key to search for (e.g., "Content-Type", "Set-Cookie")

### Returns
A pointer to the header value string, or `NULL` if the key is not found or the response has an error.
