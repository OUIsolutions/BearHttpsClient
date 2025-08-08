<div align="center">

# 📥 Handling HTTP Responses

**Your complete guide to reading data from the web** 🌐

*From status codes to JSON parsing - master every response!*

</div>

---

## 🎯 What You'll Learn

By the end of this guide, you'll know how to:

| 🎯 **Skill** | 🌟 **Difficulty** | ⏱️ **Time** |
|-------------|-------------------|-------------|
| ✅ Check for errors | 🟢 Beginner | 2 minutes |
| 📊 Get status codes | 🟢 Beginner | 2 minutes |
| 🏷️ Read headers | 🟡 Intermediate | 3 minutes |
| 📖 Read text responses | 🟡 Intermediate | 3 minutes |
| 🗃️ Handle binary data | 🔴 Advanced | 5 minutes |
| 📊 Parse JSON responses | 🔴 Advanced | 5 minutes |

---

## 🚀 The Basics: Your First Response

Here's the **fundamental pattern** you'll use for every response:

```c
#include "BearHttpsClientOne.c"

int main() {
    // 🌐 Make a request
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/get");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    // ✅ ALWAYS check for errors first!
    if (BearHttpsResponse_error(response)) {
        printf("❌ Something went wrong: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    // 🎉 Success! Now we can read the response
    const char *body = BearHttpsResponse_read_body_str(response);
    
    // ✅ Check for errors again (reading can fail too!)
    if (BearHttpsResponse_error(response)) {
        printf("❌ Couldn't read response: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    // 🎯 Use the response data
    printf("📖 Response: %s\n", body);
    
    // 🧹 Always clean up!
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

> 💡 **Golden Rule**: Always check for errors **twice** - once after fetching, once after reading!

---

## 🛡️ Error Handling: Your Safety Net

Errors can happen for **many reasons**:

| ❌ **Error Type** | 🔍 **Common Causes** | 💡 **What to Do** |
|------------------|----------------------|-------------------|
| **Network Error** | No internet, DNS failure | Check connection, retry |
| **Server Error** | Server down, 500 errors | Wait and try again |
| **SSL Error** | Certificate problems | Check URL is correct |
| **Timeout** | Slow connection | Increase timeout |
| **Format Error** | Not text when expected | Check response type |

### 🔰 Basic Error Checking

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/status/404");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    // 🛡️ Check if the request itself failed
    if (BearHttpsResponse_error(response)) {
        printf("❌ Request failed: %s\n", BearHttpsResponse_get_error_msg(response));
        
        // 🧹 Clean up and exit
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    // 📊 Request succeeded, but check the status code
    int status = BearHttpsResponse_get_status_code(response);
    printf("📊 Status Code: %d\n", status);
    
    if (status >= 400) {
        printf("⚠️ Server returned an error status!\n");
    } else {
        printf("✅ Server responded successfully!\n");
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🎯 Advanced Error Handling with Retry

```c
#include "BearHttpsClientOne.c"
#include <unistd.h>  // for sleep()

BearHttpsResponse* fetch_with_retry(const char* url, int max_retries) {
    for (int i = 0; i < max_retries; i++) {
        printf("🔄 Attempt %d/%d...\n", i + 1, max_retries);
        
        BearHttpsRequest *request = newBearHttpsRequest(url);
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        
        if (!BearHttpsResponse_error(response)) {
            printf("✅ Success on attempt %d!\n", i + 1);
            BearHttpsRequest_free(request);
            return response;
        }
        
        printf("❌ Attempt %d failed: %s\n", i + 1, BearHttpsResponse_get_error_msg(response));
        
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        
        if (i < max_retries - 1) {
            printf("⏳ Waiting 2 seconds before retry...\n");
            sleep(2);
        }
    }
    
    printf("💥 All attempts failed!\n");
    return NULL;
}

int main() {
    BearHttpsResponse *response = fetch_with_retry("https://httpbin.org/get", 3);
    
    if (response) {
        printf("🎉 Final success: %s\n", BearHttpsResponse_read_body_str(response));
        BearHttpsResponse_free(response);
    }
    
    return 0;
}
```

---

## 📊 Status Codes: The Server's Report Card

Status codes tell you **what happened** with your request:

| 📊 **Code Range** | 🎭 **Meaning** | 💡 **Examples** |
|------------------|----------------|----------------|
| **200-299** | ✅ Success! | 200 OK, 201 Created |
| **300-399** | 🔄 Redirect | 301 Moved, 304 Not Modified |
| **400-499** | 😅 Your mistake | 404 Not Found, 401 Unauthorized |
| **500-599** | 😰 Server's mistake | 500 Internal Error, 503 Unavailable |

### 🔰 Reading Status Codes

```c
#include "BearHttpsClientOne.c"

void check_status_meaning(int status) {
    if (status >= 200 && status < 300) {
        printf("✅ Success! Everything worked perfectly.\n");
    } else if (status >= 300 && status < 400) {
        printf("🔄 Redirect - the content moved somewhere else.\n");
    } else if (status >= 400 && status < 500) {
        printf("😅 Client Error - check your request (URL, headers, etc.).\n");
    } else if (status >= 500) {
        printf("😰 Server Error - the server is having problems.\n");
    }
}

int main() {
    // 🧪 Test different status codes
    const char* test_urls[] = {
        "https://httpbin.org/status/200",  // Success
        "https://httpbin.org/status/404",  // Not Found
        "https://httpbin.org/status/500"   // Server Error
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\n🧪 Testing: %s\n", test_urls[i]);
        
        BearHttpsRequest *request = newBearHttpsRequest(test_urls[i]);
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        
        if (!BearHttpsResponse_error(response)) {
            int status = BearHttpsResponse_get_status_code(response);
            printf("📊 Status Code: %d\n", status);
            check_status_meaning(status);
        }
        
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
    }
    
    return 0;
}
```

---

## 🏷️ Reading Headers: The Response Metadata

Headers contain **important information** about the response:

| 🏷️ **Common Headers** | 📖 **What They Tell You** |
|----------------------|---------------------------|
| `Content-Type` | What kind of data you received |
| `Content-Length` | How much data there is |
| `Server` | What web server sent this |
| `Date` | When the response was created |
| `Cache-Control` | How long to cache this |
| `Set-Cookie` | Cookies the server wants to set |

### 🔰 Reading Specific Headers

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/response-headers?Content-Type=application/json&Server=MyServer/1.0");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 🔍 Look for specific headers
        const char *content_type = BearHttpsResponse_get_header_value_by_key(response, "Content-Type");
        if (content_type) {
            printf("📄 Content Type: %s\n", content_type);
        } else {
            printf("❓ Content Type: Not specified\n");
        }
        
        const char *server = BearHttpsResponse_get_header_value_by_key(response, "Server");
        if (server) {
            printf("🖥️ Server: %s\n", server);
        }
        
        const char *date = BearHttpsResponse_get_header_value_by_key(response, "Date");
        if (date) {
            printf("📅 Date: %s\n", date);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🔄 Iterating Through All Headers

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/get");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("🏷️ All Response Headers:\n");
        printf("========================\n");
        
        // 🔢 Get the total number of headers
        int header_count = BearHttpsResponse_get_headers_size(response);
        
        // 🔄 Loop through each header
        for (int i = 0; i < header_count; i++) {
            const char *key = BearHttpsResponse_get_header_key_by_index(response, i);
            const char *value = BearHttpsResponse_get_header_value_by_index(response, i);
            
            printf("📝 %s: %s\n", key, value);
        }
        
        printf("========================\n");
        printf("📊 Total headers: %d\n", header_count);
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 📖 Reading Text Responses: Getting the Message

Most web responses are **text-based** - HTML, JSON, XML, plain text, etc.

### 🔰 Simple Text Reading

```c
#include "BearHttpsClientOne.c"

int main() {
    // 📖 Get a simple text response
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/uuid");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 📄 Read as text string
        const char *text = BearHttpsResponse_read_body_str(response);
        
        // ✅ Check if reading succeeded
        if (!BearHttpsResponse_error(response)) {
            printf("📖 Response text: %s\n", text);
            
            // 📏 You can also get the length
            long size = BearHttpsResponse_get_body_size(response);
            printf("📏 Text length: %ld characters\n", size);
        } else {
            printf("❌ Failed to read text: %s\n", BearHttpsResponse_get_error_msg(response));
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🌐 Reading HTML Content

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        const char *html = BearHttpsResponse_read_body_str(response);
        
        if (!BearHttpsResponse_error(response)) {
            printf("🌐 HTML Content:\n");
            printf("================\n");
            
            // 📏 Print first 500 characters (HTML can be long!)
            char preview[501];
            strncpy(preview, html, 500);
            preview[500] = '\0';
            
            printf("%s", preview);
            
            if (strlen(html) > 500) {
                printf("\n... (truncated - full length: %lu characters)\n", strlen(html));
            }
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 🗃️ Reading Binary Data: Handling Files and Raw Data

Sometimes you need to handle **binary data** - images, files, or custom formats:

### 🔰 Basic Binary Reading

```c
#include "BearHttpsClientOne.c"

int main() {
    // 📷 Download a small image
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/image/png");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 🗃️ Read as raw binary data
        unsigned char *binary_data = BearHttpsResponse_read_body(response);
        
        if (!BearHttpsResponse_error(response)) {
            long data_size = BearHttpsResponse_get_body_size(response);
            
            printf("🗃️ Binary data received!\n");
            printf("📏 Size: %ld bytes\n", data_size);
            
            // 🔍 Show first 16 bytes in hexadecimal
            printf("📄 First 16 bytes (hex): ");
            for (int i = 0; i < 16 && i < data_size; i++) {
                printf("%02x ", binary_data[i]);
            }
            printf("\n");
            
            // 🎯 You could save this to a file:
            // FILE *file = fopen("downloaded_image.png", "wb");
            // fwrite(binary_data, 1, data_size, file);
            // fclose(file);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 💾 Saving Binary Data to File

```c
#include "BearHttpsClientOne.c"

int download_file(const char* url, const char* filename) {
    printf("📥 Downloading from: %s\n", url);
    
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (BearHttpsResponse_error(response)) {
        printf("❌ Download failed: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    unsigned char *data = BearHttpsResponse_read_body(response);
    if (BearHttpsResponse_error(response)) {
        printf("❌ Failed to read data: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    long size = BearHttpsResponse_get_body_size(response);
    
    // 💾 Save to file
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("❌ Cannot create file: %s\n", filename);
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    size_t written = fwrite(data, 1, size, file);
    fclose(file);
    
    if (written == size) {
        printf("✅ File saved: %s (%ld bytes)\n", filename, size);
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    } else {
        printf("❌ File write incomplete: %zu/%ld bytes\n", written, size);
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
}

int main() {
    // 📥 Download different types of files
    download_file("https://httpbin.org/image/jpeg", "test_image.jpg");
    download_file("https://httpbin.org/json", "test_data.json");
    
    return 0;
}
```

---

## 📊 Parsing JSON Responses: Modern Web Data

Most modern web APIs use **JSON format**. BearHttpsClient includes built-in JSON support!

### 🔰 Simple JSON Reading

```c
#include "BearHttpsClientOne.c"

int main() {
    // 📊 Get some JSON data
    BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/users/1");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 📊 Parse response as JSON
        cJSON *json = BearHttpsResponse_read_body_json(response);
        
        if (!BearHttpsResponse_error(response)) {
            // 🎯 Extract specific values
            cJSON *name = cJSON_GetObjectItem(json, "name");
            cJSON *email = cJSON_GetObjectItem(json, "email");
            cJSON *id = cJSON_GetObjectItem(json, "id");
            
            printf("👤 User Information:\n");
            printf("================\n");
            
            if (cJSON_IsNumber(id)) {
                printf("🆔 ID: %d\n", id->valueint);
            }
            
            if (cJSON_IsString(name)) {
                printf("👤 Name: %s\n", name->valuestring);
            }
            
            if (cJSON_IsString(email)) {
                printf("📧 Email: %s\n", email->valuestring);
            }
            
            // 📄 Print the entire JSON (formatted)
            char *formatted = cJSON_Print(json);
            printf("\n📊 Full JSON:\n%s\n", formatted);
            free(formatted);
            
            // 🧹 Free the JSON object
            cJSON_Delete(json);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📋 Working with JSON Arrays

```c
#include "BearHttpsClientOne.c"

int main() {
    // 📋 Get a list of posts
    BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/posts?_limit=3");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        cJSON *json = BearHttpsResponse_read_body_json(response);
        
        if (!BearHttpsResponse_error(response) && cJSON_IsArray(json)) {
            int array_size = cJSON_GetArraySize(json);
            printf("📋 Found %d posts:\n", array_size);
            printf("==================\n");
            
            // 🔄 Loop through each post
            for (int i = 0; i < array_size; i++) {
                cJSON *post = cJSON_GetArrayItem(json, i);
                
                if (cJSON_IsObject(post)) {
                    cJSON *id = cJSON_GetObjectItem(post, "id");
                    cJSON *title = cJSON_GetObjectItem(post, "title");
                    cJSON *body = cJSON_GetObjectItem(post, "body");
                    
                    printf("\n📝 Post #%d:\n", i + 1);
                    
                    if (cJSON_IsNumber(id)) {
                        printf("🆔 ID: %d\n", id->valueint);
                    }
                    
                    if (cJSON_IsString(title)) {
                        printf("📌 Title: %s\n", title->valuestring);
                    }
                    
                    if (cJSON_IsString(body)) {
                        // 📏 Show first 100 chars of body
                        char preview[101];
                        strncpy(preview, body->valuestring, 100);
                        preview[100] = '\0';
                        printf("📄 Body: %s%s\n", preview, strlen(body->valuestring) > 100 ? "..." : "");
                    }
                }
            }
            
            cJSON_Delete(json);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🎯 Complex JSON Navigation

```c
#include "BearHttpsClientOne.c"

int main() {
    // 🌤️ Get weather data (this is fake data from httpbin)
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/json");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        cJSON *json = BearHttpsResponse_read_body_json(response);
        
        if (!BearHttpsResponse_error(response)) {
            printf("🧭 Navigating Complex JSON:\n");
            printf("===========================\n");
            
            // 🔍 Navigate nested objects
            cJSON *slideshow = cJSON_GetObjectItem(json, "slideshow");
            if (cJSON_IsObject(slideshow)) {
                cJSON *author = cJSON_GetObjectItem(slideshow, "author");
                if (cJSON_IsString(author)) {
                    printf("👤 Author: %s\n", author->valuestring);
                }
                
                cJSON *title = cJSON_GetObjectItem(slideshow, "title");
                if (cJSON_IsString(title)) {
                    printf("📌 Title: %s\n", title->valuestring);
                }
                
                // 📋 Navigate arrays within objects
                cJSON *slides = cJSON_GetObjectItem(slideshow, "slides");
                if (cJSON_IsArray(slides)) {
                    int slide_count = cJSON_GetArraySize(slides);
                    printf("📊 Number of slides: %d\n", slide_count);
                    
                    for (int i = 0; i < slide_count; i++) {
                        cJSON *slide = cJSON_GetArrayItem(slides, i);
                        cJSON *slide_title = cJSON_GetObjectItem(slide, "title");
                        
                        if (cJSON_IsString(slide_title)) {
                            printf("  📄 Slide %d: %s\n", i + 1, slide_title->valuestring);
                        }
                    }
                }
            }
            
            cJSON_Delete(json);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 🔄 Chunked Reading: Handling Large Responses

For **very large responses**, you can read data in chunks to **save memory**:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/drip?numbytes=1000&duration=1");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("📥 Reading response in chunks...\n");
        
        unsigned char buffer[256];  // 256-byte chunks
        long total_read = 0;
        long chunk_count = 0;
        
        // 🔄 Keep reading until no more data
        while (1) {
            long bytes_read = BearHttpsResponse_read_body_chunck(response, buffer, sizeof(buffer) - 1);
            
            if (bytes_read <= 0) {
                break;  // No more data
            }
            
            // 🔚 Null-terminate for safe printing
            buffer[bytes_read] = '\0';
            
            chunk_count++;
            total_read += bytes_read;
            
            printf("📦 Chunk %ld (%ld bytes): ", chunk_count, bytes_read);
            
            // 📄 Print first 50 chars of chunk
            char preview[51];
            strncpy(preview, (char*)buffer, 50);
            preview[50] = '\0';
            printf("%s%s\n", preview, bytes_read > 50 ? "..." : "");
        }
        
        printf("\n✅ Finished! Read %ld bytes in %ld chunks.\n", total_read, chunk_count);
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 🎓 Complete Real-World Examples

### 🌤️ Weather App Response Handler

```c
#include "BearHttpsClientOne.c"

void display_weather(cJSON *weather_data) {
    cJSON *main = cJSON_GetObjectItem(weather_data, "main");
    cJSON *weather_array = cJSON_GetObjectItem(weather_data, "weather");
    cJSON *name = cJSON_GetObjectItem(weather_data, "name");
    
    if (cJSON_IsString(name)) {
        printf("🌍 Location: %s\n", name->valuestring);
    }
    
    if (cJSON_IsObject(main)) {
        cJSON *temp = cJSON_GetObjectItem(main, "temp");
        cJSON *humidity = cJSON_GetObjectItem(main, "humidity");
        
        if (cJSON_IsNumber(temp)) {
            printf("🌡️ Temperature: %.1f°C\n", temp->valuedouble - 273.15);  // Convert from Kelvin
        }
        
        if (cJSON_IsNumber(humidity)) {
            printf("💧 Humidity: %d%%\n", humidity->valueint);
        }
    }
    
    if (cJSON_IsArray(weather_array) && cJSON_GetArraySize(weather_array) > 0) {
        cJSON *weather = cJSON_GetArrayItem(weather_array, 0);
        cJSON *description = cJSON_GetObjectItem(weather, "description");
        
        if (cJSON_IsString(description)) {
            printf("☁️ Conditions: %s\n", description->valuestring);
        }
    }
}

int main() {
    // 🔑 Replace with your actual API key from OpenWeatherMap
    const char *api_key = "your-api-key-here";
    char url[512];
    snprintf(url, sizeof(url), 
             "https://api.openweathermap.org/data/2.5/weather?q=London&appid=%s", 
             api_key);
    
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsRequest_add_header(request, "User-Agent", "WeatherApp/1.0");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (BearHttpsResponse_error(response)) {
        printf("❌ Weather request failed: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    int status = BearHttpsResponse_get_status_code(response);
    if (status != 200) {
        printf("❌ Weather API returned status %d\n", status);
        printf("📖 Response: %s\n", BearHttpsResponse_read_body_str(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    cJSON *weather_json = BearHttpsResponse_read_body_json(response);
    if (BearHttpsResponse_error(response)) {
        printf("❌ Failed to parse weather JSON: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    printf("🌤️ Current Weather:\n");
    printf("==================\n");
    display_weather(weather_json);
    
    cJSON_Delete(weather_json);
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📰 News Reader with Error Handling

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/posts");
    BearHttpsRequest_add_header(request, "User-Agent", "NewsReader/1.0");
    BearHttpsRequest_add_header(request, "Accept", "application/json");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    // 🛡️ Comprehensive error checking
    if (BearHttpsResponse_error(response)) {
        printf("❌ Network error: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    int status = BearHttpsResponse_get_status_code(response);
    printf("📊 Server response: %d\n", status);
    
    if (status != 200) {
        printf("⚠️ Unexpected status code: %d\n", status);
        const char *error_body = BearHttpsResponse_read_body_str(response);
        if (!BearHttpsResponse_error(response)) {
            printf("📖 Error message: %s\n", error_body);
        }
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    // ✅ Success - parse the news
    cJSON *articles = BearHttpsResponse_read_body_json(response);
    if (BearHttpsResponse_error(response)) {
        printf("❌ JSON parsing failed: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    if (!cJSON_IsArray(articles)) {
        printf("❌ Expected JSON array, got something else\n");
        cJSON_Delete(articles);
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    printf("\n📰 Latest News (First 5 articles):\n");
    printf("===================================\n");
    
    int article_count = cJSON_GetArraySize(articles);
    int max_articles = article_count > 5 ? 5 : article_count;
    
    for (int i = 0; i < max_articles; i++) {
        cJSON *article = cJSON_GetArrayItem(articles, i);
        
        if (cJSON_IsObject(article)) {
            cJSON *title = cJSON_GetObjectItem(article, "title");
            cJSON *body = cJSON_GetObjectItem(article, "body");
            
            printf("\n📝 Article %d:\n", i + 1);
            
            if (cJSON_IsString(title)) {
                printf("📌 %s\n", title->valuestring);
            }
            
            if (cJSON_IsString(body)) {
                // 📏 Show first 150 characters
                char preview[151];
                strncpy(preview, body->valuestring, 150);
                preview[150] = '\0';
                printf("📄 %s%s\n", preview, strlen(body->valuestring) > 150 ? "..." : "");
            }
        }
    }
    
    printf("\n📊 Total articles available: %d\n", article_count);
    
    cJSON_Delete(articles);
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 🚨 Common Mistakes and How to Avoid Them

### ❌ Mistake 1: Not Checking for Errors

```c
// ❌ Wrong - no error checking
BearHttpsResponse *response = BearHttpsRequest_fetch(request);
const char *body = BearHttpsResponse_read_body_str(response);  // Might crash!
printf("%s\n", body);

// ✅ Correct - always check errors
BearHttpsResponse *response = BearHttpsRequest_fetch(request);
if (BearHttpsResponse_error(response)) {
    printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    return 1;
}
const char *body = BearHttpsResponse_read_body_str(response);
if (BearHttpsResponse_error(response)) {
    printf("Read error: %s\n", BearHttpsResponse_get_error_msg(response));
    return 1;
}
printf("%s\n", body);
```

### ❌ Mistake 2: Ignoring Status Codes

```c
// ❌ Wrong - ignoring what the server says
BearHttpsResponse *response = BearHttpsRequest_fetch(request);
const char *body = BearHttpsResponse_read_body_str(response);
printf("Got: %s\n", body);  // Might be an error message!

// ✅ Correct - check status first
BearHttpsResponse *response = BearHttpsRequest_fetch(request);
int status = BearHttpsResponse_get_status_code(response);
if (status != 200) {
    printf("Server error: %d\n", status);
    return 1;
}
const char *body = BearHttpsResponse_read_body_str(response);
printf("Success: %s\n", body);
```

### ❌ Mistake 3: Forgetting to Free JSON

```c
// ❌ Wrong - memory leak!
cJSON *json = BearHttpsResponse_read_body_json(response);
// ... use json ...
// Missing cJSON_Delete(json)!

// ✅ Correct - always free JSON
cJSON *json = BearHttpsResponse_read_body_json(response);
// ... use json ...
cJSON_Delete(json);  // 👈 Free the JSON object
```

### ❌ Mistake 4: Reading Binary as Text

```c
// ❌ Wrong - trying to read binary as text
BearHttpsResponse *response = /* fetch image */;
const char *text = BearHttpsResponse_read_body_str(response);  // Will fail!

// ✅ Correct - use binary read for binary data
BearHttpsResponse *response = /* fetch image */;
unsigned char *data = BearHttpsResponse_read_body(response);
long size = BearHttpsResponse_get_body_size(response);
```

---

## 🎯 Quick Reference Cheat Sheet

### 🛡️ Error Checking Pattern

```c
// 1. Check fetch error
if (BearHttpsResponse_error(response)) {
    printf("Fetch failed: %s\n", BearHttpsResponse_get_error_msg(response));
    /* cleanup and return */
}

// 2. Check status code
int status = BearHttpsResponse_get_status_code(response);
if (status != 200) {
    printf("Bad status: %d\n", status);
    /* cleanup and return */
}

// 3. Read data
const char *body = BearHttpsResponse_read_body_str(response);

// 4. Check read error
if (BearHttpsResponse_error(response)) {
    printf("Read failed: %s\n", BearHttpsResponse_get_error_msg(response));
    /* cleanup and return */
}
```

### 📊 Response Functions

| 🛠️ **Function** | 📖 **What It Does** |
|-----------------|---------------------|
| `BearHttpsResponse_error(resp)` | Check if there was an error |
| `BearHttpsResponse_get_error_msg(resp)` | Get error message |
| `BearHttpsResponse_get_status_code(resp)` | Get HTTP status code |
| `BearHttpsResponse_get_header_value_by_key(resp, key)` | Get specific header |
| `BearHttpsResponse_read_body_str(resp)` | Read as text string |
| `BearHttpsResponse_read_body(resp)` | Read as binary data |
| `BearHttpsResponse_read_body_json(resp)` | Parse as JSON |
| `BearHttpsResponse_get_body_size(resp)` | Get data size |

### 📊 Status Code Meanings

```c
if (status >= 200 && status < 300) /* Success */
if (status >= 400 && status < 500) /* Client error (your fault) */
if (status >= 500) /* Server error (server's fault) */
```

---

## 🚀 What's Next?

Excellent! You now know how to **handle any kind of response**. Next steps:

1. 🌐 [Configure network settings](/docs/network_configuration.md)
2. 💾 [Optimize memory usage](/docs/memory_and_limits.md)  
3. 🔧 [Understanding ownership system](/docs/ownership_system.md)
4. 🎯 [Try advanced examples](/examples/)

---

<div align="center">

**🎉 You're now a response-handling expert!** 

[🌐 Next: Network Configuration](/docs/network_configuration.md) | [🏠 Back to Main](/README.md)

</div>
