#include "BearHttpsClientOne.c"

BearHttpsNamespace bear ;
int main(){
    bear = newBearHttpsNamespace();

    BearHttpsRequest *request = bear.request.newBearHttpsRequest("https://example.com");   
    BearHttpsResponse *response = bear.request.fetch(request);
    if(bear.response.error(response)){
        printf("Error: %s\n",bear.response.get_error_msg(response));
        bear.request.free(request);
        bear.response.free(response);
        return 1;
    }

    const char *body = bear.response.read_body_str(response);
    if(bear.response.error(response)){
            printf("Error: %s\n",bear.response.get_error_msg(response));
            bear.request.free(request);
            bear.response.free(response); 
            return 1;

    }

    printf("Body: %s\n",body);
    bear.request.free(request);
    bear.response.free(response);

    return 0;
}
