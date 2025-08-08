
These tutorial explains how to use the one file version of BearHttpsClient library.

## Step 1: Download the Library
Download the one file version of the library: [BearHttpsClientOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c) and save it as **BearHttpsClientOne.c** in your project folder.
if you have curl installed you can do it with this command:
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c -o BearHttpsClientOne.c
```

## Step 2: Creating your hello world program
Create a file called `main.c` and copy this code: 

```c
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
```
after these your project folder should look like this:

```
├── BearHttpsClientOne.c
└── main.c
```

## Step 3: Compile and Run

**On Linux/Mac:**
```bash
gcc main.c -o main.out
```
## On Windows:
```cmd
