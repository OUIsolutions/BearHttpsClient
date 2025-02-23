#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com/");
    BearHttpsResponse *response =BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("error: %s\n",BearHttpsResponse_get_error_msg(response));
    }
    BearHttpsResponse_free(response);

}
