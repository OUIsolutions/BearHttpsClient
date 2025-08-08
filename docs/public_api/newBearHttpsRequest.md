
### Function: newBearHttpsRequest
```c
BearHttpsRequest * newBearHttpsRequest(const char *url);
```
### Description
Creates a new `BearHttpsRequest` object with the specified URL. The URL is copied internally, so the original string can be freed after this call.
### Parameters
- `url`: A string representing the URL for the request. This should be a valid HTTPS URL.

### Returns
A pointer to the newly created `BearHttpsRequest` object. If the URL is invalid or memory allocation fails, it returns `NULL`.
