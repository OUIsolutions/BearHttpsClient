
This tutorial explains how to compile and use the single-file version of the BearHttpsClient library.

## Prerequisites

- A C compiler (GCC, Clang, MSVC, or MinGW)
- Basic knowledge of C programming and command-line operations

## Step 1: Download the Library

Download the single-file version of the library from the official repository and save it as `BearHttpsClientOne.c` in your project directory.

Using curl:
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.9.0/BearHttpsClientOne.c -o BearHttpsClientOne.c
```

## Step 2: Create Application Source File

Create a file named `main.c` with the following content:

```c
#include "BearHttpsClientOne.c"

int main(){
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");   
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }

    const char *body = BearHttpsResponse_read_body_str(response);
    if(BearHttpsResponse_error(response)){
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response); 
        return 1;
    }

    printf("Response body: %s\n", body);
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

Your project directory structure should now contain:
```
BearHttpsClientOne.c
main.c
```

## Step 3: Compilation Instructions

### GCC Compiler
```bash
gcc main.c -o main
./main
```

### Clang Compiler
```bash
clang main.c -o main
./main
```

### Microsoft Visual C++ Compiler
```bash
cl main.c /Fe:main.exe
main.exe
```

### MinGW Cross-Compiler
```bash
i686-w64-mingw32-gcc main.c -o main.exe -lws2_32
./main.exe
```

### Emscripten (WebAssembly)
```bash
emcc main.c -o main.js -sASYNCIFY
```

For WebAssembly compilation details, refer to the WebAssembly tutorial documentation.
