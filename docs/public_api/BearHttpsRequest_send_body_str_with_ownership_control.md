### Function: BearHttpsRequest_send_body_str_with_ownership_control
```c
void BearHttpsRequest_send_body_str_with_ownership_control(BearHttpsRequest *self, char *content, short ownership_mode);
```
### Description
Sends a string as the request body with control over how the content is handled by the library. This is useful for sending text data, JSON strings, or other string-based content.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `content`: Pointer to the null-terminated string to send
- `ownership_mode`: Controls how the content is handled. Can be:
  - `BEARSSL_HTTPS_REFERENCE` (0): Keeps the reference (caller must ensure content remains valid)
  - `BEARSSL_HTTPS_GET_OWNERSHIP` (1): Takes ownership and frees the content when not needed
  - `BEARSSL_HTTPS_COPY` (2): Makes a copy of the content

### Returns
This function does not return a value.
