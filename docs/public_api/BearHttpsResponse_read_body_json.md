### Function: BearHttpsResponse_read_body_json
```c
const cJSON * BearHttpsResponse_read_body_json(BearHttpsResponse *self);
```
### Description
Reads and parses the response body as a JSON object. This automatically handles JSON parsing and returns a cJSON object that can be used to access JSON data. **Important: Always check for NULL before using the return value. The returned cJSON is const and should not be freed manually.**

### Parameters
- `self`: Pointer to the `BearHttpsResponse` object

### Returns
A pointer to a const cJSON object representing the parsed JSON, or `NULL` if the body is not valid JSON, is empty, or the response has an error.
