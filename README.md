<div align="center">

# BearHttpsClient
![C Logo](https://img.shields.io/badge/BearHttpsClient-0.5.0-blue?style=for-the-badge&logo=c)
[![GitHub Release](https://img.shields.io/github/release/OUIsolutions/BearHttpsClient.svg?style=for-the-badge)](https://github.com/OUIsolutions/BearHttpsClient/releases)
[![License](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)](https://github.com/OUIsolutions/BearHttpsClient/blob/main/LICENSE)
![Status](https://img.shields.io/badge/Status-Alpha-orange?style=for-the-badge)

</div>

---

## ⚠️ Important Notice

> **This is alpha software!** Use at your own risk. While we're working hard to make it stable, bugs are expected. Perfect for learning and prototyping! 🧪

---

## What is BearHttpsClient?

BearHttpsClient is a lightweight HTTP/HTTPS client library for C/C++ applications. It provides a streamlined interface for network communication, eliminating the complexity of traditional networking code:

1. **Specify the target URL** 
2. **Retrieve the response**
3. **Process the data**

This library is designed for developers who need to:
- Download resources from remote servers
- Communicate with web APIs
- Retrieve data from online services
- Implement HTTP/HTTPS functionality in applications

### Key Features

- **HTTP & HTTPS support** - Secure communication with any web service
- **Simplified integration** - Single-file inclusion option
- **File transfer capabilities** - Efficient upload and download functionality
- **API compatibility** - Built-in support for JSON and modern web services
- **Cross-platform functionality** - Compatible with Windows, Linux, and WebAssembly
- **Minimal dependencies** - No additional library installations required

---

## 🚀 Quick Start (30 seconds!)

### Step 1: Download the Library
Choose your preferred way:

| 📁 **Option** | 📄 **File** | 🎯 **Best For** |
|---------------|-------------|------------------|
| **🎯 Super Easy** | [BearHttpsClientOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c) | Beginners - Just one file! |
| **📦 Full Package** | [BearHttpsClient.zip](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.zip) | Advanced users |
| **🔧 Separate Files** | [.h](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.h) + [.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClient.c) | Traditional C projects |

### Step 2: Your First Program

Create a file called `my_first_request.c` and copy this code:

```c
#include "BearHttpsClientOne.c"

int main() {
    // 🎯 Step 1: Create a request to any website
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    
    // 🚀 Step 2: Send the request and get response
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    // 🛡️ Step 3: Check if something went wrong
    if(BearHttpsResponse_error(response)) {
        printf("❌ Oops! Something went wrong: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    // 📖 Step 4: Read the website content
    const char *body = BearHttpsResponse_read_body_str(response);
    if(BearHttpsResponse_error(response)) {
        printf("❌ Couldn't read the response: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    // 🎉 Step 5: Show what we got!
    printf("✅ Success! Here's what we got:\n%s\n", body);
    
    // 🧹 Step 6: Clean up (always important!)
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Step 3: Compile and Run

**On Linux/Mac:**
```bash
gcc my_first_request.c -o my_first_request
./my_first_request
```

**On Windows:**
```cmd
gcc my_first_request.c -o my_first_request.exe
my_first_request.exe
```

**🎉 Congratulations!** You just made your first HTTPS request in C! 

---

## 🎓 Learning Examples

### 🌤️ Example 1: Get Weather Data
```c
#include "BearHttpsClientOne.c"

int main() {
    // Get weather for London (replace with your API key)
    BearHttpsRequest *request = newBearHttpsRequest(
        "https://api.openweathermap.org/data/2.5/weather?q=London&appid=YOUR_API_KEY"
    );
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(!BearHttpsResponse_error(response)) {
        printf("🌤️ Weather data: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📨 Example 2: Send JSON Data
```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // 📝 Set the content type for JSON
    BearHttpsRequest_set_header(request, "Content-Type", "application/json");
    
    // 📤 Send some JSON data
    const char *json_data = "{\"name\":\"Alice\",\"age\":25}";
    BearHttpsRequest_set_method(request, "POST");
    BearHttpsRequest_set_body_str(request, json_data);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(!BearHttpsResponse_error(response)) {
        printf("📨 Server response: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 📚 Complete Documentation

| 📖 **Topic** | 🎯 **What You'll Learn** | 🔧 **Difficulty** |
|--------------|---------------------------|-------------------|
| [🏗️ Build & Install](/docs/build_and_install.md) | How to set up the library | 🟢 Beginner |
| [📨 Making Requests](/docs/request.md) | Headers, POST data, uploads | 🟡 Intermediate |
| [📥 Handling Responses](/docs/response.md) | Reading data, status codes | 🟡 Intermediate |
| [🌐 Network Config](/docs/network_configuration.md) | Timeouts, proxies, SSL | 🔴 Advanced |
| [🕸️ WebAssembly](/docs/web_asm.md) | Running in browsers | 🔴 Advanced |
| [💾 Memory Management](/docs/memory_and_limits.md) | Performance tips | 🔴 Advanced |
| [🔧 Ownership System](/docs/ownership_system.md) | Memory safety | 🔴 Advanced |
| [📦 Dependencies](/docs/dependencies.md) | What's under the hood | 🟡 Intermediate |

---

## 🔧 Build Toolchain

This project is built using these awesome tools:

- 🛠️ [Darwin](https://github.com/OUIsolutions/Darwin) - Build system
- ⛓️ [Silver Chain](https://github.com/OUIsolutions/SilverChain) - Code generation
- 🔗 [CAmalgamator](https://github.com/OUIsolutions/CAmalgamator) - Single-file creation
- 🐻 [BearSSL](https://bearssl.org/) - Secure SSL/TLS

---

## 🤝 Contributing

We ❤️ contributions! Whether you're:

- 🐛 **Found a bug?** Open an issue!
- 💡 **Have an idea?** We'd love to hear it!
- 📝 **Want to improve docs?** Documentation PRs are super welcome!
- 🧪 **Want to add tests?** Help us make it more reliable!

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

<div align="center">

**Made with ❤️ for the C/C++ community**

⭐ **Found this helpful? Give us a star!** ⭐

[🏠 Home](https://github.com/OUIsolutions/BearHttpsClient) | [📖 Docs](/docs/) | [🐛 Issues](https://github.com/OUIsolutions/BearHttpsClient/issues) | [💬 Discussions](https://github.com/OUIsolutions/BearHttpsClient/discussions)

</div>


