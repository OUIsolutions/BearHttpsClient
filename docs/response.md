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

        char *body = bear.response.read_body_str(response);
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

With the response you can get the satus code, iterate over the headders, or read the body of the response.

### Getting Status Code 

This gets the status code of the response:
```c
    int status_code = bear.response.get_status_code(response);
    printf("Status code: %d\n",status_code);

```
### Getting a Headder by key 

If you know the headder name you can get it using the following code:
```c
    char *content_type = bear.response.get_headder_value_by_key(response,"Content-Type");
    if(content_type){
            printf("Content-Type: %s\n",content_type);
    }
    else{
        printf("Content-Type: Not Found\n");
    }
```

### Iterating over the headders

You can easily iterate over the headders of the response using the following code:
```c
    int headder_size = bear.response.get_headders_size(response);
    for(int i = 0; i < headder_size; i++){
        char *key = bear.response.get_headder_key_by_index(response,i);
        char *headder = bear.response.get_headder_value_by_index(response,i);
        printf("%s: %s\n",key,headder);
    }    
```
### Reading the body of the response

You can read the body of the response as a string/binary or JSON.

### Reading Body str

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
### Reading Body binary

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

### Reading body json 

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
