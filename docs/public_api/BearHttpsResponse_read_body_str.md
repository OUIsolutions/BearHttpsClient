### Function: BearHttpsResponse_read_body_str
```c
const char* BearHttpsResponse_read_body_str(BearHttpsResponse *self);
```
### Description
Reads the response body as a null-terminated string. This is useful for text-based responses like HTML, JSON, or plain text. **Important: Always check for NULL before using the return value.**

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object

### Returns
A pointer to the response body as a null-terminated string, or `NULL` if the body is empty, invalid, or the response has an error.
