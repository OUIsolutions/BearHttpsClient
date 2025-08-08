### Function: BearHttpsResponse_get_body_size
```c
int BearHttpsResponse_get_body_size(BearHttpsResponse *self);
```
### Description
Gets the size of the response body in bytes. This is useful for determining how much data was received.

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object

### Returns
The size of the response body in bytes, or -1 if the response is invalid or an error occurred.
