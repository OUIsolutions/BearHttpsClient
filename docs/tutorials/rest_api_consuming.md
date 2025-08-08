tutorial of consuming a REST API using BearHttpsClient
```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/posts");
    BearHttpsRequest_add_header(request, "Accept", "application/json");

    BearHttpsResponse *response = BearHttpsRequest_fetch(request);

    if (BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        int status = BearHttpsResponse_get_status_code(response);
        printf("HTTP Status: %d\n", status);

        if (status == 200) {
            const cJSON *json = BearHttpsResponse_read_body_json(response);
            const cJSON *first_post = cJSON_GetArrayItem(json, 0);
            const cJSON *title = cJSON_GetObjectItem(first_post, "title");
            if (title && cJSON_IsString(title)) {
                printf("First post title: %s\n", title->valuestring);
            }
        }
    }

    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}

```