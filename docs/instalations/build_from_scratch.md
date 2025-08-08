these tutorial explains how to build BearHttpsClient library from scratch,creating all the releases.

## Step 1: Download the source code
Download the source code from the [GitHub repository](https://github.com/OUIsolutions/BearHttpsClient)
if you have git installed you can do it with this command:
```bash
git clone https://github.com/OUIsolutions/BearHttpsClient.git
```
## Step 2: Go to the root folder of the project
Go to the root folder of the project:
```bash
cd BearHttpsClient
```

## Step 3: Install Darwin 
go to the [Darwin](https://github.com/OUIsolutions/Darwin) project, and install the 
version [0.8.0](https://github.com/OUIsolutions/Darwin/releases/tag/0.8.0) of the library.
if you are on linux you can do it with this command:
```bash
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.8.0/darwin.out -o darwin.out && sudo chmod +x darwin.out && sudo mv darwin.out /usr/bin/darwin
```

## Step 4: Build the library
Now you can build the library with the following command:
```bash
darwin run_blueprint --mode folder build/ 
```

it will create a folder called `release`with the following files:
```
release/
├── BearHttpsClient.c
├── BearHttpsClient.h
├── BearHttpsClientOne.c
└── BearHttpsClient.zip
```
