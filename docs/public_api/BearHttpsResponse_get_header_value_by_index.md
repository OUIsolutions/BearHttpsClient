### Function: BearHttpsResponse_get_header_value_by_index
```c
const char* BearHttpsResponse_get_header_value_by_index(BearHttpsResponse *self, int index);
```
### Description
Gets the value of a response header by its index position. Use this function with `BearHttpsResponse_get_headers_size()` to iterate through all headers. **Important: Always check for NULL before using the return value.**

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object
- `index`: Zero-based index of the header (0 to headers_size - 1)

### Returns
A pointer to the header value string, or `NULL` if the index is invalid or the response has an error.
