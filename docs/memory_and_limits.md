<div align="center">

# 💾 Memory Management & Limits

**Master memory optimization for blazing fast performance** 🚀

*Control every byte for maximum efficiency and reliability*

</div>

---

## 🎯 What You'll Learn

By the end of this guide, you'll know how to:

| 🎯 **Skill** | 🌟 **Difficulty** | ⏱️ **Time** |
|-------------|-------------------|-------------|
| 📊 Configure memory limits | 🟡 Intermediate | 5 minutes |
| ⚡ Optimize read performance | 🟡 Intermediate | 5 minutes |
| 🛡️ Prevent memory attacks | 🟡 Intermediate | 3 minutes |
| 🔧 Tune allocation strategies | 🔴 Advanced | 7 minutes |
| 📈 Monitor memory usage | 🔴 Advanced | 5 minutes |

---

## 💡 Why Memory Management Matters

Memory management in HTTP clients is **critical** for:

| 🎯 **Reason** | 📖 **Impact** | 💡 **Example** |
|---------------|---------------|----------------|
| **Performance** | Faster requests | Fewer memory allocations |
| **Reliability** | No crashes | Handle huge responses safely |
| **Security** | Prevent attacks | Limit malicious large responses |
| **Resource usage** | Lower memory footprint | Better for embedded systems |
| **Predictability** | Consistent behavior | No surprise memory spikes |

---

## 📊 Setting Body Size Limits: Your Safety Net

Protect your application from **accidentally downloading huge files** or **malicious responses**:

### 🔰 Basic Body Size Limiting

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/bytes/1000");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 🛡️ Limit response to 5KB (5000 bytes)
        BearHttpsResponse_set_max_body_size(response, 5000);
        
        printf("🛡️ Maximum response size set to 5KB\n");
        
        // 📖 Try to read the response
        const char *body = BearHttpsResponse_read_body_str(response);
        
        if (!BearHttpsResponse_error(response)) {
            long actual_size = BearHttpsResponse_get_body_size(response);
            printf("✅ Successfully read %ld bytes (under limit)\n", actual_size);
            printf("📄 Preview: %.100s%s\n", body, actual_size > 100 ? "..." : "");
        } else {
            printf("❌ Reading failed (possibly size limit exceeded): %s\n", 
                   BearHttpsResponse_get_error_msg(response));
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🎯 Smart Size Limiting for Different Content Types

```c
#include "BearHttpsClientOne.c"

void set_appropriate_limits(BearHttpsResponse *response, const char *content_type) {
    if (!content_type) {
        // 📄 Default for unknown content
        BearHttpsResponse_set_max_body_size(response, 1024 * 1024);  // 1MB
        printf("📄 Set default limit: 1MB\n");
        return;
    }
    
    if (strstr(content_type, "application/json")) {
        // 📊 JSON APIs usually small
        BearHttpsResponse_set_max_body_size(response, 512 * 1024);  // 512KB
        printf("📊 JSON detected - limit: 512KB\n");
    }
    else if (strstr(content_type, "text/html")) {
        // 🌐 HTML pages can be larger
        BearHttpsResponse_set_max_body_size(response, 2 * 1024 * 1024);  // 2MB
        printf("🌐 HTML detected - limit: 2MB\n");
    }
    else if (strstr(content_type, "image/")) {
        // 📷 Images can be large
        BearHttpsResponse_set_max_body_size(response, 10 * 1024 * 1024);  // 10MB
        printf("📷 Image detected - limit: 10MB\n");
    }
    else if (strstr(content_type, "application/octet-stream")) {
        // 🗃️ Binary files - be careful!
        BearHttpsResponse_set_max_body_size(response, 50 * 1024 * 1024);  // 50MB
        printf("🗃️ Binary detected - limit: 50MB\n");
    }
    else {
        // 📄 Default for other types
        BearHttpsResponse_set_max_body_size(response, 5 * 1024 * 1024);  // 5MB
        printf("📄 Other content - limit: 5MB\n");
    }
}

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/json");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 🔍 Check content type and set appropriate limit
        const char *content_type = BearHttpsResponse_get_header_value_by_key(response, "Content-Type");
        printf("📝 Content-Type: %s\n", content_type ? content_type : "Unknown");
        
        set_appropriate_limits(response, content_type);
        
        // 📖 Now read the response safely
        const char *body = BearHttpsResponse_read_body_str(response);
        
        if (!BearHttpsResponse_error(response)) {
            printf("✅ Response read successfully!\n");
            printf("📏 Size: %ld bytes\n", BearHttpsResponse_get_body_size(response));
        } else {
            printf("❌ Failed to read: %s\n", BearHttpsResponse_get_error_msg(response));
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 📋 Recommended Size Limits by Use Case

| 🎯 **Use Case** | 📊 **Typical Size** | 🛡️ **Recommended Limit** | 💡 **Reason** |
|----------------|---------------------|--------------------------|---------------|
| **JSON APIs** | 1-100KB | 512KB | APIs usually small and predictable |
| **HTML Pages** | 10-500KB | 2MB | Web pages vary but rarely huge |
| **Images** | 100KB-5MB | 10MB | Photos can be large but not infinite |
| **Documents** | 100KB-10MB | 50MB | PDFs and docs can be substantial |
| **Data feeds** | 1KB-1MB | 5MB | RSS/XML feeds are usually moderate |
| **File downloads** | Variable | No limit or very high | User expects large downloads |

---

## ⚡ Optimizing Read Performance

Fine-tune how BearHttpsClient reads data from the network:

### 🔰 Basic Read Configuration

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/bytes/10000");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        // ⚡ Configure body reading performance
        int read_size = 4096;      // Read 4KB chunks (larger = faster for big files)
        double growth_factor = 2.0; // Double buffer size when needed (aggressive growth)
        
        BearHttpsResponse_set_body_read_props(response, read_size, growth_factor);
        
        printf("⚡ Optimized for large file reading:\n");
        printf("   📊 Chunk size: %d bytes\n", read_size);
        printf("   📈 Growth factor: %.1fx\n", growth_factor);
        
        // 📖 Read the response (now optimized!)
        const char *body = BearHttpsResponse_read_body_str(response);
        
        if (!BearHttpsResponse_error(response)) {
            printf("✅ Read %ld bytes efficiently!\n", BearHttpsResponse_get_body_size(response));
        } else {
            printf("❌ Reading failed: %s\n", BearHttpsResponse_get_error_msg(response));
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🎯 Performance Tuning for Different Scenarios

```c
#include "BearHttpsClientOne.c"

typedef struct {
    const char *name;
    int read_size;
    double growth_factor;
    const char *description;
} ReadProfile;

void apply_read_profile(BearHttpsResponse *response, ReadProfile profile) {
    BearHttpsResponse_set_body_read_props(response, profile.read_size, profile.growth_factor);
    
    printf("🎯 Applied '%s' profile:\n", profile.name);
    printf("   📊 Read size: %d bytes\n", profile.read_size);
    printf("   📈 Growth factor: %.1fx\n", profile.growth_factor);
    printf("   💡 %s\n", profile.description);
}

int main() {
    // 🎯 Different performance profiles
    ReadProfile profiles[] = {
        {
            .name = "Memory Saver",
            .read_size = 512,
            .growth_factor = 1.2,
            .description = "Minimal memory usage, slower but safe"
        },
        {
            .name = "Balanced",
            .read_size = 2048,
            .growth_factor = 1.5,
            .description = "Good balance of speed and memory usage"
        },
        {
            .name = "Speed Demon",
            .read_size = 8192,
            .growth_factor = 2.0,
            .description = "Maximum speed, uses more memory"
        },
        {
            .name = "Large File",
            .read_size = 16384,
            .growth_factor = 2.5,
            .description = "Optimized for multi-megabyte downloads"
        }
    };
    
    // 🧪 Test each profile
    for (int i = 0; i < 4; i++) {
        printf("\n🧪 Testing profile %d:\n", i + 1);
        
        BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/bytes/5000");
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        
        if (!BearHttpsResponse_error(response)) {
            apply_read_profile(response, profiles[i]);
            
            clock_t start = clock();
            const char *body = BearHttpsResponse_read_body_str(response);
            clock_t end = clock();
            
            double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            
            if (!BearHttpsResponse_error(response)) {
                printf("✅ Success! Time: %.3f seconds\n", time_taken);
                printf("📏 Read %ld bytes\n", BearHttpsResponse_get_body_size(response));
            } else {
                printf("❌ Failed: %s\n", BearHttpsResponse_get_error_msg(response));
            }
        }
        
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
    }
    
    return 0;
}
```

### 📊 Header Read Optimization

Headers are read separately and can also be optimized:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/response-headers?key1=value1&key2=value2");
    
    // ⚡ Configure header reading (before fetch!)
    int header_read_size = 1024;    // Read 1KB header chunks
    double header_growth = 1.5;     // Moderate growth for headers
    
    BearHttpsRequest_set_chunk_header_read_props(request, header_read_size, header_growth);
    
    printf("⚡ Optimized header reading:\n");
    printf("   📊 Header chunk size: %d bytes\n", header_read_size);
    printf("   📈 Header growth factor: %.1fx\n", header_growth);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("✅ Headers read efficiently!\n");
        
        // 📋 Show header count
        int header_count = BearHttpsResponse_get_headers_size(response);
        printf("📋 Received %d headers\n", header_count);
        
        // 📖 Read body with default settings
        const char *body = BearHttpsResponse_read_body_str(response);
        if (!BearHttpsResponse_error(response)) {
            printf("📖 Body read successfully (%ld bytes)\n", 
                   BearHttpsResponse_get_body_size(response));
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

---

## 📈 Understanding Memory Growth Patterns

The **growth factor** determines how aggressively BearHttpsClient allocates memory:

### 📊 Growth Factor Comparison

| 📈 **Growth Factor** | 🎯 **Memory Pattern** | ⚡ **Speed** | 💾 **Memory Use** | 💡 **Best For** |
|-------------------|---------------------|-------------|------------------|----------------|
| **1.1 (Conservative)** | Slow, steady growth | Slower | Very low | Embedded systems |
| **1.5 (Balanced)** | Moderate growth | Good | Moderate | General purpose |
| **2.0 (Aggressive)** | Fast doubling | Fast | Higher | Large downloads |
| **3.0 (Very Aggressive)** | Rapid growth | Fastest | Highest | Huge files only |

### 🧪 Memory Growth Demonstration

```c
#include "BearHttpsClientOne.c"

void demonstrate_growth_pattern(double growth_factor, const char *description) {
    printf("\n📈 Growth Factor %.1f (%s):\n", growth_factor, description);
    
    // 🧮 Simulate memory growth for a 100KB response
    int initial_size = 1024;  // Start with 1KB buffer
    int current_size = initial_size;
    int total_data = 100 * 1024;  // 100KB response
    int data_read = 0;
    int allocations = 0;
    
    printf("   📊 Starting buffer: %d bytes\n", initial_size);
    
    while (data_read < total_data) {
        int chunk = current_size < (total_data - data_read) ? current_size : (total_data - data_read);
        data_read += chunk;
        allocations++;
        
        printf("   🔄 Allocation %d: %d bytes (total read: %d)\n", 
               allocations, current_size, data_read);
        
        if (data_read < total_data) {
            current_size = (int)(current_size * growth_factor);
        }
        
        // 🛡️ Safety limit to prevent infinite demo
        if (allocations > 10) {
            printf("   ⚠️ Demo truncated (would continue...)\n");
            break;
        }
    }
    
    printf("   ✅ Final: %d allocations, last buffer: %d bytes\n", allocations, current_size);
}

int main() {
    printf("🧪 Memory Growth Pattern Analysis\n");
    printf("=================================\n");
    
    demonstrate_growth_pattern(1.2, "Conservative");
    demonstrate_growth_pattern(1.5, "Balanced");
    demonstrate_growth_pattern(2.0, "Aggressive");
    demonstrate_growth_pattern(3.0, "Very Aggressive");
    
    printf("\n💡 Key Insights:\n");
    printf("   🐌 Lower factors = More allocations, less memory waste\n");
    printf("   🚀 Higher factors = Fewer allocations, more memory waste\n");
    printf("   🎯 1.5-2.0 is usually optimal for most applications\n");
    
    return 0;
}
```

---

## 🛡️ Security: Preventing Memory-Based Attacks

Protect your application from malicious responses designed to exhaust memory:

### 🔰 Basic Protection Setup

```c
#include "BearHttpsClientOne.c"

BearHttpsResponse* fetch_with_protection(const char* url, long max_size) {
    printf("🛡️ Fetching %s with %ld byte limit...\n", url, max_size);
    
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (BearHttpsResponse_error(response)) {
        printf("❌ Network error: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return NULL;
    }
    
    // 🛡️ Apply size limit BEFORE reading
    BearHttpsResponse_set_max_body_size(response, max_size);
    
    // 🔍 Check content length header if available
    const char *content_length = BearHttpsResponse_get_header_value_by_key(response, "Content-Length");
    if (content_length) {
        long declared_size = atol(content_length);
        printf("📏 Server declares %ld bytes\n", declared_size);
        
        if (declared_size > max_size) {
            printf("⚠️ Server declares size (%ld) exceeds limit (%ld)\n", declared_size, max_size);
            printf("🛡️ Proceeding with caution...\n");
        }
    }
    
    BearHttpsRequest_free(request);
    return response;
}

int main() {
    // 🧪 Test protection against different response sizes
    const char* test_urls[] = {
        "https://httpbin.org/bytes/1000",   // Safe: 1KB
        "https://httpbin.org/bytes/10000",  // Medium: 10KB  
        "https://httpbin.org/bytes/100000"  // Large: 100KB
    };
    
    long safe_limit = 50000;  // 50KB limit
    
    for (int i = 0; i < 3; i++) {
        printf("\n🧪 Test %d: %s\n", i + 1, test_urls[i]);
        
        BearHttpsResponse *response = fetch_with_protection(test_urls[i], safe_limit);
        
        if (response) {
            const char *body = BearHttpsResponse_read_body_str(response);
            
            if (!BearHttpsResponse_error(response)) {
                long actual_size = BearHttpsResponse_get_body_size(response);
                printf("✅ Successfully read %ld bytes (within limit)\n", actual_size);
            } else {
                printf("🛡️ Blocked by size limit: %s\n", BearHttpsResponse_get_error_msg(response));
            }
            
            BearHttpsResponse_free(response);
        }
    }
    
    return 0;
}
```

### 🎯 Advanced Multi-Layer Protection

```c
#include "BearHttpsClientOne.c"

typedef struct {
    long max_body_size;
    long max_header_size;
    int max_headers;
    int read_timeout;
} SecurityLimits;

BearHttpsResponse* secure_fetch(const char* url, SecurityLimits limits) {
    BearHttpsRequest *request = newBearHttpsRequest(url);
    
    // ⏱️ Set timeouts to prevent hanging
    BearHttpsRequest_set_connection_timeout(request, 10);
    BearHttpsRequest_set_read_timeout(request, limits.read_timeout);
    
    // 📊 Configure conservative header reading
    BearHttpsRequest_set_chunk_header_read_props(request, 512, 1.2);
    
    printf("🔒 Secure fetch with limits:\n");
    printf("   📏 Max body: %ld bytes\n", limits.max_body_size);
    printf("   📋 Max headers: %d\n", limits.max_headers);
    printf("   ⏱️ Timeout: %d seconds\n", limits.read_timeout);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    BearHttpsRequest_free(request);
    
    if (BearHttpsResponse_error(response)) {
        return response;  // Return error response
    }
    
    // 🛡️ Check header count
    int header_count = BearHttpsResponse_get_headers_size(response);
    if (header_count > limits.max_headers) {
        printf("⚠️ Too many headers: %d > %d\n", header_count, limits.max_headers);
        // Continue anyway, but log the issue
    }
    
    // 🛡️ Apply body size limit
    BearHttpsResponse_set_max_body_size(response, limits.max_body_size);
    
    // ⚡ Configure conservative body reading
    BearHttpsResponse_set_body_read_props(response, 1024, 1.3);
    
    return response;
}

int main() {
    // 🛡️ Define security profiles
    SecurityLimits profiles[] = {
        {
            .max_body_size = 10 * 1024,      // 10KB
            .max_header_size = 2048,         // 2KB headers
            .max_headers = 20,               // Max 20 headers
            .read_timeout = 10               // 10 second timeout
        },
        {
            .max_body_size = 100 * 1024,     // 100KB
            .max_header_size = 4096,         // 4KB headers
            .max_headers = 50,               // Max 50 headers
            .read_timeout = 30               // 30 second timeout
        },
        {
            .max_body_size = 1024 * 1024,    // 1MB
            .max_header_size = 8192,         // 8KB headers
            .max_headers = 100,              // Max 100 headers
            .read_timeout = 60               // 60 second timeout
        }
    };
    
    const char* profile_names[] = {"Paranoid", "Cautious", "Permissive"};
    
    // 🧪 Test with different security levels
    for (int i = 0; i < 3; i++) {
        printf("\n🛡️ Testing '%s' security profile:\n", profile_names[i]);
        
        BearHttpsResponse *response = secure_fetch("https://httpbin.org/json", profiles[i]);
        
        if (!BearHttpsResponse_error(response)) {
            const char *body = BearHttpsResponse_read_body_str(response);
            
            if (!BearHttpsResponse_error(response)) {
                printf("✅ Secure read successful: %ld bytes\n", 
                       BearHttpsResponse_get_body_size(response));
            } else {
                printf("🛡️ Blocked by security limits: %s\n", 
                       BearHttpsResponse_get_error_msg(response));
            }
        } else {
            printf("❌ Network error: %s\n", BearHttpsResponse_get_error_msg(response));
        }
        
        if (response) {
            BearHttpsResponse_free(response);
        }
    }
    
    return 0;
}
```

---

## 🎓 Real-World Memory Optimization Examples

### 📱 Mobile App Configuration (Low Memory)

```c
#include "BearHttpsClientOne.c"

BearHttpsResponse* mobile_optimized_request(const char* url) {
    BearHttpsRequest *request = newBearHttpsRequest(url);
    
    // ⚡ Mobile-optimized settings: prioritize battery and memory
    BearHttpsRequest_set_chunk_header_read_props(request, 256, 1.2);  // Small header chunks
    BearHttpsRequest_set_connection_timeout(request, 15);  // Allow for slower mobile networks
    BearHttpsRequest_set_read_timeout(request, 45);
    
    // 📱 Add mobile-specific headers
    BearHttpsRequest_add_header(request, "Accept-Encoding", "gzip");  // Save bandwidth
    BearHttpsRequest_add_header(request, "Cache-Control", "max-age=300");  // Enable caching
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    BearHttpsRequest_free(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 📱 Conservative memory settings for mobile
        BearHttpsResponse_set_max_body_size(response, 256 * 1024);  // 256KB max
        BearHttpsResponse_set_body_read_props(response, 512, 1.3);  // Small chunks, slow growth
    }
    
    return response;
}

int main() {
    printf("📱 Mobile-optimized HTTP client demo\n");
    printf("===================================\n");
    
    const char* mobile_urls[] = {
        "https://httpbin.org/json",
        "https://jsonplaceholder.typicode.com/posts/1",
        "https://httpbin.org/user-agent"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\n📱 Mobile request %d: %s\n", i + 1, mobile_urls[i]);
        
        BearHttpsResponse *response = mobile_optimized_request(mobile_urls[i]);
        
        if (!BearHttpsResponse_error(response)) {
            const char *body = BearHttpsResponse_read_body_str(response);
            
            if (!BearHttpsResponse_error(response)) {
                long size = BearHttpsResponse_get_body_size(response);
                printf("✅ Mobile request successful: %ld bytes\n", size);
                
                // 📊 Show memory efficiency
                printf("💾 Memory efficient: Used small chunks and conservative growth\n");
            } else {
                printf("❌ Read failed: %s\n", BearHttpsResponse_get_error_msg(response));
            }
        } else {
            printf("❌ Request failed: %s\n", BearHttpsResponse_get_error_msg(response));
        }
        
        BearHttpsResponse_free(response);
    }
    
    return 0;
}
```

### 🖥️ Server/Desktop Configuration (High Performance)

```c
#include "BearHttpsClientOne.c"

BearHttpsResponse* server_optimized_request(const char* url) {
    BearHttpsRequest *request = newBearHttpsRequest(url);
    
    // 🚀 Server-optimized settings: maximize throughput
    BearHttpsRequest_set_chunk_header_read_props(request, 4096, 2.0);  // Large header chunks
    BearHttpsRequest_set_connection_timeout(request, 5);   // Fast connections expected
    BearHttpsRequest_set_read_timeout(request, 30);
    
    // 🖥️ Server-optimized headers
    BearHttpsRequest_add_header(request, "Connection", "keep-alive");
    BearHttpsRequest_add_header(request, "Accept-Encoding", "gzip, deflate, br");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    BearHttpsRequest_free(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 🚀 Aggressive memory settings for servers
        BearHttpsResponse_set_max_body_size(response, 100 * 1024 * 1024);  // 100MB max
        BearHttpsResponse_set_body_read_props(response, 8192, 2.0);  // Large chunks, fast growth
    }
    
    return response;
}

int main() {
    printf("🖥️ Server-optimized HTTP client demo\n");
    printf("====================================\n");
    
    // 🧪 Test with various response sizes
    const char* server_urls[] = {
        "https://httpbin.org/bytes/1000",    // Small
        "https://httpbin.org/bytes/50000",   // Medium
        "https://httpbin.org/bytes/500000"   // Large
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\n🖥️ Server request %d: %s\n", i + 1, server_urls[i]);
        
        clock_t start = clock();
        BearHttpsResponse *response = server_optimized_request(server_urls[i]);
        
        if (!BearHttpsResponse_error(response)) {
            const char *body = BearHttpsResponse_read_body_str(response);
            clock_t end = clock();
            
            double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            
            if (!BearHttpsResponse_error(response)) {
                long size = BearHttpsResponse_get_body_size(response);
                printf("✅ Server request successful: %ld bytes in %.3f seconds\n", 
                       size, time_taken);
                printf("🚀 Throughput: %.2f KB/s\n", (size / 1024.0) / time_taken);
            } else {
                printf("❌ Read failed: %s\n", BearHttpsResponse_get_error_msg(response));
            }
        } else {
            printf("❌ Request failed: %s\n", BearHttpsResponse_get_error_msg(response));
        }
        
        BearHttpsResponse_free(response);
    }
    
    return 0;
}
```

### 🔬 IoT/Embedded Configuration (Minimal Memory)

```c
#include "BearHttpsClientOne.c"

BearHttpsResponse* iot_minimal_request(const char* url) {
    BearHttpsRequest *request = newBearHttpsRequest(url);
    
    // 🔬 IoT settings: absolute minimum memory usage
    BearHttpsRequest_set_chunk_header_read_props(request, 128, 1.1);  // Tiny header chunks
    BearHttpsRequest_set_connection_timeout(request, 30);   // IoT networks can be slow
    BearHttpsRequest_set_read_timeout(request, 60);
    
    // 🔬 Minimal headers
    BearHttpsRequest_add_header(request, "User-Agent", "IoTDevice/1.0");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    BearHttpsRequest_free(request);
    
    if (!BearHttpsResponse_error(response)) {
        // 🔬 Extremely conservative memory settings
        BearHttpsResponse_set_max_body_size(response, 4096);  // Only 4KB max!
        BearHttpsResponse_set_body_read_props(response, 64, 1.1);  // Tiny chunks, minimal growth
    }
    
    return response;
}

int main() {
    printf("🔬 IoT/Embedded HTTP client demo\n");
    printf("================================\n");
    
    // 🔬 IoT typically deals with small data
    const char* iot_urls[] = {
        "https://httpbin.org/json",           // Small JSON
        "https://httpbin.org/bytes/100",      // Tiny response
        "https://httpbin.org/uuid"            // Very small
    };
    
    // 📊 Track memory usage simulation
    printf("💾 Simulating minimal memory environment...\n");
    printf("📏 Available RAM: 64KB (simulated)\n");
    printf("🛡️ Reserved for system: 32KB\n");
    printf("📱 Available for HTTP: 32KB\n\n");
    
    for (int i = 0; i < 3; i++) {
        printf("🔬 IoT request %d: %s\n", i + 1, iot_urls[i]);
        
        BearHttpsResponse *response = iot_minimal_request(iot_urls[i]);
        
        if (!BearHttpsResponse_error(response)) {
            const char *body = BearHttpsResponse_read_body_str(response);
            
            if (!BearHttpsResponse_error(response)) {
                long size = BearHttpsResponse_get_body_size(response);
                printf("✅ IoT request successful: %ld bytes\n", size);
                
                // 📊 Simulated memory usage
                long estimated_memory = size * 2;  // Rough estimate including buffers
                printf("💾 Estimated memory used: ~%ld bytes (%.1f%% of available)\n", 
                       estimated_memory, (estimated_memory / 327.68));  // 32KB = 32768 bytes
                
                if (estimated_memory > 16384) {  // More than half available memory
                    printf("⚠️ High memory usage for IoT device!\n");
                } else {
                    printf("✅ Memory usage acceptable for IoT\n");
                }
            } else {
                printf("❌ Read failed: %s\n", BearHttpsResponse_get_error_msg(response));
            }
        } else {
            printf("❌ Request failed: %s\n", BearHttpsResponse_get_error_msg(response));
        }
        
        BearHttpsResponse_free(response);
        printf("\n");
    }
    
    return 0;
}
```

---

## 🚨 Common Memory Management Mistakes

### ❌ Mistake 1: No Size Limits

```c
// ❌ Wrong - no protection against huge responses
BearHttpsResponse *response = BearHttpsRequest_fetch(request);
const char *body = BearHttpsResponse_read_body_str(response);  // Could be gigabytes!

// ✅ Correct - always set reasonable limits
BearHttpsResponse *response = BearHttpsRequest_fetch(request);
BearHttpsResponse_set_max_body_size(response, 1024 * 1024);  // 1MB limit
const char *body = BearHttpsResponse_read_body_str(response);
```

### ❌ Mistake 2: Extreme Growth Factors

```c
// ❌ Wrong - extreme growth wastes memory
BearHttpsResponse_set_body_read_props(response, 1024, 10.0);  // 10x growth is excessive!

// ✅ Correct - reasonable growth factors
BearHttpsResponse_set_body_read_props(response, 2048, 1.5);  // 1.5x growth is efficient
```

### ❌ Mistake 3: Ignoring Content-Type for Limits

```c
// ❌ Wrong - same limit for everything
BearHttpsResponse_set_max_body_size(response, 1024);  // Too small for images!

// ✅ Correct - adjust limits based on content type
const char *content_type = BearHttpsResponse_get_header_value_by_key(response, "Content-Type");
if (strstr(content_type, "image/")) {
    BearHttpsResponse_set_max_body_size(response, 10 * 1024 * 1024);  // 10MB for images
} else {
    BearHttpsResponse_set_max_body_size(response, 1024 * 1024);       // 1MB for others
}
```

### ❌ Mistake 4: Forgetting Mobile Constraints

```c
// ❌ Wrong - desktop settings on mobile
BearHttpsResponse_set_body_read_props(response, 16384, 3.0);  // Too aggressive for mobile

// ✅ Correct - mobile-optimized settings
#ifdef MOBILE_BUILD
BearHttpsResponse_set_body_read_props(response, 512, 1.2);  // Conservative for mobile
#else
BearHttpsResponse_set_body_read_props(response, 4096, 2.0);  // More aggressive for desktop
#endif
```

---

## 🎯 Quick Reference Guide

### 📏 Setting Limits
```c
// Body size limit (bytes)
BearHttpsResponse_set_max_body_size(response, 1024 * 1024);  // 1MB

// Read configuration (chunk_size, growth_factor)
BearHttpsResponse_set_body_read_props(response, 2048, 1.5);

// Header configuration (chunk_size, growth_factor) 
BearHttpsRequest_set_chunk_header_read_props(request, 1024, 1.3);
```

### 📊 Recommended Configurations

| 🎯 **Platform** | 📊 **Read Size** | 📈 **Growth** | 📏 **Max Size** |
|----------------|------------------|---------------|----------------|
| **IoT/Embedded** | 64-256 bytes | 1.1-1.2x | 4KB-64KB |
| **Mobile** | 512-1024 bytes | 1.2-1.5x | 256KB-1MB |
| **Desktop** | 2048-4096 bytes | 1.5-2.0x | 10MB-100MB |
| **Server** | 4096-8192 bytes | 2.0-2.5x | 100MB+ |

### ⚡ Performance vs Memory Trade-offs

```c
// 💾 Memory-optimized (slow but safe)
BearHttpsResponse_set_body_read_props(response, 512, 1.2);

// ⚖️ Balanced (good for most apps)  
BearHttpsResponse_set_body_read_props(response, 2048, 1.5);

// 🚀 Speed-optimized (fast but uses more memory)
BearHttpsResponse_set_body_read_props(response, 8192, 2.0);
```

---

## 🚀 What's Next?

Excellent! You now know how to optimize BearHttpsClient's memory usage for any scenario. Next steps:

1. 🔧 [Understanding ownership system](/docs/ownership_system.md)
2. 🌐 [WebAssembly deployment](/docs/web_asm.md) 
3. 📦 [Dependencies deep dive](/docs/dependencies.md)
4. 🎯 [Try advanced examples](/examples/)

---

<div align="center">

**🎉 You're now a memory optimization expert!**

[🔧 Next: Ownership System](/docs/ownership_system.md) | [🏠 Back to Main](/README.md)

</div>
