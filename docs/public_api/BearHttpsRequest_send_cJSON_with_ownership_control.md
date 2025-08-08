### Function: BearHttpsRequest_send_cJSON_with_ownership_control
```c
void BearHttpsRequest_send_cJSON_with_ownership_control(BearHttpsRequest *self, cJSON *json, short ownership_mode);
```
### Description
Sends a cJSON object as the request body with control over how the JSON object is handled by the library. The JSON is automatically serialized to a string before sending.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `json`: Pointer to the cJSON object to send
- `ownership_mode`: Controls how the JSON object is handled. Can be:
  - `BEARSSL_HTTPS_REFERENCE` (0): Keeps the reference (caller must ensure JSON remains valid)
  - `BEARSSL_HTTPS_GET_OWNERSHIP` (1): Takes ownership and frees the JSON when not needed
  - `BEARSSL_HTTPS_COPY` (2): Makes a copy of the JSON

### Returns
This function does not return a value.
