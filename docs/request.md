<div align="center">

# 📨 Making HTTP Requests

**Your complete guide to sending data to the web** 🌐

*From simple GET requests to file uploads - we've got you covered!*

</div>

---

## 🎯 What You'll Learn

By the end of this guide, you'll know how to:

| 🎯 **Skill** | 🌟 **Difficulty** | ⏱️ **Time** |
|-------------|-------------------|-------------|
| 📝 Add headers | 🟢 Beginner | 2 minutes |
| 🔄 Change HTTP methods | 🟢 Beginner | 2 minutes |
| 🌐 Set custom URLs | 🟢 Beginner | 1 minute |
| 📤 Send text data | 🟡 Intermediate | 3 minutes |
| 📁 Upload files | 🟡 Intermediate | 5 minutes |
| 🗃️ Send raw binary data | 🔴 Advanced | 5 minutes |

---

## 🚀 The Basics: Your First Request

Before we dive into the details, here's the **simplest possible request**:

```c
#include "BearHttpsClientOne.c"

int main() {
    // 🌐 Create a request to any website
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/get");
    
    // 🚀 Send it and get the response
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    // 📖 Read what we got back
    if (!BearHttpsResponse_error(response)) {
        printf("Response: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    // 🧹 Always clean up!
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

Now let's learn how to customize your requests! 🎛️

---

## 📝 Adding Headers: Tell the Server Who You Are

Headers are like **business cards for your request** - they tell the server important information about what you're sending.

### 🔰 Basic Header Example

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/headers");
    
    // 📝 Add some headers
    BearHttpsRequest_add_header(request, "User-Agent", "MyAwesomeApp/1.0");
    BearHttpsRequest_add_header(request, "Accept", "application/json");
    BearHttpsRequest_add_header(request, "Content-Type", "application/json");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Server received these headers: %s\n", 
               BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📋 Common Headers You Might Need

| 🏷️ **Header Name** | 📖 **What It Does** | 💡 **Example Value** |
|-------------------|---------------------|---------------------|
| `Content-Type` | Tells what kind of data you're sending | `application/json` |
| `User-Agent` | Identifies your application | `MyApp/1.0` |
| `Authorization` | Provides authentication | `Bearer your-token-here` |
| `Accept` | Tells what response formats you want | `application/json` |
| `Accept-Language` | Your preferred language | `en-US,en;q=0.9` |

### 🔐 Authentication Example

```c
// 🔑 API key authentication
BearHttpsRequest_add_header(request, "Authorization", "Bearer your-secret-api-key");

// 🔑 Basic authentication (username:password in base64)
BearHttpsRequest_add_header(request, "Authorization", "Basic dXNlcjpwYXNzd29yZA==");
```

---

## 🔄 HTTP Methods: Different Ways to Talk to Servers

Think of HTTP methods like **different ways to ask for things**:

| 🎯 **Method** | 📖 **What It Does** | 🎭 **Real World Analogy** |
|--------------|---------------------|---------------------------|
| `GET` | Get data (default) | 📖 "Can I read this book?" |
| `POST` | Send new data | 📮 "Here's a new letter for you" |
| `PUT` | Replace data | 🔄 "Replace this with that" |
| `DELETE` | Remove data | 🗑️ "Please delete this" |
| `PATCH` | Update part of data | ✏️ "Just fix this typo" |

### 🔰 Changing the Method

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // 🔄 Change from GET (default) to POST
    BearHttpsRequest_set_method(request, "POST");
    
    // 📝 Add some data to send
    BearHttpsRequest_send_body_str(request, "Hello, server!");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Server response: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 🌐 Setting Custom URLs: Go Anywhere!

Sometimes you need to **change where your request goes** after creating it:

```c
#include "BearHttpsClientOne.c"

int main() {
    // 🌐 Start with one URL
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    
    // 🔄 Change your mind? No problem!
    BearHttpsRequest_set_url(request, "https://httpbin.org/get");
    
    // 🔄 Change again if needed
    BearHttpsRequest_set_url(request, "https://jsonplaceholder.typicode.com/posts/1");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Final destination response: %s\n", 
               BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 📤 Sending Text Data: Your Message to the World

### 🔰 Simple Text Sending

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // 🔄 Set method to POST (required for sending data)
    BearHttpsRequest_set_method(request, "POST");
    
    // 📝 Set content type so server knows what we're sending
    BearHttpsRequest_add_header(request, "Content-Type", "text/plain");
    
    // 📤 Send your message!
    BearHttpsRequest_send_body_str(request, "Hello from BearHttpsClient! 🐻");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Server got our message: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📊 Sending JSON Data (API Favorite!)

Most modern web APIs **love JSON**! Here's how to send it:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // 🔄 Set to POST method
    BearHttpsRequest_set_method(request, "POST");
    
    // 📝 Tell server we're sending JSON
    BearHttpsRequest_add_header(request, "Content-Type", "application/json");
    
    // 📊 Create some JSON data
    const char *json_data = "{"
        "\"name\": \"Alice\","
        "\"age\": 25,"
        "\"city\": \"New York\","
        "\"hobbies\": [\"reading\", \"coding\", \"hiking\"]"
    "}";
    
    // 📤 Send the JSON!
    BearHttpsRequest_send_body_str(request, json_data);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("API response: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📝 Sending Form Data

Sometimes servers expect **form data** (like HTML forms):

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    BearHttpsRequest_set_method(request, "POST");
    
    // 📝 Set content type for form data
    BearHttpsRequest_add_header(request, "Content-Type", "application/x-www-form-urlencoded");
    
    // 📤 Send form data (key=value&key2=value2 format)
    BearHttpsRequest_send_body_str(request, "username=alice&password=secret123&email=alice@example.com");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Form submission response: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 📁 File Uploads: Share Your Files with the World

### 🔰 Auto-Detect File Type (Recommended!)

Let BearHttpsClient **automatically figure out** what type of file you're sending:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    BearHttpsRequest_set_method(request, "POST");
    
    // 📁 Upload a file - BearHttpsClient will auto-detect the type!
    // This works with: .jpg, .png, .gif, .pdf, .txt, .json, etc.
    BearHttpsRequest_send_file_auto_detect_content_type(request, "/path/to/your/photo.jpg");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("File uploaded successfully: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🎯 Specify File Type Manually

Sometimes you need **precise control** over the file type:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    BearHttpsRequest_set_method(request, "POST");
    
    // 📁 Upload with specific content type
    BearHttpsRequest_send_file(request, "/home/user/document.pdf", "application/pdf");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("PDF uploaded: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📋 Common File Types Reference

| 📁 **File Type** | 📝 **Content-Type** | 💡 **Example** |
|------------------|---------------------|----------------|
| 📷 Images (JPEG) | `image/jpeg` | `photo.jpg` |
| 🖼️ Images (PNG) | `image/png` | `screenshot.png` |
| 📄 PDF Documents | `application/pdf` | `report.pdf` |
| 📝 Text Files | `text/plain` | `notes.txt` |
| 📊 JSON Files | `application/json` | `data.json` |
| 📹 Videos (MP4) | `video/mp4` | `video.mp4` |
| 🎵 Audio (MP3) | `audio/mpeg` | `song.mp3` |

---

## 🗃️ Sending Raw Binary Data (Advanced)

Sometimes you need to send **raw bytes** - maybe from memory or a custom format:

### 🔰 Basic Raw Data Sending

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    BearHttpsRequest_set_method(request, "POST");
    BearHttpsRequest_add_header(request, "Content-Type", "application/octet-stream");
    
    // 🗃️ Your raw data (could be anything!)
    unsigned char raw_data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21}; // "Hello!" in hex
    
    // 📤 Send raw data - it gets copied to the request
    BearHttpsRequest_send_any(request, raw_data, sizeof(raw_data));
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Raw data sent: %s\n", BearHttpsResponse_read_body_str(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🧠 Memory Management: Choose Your Style

BearHttpsClient gives you **three ways** to handle memory when sending data:

#### 📚 Option 1: REFERENCE (For Static Data)

Use this when your data **won't change** and **won't be freed**:

```c
// 📚 Static data that never changes
unsigned char static_data[] = "This data lives forever";

BearHttpsRequest_send_any_with_ownership_control(
    request, 
    static_data, 
    sizeof(static_data)-1, 
    BEARSSL_HTTPS_REFERENCE  // 👈 Just reference it, don't copy
);

// ✅ No need to worry about freeing - it's static!
```

#### 📋 Option 2: COPY (Safe and Simple)

Use this when you **want to keep your original data**:

```c
// 📋 Dynamic data that you want to keep
unsigned char *my_data = strdup("I want to keep this data");

BearHttpsRequest_send_any_with_ownership_control(
    request, 
    my_data, 
    strlen(my_data), 
    BEARSSL_HTTPS_COPY  // 👈 Make a copy for the request
);

// ✅ You can still use my_data after sending
printf("I still have: %s\n", my_data);
free(my_data);  // 👈 You free your copy
```

#### 🎯 Option 3: GET_OWNERSHIP (Transfer Control)

Use this when you want to **give your data to BearHttpsClient**:

```c
// 🎯 Dynamic data that you want to transfer
unsigned char *transferred_data = strdup("Take ownership of this");

BearHttpsRequest_send_any_with_ownership_control(
    request, 
    transferred_data, 
    strlen(transferred_data), 
    BEARSSL_HTTPS_GET_OWNERSHIP  // 👈 BearHttpsClient now owns this
);

// ❌ DON'T free transferred_data - BearHttpsClient will handle it!
// free(transferred_data);  // 👈 DON'T DO THIS!
```

---

## 🎓 Complete Real-World Examples

### 🌤️ Weather API Request

```c
#include "BearHttpsClientOne.c"

int main() {
    // 🌤️ Get weather data (replace YOUR_API_KEY with real key)
    BearHttpsRequest *request = newBearHttpsRequest(
        "https://api.openweathermap.org/data/2.5/weather?q=London&appid=YOUR_API_KEY"
    );
    
    // 📱 Identify our app
    BearHttpsRequest_add_header(request, "User-Agent", "WeatherApp/1.0");
    BearHttpsRequest_add_header(request, "Accept", "application/json");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("🌤️ Weather data: %s\n", BearHttpsResponse_read_body_str(response));
    } else {
        printf("❌ Weather request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📝 Blog Post Creation

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/posts");
    
    // 📝 Creating a new blog post
    BearHttpsRequest_set_method(request, "POST");
    BearHttpsRequest_add_header(request, "Content-Type", "application/json");
    BearHttpsRequest_add_header(request, "User-Agent", "BlogApp/2.1");
    
    // 📊 Blog post data
    const char *post_data = "{"
        "\"title\": \"My Awesome C Project\","
        "\"body\": \"I just learned how to make HTTPS requests in C!\","
        "\"userId\": 1"
    "}";
    
    BearHttpsRequest_send_body_str(request, post_data);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("📝 Blog post created: %s\n", BearHttpsResponse_read_body_str(response));
    } else {
        printf("❌ Failed to create post: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📁 File Upload to Cloud Storage

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // 📁 Upload a profile picture
    BearHttpsRequest_set_method(request, "POST");
    BearHttpsRequest_add_header(request, "User-Agent", "PhotoApp/1.5");
    BearHttpsRequest_add_header(request, "Authorization", "Bearer your-upload-token");
    
    // 📷 Upload the image (auto-detects it's a JPEG)
    BearHttpsRequest_send_file_auto_detect_content_type(request, "/home/user/profile.jpg");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("📷 Photo uploaded successfully: %s\n", BearHttpsResponse_read_body_str(response));
    } else {
        printf("❌ Upload failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 🚨 Common Mistakes and How to Avoid Them

### ❌ Mistake 1: Forgetting to Set Method for Data

```c
// ❌ Wrong - sending data with GET method
BearHttpsRequest *request = newBearHttpsRequest("https://api.example.com/data");
BearHttpsRequest_send_body_str(request, "some data");  // Won't work!

// ✅ Correct - set POST method first
BearHttpsRequest *request = newBearHttpsRequest("https://api.example.com/data");
BearHttpsRequest_set_method(request, "POST");  // 👈 Set method first
BearHttpsRequest_send_body_str(request, "some data");  // Now it works!
```

### ❌ Mistake 2: Wrong Content-Type for JSON

```c
// ❌ Wrong - server won't understand it's JSON
BearHttpsRequest_add_header(request, "Content-Type", "text/plain");
BearHttpsRequest_send_body_str(request, "{\"key\": \"value\"}");

// ✅ Correct - tell server it's JSON
BearHttpsRequest_add_header(request, "Content-Type", "application/json");
BearHttpsRequest_send_body_str(request, "{\"key\": \"value\"}");
```

### ❌ Mistake 3: File Path Issues

```c
// ❌ Wrong - relative paths can be tricky
BearHttpsRequest_send_file_auto_detect_content_type(request, "photo.jpg");

// ✅ Correct - use absolute paths
BearHttpsRequest_send_file_auto_detect_content_type(request, "/home/user/photos/photo.jpg");
```

### ❌ Mistake 4: Memory Management Confusion

```c
// ❌ Wrong - freeing data that BearHttpsClient now owns
char *data = strdup("important data");
BearHttpsRequest_send_any_with_ownership_control(request, data, strlen(data), BEARSSL_HTTPS_GET_OWNERSHIP);
free(data);  // 👈 DON'T DO THIS!

// ✅ Correct - let BearHttpsClient handle it
char *data = strdup("important data");
BearHttpsRequest_send_any_with_ownership_control(request, data, strlen(data), BEARSSL_HTTPS_GET_OWNERSHIP);
// BearHttpsClient will free it when the request is freed
```

---

## 🎯 Quick Reference Cheat Sheet

### 🔧 Essential Functions

| 🛠️ **Function** | 📖 **What It Does** |
|-----------------|---------------------|
| `BearHttpsRequest_add_header(req, key, value)` | Add a header |
| `BearHttpsRequest_set_method(req, method)` | Set HTTP method |
| `BearHttpsRequest_set_url(req, url)` | Change destination |
| `BearHttpsRequest_send_body_str(req, text)` | Send text data |
| `BearHttpsRequest_send_file_auto_detect_content_type(req, path)` | Upload file |
| `BearHttpsRequest_send_any(req, data, size)` | Send raw data |

### 📝 Common Headers

```c
// Authentication
BearHttpsRequest_add_header(request, "Authorization", "Bearer your-token");

// Content type for JSON
BearHttpsRequest_add_header(request, "Content-Type", "application/json");

// Content type for forms
BearHttpsRequest_add_header(request, "Content-Type", "application/x-www-form-urlencoded");

// User identification
BearHttpsRequest_add_header(request, "User-Agent", "YourApp/1.0");
```

### 🔄 HTTP Methods

```c
BearHttpsRequest_set_method(request, "GET");     // Get data (default)
BearHttpsRequest_set_method(request, "POST");    // Send new data
BearHttpsRequest_set_method(request, "PUT");     // Replace data
BearHttpsRequest_set_method(request, "DELETE");  // Delete data
BearHttpsRequest_set_method(request, "PATCH");   // Update data
```

---

## 🚀 What's Next?

Great job! You now know how to **send any kind of data** to web servers. Next up:

1. 📥 [Learn how to handle responses](/docs/response.md)
2. 🌐 [Configure network settings](/docs/network_configuration.md)
3. 💾 [Optimize memory usage](/docs/memory_and_limits.md)
4. 🎯 [Try advanced examples](/examples/)

---

<div align="center">

**🎉 You're now a request-sending pro!** 

[📥 Next: Handling Responses](/docs/response.md) | [🏠 Back to Main](/README.md)

</div>
