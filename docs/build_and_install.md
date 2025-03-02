

## Install 
for install the lib , just copy the [Amalgamation](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.002/BearHttpsClientOne.c) into your project and include it in your code:
```c
#include "BearHttpsClientOne.c"
```


if  you are on linux, you can download the lib with: 
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.002/BearHttpsClientOne.c -o BearHttpsClientOne.c
```

## Compile on Linux
for compile the lib in **Linux** you can use the following command:
```bash
gcc main.c -o your_output.out
```
## Compile on Windows
for compile the lib in **Windows** you can use the following command (recomended mingw64 installed):
```bash
i686-w64-mingw32-gcc teste.c  main.c -o your_output.exe -lws2_32
```

## Build from scratch
for buiding from scratch you need to have darwin installed on your machine, for install darwin:
```bash
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.020/darwin.out -o darwin.out && chmod +x darwin.out &&  sudo  mv darwin.out /usr/bin/darwin
```
then you can build the project with in the root dir of the project:
```bash
darwin run_blueprint build/  --mode folder
```
it will generate all the **releases** in the **/release** dir



### Mocking dependencies or dependencies implementations 

the following flags hide,dependencies entire, or just the definitions
(its usefull) for  working with dynamic compilation, or mocking dependencies
```c
// unallow universal socket definition
#define BEARSSL_HTTPS_MOCK_UNIVERSAL_SOCKET_DEFINE
// unallow universal socket implementation
#define BEARSSL_HTTPS_MOCK_BEARSSL_DEFINE
// unallow cjson at all
// note ,that hiding cjson, will obrigate the lib to use **get_addrinfo**, witch,
//can geerate leak memorys in some cases
#define BEARSSL_HTTPS_MOCK_CJSON

// unallow cjson definition
#define BEARSSL_HTTPS_MOCK_CJSON_DEFINE

//import the lib , only after the flags
#include "BearHttpsClientOne.c"
``` 
