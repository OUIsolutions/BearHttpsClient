
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

## Step 3: Compile and rum 

**gcc:**
```bash     
gcc main.c -o main.out
./main.out
```
**clang:**
```bash
clang main.c -o main.out
./main.out
```
**Microsoft Visual Compiler (cl.exe):**
```bash
cl main.c /Fe:main.exe
main.exe
```
**mingw32**
```bash
i686-w64-mingw32-gcc your_program.c -o main.exe -lws2_32
./main.exe
```

**emscripten:** (check [webassembly](docs/tutorials/webassembly.md) for more details)
```bash
emcc main.c -o main.js -sASYNCIFY
```
