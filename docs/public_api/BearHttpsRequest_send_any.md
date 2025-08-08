### Function: BearHttpsRequest_send_any
```c
void BearHttpsRequest_send_any(BearHttpsRequest *self, unsigned char *content, long size);
```
### Description
Sends binary data as the request body using the default ownership strategy (copy). This is a convenience function that calls `BearHttpsRequest_send_any_with_ownership_control` with `BEARSSL_HTTPS_COPY`.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `content`: Pointer to the binary data to send
- `size`: Size of the data in bytes

### Returns
This function does not return a value.
