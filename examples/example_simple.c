#include "BearHttpsClientOne.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");   
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n",BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }

    const char *body = BearHttpsResponse_read_body_str(response);
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n",BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response); 
        return 1;
    }

    printf("Body: %s\n",body);
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);

    return 0;
}