# BearHttpsClient
a Unoficial BearSSL Https Client
## Build from scratch
for buiding from scratch you need to have darwin installed on your machine, for install darwin: 
```bash
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.020/darwin.out -o darwin.out && chmod +x darwin.out &&  sudo  mv darwin.out /usr/bin/darwin
```
then you can build the project with:
```bash
darwin run_blueprint build/  --mode folder
``` 
it will generate all the **releases** in the **/release** dir 
