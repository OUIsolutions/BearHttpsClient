These tutorial explains how to use the zip file version of BearHttpsClient library.

## Step 1: Download the Library
Download the zip file version of the library: [BearHttpsClient.zip](https://github.
com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.zip) and extract it in your project folder.

```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.zip -o BearHttpsClient.zip
```
You can use any unzip tool to extract the zip file, or you can use the command line:

```bash
unzip BearHttpsClient.zip
```
After extracting, your project folder should look like this:

```
├── BearHttpsClient.zip
├── build
├── dependencies
└── src
```
## Step 2: Creating your hello world program
Create a file called `main.c` and copy this code:

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
After this, your project folder should look like this:

```├── BearHttpsClient.zip
├── build
├── dependencies
├── src
└── main.c
```


## Step 3: Compile and run 

Now you can chose by compile enverything at once or compile the library and then your program.
### Compile everything at once
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
i686-w64-mingw32-gcc main.c -o main.exe -lws2_32
./main.exe
```
**emscripten:** (check [webassembly](docs/tutorials/webassembly.md) for more details)
```bash
emcc main.c -o main.js -sASYNCIFY
```

