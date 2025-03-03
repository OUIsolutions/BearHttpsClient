
## Configuring Headders

With the function **bear.request.add_headder** you can add a headder to the request. The function takes two arguments: the first is the headder name and the second is the headder value.

```c
    bear.request.add_headder("Content-Type", "application/json");
```

## Configuring Methods 

With the function **bear.request.set_method** you can set the method of the request. The function takes one argument: the method name.

```c
    bear.request.set_method("POST");
```

## Configuring URL

With the function **bear.request.set_url** you can set the URL of the request. The function takes one argument: the URL.

```c
    bear.request.set_url("https://jsonplaceholder.typicode.com/posts");
```

## Upload Data

You can upload text/files/json using diferent **send** functions.

### Send Text

Send a text using the function **bear.request.send_body_str**. The function takes two arguments: the first is the request and the second is the text.
```c
    bear.request.send_body_str(request,"teste de texto");
```


### Send File

Send a file using the function **bear.request.send_file_auto_detect_content_type**. The function takes two arguments: the first is the request and the second is the file path.
```c
 bear.request.send_file_auto_detect_content_type(request,"/home/user/file.png");
```

If you want to explicit the content type you can use the function **bear.request.send_file**. The function takes three arguments, the first is the request, the second is the file path, and the third is the content type.
```c
 bear.request.send_file(request,"/home/user/file.png","image/png");
```

### Sending Raw Data

You can send raw data using the function **send_anya**. The function takes three arguments: the first is the request, the second is the data and the third is the size. Note that these data will be copied to the **request** struct.
```c
    unsigned char text[] = "whatever";
    bear.request.send_any(request,text,sizeof(text)-1);
```

If you dont want the data to be copied to the request struct, you can use the **bear.request.send_any_with_ownership_control** function that allows you define the ownership mode. Check the [Ownership System](/doc/ownership_system.md) section for more information.

```c
    unsigned char text[] = "what ever";
    bear.request.send_any_with_ownership_control(request2,text,sizeof(text)-1,bear.REFERENCE);
```
