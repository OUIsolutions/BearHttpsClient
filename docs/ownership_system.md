<div align="center">

# 🔧 Ownership System

**Master memory ownership for rock-solid applications** 💎

*Never worry about memory leaks or crashes again!*

</div>

---

## 🎯 What You'll Learn

By the end of this guide, you'll know how to:

| 🎯 **Skill** | 🌟 **Difficulty** | ⏱️ **Time** |
|-------------|-------------------|-------------|
| 🧠 Understand memory ownership | 🟡 Intermediate | 5 minutes |
| 📚 Use REFERENCE ownership | 🟢 Beginner | 3 minutes |
| 📋 Use COPY ownership | 🟡 Intermediate | 5 minutes |
| 🎯 Use GET_OWNERSHIP | 🔴 Advanced | 7 minutes |
| 🛡️ Avoid memory bugs | 🔴 Advanced | 5 minutes |

---

## 💡 What is the Ownership System?

Think of ownership like **borrowing vs owning a book**:

| 🎭 **Ownership Type** | 📚 **Book Analogy** | 💾 **Memory Reality** |
|---------------------|---------------------|----------------------|
| **REFERENCE** | 📖 "I'll just look at your book" | BearHttpsClient just points to your data |
| **COPY** | 📄 "Let me photocopy this" | BearHttpsClient makes its own copy |
| **GET_OWNERSHIP** | 🎁 "You can give me the book" | BearHttpsClient takes full control |

The ownership system **prevents memory bugs** and gives you **complete control** over how your data is managed.

---

## 📚 REFERENCE: "Just Look, Don't Touch"

Use **REFERENCE** when your data is **permanent** and **won't change**:

### 🔰 Basic REFERENCE Example

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // 📚 Static data that lives forever
    unsigned char static_message[] = "Hello from static memory!";
    
    // 🔗 Just reference it - don't copy
    BearHttpsRequest_send_any_with_ownership_control(
        request, 
        static_message, 
        sizeof(static_message) - 1,  // Don't count null terminator
        BEARSSL_HTTPS_REFERENCE      // 👈 Just point to it!
    );
    
    printf("📚 Using REFERENCE ownership\n");
    printf("💡 BearHttpsClient points to our static data\n");
    printf("✅ No copying = faster and less memory used\n");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("🎉 Request successful with REFERENCE data!\n");
        // 📖 You can still use static_message here
        printf("📄 Original data still accessible: %s\n", static_message);
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🎯 Advanced REFERENCE Usage

```c
#include "BearHttpsClientOne.c"

// 📚 Global configuration data (perfect for REFERENCE)
const char* API_ENDPOINTS[] = {
    "https://api.example.com/users",
    "https://api.example.com/posts", 
    "https://api.example.com/comments"
};

const char* STATIC_HEADERS[][2] = {
    {"User-Agent", "MyApp/1.0"},
    {"Accept", "application/json"},
    {"Content-Type", "application/json"}
};

void send_to_endpoint(int endpoint_index, const char* json_data) {
    BearHttpsRequest *request = newBearHttpsRequest(API_ENDPOINTS[endpoint_index]);
    BearHttpsRequest_set_method(request, "POST");
    
    // 📚 Add headers using REFERENCE (they're static!)
    for (int i = 0; i < 3; i++) {
        BearHttpsRequest_add_header_with_ownership_config(
            request,
            STATIC_HEADERS[i][0], BEARSSL_HTTPS_REFERENCE,  // Header name
            STATIC_HEADERS[i][1], BEARSSL_HTTPS_REFERENCE   // Header value
        );
    }
    
    // 🔗 Send JSON data as reference (if it's static)
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)json_data,
        strlen(json_data),
        BEARSSL_HTTPS_REFERENCE
    );
    
    printf("📚 Sending to %s with all REFERENCE data\n", API_ENDPOINTS[endpoint_index]);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("✅ Request successful!\n");
        printf("💡 No memory was copied - super efficient!\n");
    } else {
        printf("❌ Request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
}

int main() {
    // 📊 Static JSON data (perfect for REFERENCE)
    const char* user_data = "{\"name\":\"Alice\",\"role\":\"admin\"}";
    const char* post_data = "{\"title\":\"My Post\",\"content\":\"Hello world\"}";
    
    send_to_endpoint(0, user_data);  // Send to users endpoint
    send_to_endpoint(1, post_data);  // Send to posts endpoint
    
    return 0;
}
```

### ✅ When to Use REFERENCE

| ✅ **Perfect For** | ❌ **Don't Use For** |
|-------------------|---------------------|
| String literals: `"hello"` | Variables that might change |
| Global constants | Local variables that go out of scope |
| Configuration data | Data from malloc() that gets freed |
| Static arrays | Temporary buffers |
| Compile-time known data | User input that varies |

---

## 📋 COPY: "Let Me Make My Own Version"

Use **COPY** when you want to **keep your original data** but let BearHttpsClient have its own copy:

### 🔰 Basic COPY Example

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // 📋 Dynamic data that we want to keep
    char *user_input = strdup("User typed this message");
    
    printf("📋 Original data before sending: %s\n", user_input);
    
    // 📋 Copy the data to the request
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)user_input,
        strlen(user_input),
        BEARSSL_HTTPS_COPY  // 👈 Make a copy for BearHttpsClient
    );
    
    printf("📋 Using COPY ownership\n");
    printf("💡 BearHttpsClient has its own copy now\n");
    
    // ✅ We can still modify our original data
    strcat(user_input, " - modified after copy!");
    printf("📝 Modified original: %s\n", user_input);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("✅ Request sent successfully!\n");
        printf("💡 Our modification didn't affect the request\n");
    }
    
    // 🧹 We're responsible for freeing our original data
    free(user_input);
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### 🎯 Advanced COPY Usage

```c
#include "BearHttpsClientOne.c"
#include <time.h>

char* create_timestamped_message(const char* base_message) {
    time_t now = time(NULL);
    char* message = malloc(256);
    
    snprintf(message, 256, "{\"message\":\"%s\",\"timestamp\":%ld}", 
             base_message, now);
    
    return message;
}

void send_multiple_messages(const char* base_messages[], int count) {
    printf("📋 Sending %d timestamped messages using COPY\n", count);
    
    for (int i = 0; i < count; i++) {
        // 📋 Create dynamic message
        char* timestamped = create_timestamped_message(base_messages[i]);
        
        BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
        BearHttpsRequest_set_method(request, "POST");
        BearHttpsRequest_add_header(request, "Content-Type", "application/json");
        
        printf("📝 Message %d: %s\n", i + 1, timestamped);
        
        // 📋 Copy the dynamic message
        BearHttpsRequest_send_any_with_ownership_control(
            request,
            (unsigned char*)timestamped,
            strlen(timestamped),
            BEARSSL_HTTPS_COPY  // 👈 BearHttpsClient makes its own copy
        );
        
        // 🚀 Send the request
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        
        if (!BearHttpsResponse_error(response)) {
            printf("✅ Message %d sent successfully!\n", i + 1);
        } else {
            printf("❌ Message %d failed: %s\n", i + 1, BearHttpsResponse_get_error_msg(response));
        }
        
        // 🧹 We free our original data (BearHttpsClient has its copy)
        free(timestamped);
        
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        
        // ⏳ Small delay between requests
        sleep(1);
    }
}

int main() {
    const char* messages[] = {
        "System startup complete",
        "User logged in",
        "Data backup finished",
        "Daily report generated"
    };
    
    send_multiple_messages(messages, 4);
    
    printf("🎉 All messages sent! Each had its own timestamp.\n");
    printf("💡 COPY ownership allowed us to safely free our buffers.\n");
    
    return 0;
}
```

### 🎯 COPY with Headers

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/headers");
    
    // 📋 Dynamic header values that might change
    char* session_id = strdup("sess_123456789");
    char* user_agent = malloc(100);
    snprintf(user_agent, 100, "MyApp/1.0 (Build %d)", rand() % 1000);
    
    printf("📋 Adding headers with COPY ownership:\n");
    printf("🆔 Session ID: %s\n", session_id);
    printf("🔧 User Agent: %s\n", user_agent);
    
    // 📋 Copy both header name and value (if needed)
    BearHttpsRequest_add_header_with_ownership_config(
        request,
        "X-Session-ID", BEARSSL_HTTPS_REFERENCE,  // Header name is static
        session_id,     BEARSSL_HTTPS_COPY        // Header value is dynamic
    );
    
    BearHttpsRequest_add_header_with_ownership_config(
        request,
        "User-Agent",   BEARSSL_HTTPS_REFERENCE,  // Header name is static
        user_agent,     BEARSSL_HTTPS_COPY        // Header value is dynamic
    );
    
    // ✅ Now we can safely modify our original variables
    strcpy(session_id, "MODIFIED");
    strcpy(user_agent, "CHANGED");
    
    printf("📝 After modification (won't affect request):\n");
    printf("🆔 Session ID: %s\n", session_id);
    printf("🔧 User Agent: %s\n", user_agent);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("✅ Headers sent with original values (unaffected by changes)\n");
        const char* body = BearHttpsResponse_read_body_str(response);
        if (!BearHttpsResponse_error(response)) {
            printf("📖 Response: %s\n", body);
        }
    }
    
    // 🧹 Clean up our original data
    free(session_id);
    free(user_agent);
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### ✅ When to Use COPY

| ✅ **Perfect For** | ❌ **Don't Use For** |
|-------------------|---------------------|
| User input that changes | Data you'll never use again |
| Temporary buffers | Very large data (waste of memory) |
| Data from functions | Static/constant data |
| Values that might be modified | Data you want to transfer ownership |
| Safety-critical applications | Performance-critical large transfers |

---

## 🎯 GET_OWNERSHIP: "You Take It, I'm Done With It"

Use **GET_OWNERSHIP** when you're **done with your data** and want BearHttpsClient to take full control:

### 🔰 Basic GET_OWNERSHIP Example

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // 🎯 Create data that we'll transfer ownership of
    char *message = strdup("This message will be owned by BearHttpsClient");
    
    printf("🎯 Before transfer - our data: %s\n", message);
    printf("📍 Data address: %p\n", (void*)message);
    
    // 🎯 Transfer ownership to BearHttpsClient
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)message,
        strlen(message),
        BEARSSL_HTTPS_GET_OWNERSHIP  // 👈 BearHttpsClient now owns this!
    );
    
    printf("🎯 Using GET_OWNERSHIP\n");
    printf("⚠️ WARNING: Don't touch 'message' anymore!\n");
    printf("💡 BearHttpsClient will free it when the request is destroyed\n");
    
    // ❌ DON'T DO THIS: free(message);  // BearHttpsClient owns it now!
    // ❌ DON'T DO THIS: strcpy(message, "new text");  // Could crash!
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("✅ Request sent! BearHttpsClient managed the memory perfectly\n");
    }
    
    // 🧹 When we free the request, it will automatically free the message
    BearHttpsRequest_free(request);  // This frees 'message' too!
    BearHttpsResponse_free(response);
    
    printf("🎉 All done! BearHttpsClient handled the cleanup\n");
    return 0;
}
```

### ⚠️ GET_OWNERSHIP Safety Demo

```c
#include "BearHttpsClientOne.c"

void demonstrate_ownership_transfer() {
    printf("🧪 Demonstrating GET_OWNERSHIP safety\n");
    printf("=====================================\n");
    
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // 🎯 Create data for transfer
    char *data = malloc(100);
    strcpy(data, "Data created at address");
    
    printf("📍 Data created at: %p\n", (void*)data);
    printf("📄 Data content: %s\n", data);
    
    // 🎯 Transfer ownership
    printf("\n🔄 Transferring ownership to BearHttpsClient...\n");
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)data,
        strlen(data),
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    printf("✅ Ownership transferred!\n");
    printf("⚠️ The 'data' pointer is now DANGEROUS to use!\n");
    printf("💀 Accessing it could cause crashes or corruption\n");
    
    // 🚀 Use the request (BearHttpsClient manages the data)
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("✅ Request successful - BearHttpsClient handled everything!\n");
    }
    
    printf("\n🧹 Cleaning up request...\n");
    BearHttpsRequest_free(request);  // This also frees the transferred data
    printf("✅ Request freed - transferred data was automatically cleaned up!\n");
    
    BearHttpsResponse_free(response);
}

int main() {
    demonstrate_ownership_transfer();
    
    printf("\n🎓 Key Lessons:\n");
    printf("   1. After GET_OWNERSHIP, don't touch the original pointer\n");
    printf("   2. BearHttpsClient will free the data when cleaning up\n");
    printf("   3. This is perfect for 'fire and forget' scenarios\n");
    
    return 0;
}
```

### 🎯 Advanced GET_OWNERSHIP Pattern

```c
#include "BearHttpsClientOne.c"

// 🏭 Factory function that creates data for transfer
char* create_json_payload(const char* action, int user_id, const char* details) {
    char* json = malloc(512);
    snprintf(json, 512, 
        "{"
        "\"action\":\"%s\","
        "\"user_id\":%d,"
        "\"details\":\"%s\","
        "\"timestamp\":%ld"
        "}",
        action, user_id, details, time(NULL)
    );
    return json;  // Caller will transfer ownership
}

int send_audit_log(const char* action, int user_id, const char* details) {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    BearHttpsRequest_add_header(request, "Content-Type", "application/json");
    
    // 🏭 Create the payload
    char* json_payload = create_json_payload(action, user_id, details);
    
    printf("📊 Created audit log: %s\n", json_payload);
    printf("📍 Payload at address: %p\n", (void*)json_payload);
    
    // 🎯 Transfer ownership immediately
    printf("🔄 Transferring to BearHttpsClient...\n");
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)json_payload,
        strlen(json_payload),
        BEARSSL_HTTPS_GET_OWNERSHIP  // 👈 BearHttpsClient owns it now
    );
    
    // 🚨 From this point, json_payload is DANGEROUS to use!
    printf("⚠️ json_payload is now off-limits!\n");
    
    // 🚀 Send the request
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    int success = 0;
    if (!BearHttpsResponse_error(response)) {
        int status = BearHttpsResponse_get_status_code(response);
        printf("✅ Audit log sent! Status: %d\n", status);
        success = 1;
    } else {
        printf("❌ Failed to send audit log: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    // 🧹 Cleanup (this will free the transferred json_payload)
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    
    return success;
}

int main() {
    printf("🔍 Advanced GET_OWNERSHIP pattern demo\n");
    printf("======================================\n");
    
    // 📋 Send multiple audit logs
    const char* actions[] = {"login", "data_access", "logout"};
    const char* details[] = {
        "User logged in from IP 192.168.1.100",
        "Accessed customer database table",
        "User session ended normally"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\n🔍 Audit log %d:\n", i + 1);
        
        int result = send_audit_log(actions[i], 12345, details[i]);
        
        if (result) {
            printf("✅ Audit log %d sent successfully\n", i + 1);
        } else {
            printf("❌ Audit log %d failed\n", i + 1);
        }
        
        printf("💡 Memory was automatically managed by GET_OWNERSHIP\n");
    }
    
    printf("\n🎉 All audit logs processed!\n");
    printf("💎 Zero memory leaks thanks to GET_OWNERSHIP\n");
    
    return 0;
}
```

### ✅ When to Use GET_OWNERSHIP

| ✅ **Perfect For** | ❌ **Don't Use For** |
|-------------------|---------------------|
| Data you'll never need again | Data you might need later |
| One-time message sending | Data used in multiple places |
| Fire-and-forget scenarios | Debugging (harder to track) |
| Clean memory management | Small static data |
| Large data transfers | Data with complex ownership |

---

## 🛡️ Avoiding Memory Bugs: Best Practices

### 🔰 The Golden Rules

```c
#include "BearHttpsClientOne.c"

void demonstrate_ownership_rules() {
    printf("🛡️ Memory Ownership Golden Rules\n");
    printf("===============================\n");
    
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // 🥇 Rule 1: Static data = REFERENCE
    printf("\n🥇 Rule 1: Static data = REFERENCE\n");
    const char* static_msg = "I'm a string literal";
    BearHttpsRequest_send_any_with_ownership_control(
        request, (unsigned char*)static_msg, strlen(static_msg), 
        BEARSSL_HTTPS_REFERENCE  // ✅ Perfect for static data
    );
    printf("✅ Static string used with REFERENCE\n");
    
    // 🥈 Rule 2: Keep using data = COPY
    printf("\n🥈 Rule 2: Keep using your data = COPY\n");
    char* keep_data = strdup("I want to keep this");
    BearHttpsRequest_send_any_with_ownership_control(
        request, (unsigned char*)keep_data, strlen(keep_data),
        BEARSSL_HTTPS_COPY  // ✅ We can still use keep_data
    );
    printf("✅ Data copied, original still usable: %s\n", keep_data);
    free(keep_data);  // ✅ We free our copy
    
    // 🥉 Rule 3: Done with data = GET_OWNERSHIP
    printf("\n🥉 Rule 3: Done with your data = GET_OWNERSHIP\n");
    char* transfer_data = strdup("Take this away from me");
    printf("💡 Before transfer: %s\n", transfer_data);
    BearHttpsRequest_send_any_with_ownership_control(
        request, (unsigned char*)transfer_data, strlen(transfer_data),
        BEARSSL_HTTPS_GET_OWNERSHIP  // ✅ BearHttpsClient takes it
    );
    printf("✅ Ownership transferred - don't touch transfer_data anymore!\n");
    // ❌ Don't do: free(transfer_data);  // BearHttpsClient will handle it
    
    BearHttpsRequest_free(request);  // This will free transfer_data too
    printf("🧹 Request freed - all transferred data cleaned up!\n");
}

int main() {
    demonstrate_ownership_rules();
    
    printf("\n📚 Summary:\n");
    printf("   📌 REFERENCE: For data that won't change or disappear\n");
    printf("   📄 COPY: For data you want to keep using\n");
    printf("   🎯 GET_OWNERSHIP: For data you're done with\n");
    
    return 0;
}
```

### 🚨 Common Ownership Mistakes

```c
#include "BearHttpsClientOne.c"

void demonstrate_common_mistakes() {
    printf("🚨 Common Ownership Mistakes (DON'T DO THESE!)\n");
    printf("===============================================\n");
    
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // ❌ Mistake 1: Using REFERENCE with temporary data
    printf("\n❌ Mistake 1: REFERENCE with temporary data\n");
    {
        char temp_buffer[100];
        strcpy(temp_buffer, "This buffer will disappear!");
        
        // ❌ WRONG: temp_buffer will go out of scope!
        printf("💀 DON'T DO: REFERENCE with local buffer\n");
        printf("⚠️ This could crash when buffer goes out of scope\n");
        
        // ✅ CORRECT: Use COPY instead
        BearHttpsRequest_send_any_with_ownership_control(
            request, (unsigned char*)temp_buffer, strlen(temp_buffer),
            BEARSSL_HTTPS_COPY  // ✅ Safe - makes a copy
        );
        printf("✅ Used COPY instead - safe!\n");
    } // temp_buffer goes out of scope here, but we're safe with COPY
    
    // ❌ Mistake 2: Freeing data after GET_OWNERSHIP
    printf("\n❌ Mistake 2: Freeing after GET_OWNERSHIP\n");
    char *dangerous_data = strdup("Don't free me!");
    
    BearHttpsRequest_send_any_with_ownership_control(
        request, (unsigned char*)dangerous_data, strlen(dangerous_data),
        BEARSSL_HTTPS_GET_OWNERSHIP  // BearHttpsClient now owns it
    );
    
    // ❌ WRONG: double-free will crash!
    printf("💀 DON'T DO: free(data) after GET_OWNERSHIP\n");
    printf("⚠️ This would cause a double-free crash\n");
    // free(dangerous_data);  // DON'T DO THIS!
    
    printf("✅ Letting BearHttpsClient handle the free\n");
    
    // ❌ Mistake 3: Using wrong ownership for large static data
    printf("\n❌ Mistake 3: COPY with large static data\n");
    static const char large_static_data[10000] = {0}; // Imagine this is big
    
    printf("💀 DON'T DO: COPY with large static data\n");
    printf("⚠️ Wastes memory by duplicating static data\n");
    
    // ✅ CORRECT: Use REFERENCE for static data
    printf("✅ Use REFERENCE for static data instead\n");
    
    BearHttpsRequest_free(request);  // This frees dangerous_data safely
    printf("🧹 All cleaned up safely!\n");
}

int main() {
    demonstrate_common_mistakes();
    
    printf("\n🎓 Key Takeaways:\n");
    printf("   1. Never REFERENCE temporary/local buffers\n");
    printf("   2. Never free() after GET_OWNERSHIP\n");
    printf("   3. Don't COPY large static data unnecessarily\n");
    printf("   4. When in doubt, use COPY (safer but slower)\n");
    
    return 0;
}
```

### 🧪 Ownership Testing and Debugging

```c
#include "BearHttpsClientOne.c"

typedef struct {
    char* data;
    size_t size;
    const char* description;
    int ownership_type;
} MemoryTestCase;

void test_ownership_patterns() {
    printf("🧪 Testing Different Ownership Patterns\n");
    printf("=======================================\n");
    
    MemoryTestCase test_cases[] = {
        {
            .data = "Static string literal",
            .size = 0,  // Will calculate
            .description = "String literal",
            .ownership_type = BEARSSL_HTTPS_REFERENCE
        },
        {
            .data = NULL,  // Will allocate
            .size = 0,     // Will set
            .description = "Dynamic data for COPY",
            .ownership_type = BEARSSL_HTTPS_COPY
        },
        {
            .data = NULL,  // Will allocate  
            .size = 0,     // Will set
            .description = "Dynamic data for GET_OWNERSHIP",
            .ownership_type = BEARSSL_HTTPS_GET_OWNERSHIP
        }
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\n🧪 Test Case %d: %s\n", i + 1, test_cases[i].description);
        
        BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
        BearHttpsRequest_set_method(request, "POST");
        
        // 🔧 Prepare test data
        if (test_cases[i].data && test_cases[i].ownership_type == BEARSSL_HTTPS_REFERENCE) {
            // Static data
            test_cases[i].size = strlen(test_cases[i].data);
            printf("📌 Using static data: %s\n", test_cases[i].data);
        } else {
            // Dynamic data
            test_cases[i].data = malloc(100);
            snprintf(test_cases[i].data, 100, "Dynamic data for test %d", i + 1);
            test_cases[i].size = strlen(test_cases[i].data);
            printf("🏗️ Created dynamic data: %s\n", test_cases[i].data);
        }
        
        // 📊 Show ownership type
        const char* ownership_names[] = {"REFERENCE", "COPY", "GET_OWNERSHIP"};
        printf("🔧 Ownership: %s\n", ownership_names[test_cases[i].ownership_type]);
        
        // 🚀 Apply ownership
        BearHttpsRequest_send_any_with_ownership_control(
            request,
            (unsigned char*)test_cases[i].data,
            test_cases[i].size,
            test_cases[i].ownership_type
        );
        
        // 📋 Ownership-specific behavior
        switch (test_cases[i].ownership_type) {
            case BEARSSL_HTTPS_REFERENCE:
                printf("✅ REFERENCE: BearHttpsClient just points to the data\n");
                break;
                
            case BEARSSL_HTTPS_COPY:
                printf("✅ COPY: BearHttpsClient made its own copy\n");
                printf("📝 We can still access our data: %s\n", test_cases[i].data);
                break;
                
            case BEARSSL_HTTPS_GET_OWNERSHIP:
                printf("✅ GET_OWNERSHIP: BearHttpsClient now owns the data\n");
                printf("⚠️ Our pointer is now dangerous to use!\n");
                break;
        }
        
        // 🧹 Cleanup based on ownership
        if (test_cases[i].ownership_type == BEARSSL_HTTPS_COPY && 
            test_cases[i].data && 
            strcmp(test_cases[i].data, "Static string literal") != 0) {
            printf("🧹 Freeing our copy (COPY ownership)\n");
            free(test_cases[i].data);
        } else if (test_cases[i].ownership_type == BEARSSL_HTTPS_GET_OWNERSHIP) {
            printf("🧹 Not freeing - BearHttpsClient will handle it\n");
        }
        
        BearHttpsRequest_free(request);
        printf("✅ Test case %d completed successfully\n", i + 1);
    }
}

int main() {
    test_ownership_patterns();
    
    printf("\n🎯 All ownership patterns tested successfully!\n");
    printf("💎 Each ownership type has its perfect use case\n");
    
    return 0;
}
```

---

## 🎓 Real-World Ownership Examples

### 🌐 Web API Client with Mixed Ownership

```c
#include "BearHttpsClientOne.c"

// 📚 Static configuration (perfect for REFERENCE)
static const char* API_BASE_URL = "https://api.example.com";
static const char* API_VERSION = "v1";

typedef struct {
    char* auth_token;      // Dynamic - will use COPY
    char* session_id;      // Dynamic - will use GET_OWNERSHIP  
    int user_id;           // Will embed in dynamic JSON
} UserSession;

char* create_auth_header(const char* token) {
    char* header = malloc(256);
    snprintf(header, 256, "Bearer %s", token);
    return header;  // Caller will manage ownership
}

char* create_json_request(int user_id, const char* action, const char* data) {
    char* json = malloc(512);
    snprintf(json, 512,
        "{"
        "\"user_id\":%d,"
        "\"action\":\"%s\","
        "\"data\":\"%s\","
        "\"timestamp\":%ld"
        "}",
        user_id, action, data, time(NULL)
    );
    return json;
}

int send_api_request(UserSession* session, const char* endpoint, const char* action, const char* data) {
    // 🔧 Build URL
    char url[256];
    snprintf(url, sizeof(url), "%s/%s/%s", API_BASE_URL, API_VERSION, endpoint);
    
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsRequest_set_method(request, "POST");
    
    // 📚 Static headers (REFERENCE)
    BearHttpsRequest_add_header_with_ownership_config(
        request,
        "Content-Type", BEARSSL_HTTPS_REFERENCE,
        "application/json", BEARSSL_HTTPS_REFERENCE
    );
    
    // 📋 Dynamic auth header (COPY - we'll keep using the token)
    char* auth_header = create_auth_header(session->auth_token);
    BearHttpsRequest_add_header_with_ownership_config(
        request,
        "Authorization", BEARSSL_HTTPS_REFERENCE,
        auth_header, BEARSSL_HTTPS_COPY  // Copy because we'll free it
    );
    
    // 🎯 Dynamic JSON payload (GET_OWNERSHIP - we're done with it)
    char* json_payload = create_json_request(session->user_id, action, data);
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)json_payload,
        strlen(json_payload),
        BEARSSL_HTTPS_GET_OWNERSHIP  // BearHttpsClient takes it
    );
    
    printf("🚀 Sending API request to: %s\n", url);
    printf("🔑 Auth: %s\n", auth_header);
    printf("📊 JSON: %s\n", json_payload);  // Still safe to read
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    int success = 0;
    if (!BearHttpsResponse_error(response)) {
        int status = BearHttpsResponse_get_status_code(response);
        if (status == 200) {
            printf("✅ API request successful!\n");
            success = 1;
        } else {
            printf("⚠️ API returned status %d\n", status);
        }
    } else {
        printf("❌ API request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    // 🧹 Clean up what we own
    free(auth_header);  // We created this with COPY
    // Don't free json_payload - BearHttpsClient owns it now
    
    BearHttpsRequest_free(request);   // This frees json_payload
    BearHttpsResponse_free(response);
    
    return success;
}

int main() {
    printf("🌐 Real-world API client with mixed ownership\n");
    printf("=============================================\n");
    
    // 🔧 Set up user session
    UserSession session = {
        .auth_token = strdup("abc123def456"),
        .session_id = strdup("sess_789xyz"),
        .user_id = 12345
    };
    
    // 📊 Make several API calls
    send_api_request(&session, "users", "update_profile", "name=John");
    send_api_request(&session, "logs", "activity", "page_viewed");
    send_api_request(&session, "settings", "preferences", "theme=dark");
    
    printf("\n💡 Ownership Summary:\n");
    printf("   📚 Static URLs/headers: REFERENCE (no copying)\n");
    printf("   📋 Auth tokens: COPY (we keep using them)\n");
    printf("   🎯 JSON payloads: GET_OWNERSHIP (fire and forget)\n");
    
    // 🧹 Clean up session data we own
    free(session.auth_token);
    free(session.session_id);
    
    printf("🎉 All API calls completed with perfect memory management!\n");
    return 0;
}
```

### 📊 Data Pipeline with Smart Ownership

```c
#include "BearHttpsClientOne.c"

typedef enum {
    DATA_SMALL_JSON,   // Use COPY
    DATA_LARGE_BUFFER, // Use GET_OWNERSHIP  
    DATA_CONFIG        // Use REFERENCE
} DataType;

typedef struct {
    unsigned char* data;
    size_t size;
    DataType type;
    const char* description;
} DataPacket;

int send_data_packet(DataPacket* packet, const char* destination) {
    BearHttpsRequest *request = newBearHttpsRequest(destination);
    BearHttpsRequest_set_method(request, "POST");
    
    // 🎯 Choose ownership based on data type
    int ownership;
    const char* ownership_name;
    
    switch (packet->type) {
        case DATA_SMALL_JSON:
            ownership = BEARSSL_HTTPS_COPY;
            ownership_name = "COPY";
            printf("📋 Small JSON data - using COPY for safety\n");
            break;
            
        case DATA_LARGE_BUFFER:
            ownership = BEARSSL_HTTPS_GET_OWNERSHIP;
            ownership_name = "GET_OWNERSHIP";
            printf("🎯 Large buffer - using GET_OWNERSHIP for efficiency\n");
            break;
            
        case DATA_CONFIG:
            ownership = BEARSSL_HTTPS_REFERENCE;
            ownership_name = "REFERENCE";
            printf("📚 Configuration data - using REFERENCE (static)\n");
            break;
    }
    
    printf("📦 Sending %s (%zu bytes) with %s ownership\n", 
           packet->description, packet->size, ownership_name);
    
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        packet->data,
        packet->size,
        ownership
    );
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    int success = 0;
    if (!BearHttpsResponse_error(response)) {
        printf("✅ Data packet sent successfully!\n");
        success = 1;
    } else {
        printf("❌ Failed to send: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    // 🧹 Cleanup based on what we still own
    if (packet->type == DATA_SMALL_JSON) {
        // We used COPY, so we still own the data
        if (packet->data) {
            free(packet->data);
            printf("🧹 Freed our copy of small JSON data\n");
        }
    } else if (packet->type == DATA_LARGE_BUFFER) {
        // We used GET_OWNERSHIP, so don't free
        printf("🎯 Large buffer ownership transferred - not freeing\n");
    }
    // For DATA_CONFIG, it's static so nothing to free
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    
    return success;
}

int main() {
    printf("📊 Smart Data Pipeline with Ownership Selection\n");
    printf("===============================================\n");
    
    // 📚 Static configuration (REFERENCE)
    static const char config_data[] = "{\"version\":\"1.0\",\"mode\":\"production\"}";
    DataPacket config_packet = {
        .data = (unsigned char*)config_data,
        .size = sizeof(config_data) - 1,
        .type = DATA_CONFIG,
        .description = "Configuration data"
    };
    
    // 📋 Small dynamic JSON (COPY)
    char* small_json = malloc(200);
    snprintf(small_json, 200, "{\"message\":\"Small data packet\",\"id\":%d}", rand());
    DataPacket small_packet = {
        .data = (unsigned char*)small_json,
        .size = strlen(small_json),
        .type = DATA_SMALL_JSON,
        .description = "Small JSON packet"
    };
    
    // 🎯 Large buffer (GET_OWNERSHIP)
    size_t large_size = 10000;
    char* large_buffer = malloc(large_size);
    memset(large_buffer, 'X', large_size - 1);
    large_buffer[large_size - 1] = '\0';
    DataPacket large_packet = {
        .data = (unsigned char*)large_buffer,
        .size = large_size - 1,
        .type = DATA_LARGE_BUFFER,
        .description = "Large binary buffer"
    };
    
    // 🚀 Send all packets
    send_data_packet(&config_packet, "https://httpbin.org/post");
    send_data_packet(&small_packet, "https://httpbin.org/post");
    send_data_packet(&large_packet, "https://httpbin.org/post");
    
    printf("\n📊 Pipeline Summary:\n");
    printf("   📚 Config: REFERENCE - no memory overhead\n");
    printf("   📋 Small JSON: COPY - safe and predictable\n");
    printf("   🎯 Large buffer: GET_OWNERSHIP - efficient transfer\n");
    
    printf("\n🎉 Data pipeline completed with optimal memory usage!\n");
    return 0;
}
```

---

## 🎯 Quick Reference Guide

### 🔧 Ownership Functions

```c
// Basic data sending with ownership
BearHttpsRequest_send_any_with_ownership_control(request, data, size, ownership);

// Header with ownership control
BearHttpsRequest_add_header_with_ownership_config(request, key, key_ownership, value, value_ownership);

// URL with ownership control
BearHttpsRequest_set_url_with_ownership_config(request, url, ownership);
```

### 📊 Ownership Decision Matrix

| 🎯 **Data Type** | 📚 **REFERENCE** | 📋 **COPY** | 🎯 **GET_OWNERSHIP** |
|-----------------|------------------|-------------|---------------------|
| **String literals** | ✅ Perfect | ❌ Wasteful | ❌ Impossible |
| **Static arrays** | ✅ Perfect | ❌ Wasteful | ❌ Impossible |
| **Small dynamic data** | ❌ Unsafe | ✅ Perfect | ✅ Good |
| **Large dynamic data** | ❌ Unsafe | ❌ Wasteful | ✅ Perfect |
| **Temporary buffers** | ❌ Dangerous | ✅ Safe | ❌ Dangerous |
| **One-time use data** | ❌ Depends | ✅ Safe | ✅ Perfect |

### 🛡️ Safety Checklist

```c
// ✅ Safe REFERENCE usage
const char* static_data = "hello";  // Lives forever
BearHttpsRequest_send_any_with_ownership_control(req, static_data, 5, BEARSSL_HTTPS_REFERENCE);

// ✅ Safe COPY usage  
char* my_data = strdup("hello");
BearHttpsRequest_send_any_with_ownership_control(req, my_data, 5, BEARSSL_HTTPS_COPY);
free(my_data);  // ✅ I free my copy

// ✅ Safe GET_OWNERSHIP usage
char* transfer_data = strdup("hello");
BearHttpsRequest_send_any_with_ownership_control(req, transfer_data, 5, BEARSSL_HTTPS_GET_OWNERSHIP);
// ✅ Don't free - BearHttpsClient owns it now
```

---

## 🚀 What's Next?

Congratulations! You now understand BearHttpsClient's ownership system completely. Next steps:

1. 🌐 [WebAssembly deployment](/docs/web_asm.md)
2. 📦 [Dependencies deep dive](/docs/dependencies.md)  
3. 🎯 [Try advanced examples](/examples/)
4. 🏗️ [Build your own projects](/docs/build_and_install.md)

---

<div align="center">

**🎉 You're now a memory ownership master!**

[🌐 Next: WebAssembly](/docs/web_asm.md) | [🏠 Back to Main](/README.md)

</div>
