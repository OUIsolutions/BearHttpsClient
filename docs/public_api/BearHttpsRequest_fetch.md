### Function: BearHttpsRequest_fetch
```c
BearHttpsResponse * BearHttpsRequest_fetch(BearHttpsRequest *self);
```
### Description
Executes the HTTP request and returns the response. This function performs the actual network communication, including DNS resolution, SSL/TLS handshake, and data transfer.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object

### Returns
A pointer to a `BearHttpsResponse` object containing the server's response. The caller is responsible for freeing the response using `BearHttpsResponse_free()`.
