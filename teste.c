#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com/");
    BearHttpsResponse *response =BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("error: %s\n",BearHttpsResponse_get_error_msg(response));
    }
    printf("chegou aqui\n");
    char buffer[1024];
    while(private_BearHttpsResponse_read(response, (unsigned char*)buffer, 1024) > 0){
        printf("%s",buffer);
    }
    BearHttpsResponse_free(response);

}
