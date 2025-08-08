
in these tutorial how to install the BearHttpsClient library precompiling the library and then your program.

## Step 1: Download the [BearHttpsClient.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.c) and [BearHttpsClient.h](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.h)
Download the precompiled library: [BearHttpsClient.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.c) as **BearHttpsClient.c** and [BearHttpsClient.h](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.h) as **BearHttpsClient.h** in your project folder.
if you have curl installed you can do it with this command:
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.c -o BearHttpsClient.c
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.h -o BearHttpsClient.h
```

## Step 2: Creating your hello world program
Create a file called `main.c` and copy this code:
```c
#include "BearHttpsClient.h"
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
├── BearHttpsClient.c
├── BearHttpsClient.h
└── main.c
```
## Step 3: Compile and run
**gcc:**
```bash     
gcc -c BearHttpsClient.c -o BearHttpsClient.o 
gcc main.c BearHttpsClient.o -o main.o
./main.o
```
**clang:**
```bash
clang -c BearHttpsClient.c -o BearHttpsClient.o
clang main.c BearHttpsClient.o -o main.o
./main.o
```
**Microsoft Visual Compiler (cl.exe):**
```bash
cl BearHttpsClient.c /Fe:BearHttpsClient.o
cl main.c BearHttpsClient.o /Fe:main.exe
main.exe
```
**mingw32**
```bash
i686-w64-mingw32-gcc -c BearHttpsClient.c -o BearHttpsClient.o
i686-w64-mingw32-gcc  main.c BearHttpsClient.o -o main.exe -lws2_32
./main.exe
```

