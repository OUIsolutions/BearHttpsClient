

<div align="center">

# 🏗️ Build & Install Guide

**Getting BearHttpsClient up and running in your project** 🚀

*The simplest setup process you'll ever see!*

</div>

---

## 🎯 Choose Your Installation Method

Don't worry - we've made this **super easy**! Pick the method that feels right for you:

| 🎲 **Method** | 🎯 **Best For** | ⏱️ **Time Needed** | 🌟 **Difficulty** |
|---------------|-----------------|---------------------|-------------------|
| **🚀 Quick Start** | Beginners | 30 seconds | 🟢 Super Easy |
| **📦 Download** | Most users | 1 minute | 🟢 Easy |
| **🔧 Build from Source** | Advanced users | 5 minutes | 🔴 Advanced |

---

## 🚀 Method 1: Quick Start (Recommended!)

This is the **easiest way** to get started! Perfect for beginners.

### Step 1: Download the Magic File ✨

**Option A: Auto-Download (Linux/Mac)**
```bash
# 📥 Download the single-file version
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c -o BearHttpsClientOne.c

# ✅ That's it! You now have everything you need!
```

**Option B: Manual Download (Any OS)**
1. 🌐 Go to: [BearHttpsClientOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c)
2. 💾 Right-click → "Save As..." → Save to your project folder
3. ✅ Done!

### Step 2: Include in Your Code 📝

Just add this **one line** at the top of your C file:

```c
#include "BearHttpsClientOne.c"

int main() {
    // 🎉 You can now use BearHttpsClient!
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    // ... rest of your code
    return 0;
}
```

### Step 3: Compile and Run! 🎯

**Linux/Mac:**
```bash
gcc your_program.c -o your_program
./your_program
```

**Windows:**
```cmd
gcc your_program.c -o your_program.exe
your_program.exe
```

**🎉 Congratulations!** You're ready to make HTTPS requests!

---

## 📦 Method 2: Traditional C Library Setup

For those who prefer separate header and source files:

### Step 1: Download Both Files 📄

Download these two files to your project:
- 📄 [BearHttpsClient.h](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.h) - The header file
- 📄 [BearHttpsClient.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.c) - The implementation

### Step 2: Include the Header 📝

```c
#include "BearHttpsClient.h"

int main() {
    // 🎉 Ready to use!
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    // ... your code here
    return 0;
}
```

### Step 3: Compile Both Files 🔧

```bash
gcc your_program.c BearHttpsClient.c -o your_program
```

---

## 🖥️ Platform-Specific Compilation

### 🐧 Linux Compilation

**Simple compile:**
```bash
gcc your_program.c -o your_program.out
```

**With optimizations (faster code):**
```bash
gcc -O2 your_program.c -o your_program.out
```

**Debug mode (for development):**
```bash
gcc -g -DDEBUG your_program.c -o your_program.out
```

### 🪟 Windows Compilation

**Using MinGW (Recommended):**
```bash
gcc your_program.c -o your_program.exe -lws2_32
```

**Using Microsoft Visual C++:**
```cmd
cl.exe your_program.c /Fe:your_program.exe
```

**Using MinGW-w64:**
```bash
i686-w64-mingw32-gcc your_program.c -o your_program.exe -lws2_32
```

> 💡 **Tip:** The `-lws2_32` flag is needed on Windows for socket functionality!

---

## 🔧 Method 3: Build from Source (Advanced)

Want to build everything from scratch? Here's how the experts do it!

### Prerequisites 📋

You'll need these tools installed:
- 🛠️ [Darwin Build System](https://github.com/OUIsolutions/Darwin) 
- 🔧 GCC or Clang compiler
- 📦 Git (for cloning)

### Step 1: Install Darwin 🛠️

**Linux/Mac:**
```bash
# 📥 Download Darwin build tool
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.020/darwin.out -o darwin.out

# 🔐 Make it executable
chmod +x darwin.out

# 📦 Install system-wide
sudo mv darwin.out /usr/bin/darwin

# ✅ Verify installation
darwin --version
```

### Step 2: Clone and Build 🏗️

```bash
# 📥 Clone the repository
git clone https://github.com/OUIsolutions/BearHttpsClient.git
cd BearHttpsClient

# 🔨 Build everything!
darwin run_blueprint build/ --mode folder

# 🎉 Find your built files in the 'release' folder!
ls release/
```

**What you'll find in `release/`:**
- 📄 `BearHttpsClientOne.c` - Single file version
- 📄 `BearHttpsClient.h` + `BearHttpsClient.c` - Traditional files
- 📦 `BearHttpsClient.zip` - Complete package

---

## 🎛️ Advanced Configuration Options

Sometimes you need more control! Here are some powerful options:

### 🔧 Dependency Mocking

Want to customize which parts of the library are included? Use these defines **before** including the library:

```c
// 🚫 Disable Universal Socket (for custom networking)
#define BEARSSL_HTTPS_MOCK_UNIVERSAL_SOCKET_DEFINE

// 🚫 Disable BearSSL (use system SSL instead)
#define BEARSSL_HTTPS_MOCK_BEARSSL_DEFINE

// 🚫 Disable JSON parsing (smaller binary size)
#define BEARSSL_HTTPS_MOCK_CJSON

// ⚠️ WARNING: Disabling JSON will use getaddrinfo() 
// which can cause memory leaks on some systems!

// ✅ Now include the library
#include "BearHttpsClientOne.c"
```

### 🌐 Enable System DNS Resolution

By default, BearHttpsClient uses its own DNS resolver. To use system DNS instead:

```c
// 🌐 Use system's getaddrinfo() for DNS resolution
#define BEARSSL_USE_GET_ADDRINFO

#include "BearHttpsClientOne.c"
```

> ⚠️ **Warning:** System DNS functions can leak memory on some platforms. Use with caution!

---

## 🚨 Troubleshooting Common Issues

### ❌ Problem: "Cannot find BearHttpsClientOne.c"

**Solution:**
- ✅ Make sure the file is in the same folder as your program
- ✅ Check the filename is exactly `BearHttpsClientOne.c`
- ✅ Try downloading the file again

### ❌ Problem: Compilation errors on Windows

**Solution:**
```bash
# Add this flag when compiling on Windows:
gcc your_program.c -o your_program.exe -lws2_32
```

### ❌ Problem: SSL/TLS errors

**Solution:**
- ✅ Make sure you're using `https://` not `http://`
- ✅ Check your internet connection
- ✅ The website's SSL certificate might be invalid

### ❌ Problem: "Darwin command not found"

**Solution:**
```bash
# Re-install Darwin:
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.020/darwin.out -o darwin.out
chmod +x darwin.out
sudo mv darwin.out /usr/bin/darwin
```

---

## 🎓 What's Next?

Now that you have BearHttpsClient installed, you're ready to:

1. 📖 [Learn how to make requests](/docs/request.md)
2. 📥 [Handle responses like a pro](/docs/response.md)
3. 🌐 [Configure network settings](/docs/network_configuration.md)
4. 🎯 [Try the examples](/examples/)

---

<div align="center">

**🎉 Installation Complete! You're ready to make HTTPS requests!** 

[📖 Next: Making Your First Request](/docs/request.md) | [🏠 Back to Main](/README.md)

</div>
