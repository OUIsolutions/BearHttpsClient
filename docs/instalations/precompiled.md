
This tutorial demonstrates how to install and compile the BearHttpsClient library using precompiled files.

## Step 1: Download Library Files
Download the precompiled library files to your project directory:
- [BearHttpsClient.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.c)
- [BearHttpsClient.h](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.h)

Using curl:
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.c -o BearHttpsClient.c
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.h -o BearHttpsClient.h
```

## Step 2: Create Application File
Create a file named `main.c` with the following content:
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

Project directory structure:
```
BearHttpsClient.c
BearHttpsClient.h
main.c
```

## Step 3: Compilation Instructions

**GCC Compiler:**
```bash     
gcc -c BearHttpsClient.c -o BearHttpsClient.o 
gcc main.c BearHttpsClient.o -o main
./main
```

**Clang Compiler:**
```bash
clang -c BearHttpsClient.c -o BearHttpsClient.o
clang main.c BearHttpsClient.o -o main
./main
```

**Microsoft Visual C++ Compiler:**
```bash
cl BearHttpsClient.c /c
cl main.c BearHttpsClient.obj /Fe:main.exe
main.exe
```

**MinGW Cross-Compiler:**
```bash
i686-w64-mingw32-gcc -c BearHttpsClient.c -o BearHttpsClient.o
i686-w64-mingw32-gcc main.c BearHttpsClient.o -o main.exe -lws2_32
./main.exe
```

