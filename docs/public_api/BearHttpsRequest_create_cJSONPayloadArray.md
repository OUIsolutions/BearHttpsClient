### Function: BearHttpsRequest_create_cJSONPayloadArray
```c
const cJSON * BearHttpsRequest_create_cJSONPayloadArray(BearHttpsRequest *self);
```
### Description
Creates a new cJSON array and stores it internally in the request for later use as the request body. This is useful for building JSON array payloads programmatically. The returned JSON array is managed by the request and will be freed automatically.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object

### Returns
A pointer to the newly created cJSON array, or `NULL` if creation fails. The returned pointer is const and should not be freed manually.
