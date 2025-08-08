### Function: BearHttpsResponse_get_headers_size
```c
int BearHttpsResponse_get_headers_size(BearHttpsResponse *self);
```
### Description
Gets the number of headers in the HTTP response. This is useful for iterating through all response headers.

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object

### Returns
The number of headers in the response, or -1 if the response is invalid or an error occurred.
