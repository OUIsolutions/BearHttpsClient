### Function: BearHttpsResponse_get_header_value_by_sanitized_key
```c
const char* BearHttpsResponse_get_header_value_by_sanitized_key(BearHttpsResponse *self, const char *key);
```
### Description
Gets the value of a response header by its key (name) using case-insensitive matching. This function normalizes the key for better compatibility with different server implementations. **Important: Always check for NULL before using the return value.**

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object
- `key`: The header key to search for (case-insensitive, e.g., "content-type", "Content-Type")

### Returns
A pointer to the header value string, or `NULL` if the key is not found or the response has an error.
