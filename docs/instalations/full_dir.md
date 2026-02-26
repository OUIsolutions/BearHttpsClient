WThis tutorial explains how to compile and use the BearHttpsClient library from the full directory distribution.

## Step 1: Download and Extract the Library
Download the BearHttpsClient library distribution from the releases page and extract it to your project directory.

```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.9.0/BearHttpsClient.zip -o BearHttpsClient.zip
unzip BearHttpsClient.zip
```

After extraction, your project directory will contain:

```
BearHttpsClient/
├── build/
├── dependencies/
└── src/
```
## Step 2: Create Your Application
Create your application file (main.c) in the project directory:

```c
#include "src/one.c"

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

Your project structure should now be:

```
BearHttpsClient/
├── build/
├── dependencies/
├── src/
└── main.c
```


## Step 3: Compilation

The library can be compiled using various C compilers. The following compilation commands will build your application with the BearHttpsClient library.

### GCC Compilation
```bash
gcc main.c -o main.out 
./main.out
```

### Clang Compilation
```bash
clang main.c -o main.out
./main.out
```

### Microsoft Visual Studio Compiler
```bash
cl main.c /Fe:main.exe
main.exe        
```

### MinGW Cross-Compilation
```bash
i686-w64-mingw32-gcc main.c -o main.exe -lws2_32
./main.exe
```

### Emscripten (WebAssembly)
For WebAssembly compilation, refer to the WebAssembly tutorial documentation for detailed instructions.

```bash
emcc main.c -o main.js -sASYNCIFY
```

