### Function: BearHttpsRequest_send_body_str
```c
void BearHttpsRequest_send_body_str(BearHttpsRequest *self, char *content);
```
### Description
Sends a string as the request body using the default ownership strategy (copy). This is a convenience function for sending text data, JSON strings, or other string-based content.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `content`: Pointer to the null-terminated string to send

### Returns
This function does not return a value.
