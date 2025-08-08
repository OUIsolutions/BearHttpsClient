### Function: BearHttpsRequest_set_chunk_header_read_props
```c
void BearHttpsRequest_set_chunk_header_read_props(BearHttpsRequest *self, int chunk_size, int max_chunk_size);
```
### Description
Configures the chunk reading properties for HTTP response headers. This controls how the client reads response data in chunks, which can be useful for memory management and large responses.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `chunk_size`: The size of each chunk to read
- `max_chunk_size`: The maximum chunk size allowed

### Returns
This function does not return a value.
