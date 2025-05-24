# Reading Responses

After you configure your request, you can use the fetch function to send the request and get the response.
```c
    #include "BearHttpsClientOne.c"

    int main(){
        BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        if(BearHttpsResponse_error(response)){
            printf("Error: %s\n",BearHttpsResponse_get_error_msg(response));
            BearHttpsRequest_free(request);
            BearHttpsResponse_free(response);
            return 1;
        }

        const char *body = BearHttpsResponse_read_body_str(response);
        if(BearHttpsResponse_error(response)){
            printf("Error: %s\n",BearHttpsResponse_get_error_msg(response));
            BearHttpsRequest_free(request);
            BearHttpsResponse_free(response);
            return 1;
        }

        printf("Body: %s\n",body);
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);

        return 0;
    }
```

With the response, you can get the status code, iterate over the headers, or read the body of the response.

### Getting Status Code 

This gets the status code of the response:
```c
    int status_code = BearHttpsResponse_get_status_code(response);
    printf("Status code: %d\n",status_code);
```

### Getting a Header by Key

If you know the header name, you can get it using the following code:
```c
    char *content_type = BearHttpsResponse_get_header_value_by_key(response,"Content-Type");
    if(content_type){
        printf("Content-Type: %s\n",content_type);
    }
    else{
        printf("Content-Type: Not Found\n");
    }
```

### Iterating over the Headers

You can easily iterate over the headers of the response using the following code:
```c
    int header_size = BearHttpsResponse_get_headers_size(response);
    for(int i = 0; i < header_size; i++){
        char *key = BearHttpsResponse_get_header_key_by_index(response,i);
        char *header = BearHttpsResponse_get_header_value_by_index(response,i);
        printf("%s: %s\n",key,header);
    }    
```

### Reading the Body of the Response

You can read the body of the response as a string/binary or JSON.

#### Reading Body as String

Note that these functions will drop an error if the response is binary:
```c 
    const char *body = BearHttpsResponse_read_body_str(response);
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n",BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response); 
        return 1;
    }
    printf("Body: %s\n",body);
```

#### Reading Body as Binary

```c 
    unsigned char *body = BearHttpsResponse_read_body(response);
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n",BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response); 
        return 1;
    }
    long body_size = BearHttpsResponse_get_body_size(response);
    for(int i = 0; i < body_size;i++){
        printf("%c",body[i]);
    }
    printf("\n");
```

#### Reading Body as JSON

```c
    #include "src/one.c"

    int main(){
        BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/todos/1");
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        if(BearHttpsResponse_error(response)){
            printf("Error: %s\n",BearHttpsResponse_get_error_msg(response));
            BearHttpsRequest_free(request);
            BearHttpsResponse_free(response);
            return 1;
        }

        cJSON *json = BearHttpsResponse_read_body_json(response);
        char *dumped = cJSON_Print(json);
        printf("%s\n",dumped);
        free(dumped);
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);

        return 0;
    }
```

### Reading Body Chunk

You can read the body of the response in chunks using the following code:
```c
    #include "src/one.c"

    int main(){
        BearHttpsRequest *request = newBearHttpsRequest("https://example.com/");
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        if(BearHttpsResponse_error(response)){
            printf("Error: %s\n",BearHttpsResponse_get_error_msg(response));
            BearHttpsRequest_free(request);
            BearHttpsResponse_free(response);
            return 1;
        }
        unsigned char chunk[1024];
        while(BearHttpsResponse_read_body_chunck(response,chunk,sizeof(chunk)-1) > 0){
            printf("%s",chunk);
        }
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);

        return 0;
    }
```
