#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://pt.wikipedia.org/wiki/China");
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
