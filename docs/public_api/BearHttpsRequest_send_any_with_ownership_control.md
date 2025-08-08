### Function: BearHttpsRequest_send_any_with_ownership_control
```c
void BearHttpsRequest_send_any_with_ownership_control(BearHttpsRequest *self, unsigned char *content, long size, short ownership_mode);
```
### Description
Sends binary data as the request body with control over how the content is handled by the library. This function allows sending any type of binary data.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `content`: Pointer to the binary data to send
- `size`: Size of the data in bytes
- `ownership_mode`: Controls how the content is handled. Can be:
  - `BEARSSL_HTTPS_REFERENCE` (0): Keeps the reference (caller must ensure content remains valid)
  - `BEARSSL_HTTPS_GET_OWNERSHIP` (1): Takes ownership and frees the content when not needed
  - `BEARSSL_HTTPS_COPY` (2): Makes a copy of the content

### Returns
This function does not return a value.
