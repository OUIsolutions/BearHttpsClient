#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com/");
    BearHttpsResponse *response =BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("error: %s\n",BearHttpsResponse_get_error_msg(response));
    }
    char buffer[1024];
    while(BearHttpsResponse_read(response,(unsigned char*)buffer,sizeof(buffer)) > 0){
        printf("buffer: %s\n",buffer);
    }

    BearHttpsResponse_free(response);

}
