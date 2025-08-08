This tutorial explains how to build the BearHttpsClient library from scratch, creating all the release files. This process will compile the source code into ready-to-use library files that you can include in your own projects.

## Prerequisites

Before starting, ensure you have the following installed on your system:
- Git (for downloading the source code)
- A terminal or command prompt
- Internet connection (for downloading dependencies)
- Linux, macOS, or Windows with WSL (this tutorial focuses on Linux commands)

## Step 1: Download the source code

First, you need to obtain the BearHttpsClient source code from the GitHub repository.

If you have Git installed, open a terminal and run the following command:
```bash
git clone https://github.com/OUIsolutions/BearHttpsClient.git
```

If you don't have Git installed, you can:
- Install Git using your system's package manager (e.g., `sudo apt install git` on Ubuntu)
- Or download the source code as a ZIP file from the [GitHub repository](https://github.com/OUIsolutions/BearHttpsClient) and extract it

## Step 2: Navigate to the project directory

After downloading, you need to enter the project folder. In your terminal, run:
```bash
cd BearHttpsClient
```

This command changes your current directory to the BearHttpsClient folder where all the source files are located.

## Step 3: Install the Darwin build tool

The BearHttpsClient library uses Darwin as its build system. Darwin is a tool that helps compile and package the library files.

You need to install Darwin version 0.8.0 specifically. On Linux, you can install it with this command:
```bash
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.8.0/darwin.out -o darwin.out && sudo chmod +x darwin.out && sudo mv darwin.out /usr/bin/darwin
```

This command does three things:
1. Downloads the Darwin executable
2. Makes it executable (`chmod +x`)
3. Moves it to `/usr/bin/` so you can run it from anywhere (`sudo mv`)

Note: You may be prompted for your password when using `sudo`.

For other operating systems, visit the [Darwin releases page](https://github.com/OUIsolutions/Darwin/releases/tag/0.8.0) and download the appropriate version for your system.

## Step 4: Build the library

Now you can compile the library using Darwin. Make sure you're still in the BearHttpsClient directory and run:
```bash
darwin run_blueprint --mode folder build/
```

This command tells Darwin to:
- Run the build blueprint (the build instructions)
- Use folder mode to process the build directory
- Look for build instructions in the `build/` folder

The build process may take a few moments as it compiles all the source files and dependencies.

## Step 5: Verify the build output

After the build completes successfully, Darwin will create a `release` folder containing the compiled library files:
```
release/
├── BearHttpsClient.c
├── BearHttpsClient.h
├── BearHttpsClientOne.c
└── BearHttpsClient.zip
```

## Understanding the output files

The build process creates several important files:

- **BearHttpsClient.h**: The header file containing function declarations and type definitions. Include this file in your C projects.
- **BearHttpsClient.c**: The main implementation file containing all the library code. Compile this with your project.
- **BearHttpsClientOne.c**: A single-file version that combines both header and implementation. Use this if you prefer a single-file approach.
- **BearHttpsClient.zip**: A compressed archive containing all the release files for easy distribution.

## Next steps

After successfully building the library, you can:
1. Use the generated files in your own C projects
2. Refer to the examples in the `examples/` folder to learn how to use the library
3. Read the documentation in the `docs/` folder for detailed usage instructions

## Troubleshooting

If you encounter issues during the build process:

- **Darwin not found**: Make sure Darwin is properly installed and in your system PATH
- **Permission denied**: Ensure you have the necessary permissions to write files in the current directory
- **Build fails**: Check that all dependencies are properly installed and your system meets the requirements
- **Network issues**: Ensure you have an active internet connection for downloading dependencies

For additional help, visit the [project's GitHub repository](https://github.com/OUIsolutions/BearHttpsClient) and check the issues section or create a new issue if needed.
