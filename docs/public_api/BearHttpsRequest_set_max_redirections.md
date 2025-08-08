### Function: BearHttpsRequest_set_max_redirections
```c
void BearHttpsRequest_set_max_redirections(BearHttpsRequest *self, int max_redirections);
```
### Description
Sets the maximum number of HTTP redirections (3xx responses) that the client will automatically follow. By default, redirections are followed up to a reasonable limit.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `max_redirections`: Maximum number of redirections to follow (0 to disable redirections)

### Returns
This function does not return a value.
