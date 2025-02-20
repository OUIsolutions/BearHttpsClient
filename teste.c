#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("teste");
    BearHttpsRequest_set_method(request,"POST");
    BearHttpsRequest_add_headder(request, "name", "mateus");
    BearHttpsRequest_set_body_file_stream(request,"teste.png");

    BearHttpsResponse *response =BearHttpsRequest_fetch(request);
    //BearHttpsRequest_free(request);
    //BearHttpsResponse_free(response);
}
