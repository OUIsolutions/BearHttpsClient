### Function: BearHttpsResponse_get_status_code
```c
int BearHttpsResponse_get_status_code(BearHttpsResponse *self);
```
### Description
Gets the HTTP status code from the response (e.g., 200 for OK, 404 for Not Found, 500 for Internal Server Error).

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object

### Returns
The HTTP status code as an integer, or -1 if the response is invalid or an error occurred.
