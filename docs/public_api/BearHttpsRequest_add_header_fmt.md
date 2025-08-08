### Function: BearHttpsRequest_add_header_fmt
```c
void BearHttpsRequest_add_header_fmt(BearHttpsRequest *self, const char *key, const char *format, ...);
```
### Description
Adds a header to the HTTP request using printf-style formatting for the value. This is useful when you need to construct header values dynamically.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `key`: The header key/name (e.g., "Content-Length", "Authorization")
- `format`: Printf-style format string for the header value
- `...`: Variable arguments matching the format string

### Returns
This function does not return a value.
