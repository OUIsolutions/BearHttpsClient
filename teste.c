#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("ouo");
    BearHttpsRequest_set_method(request,"POST");
    BearHttpsRequest_add_headder(request, "name", "mateus");
    BearHttpsRequest_set_body_file_stream(request,"teste.png");

//    BearHttpsResponse *response =BearHttpsRequest_fetch(request);

}
