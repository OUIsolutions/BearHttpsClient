#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com/");
    BearHttpsResponse *response =BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("error: %s\n",BearHttpsResponse_get_error_msg(response));
    }
    printf("chegou aqui\n");
    char buffer[1024];
    int size = BearHttpsResponse_read(response,(unsigned char*)buffer,1024);
    printf("size: %d\n",size);
    printf("buffer: %s\n",buffer);

    BearHttpsResponse_free(response);

}
