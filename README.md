# BearHttpsClient
a Unoficial BearSSL Https Client
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


### Most simple example
```c
#include "BearHttpsClientOne.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://serjaoberranteiroserver.com.br/");
    BearHttpsRequest_add_headder(request, "nome", "samuel");
    BearHttpsRequest_set_method(request, "GET");
    BearHttpsResponse *response =BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("error: %s\n",BearHttpsResponse_get_error_msg(response));
    }
    char buffer[1024] ={0};
    while(BearHttpsResponse_read_body_chunck(response,(unsigned char*)buffer,sizeof(buffer)) > 0){
        printf("buffer: %s\n",buffer);
        memset(buffer,0,sizeof(buffer));
    }
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);

}
```
