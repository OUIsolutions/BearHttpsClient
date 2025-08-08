### Function: BearHttpsRequest_free
```c
void BearHttpsRequest_free(BearHttpsRequest *self);
```
### Description
Frees the memory allocated for a `BearHttpsRequest` object and all its associated resources. This function should be called for every request object created to prevent memory leaks.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object to free

### Returns
This function does not return a value.
