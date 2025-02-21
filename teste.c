#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    BearHttpsRequest_set_method(request,"POST");
    BearHttpsRequest_add_headder(request, "name", "mateus");
    BearHttpsRequest_set_body_file_stream(request,"teste.png");
    BearHttpsResponse *response =BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("error: %s\n",BearHttpsResponse_get_error_msg(response));
    }
    BearHttpsResponse_free(response);

}
