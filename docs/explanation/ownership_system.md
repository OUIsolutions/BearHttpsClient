# Ownership System

**Master memory ownership for reliable applications**

*Comprehensive guide to memory management in BearHttpsClient*

---

## Learning Objectives

By the end of this guide, you will understand:

| Skill | Difficulty | Time Required |
|-------|-----------|---------------|
| Memory ownership concepts | Intermediate | 5 minutes |
| REFERENCE ownership usage | Beginner | 3 minutes |
| COPY ownership implementation | Intermediate | 5 minutes |
| GET_OWNERSHIP pattern | Advanced | 7 minutes |
| Memory bug prevention | Advanced | 5 minutes |

---

## Introduction to the Ownership System

The ownership system in BearHttpsClient provides explicit control over memory management, similar to ownership models in modern systems programming languages. This system prevents common memory-related bugs while offering flexibility in how data is managed.

| Ownership Type | Description | Memory Behavior |
|----------------|-------------|-----------------|
| **REFERENCE** | Library points to existing data | No memory allocation or copying |
| **COPY** | Library creates its own copy | Allocates and copies data |
| **GET_OWNERSHIP** | Library takes control of data | Assumes responsibility for deallocation |

The ownership system prevents memory leaks, double-free errors, and use-after-free vulnerabilities while providing complete control over data lifecycle management.

---

## REFERENCE Ownership: Zero-Copy Data Access

Use **REFERENCE** ownership when your data has a lifetime that exceeds the HTTP request lifecycle and will not be modified or freed during the request processing.

### Basic REFERENCE Implementation

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // Static data with program lifetime
    unsigned char static_message[] = "Hello from static memory!";
    
    // Reference the static data without copying
    BearHttpsRequest_send_any_with_ownership_control(
        request, 
        static_message, 
        sizeof(static_message) - 1,  // Exclude null terminator
        BEARSSL_HTTPS_REFERENCE
    );
    
    printf("Using REFERENCE ownership\n");
    printf("BearHttpsClient references static data without copying\n");
    printf("No memory allocation overhead\n");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Request successful with REFERENCE data\n");
        printf("Original data remains accessible: %s\n", static_message);
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Advanced REFERENCE Usage with Global Configuration

```c
#include "BearHttpsClientOne.c"

// Global configuration data suitable for REFERENCE ownership
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
    
    // Add headers using REFERENCE ownership for static data
    for (int i = 0; i < 3; i++) {
        BearHttpsRequest_add_header_with_ownership_config(
            request,
            STATIC_HEADERS[i][0], BEARSSL_HTTPS_REFERENCE,  // Header name
            STATIC_HEADERS[i][1], BEARSSL_HTTPS_REFERENCE   // Header value
        );
    }
    
    // Send JSON data with REFERENCE ownership
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)json_data,
        strlen(json_data),
        BEARSSL_HTTPS_REFERENCE
    );
    
    printf("Sending to %s with REFERENCE data\n", API_ENDPOINTS[endpoint_index]);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Request successful with zero memory overhead\n");
    } else {
        printf("Request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
}

int main() {
    // Static JSON data suitable for REFERENCE ownership
    const char* user_data = "{\"name\":\"Alice\",\"role\":\"admin\"}";
    const char* post_data = "{\"title\":\"My Post\",\"content\":\"Hello world\"}";
    
    send_to_endpoint(0, user_data);  // Send to users endpoint
    send_to_endpoint(1, post_data);  // Send to posts endpoint
    
    return 0;
}
```

### REFERENCE Ownership Guidelines

| Appropriate Use Cases | Inappropriate Use Cases |
|-----------------------|------------------------|
| String literals | Variables that may change |
| Global constants | Local variables with limited scope |
| Configuration data | Data allocated with malloc() |
| Static arrays | Temporary buffers |
| Compile-time known data | Dynamic user input |

---

## COPY Ownership: Safe Data Duplication

Use **COPY** ownership when you need to retain access to your original data while ensuring the HTTP request has its own independent copy.

### Basic COPY Implementation

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // Dynamic data that we want to retain
    char *user_input = strdup("User typed this message");
    
    printf("Original data before sending: %s\n", user_input);
    
    // Copy the data to the request
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)user_input,
        strlen(user_input),
        BEARSSL_HTTPS_COPY
    );
    
    printf("Using COPY ownership\n");
    printf("BearHttpsClient has created its own copy\n");
    
    // Original data remains accessible and modifiable
    strcat(user_input, " - modified after copy");
    printf("Modified original: %s\n", user_input);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Request sent successfully\n");
        printf("Original data modification did not affect the request\n");
    }
    
    // Caller responsible for freeing original data
    free(user_input);
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Advanced COPY Usage with Dynamic Content

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
    printf("Sending %d timestamped messages using COPY ownership\n", count);
    
    for (int i = 0; i < count; i++) {
        // Create dynamic message
        char* timestamped = create_timestamped_message(base_messages[i]);
        
        BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
        BearHttpsRequest_set_method(request, "POST");
        BearHttpsRequest_add_header(request, "Content-Type", "application/json");
        
        printf("Message %d: %s\n", i + 1, timestamped);
        
        // Copy the dynamic message
        BearHttpsRequest_send_any_with_ownership_control(
            request,
            (unsigned char*)timestamped,
            strlen(timestamped),
            BEARSSL_HTTPS_COPY
        );
        
        // Send the request
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        
        if (!BearHttpsResponse_error(response)) {
            printf("Message %d sent successfully\n", i + 1);
        } else {
            printf("Message %d failed: %s\n", i + 1, BearHttpsResponse_get_error_msg(response));
        }
        
        // Free original data (BearHttpsClient has its own copy)
        free(timestamped);
        
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        
        // Delay between requests
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
    
    printf("All messages sent with individual timestamps\n");
    printf("COPY ownership allowed safe memory management\n");
    
    return 0;
}
```

### COPY Ownership with Headers

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/headers");
    
    // Dynamic header values
    char* session_id = strdup("sess_123456789");
    char* user_agent = malloc(100);
    snprintf(user_agent, 100, "MyApp/1.0 (Build %d)", rand() % 1000);
    
    printf("Adding headers with COPY ownership:\n");
    printf("Session ID: %s\n", session_id);
    printf("User Agent: %s\n", user_agent);
    
    // Copy header values while referencing static names
    BearHttpsRequest_add_header_with_ownership_config(
        request,
        "X-Session-ID", BEARSSL_HTTPS_REFERENCE,  // Static header name
        session_id,     BEARSSL_HTTPS_COPY        // Dynamic header value
    );
    
    BearHttpsRequest_add_header_with_ownership_config(
        request,
        "User-Agent",   BEARSSL_HTTPS_REFERENCE,  // Static header name
        user_agent,     BEARSSL_HTTPS_COPY        // Dynamic header value
    );
    
    // Modify original variables after copying
    strcpy(session_id, "MODIFIED");
    strcpy(user_agent, "CHANGED");
    
    printf("After modification (will not affect request):\n");
    printf("Session ID: %s\n", session_id);
    printf("User Agent: %s\n", user_agent);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Headers sent with original values (unaffected by changes)\n");
        const char* body = BearHttpsResponse_read_body_str(response);
        if (!BearHttpsResponse_error(response)) {
            printf("Response: %s\n", body);
        }
    }
    
    // Clean up original data
    free(session_id);
    free(user_agent);
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### COPY Ownership Guidelines

| Appropriate Use Cases | Inappropriate Use Cases |
|-----------------------|------------------------|
| User input that may change | Data you will never access again |
| Temporary buffers | Very large data sets (memory waste) |
| Data from functions | Static/constant data |
| Values that might be modified | Data for one-time transfer |
| Safety-critical applications | Performance-critical large transfers |

---

## GET_OWNERSHIP: Transfer Memory Control

Use **GET_OWNERSHIP** when you are finished with your data and want to transfer complete responsibility for its lifecycle to BearHttpsClient.

### Basic GET_OWNERSHIP Implementation

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // Create data for ownership transfer
    char *message = strdup("This message will be owned by BearHttpsClient");
    
    printf("Before transfer - data: %s\n", message);
    printf("Data address: %p\n", (void*)message);
    
    // Transfer ownership to BearHttpsClient
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)message,
        strlen(message),
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    printf("Using GET_OWNERSHIP\n");
    printf("WARNING: Do not access 'message' pointer after this point\n");
    printf("BearHttpsClient will free the memory when request is destroyed\n");
    
    // CRITICAL: Do not free or modify the transferred data
    // free(message);  // This would cause a double-free error
    // strcpy(message, "new text");  // This could cause memory corruption
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Request sent successfully with transferred data\n");
    }
    
    // When request is freed, it will automatically free the transferred message
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    
    printf("All memory automatically cleaned up by BearHttpsClient\n");
    return 0;
}
```

### GET_OWNERSHIP Safety Demonstration

```c
#include "BearHttpsClientOne.c"

void demonstrate_ownership_transfer() {
    printf("Demonstrating GET_OWNERSHIP safety\n");
    printf("===================================\n");
    
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // Create data for transfer
    char *data = malloc(100);
    strcpy(data, "Data created for ownership transfer");
    
    printf("Data created at address: %p\n", (void*)data);
    printf("Data content: %s\n", data);
    
    // Transfer ownership
    printf("\nTransferring ownership to BearHttpsClient...\n");
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)data,
        strlen(data),
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    printf("Ownership transferred successfully\n");
    printf("WARNING: The 'data' pointer is now unsafe to use\n");
    printf("Accessing it could cause crashes or memory corruption\n");
    
    // Use the request (BearHttpsClient manages the data)
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Request successful - BearHttpsClient handled memory management\n");
    }
    
    printf("\nCleaning up request...\n");
    BearHttpsRequest_free(request);  // This also frees the transferred data
    printf("Request freed - transferred data was automatically cleaned up\n");
    
    BearHttpsResponse_free(response);
}

int main() {
    demonstrate_ownership_transfer();
    
    printf("\nKey principles:\n");
    printf("1. After GET_OWNERSHIP, do not access the original pointer\n");
    printf("2. BearHttpsClient will free the data during cleanup\n");
    printf("3. This pattern is ideal for one-time data transfers\n");
    
    return 0;
}
```

### Advanced GET_OWNERSHIP Pattern

```c
#include "BearHttpsClientOne.c"

// Factory function that creates data for ownership transfer
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
    
    // Create the payload
    char* json_payload = create_json_payload(action, user_id, details);
    
    printf("Created audit log: %s\n", json_payload);
    printf("Payload at address: %p\n", (void*)json_payload);
    
    // Transfer ownership immediately
    printf("Transferring ownership to BearHttpsClient...\n");
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)json_payload,
        strlen(json_payload),
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    // CRITICAL: json_payload is now unsafe to use
    printf("WARNING: json_payload pointer is now unsafe to access\n");
    
    // Send the request
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    int success = 0;
    if (!BearHttpsResponse_error(response)) {
        int status = BearHttpsResponse_get_status_code(response);
        printf("Audit log sent successfully. Status: %d\n", status);
        success = 1;
    } else {
        printf("Failed to send audit log: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    // Cleanup (this will free the transferred json_payload)
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    
    return success;
}

int main() {
    printf("Advanced GET_OWNERSHIP pattern demonstration\n");
    printf("==========================================\n");
    
    // Send multiple audit logs
    const char* actions[] = {"login", "data_access", "logout"};
    const char* details[] = {
        "User logged in from IP 192.168.1.100",
        "Accessed customer database table",
        "User session ended normally"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\nAudit log %d:\n", i + 1);
        
        int result = send_audit_log(actions[i], 12345, details[i]);
        
        if (result) {
            printf("Audit log %d sent successfully\n", i + 1);
        } else {
            printf("Audit log %d failed\n", i + 1);
        }
        
        printf("Memory automatically managed by GET_OWNERSHIP\n");
    }
    
    printf("\nAll audit logs processed successfully\n");
    printf("Zero memory leaks achieved through GET_OWNERSHIP pattern\n");
    
    return 0;
}
```

### GET_OWNERSHIP Guidelines

| Appropriate Use Cases | Inappropriate Use Cases |
|-----------------------|------------------------|
| One-time data transfers | Data needed elsewhere in the program |
| Fire-and-forget scenarios | Data with complex shared ownership |
| Large data that won't be reused | Small static data |
| Memory management simplification | Debugging scenarios (harder to track) |
| Clean API patterns | Data used in multiple contexts |

---

## Memory Safety Best Practices

### Fundamental Ownership Rules

```c
#include "BearHttpsClientOne.c"

void demonstrate_ownership_rules() {
    printf("Memory Ownership Best Practices\n");
    printf("==============================\n");
    
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // Rule 1: Static data uses REFERENCE
    printf("\nRule 1: Static data should use REFERENCE ownership\n");
    const char* static_msg = "Static string literal";
    BearHttpsRequest_send_any_with_ownership_control(
        request, (unsigned char*)static_msg, strlen(static_msg), 
        BEARSSL_HTTPS_REFERENCE
    );
    printf("Static string used with REFERENCE ownership\n");
    
    // Rule 2: Retained data uses COPY
    printf("\nRule 2: Data you want to keep should use COPY ownership\n");
    char* keep_data = strdup("Data to be retained");
    BearHttpsRequest_send_any_with_ownership_control(
        request, (unsigned char*)keep_data, strlen(keep_data),
        BEARSSL_HTTPS_COPY
    );
    printf("Data copied, original still accessible: %s\n", keep_data);
    free(keep_data);  // We are responsible for freeing our copy
    
    // Rule 3: Transferred data uses GET_OWNERSHIP
    printf("\nRule 3: Data you're finished with should use GET_OWNERSHIP\n");
    char* transfer_data = strdup("Data for transfer");
    printf("Before transfer: %s\n", transfer_data);
    BearHttpsRequest_send_any_with_ownership_control(
        request, (unsigned char*)transfer_data, strlen(transfer_data),
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    printf("Ownership transferred - do not access transfer_data anymore\n");
    // CRITICAL: Do not call free(transfer_data) - BearHttpsClient owns it
    
    BearHttpsRequest_free(request);  // This will free transfer_data
    printf("Request freed - all transferred data cleaned up automatically\n");
}

int main() {
    demonstrate_ownership_rules();
    
    printf("\nOwnership Summary:\n");
    printf("REFERENCE: For data that persists and won't change\n");
    printf("COPY: For data you want to continue using\n");
    printf("GET_OWNERSHIP: For data you're finished with\n");
    
    return 0;
}
```

### Common Ownership Errors

```c
#include "BearHttpsClientOne.c"

void demonstrate_common_mistakes() {
    printf("Common Ownership Errors (Avoid These Patterns)\n");
    printf("==============================================\n");
    
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    BearHttpsRequest_set_method(request, "POST");
    
    // Error 1: Using REFERENCE with temporary data
    printf("\nError 1: REFERENCE with temporary/local data\n");
    {
        char temp_buffer[100];
        strcpy(temp_buffer, "This buffer will go out of scope");
        
        printf("INCORRECT: Using REFERENCE with local buffer\n");
        printf("WARNING: Buffer will become invalid when scope ends\n");
        
        // CORRECT: Use COPY instead for local buffers
        BearHttpsRequest_send_any_with_ownership_control(
            request, (unsigned char*)temp_buffer, strlen(temp_buffer),
            BEARSSL_HTTPS_COPY
        );
        printf("CORRECT: Used COPY for local buffer\n");
    } // temp_buffer scope ends here, but COPY makes us safe
    
    // Error 2: Freeing data after GET_OWNERSHIP
    printf("\nError 2: Freeing data after transferring ownership\n");
    char *transfer_data = strdup("Data for ownership transfer");
    
    BearHttpsRequest_send_any_with_ownership_control(
        request, (unsigned char*)transfer_data, strlen(transfer_data),
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    printf("INCORRECT: Calling free() after GET_OWNERSHIP\n");
    printf("WARNING: This would cause a double-free error\n");
    // free(transfer_data);  // DO NOT DO THIS - causes double-free
    
    printf("CORRECT: Let BearHttpsClient handle the memory\n");
    
    // Error 3: Using COPY for large static data
    printf("\nError 3: Unnecessary copying of static data\n");
    static const char large_static_data[1000] = {0};
    
    printf("INEFFICIENT: Using COPY with large static data\n");
    printf("WARNING: Wastes memory by duplicating static data\n");
    printf("EFFICIENT: Use REFERENCE for static data instead\n");
    
    BearHttpsRequest_free(request);  // This frees transfer_data safely
    printf("All memory cleaned up correctly\n");
}

int main() {
    demonstrate_common_mistakes();
    
    printf("\nError Prevention Guidelines:\n");
    printf("1. Never use REFERENCE with temporary/local buffers\n");
    printf("2. Never call free() after transferring ownership\n");
    printf("3. Avoid unnecessary copying of large static data\n");
    printf("4. When uncertain, COPY is the safest option\n");
    
    return 0;
}
```

### Ownership Testing and Validation

```c
#include "BearHttpsClientOne.c"

typedef struct {
    char* data;
    size_t size;
    const char* description;
    int ownership_type;
} MemoryTestCase;

void test_ownership_patterns() {
    printf("Ownership Pattern Validation\n");
    printf("===========================\n");
    
    MemoryTestCase test_cases[] = {
        {
            .data = "Static string literal",
            .size = 0,  // Will be calculated
            .description = "String literal",
            .ownership_type = BEARSSL_HTTPS_REFERENCE
        },
        {
            .data = NULL,  // Will be allocated
            .size = 0,     // Will be set
            .description = "Dynamic data for COPY",
            .ownership_type = BEARSSL_HTTPS_COPY
        },
        {
            .data = NULL,  // Will be allocated  
            .size = 0,     // Will be set
            .description = "Dynamic data for GET_OWNERSHIP",
            .ownership_type = BEARSSL_HTTPS_GET_OWNERSHIP
        }
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\nTest Case %d: %s\n", i + 1, test_cases[i].description);
        
        BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
        BearHttpsRequest_set_method(request, "POST");
        
        // Prepare test data
        if (test_cases[i].data && test_cases[i].ownership_type == BEARSSL_HTTPS_REFERENCE) {
            // Static data
            test_cases[i].size = strlen(test_cases[i].data);
            printf("Using static data: %s\n", test_cases[i].data);
        } else {
            // Dynamic data
            test_cases[i].data = malloc(100);
            snprintf(test_cases[i].data, 100, "Dynamic data for test %d", i + 1);
            test_cases[i].size = strlen(test_cases[i].data);
            printf("Created dynamic data: %s\n", test_cases[i].data);
        }
        
        // Display ownership type
        const char* ownership_names[] = {"REFERENCE", "COPY", "GET_OWNERSHIP"};
        printf("Ownership type: %s\n", ownership_names[test_cases[i].ownership_type]);
        
        // Apply ownership pattern
        BearHttpsRequest_send_any_with_ownership_control(
            request,
            (unsigned char*)test_cases[i].data,
            test_cases[i].size,
            test_cases[i].ownership_type
        );
        
        // Demonstrate ownership-specific behavior
        switch (test_cases[i].ownership_type) {
            case BEARSSL_HTTPS_REFERENCE:
                printf("REFERENCE: BearHttpsClient points to existing data\n");
                break;
                
            case BEARSSL_HTTPS_COPY:
                printf("COPY: BearHttpsClient created its own copy\n");
                printf("Original data still accessible: %s\n", test_cases[i].data);
                break;
                
            case BEARSSL_HTTPS_GET_OWNERSHIP:
                printf("GET_OWNERSHIP: BearHttpsClient now owns the data\n");
                printf("WARNING: Original pointer is now unsafe to use\n");
                break;
        }
        
        // Handle cleanup based on ownership type
        if (test_cases[i].ownership_type == BEARSSL_HTTPS_COPY && 
            test_cases[i].data && 
            strcmp(test_cases[i].data, "Static string literal") != 0) {
            printf("Freeing our copy (COPY ownership)\n");
            free(test_cases[i].data);
        } else if (test_cases[i].ownership_type == BEARSSL_HTTPS_GET_OWNERSHIP) {
            printf("Not freeing - BearHttpsClient will handle cleanup\n");
        }
        
        BearHttpsRequest_free(request);
        printf("Test case %d completed successfully\n", i + 1);
    }
}

int main() {
    test_ownership_patterns();
    
    printf("\nAll ownership patterns validated successfully\n");
    printf("Each ownership type serves specific use cases effectively\n");
    
    return 0;
}
```

---

## Real-World Implementation Examples

### Web API Client with Mixed Ownership

```c
#include "BearHttpsClientOne.c"

// Static configuration suitable for REFERENCE ownership
static const char* API_BASE_URL = "https://api.example.com";
static const char* API_VERSION = "v1";

typedef struct {
    char* auth_token;      // Dynamic - will use COPY
    char* session_id;      // Dynamic - will use GET_OWNERSHIP  
    int user_id;           // Will be embedded in dynamic JSON
} UserSession;

char* create_auth_header(const char* token) {
    char* header = malloc(256);
    snprintf(header, 256, "Bearer %s", token);
    return header;  // Caller manages ownership
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
    // Build URL using static components
    char url[256];
    snprintf(url, sizeof(url), "%s/%s/%s", API_BASE_URL, API_VERSION, endpoint);
    
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsRequest_set_method(request, "POST");
    
    // Static headers use REFERENCE ownership
    BearHttpsRequest_add_header_with_ownership_config(
        request,
        "Content-Type", BEARSSL_HTTPS_REFERENCE,
        "application/json", BEARSSL_HTTPS_REFERENCE
    );
    
    // Dynamic auth header uses COPY (we will reuse the token)
    char* auth_header = create_auth_header(session->auth_token);
    BearHttpsRequest_add_header_with_ownership_config(
        request,
        "Authorization", BEARSSL_HTTPS_REFERENCE,
        auth_header, BEARSSL_HTTPS_COPY
    );
    
    // Dynamic JSON payload uses GET_OWNERSHIP (one-time use)
    char* json_payload = create_json_request(session->user_id, action, data);
    BearHttpsRequest_send_any_with_ownership_control(
        request,
        (unsigned char*)json_payload,
        strlen(json_payload),
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    printf("Sending API request to: %s\n", url);
    printf("Authorization: %s\n", auth_header);
    printf("JSON payload: %s\n", json_payload);  // Still safe to read
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    int success = 0;
    if (!BearHttpsResponse_error(response)) {
        int status = BearHttpsResponse_get_status_code(response);
        if (status == 200) {
            printf("API request successful\n");
            success = 1;
        } else {
            printf("API returned status %d\n", status);
        }
    } else {
        printf("API request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    // Clean up data we own
    free(auth_header);  // We created this with COPY ownership
    // Do not free json_payload - BearHttpsClient owns it via GET_OWNERSHIP
    
    BearHttpsRequest_free(request);   // This frees json_payload
    BearHttpsResponse_free(response);
    
    return success;
}

int main() {
    printf("Real-world API client with mixed ownership patterns\n");
    printf("==================================================\n");
    
    // Initialize user session
    UserSession session = {
        .auth_token = strdup("abc123def456"),
        .session_id = strdup("sess_789xyz"),
        .user_id = 12345
    };
    
    // Make several API calls
    send_api_request(&session, "users", "update_profile", "name=John");
    send_api_request(&session, "logs", "activity", "page_viewed");
    send_api_request(&session, "settings", "preferences", "theme=dark");
    
    printf("\nOwnership pattern summary:\n");
    printf("Static URLs/headers: REFERENCE (zero copy overhead)\n");
    printf("Auth tokens: COPY (retained for multiple requests)\n");
    printf("JSON payloads: GET_OWNERSHIP (efficient one-time transfer)\n");
    
    // Clean up session data
    free(session.auth_token);
    free(session.session_id);
    
    printf("All API calls completed with optimal memory management\n");
    return 0;
}
```

### Data Pipeline with Smart Ownership Selection

```c
#include "BearHttpsClientOne.c"

typedef enum {
    DATA_SMALL_JSON,   // Use COPY for safety
    DATA_LARGE_BUFFER, // Use GET_OWNERSHIP for efficiency
    DATA_CONFIG        // Use REFERENCE for static data
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
    
    // Select ownership based on data characteristics
    int ownership;
    const char* ownership_name;
    
    switch (packet->type) {
        case DATA_SMALL_JSON:
            ownership = BEARSSL_HTTPS_COPY;
            ownership_name = "COPY";
            printf("Small JSON data - using COPY for safety\n");
            break;
            
        case DATA_LARGE_BUFFER:
            ownership = BEARSSL_HTTPS_GET_OWNERSHIP;
            ownership_name = "GET_OWNERSHIP";
            printf("Large buffer - using GET_OWNERSHIP for efficiency\n");
            break;
            
        case DATA_CONFIG:
            ownership = BEARSSL_HTTPS_REFERENCE;
            ownership_name = "REFERENCE";
            printf("Configuration data - using REFERENCE (static)\n");
            break;
    }
    
    printf("Sending %s (%zu bytes) with %s ownership\n", 
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
        printf("Data packet sent successfully\n");
        success = 1;
    } else {
        printf("Failed to send: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    // Handle cleanup based on ownership type
    if (packet->type == DATA_SMALL_JSON) {
        // COPY was used, we still own the data
        if (packet->data) {
            free(packet->data);
            printf("Freed our copy of small JSON data\n");
        }
    } else if (packet->type == DATA_LARGE_BUFFER) {
        // GET_OWNERSHIP was used, do not free
        printf("Large buffer ownership transferred - not freeing\n");
    }
    // For DATA_CONFIG, it's static so no cleanup needed
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    
    return success;
}

int main() {
    printf("Smart Data Pipeline with Ownership Selection\n");
    printf("===========================================\n");
    
    // Static configuration data (REFERENCE)
    static const char config_data[] = "{\"version\":\"1.0\",\"mode\":\"production\"}";
    DataPacket config_packet = {
        .data = (unsigned char*)config_data,
        .size = sizeof(config_data) - 1,
        .type = DATA_CONFIG,
        .description = "Configuration data"
    };
    
    // Small dynamic JSON (COPY)
    char* small_json = malloc(200);
    snprintf(small_json, 200, "{\"message\":\"Small data packet\",\"id\":%d}", rand());
    DataPacket small_packet = {
        .data = (unsigned char*)small_json,
        .size = strlen(small_json),
        .type = DATA_SMALL_JSON,
        .description = "Small JSON packet"
    };
    
    // Large buffer (GET_OWNERSHIP)
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
    
    // Send all packets with appropriate ownership
    send_data_packet(&config_packet, "https://httpbin.org/post");
    send_data_packet(&small_packet, "https://httpbin.org/post");
    send_data_packet(&large_packet, "https://httpbin.org/post");
    
    printf("\nPipeline optimization summary:\n");
    printf("Configuration: REFERENCE - no memory overhead\n");
    printf("Small JSON: COPY - safe and predictable\n");
    printf("Large buffer: GET_OWNERSHIP - efficient transfer\n");
    
    printf("Data pipeline completed with optimal memory usage\n");
    return 0;
}
```

---

## Quick Reference Guide

### Ownership Function Reference

```c
// Data transmission with ownership control
BearHttpsRequest_send_any_with_ownership_control(request, data, size, ownership);

// Header configuration with ownership control
BearHttpsRequest_add_header_with_ownership_config(request, key, key_ownership, value, value_ownership);

// URL configuration with ownership control
BearHttpsRequest_set_url_with_ownership_config(request, url, ownership);
```

### Ownership Decision Matrix

| Data Type | REFERENCE | COPY | GET_OWNERSHIP |
|-----------|-----------|------|---------------|
| **String literals** | Optimal | Wasteful | Invalid |
| **Static arrays** | Optimal | Wasteful | Invalid |
| **Small dynamic data** | Unsafe | Optimal | Acceptable |
| **Large dynamic data** | Unsafe | Wasteful | Optimal |
| **Temporary buffers** | Dangerous | Safe | Dangerous |
| **One-time use data** | Context-dependent | Safe | Optimal |

### Safety Verification Checklist

```c
// Safe REFERENCE usage
const char* static_data = "hello";  // Permanent lifetime
BearHttpsRequest_send_any_with_ownership_control(req, static_data, 5, BEARSSL_HTTPS_REFERENCE);

// Safe COPY usage  
char* my_data = strdup("hello");
BearHttpsRequest_send_any_with_ownership_control(req, my_data, 5, BEARSSL_HTTPS_COPY);
free(my_data);  // Caller frees original

// Safe GET_OWNERSHIP usage
char* transfer_data = strdup("hello");
BearHttpsRequest_send_any_with_ownership_control(req, transfer_data, 5, BEARSSL_HTTPS_GET_OWNERSHIP);
// Do not free transfer_data - BearHttpsClient manages it
```

---

## Further Reading

Having mastered the ownership system, you can now explore:

1. [WebAssembly deployment](/docs/web_asm.md)
2. [Dependencies and build system](/docs/dependencies.md)  
