#include "src/one.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("teste");
    BearHttpsRequest_set_method(request,"POST");
    BearHttpsRequest_add_headder(request, "name", "mateus");
    BearHttpsRequest_set_body_file_stream(request,"teste.png");

    BearHttpsResponse *response =BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("error %s",BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    printf("status: %d",BearHttpsResponse_get_status_code(response));
    if(BearHttpsResponse_is_body_str(response)){
        printf("data: %s\n",BearHttpsResponse_get_body_str(response));
    }
    else{
        printf("body its not a string\n");
    }
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
}
