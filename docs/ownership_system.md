### Ownership System

The Ownership System allows you to determine the way the lib will handle data.
It can be one of the following:

- **BEARSSL_HTTPS_REFERENCE**: The lib will not handle the data. It's useful for **HARDCODED** information.

```c
    unsigned char text[] = "what ever";
    BearHttpsRequest_send_any_with_ownership_control(request2, text, sizeof(text)-1, BEARSSL_HTTPS_REFERENCE);
```

- **BEARSSL_HTTPS_COPY**: The lib will copy the data to the request struct. It's useful for **DYNAMIC** information.
NOTE that you MUST free after the request.
```c
    unsigned char *text = strdup("what ever");
    BearHttpsRequest_send_any_with_ownership_control(request2, text, strlen(text), BEARSSL_HTTPS_COPY);
    free(text);
```

- **BEARSSL_HTTPS_GET_OWNERSHIP**: The lib will take the ownership of the data. It's useful for **DYNAMIC** information.
NOTE that you must NOT free after the request.
```c
    unsigned char *text = strdup("what ever");
    BearHttpsRequest_send_any_with_ownership_control(request2, text, strlen(text), BEARSSL_HTTPS_GET_OWNERSHIP);
```


The following functions allow you to get the ownership of the data:
```c
    BearHttpsRequest_add_header_with_ownership_config(request2, "key", BEARSSL_HTTPS_REFERENCE, "value", BEARSSL_HTTPS_REFERENCE);
```

and

```c
    BearHttpsRequest_set_url_with_ownership_config(request2, "example.com", BEARSSL_HTTPS_REFERENCE);
```
