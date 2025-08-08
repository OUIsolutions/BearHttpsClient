### Function: BearHttpsResponse_get_error_code
```c
int BearHttpsResponse_get_error_code(BearHttpsResponse *self);
```
### Description
Gets a numeric error code if an error occurred during the HTTP request. This provides a machine-readable way to identify specific error conditions.

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object

### Returns
A numeric error code, or 0 if no error occurred.
