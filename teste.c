#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("teste");
    BearHttpsRequest_set_method(request,"POST");
    BearHttpsRequest_set_headder(request, "name", "mateus");
    BearHttpsRequest_set_body_file_stream(request,"teste.png");



}
