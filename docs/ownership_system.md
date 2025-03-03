
### Ownership System

The Ownership System allows you to determine the way the lib will handle data.
It can be one of the following:

- **bear.REFERENCE**: The lib will not handle the data. It's useful for **HARDCODED** information.

```c
    unsigned char text[] = "what ever";
    bear.request.send_any_with_ownership_control(request2,text,sizeof(text)-1,bear.REFERENCE);
```

- **bear.COPY**: The lib will copy the data to the request struct. It's useful for **DYNAMIC** information.
NOTE that you MUST free after the request.
```c
    unsigned char *text = strdup("what ever");
    bear.request.send_any_with_ownership_control(request2,text,strlen(text),bear.COPY);
    free(text);
```

- **bear.GET_OWNERSHIP**: The lib will take the ownership of the data. It's useful for **DYNAMIC** information.
NOTE that you must NOT free after the request.
```c
    unsigned char *text = strdup("what ever");
    bear.request.send_any_with_ownership_control(request2,text,strlen(text),bear.GET_OWNERSHIP);
```


The following functions allow you to get the ownership of the data:
```c
    bear.request.add_headder_with_ownership_config(request2,"key",bear.REFERENCE,"value",bear.REFERENCE);
```

and

```c
    bear.request.set_url_with_ownership_config(request2,"example.com",bear.REFERENCE);
```
