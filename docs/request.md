## Configuring Headers

With the function **BearHttpsRequest_add_header** you can add a header to the request. The function takes two arguments: the first is the header name and the second is the header value.

```c
    BearHttpsRequest_add_header(request, "Content-Type", "application/json");
```

## Configuring Methods 

With the function **BearHttpsRequest_set_method** you can set the method of the request. The function takes one argument: the method name.

```c
    BearHttpsRequest_set_method(request, "POST");
```

## Configuring URL

With the function **BearHttpsRequest_set_url** you can set the URL of the request. The function takes one argument: the URL.

```c
    BearHttpsRequest_set_url(request, "https://jsonplaceholder.typicode.com/posts");
```

## Upload Data

You can upload text/files/json using different **send** functions.

### Send Text

Send a text using the function **BearHttpsRequest_send_body_str**. The function takes two arguments: the first is the request and the second is the text.
```c
    BearHttpsRequest_send_body_str(request, "teste de texto");
```


### Send File

Send a file using the function **BearHttpsRequest_send_file_auto_detect_content_type**. The function takes two arguments: the first is the request and the second is the file path.
```c
    BearHttpsRequest_send_file_auto_detect_content_type(request, "/home/user/file.png");
```

If you want to explicitly set the content type, you can use the function **BearHttpsRequest_send_file**. The function takes three arguments: the first is the request, the second is the file path, and the third is the content type.
```c
    BearHttpsRequest_send_file(request, "/home/user/file.png", "image/png");
```

### Sending Raw Data

You can send raw data using the function **BearHttpsRequest_send_any**. The function takes three arguments: the first is the request, the second is the data, and the third is the size. Note that these data will be copied to the **request** struct.
```c
    unsigned char text[] = "whatever";
    BearHttpsRequest_send_any(request, text, sizeof(text)-1);
```

If you don't want the data to be copied to the request struct, you can use the **BearHttpsRequest_send_any_with_ownership_control** function that allows you to define the ownership mode. Check the [Ownership System](/doc/ownership_system.md) section for more information.

```c
    unsigned char text[] = "what ever";
    BearHttpsRequest_send_any_with_ownership_control(request2, text, sizeof(text)-1, BEARSSL_HTTPS_REFERENCE);
```
