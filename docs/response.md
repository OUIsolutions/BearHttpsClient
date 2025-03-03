# Reading Reponses

After you configure your request you can use the fetch function to send the request and get the response.
```c
    #include "BearHttpsClientOne.c"

    BearHttpsNamespace bear ;
    int main(){
        bear = newBearHttpsNamespace();

        BearHttpsRequest *request = bear.request.newBearHttpsRequest("https://example.com");
        BearHttpsResponse *response = bear.request.fetch(request);
        if(bear.response.error(response)){
            printf("Error: %s\n",bear.response.get_error_msg(response));
            bear.request.free(request);
            bear.response.free(response);
            return 1;
        }

        const char *body = bear.response.read_body_str(response);
        if(bear.response.error(response)){
                printf("Error: %s\n",bear.response.get_error_msg(response));
                bear.request.free(request);
                bear.response.free(response);
                            return 1;
        }

        printf("Body: %s\n",body);
        bear.request.free(request);
        bear.response.free(response);

        return 0;
    }
```

With the response you can get the status code, iterate over the headers, or read the body of the response.

### Getting Status Code 

This gets the status code of the response:
```c
    int status_code = bear.response.get_status_code(response);
    printf("Status code: %d\n",status_code);

```
### Getting a Header by Key

If you know the header name you can get it using the following code:
```c
    char *content_type = bear.response.get_header_value_by_key(response,"Content-Type");
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
    int header_size = bear.response.get_headers_size(response);
    for(int i = 0; i < header_size; i++){
        char *key = bear.response.get_header_key_by_index(response,i);
        char *header = bear.response.get_header_value_by_index(response,i);
        printf("%s: %s\n",key,header);
    }    
```
### Reading the Body of the Response

You can read the body of the response as a string/binary or JSON.

#### Reading Body as String

Note that these function will drop a error if the response its a binary:
```c 
    const char *body = bear.response.read_body_str(response);
    if(bear.response.error(response)){
            printf("Error: %s\n",bear.response.get_error_msg(response));
            bear.request.free(request);
            bear.response.free(response); 
            return 1;
    }
    printf("Body: %s\n",body);
```
#### Reading Body as Binary

```c 
    unsigned  char *body = bear.response.read_body(response);
    if(bear.response.error(response)){
            printf("Error: %s\n",bear.response.get_error_msg(response));
            bear.request.free(request);
            bear.response.free(response); 
            return 1;
    }
    long body_size = bear.response.get_body_size(response);
    for(int i = 0; i < body_size;i++){
        printf("%c",body[i]);
    }
    printf("\n");
```

#### Reading Body as JSON

```c
    #include "src/one.c"

    BearHttpsNamespace bear ;
    int main(){
        bear = newBearHttpsNamespace();

        BearHttpsRequest *request = bear.request.newBearHttpsRequest("https://jsonplaceholder.typicode.com/todos/1");
        BearHttpsResponse *response = bear.request.fetch(request);
        if(bear.response.error(response)){
            printf("Error: %s\n",bear.response.get_error_msg(response));
            bear.request.free(request);
            bear.response.free(response);
            return 1;
        }

        cJSON *json = bear.response.read_body_json(response);
        char *dumped = cJSON_Print(json);
        printf("%s\n",dumped);
        free(dumped);
        bear.request.free(request);
        bear.response.free(response);

        return 0;
}
```

### Reading Body Chunk

You can read the body of the response in chunks using the following code:
```c
    #include "src/one.c"

    BearHttpsNamespace bear ;
    int main(){
        bear = newBearHttpsNamespace();

        BearHttpsRequest *request = bear.request.newBearHttpsRequest("https://example.com/");
        BearHttpsResponse *response = bear.request.fetch(request);
        if(bear.response.error(response)){
            printf("Error: %s\n",bear.response.get_error_msg(response));
            bear.request.free(request);
            bear.response.free(response);
            return 1;
        }
        unsigned char chunk[1024];
        while(bear.response.read_body_chunck(response,chunk,sizeof(chunk)-1) > 0){
            printf("%s",chunk);
        }
        bear.request.free(request);
        bear.response.free(response);

        return 0;
    }
```
