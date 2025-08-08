### Function: BearHttpsResponse_get_error_msg
```c
const char* BearHttpsResponse_get_error_msg(BearHttpsResponse *self);
```
### Description
Gets a human-readable error message if an error occurred during the HTTP request. This function should only be called if `BearHttpsResponse_error()` returns `true`.

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object

### Returns
A pointer to a null-terminated string containing the error message, or `NULL` if no error occurred or the response is invalid.
