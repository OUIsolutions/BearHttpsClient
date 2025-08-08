### Function: BearHttpsResponse_free
```c
void BearHttpsResponse_free(BearHttpsResponse *self);
```
### Description
Frees the memory allocated for a `BearHttpsResponse` object and all its associated resources. This function should be called for every response object received to prevent memory leaks.

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object to free

### Returns
This function does not return a value.
