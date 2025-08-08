### Function: newBearHttpsRequest_with_url_ownership_config
```c
BearHttpsRequest * newBearHttpsRequest_with_url_ownership_config(char *url, short url_ownership_mode);
```
### Description
Creates a new `BearHttpsRequest` object with the specified URL and ownership mode. This function allows control over how the URL string is handled by the library.

### Parameters
- `url`: A string representing the URL for the request. This should be a valid HTTPS URL.
- `url_ownership_mode`: Controls how the URL string is handled. Can be:
  - `BEARSSL_HTTPS_REFERENCE` (0): Keeps the reference of the URL (caller must ensure URL remains valid)
  - `BEARSSL_HTTPS_GET_OWNERSHIP` (1): Takes ownership of the URL and frees it when not needed
  - `BEARSSL_HTTPS_COPY` (2): Makes a copy of the URL

### Returns
A pointer to the newly created `BearHttpsRequest` object. If the URL is invalid or memory allocation fails, it returns `NULL`.
