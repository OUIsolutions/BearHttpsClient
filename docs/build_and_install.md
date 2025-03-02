

## Install 
for install the lib , just copy the [Amalgamation](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.000/BearHttpsClientOne.c) into your project and include it in your code:
```c
#include "BearHttpsClientOne.c"
```


if  you are on linux, you can download the lib with: 
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.1.000/BearHttpsClientOne.c -o BearHttpsClientOne.c
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


## Removing CJSON
if you want to remove the CJSON dependency, you can remove the **CJSON** defined in the **BearHttpsClientOne.c** file:
```c
#define BEARSSL_HTTPS_MOCK_CJSON
#include "BearHttpsClientOne.c"
```
Note ,that these will obrigate the lib, to use the **getaddrinfo** functions, that can generate leak memorys on some platforms, so use it with caution