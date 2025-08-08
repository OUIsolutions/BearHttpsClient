### Function: BearHttpsRequest_send_cJSON
```c
void BearHttpsRequest_send_cJSON(BearHttpsRequest *self, cJSON *json);
```
### Description
Sends a cJSON object as the request body using the default ownership strategy (copy). This is a convenience function for sending JSON data. The JSON is automatically serialized to a string before sending.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `json`: Pointer to the cJSON object to send

### Returns
This function does not return a value.
