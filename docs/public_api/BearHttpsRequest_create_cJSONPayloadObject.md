### Function: BearHttpsRequest_create_cJSONPayloadObject
```c
const cJSON * BearHttpsRequest_create_cJSONPayloadObject(BearHttpsRequest *self);
```
### Description
Creates a new cJSON object and stores it internally in the request for later use as the request body. This is useful for building JSON payloads programmatically. The returned JSON object is managed by the request and will be freed automatically.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object

### Returns
A pointer to the newly created cJSON object, or `NULL` if creation fails. The returned pointer is const and should not be freed manually.
