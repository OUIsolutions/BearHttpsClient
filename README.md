# BearHttpsClient
a Unoficial BearSSL Https Client


## Releases

| item          | What Is |
|-------        |-----------|
| [BearSSLSingleUnit.zip](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.0.8/BearHttpsClient.zip)| Full Folder Mode  |
| [BearSSLSingleUnitOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.0.8/BearHttpsClientOne.c)| Single File Version|
| [BearHttpsClient.h](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.0.8/BearHttpsClient.h)|Declaration |
| [BearHttpsClient.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.0.8/BearHttpsClient.c)|Definition |


### Most simple example
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


## Build from scratch
for buiding from scratch you need to have darwin installed on your machine, for install darwin:
```bash
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.020/darwin.out -o darwin.out && chmod +x darwin.out &&  sudo  mv darwin.out /usr/bin/darwin
```
then you can build the project with in the root dir of the project:
```bash
darwin run_blueprint build/  --mode folder
```
it will generate all the **releases** in the **/release** dir
