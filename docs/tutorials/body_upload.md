# Body Upload Tutorial

This tutorial covers all the methods available in BearHttpsClient for uploading different types of body content in HTTP requests.

## Overview

BearHttpsClient provides several methods to send body content with your HTTP requests:
- Raw binary data
- String content
- JSON data
- File uploads (via binary data)

## 🚨 Important Notes

- All body content functions support **ownership modes** for memory management
- JSON returns are `const` - never free them manually
- Always check for NULL returns from response functions
- All dependencies are bundled - don't include additional headers

## Ownership Modes

```c
BEARSSL_HTTPS_REFERENCE     = 0  // Keeps reference (data must remain valid)
BEARSSL_HTTPS_GET_OWNERSHIP = 1  // Takes ownership (will free when done)
BEARSSL_HTTPS_COPY          = 2  // Makes a copy (default)
BEARSSL_DEFAULT_STRATEGY    = BEARSSL_HTTPS_COPY
```

## 1. Sending Raw Binary Data

### Basic Binary Upload
```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Raw binary data
    unsigned char data[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}; // PNG header
    long size = sizeof(data);
    
    // Send binary data (makes a copy by default)
    BearHttpsRequest_send_any(request, data, size);
    
    // Add appropriate content type
    BearHttpsRequest_add_header(request, "Content-Type", "application/octet-stream");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        printf("Status: %d\n", BearHttpsResponse_get_status_code(response));
        const char *body = BearHttpsResponse_read_body_str(response);
        if (body != NULL) {
            printf("Response: %s\n", body);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Binary Upload with Ownership Control
```c
int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Allocate data dynamically
    unsigned char *data = malloc(1024);
    // ... fill data ...
    
    // Transfer ownership - BearHttpsClient will free this memory
    BearHttpsRequest_send_any_with_ownership_control(
        request, 
        data, 
        1024, 
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    // Don't free 'data' - BearHttpsClient owns it now
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    // ... handle response ...
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## 2. Sending String Content

### Basic String Upload
```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Send string content
    char *text_data = "Hello, World! This is my text content.";
    BearHttpsRequest_send_body_str(request, text_data);
    
    // Set content type for text
    BearHttpsRequest_add_header(request, "Content-Type", "text/plain");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        printf("Status: %d\n", BearHttpsResponse_get_status_code(response));
        const char *body = BearHttpsResponse_read_body_str(response);
        if (body != NULL) {
            printf("Response: %s\n", body);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### String Upload with Ownership Control
```c
int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Dynamically allocated string
    char *dynamic_text = malloc(256);
    strcpy(dynamic_text, "This is dynamically allocated text");
    
    // Transfer ownership
    BearHttpsRequest_send_body_str_with_ownership_control(
        request, 
        dynamic_text, 
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    // Don't free dynamic_text - BearHttpsClient owns it now
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    // ... handle response ...
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## 3. Sending JSON Data

### Creating and Sending JSON
```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Create JSON object using BearHttpsClient's built-in method
    const cJSON *json_obj = BearHttpsRequest_create_cJSONPayloadObject(request);
    
    // Add data to JSON (remember: json_obj is const, so cast when needed)
    cJSON_AddStringToObject((cJSON*)json_obj, "name", "John Doe");
    cJSON_AddNumberToObject((cJSON*)json_obj, "age", 30);
    cJSON_AddStringToObject((cJSON*)json_obj, "email", "john@example.com");
    
    // Create nested object
    cJSON *address = cJSON_CreateObject();
    cJSON_AddStringToObject(address, "street", "123 Main St");
    cJSON_AddStringToObject(address, "city", "New York");
    cJSON_AddItemToObject((cJSON*)json_obj, "address", address);
    
    // Send JSON (BearHttpsClient handles the JSON automatically)
    BearHttpsRequest_send_cJSON(request, (cJSON*)json_obj);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        printf("Status: %d\n", BearHttpsResponse_get_status_code(response));
        
        // Read JSON response
        const cJSON *response_json = BearHttpsResponse_read_body_json(response);
        if (response_json != NULL) {
            char *json_string = cJSON_Print(response_json);
            if (json_string != NULL) {
                printf("JSON Response: %s\n", json_string);
                free(json_string); // This is our allocated string, so we free it
            }
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### JSON Array Upload
```c
int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Create JSON array
    const cJSON *json_array = BearHttpsRequest_create_cJSONPayloadArray(request);
    
    // Add items to array
    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddStringToObject(item1, "product", "laptop");
    cJSON_AddNumberToObject(item1, "price", 999.99);
    cJSON_AddItemToArray((cJSON*)json_array, item1);
    
    cJSON *item2 = cJSON_CreateObject();
    cJSON_AddStringToObject(item2, "product", "mouse");
    cJSON_AddNumberToObject(item2, "price", 29.99);
    cJSON_AddItemToArray((cJSON*)json_array, item2);
    
    // Send JSON array
    BearHttpsRequest_send_cJSON(request, (cJSON*)json_array);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    // ... handle response ...
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### External JSON with Ownership Control
```c
int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Create JSON externally
    cJSON *external_json = cJSON_CreateObject();
    cJSON_AddStringToObject(external_json, "message", "Hello from external JSON");
    cJSON_AddBoolToObject(external_json, "success", cJSON_True);
    
    // Send with ownership transfer
    BearHttpsRequest_send_cJSON_with_ownership_control(
        request, 
        external_json, 
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    // Don't free external_json - BearHttpsClient owns it now
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    // ... handle response ...
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## 4. File Upload Example

### Reading and Uploading a File
```c
#include "BearHttpsClientOne.c"

// Helper function to read file into memory
unsigned char* read_file(const char* filename, long* file_size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        *file_size = 0;
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    unsigned char* buffer = malloc(*file_size);
    if (!buffer) {
        fclose(file);
        *file_size = 0;
        return NULL;
    }
    
    fread(buffer, 1, *file_size, file);
    fclose(file);
    return buffer;
}

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Read file
    long file_size;
    unsigned char* file_data = read_file("example.jpg", &file_size);
    
    if (file_data == NULL) {
        printf("Failed to read file\n");
        BearHttpsRequest_free(request);
        return 1;
    }
    
    // Upload file with ownership transfer
    BearHttpsRequest_send_any_with_ownership_control(
        request, 
        file_data, 
        file_size, 
        BEARSSL_HTTPS_GET_OWNERSHIP
    );
    
    // Set appropriate content type
    BearHttpsRequest_add_header(request, "Content-Type", "image/jpeg");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        printf("File uploaded successfully! Status: %d\n", BearHttpsResponse_get_status_code(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## 5. Form Data Upload

### URL-Encoded Form Data
```c
int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/post");
    
    // Create form data string
    char *form_data = "username=johndoe&password=secret123&email=john%40example.com";
    
    BearHttpsRequest_send_body_str(request, form_data);
    BearHttpsRequest_add_header(request, "Content-Type", "application/x-www-form-urlencoded");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        printf("Form submitted! Status: %d\n", BearHttpsResponse_get_status_code(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## 6. Complete Example: API Client

### REST API Client with JSON
```c
#include "BearHttpsClientOne.c"

typedef struct {
    int id;
    char name[100];
    char email[100];
} User;

int create_user(const char* api_url, User* user) {
    BearHttpsRequest *request = newBearHttpsRequest(api_url);
    
    // Create JSON payload
    const cJSON *json = BearHttpsRequest_create_cJSONPayloadObject(request);
    cJSON_AddStringToObject((cJSON*)json, "name", user->name);
    cJSON_AddStringToObject((cJSON*)json, "email", user->email);
    
    // Send JSON
    BearHttpsRequest_send_cJSON(request, (cJSON*)json);
    
    // Add headers
    BearHttpsRequest_add_header(request, "Authorization", "Bearer your-token-here");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    int status_code = 0;
    if (BearHttpsResponse_error(response)) {
        printf("Error creating user: %s\n", BearHttpsResponse_get_error_msg(response));
        status_code = -1;
    } else {
        status_code = BearHttpsResponse_get_status_code(response);
        printf("User creation status: %d\n", status_code);
        
        // Parse response to get user ID
        const cJSON *response_json = BearHttpsResponse_read_body_json(response);
        if (response_json != NULL) {
            const cJSON *id_json = cJSON_GetObjectItem(response_json, "id");
            if (cJSON_IsNumber(id_json)) {
                user->id = id_json->valueint;
                printf("Created user with ID: %d\n", user->id);
            }
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return status_code;
}

int main() {
    User new_user = {0};
    strcpy(new_user.name, "Alice Smith");
    strcpy(new_user.email, "alice@example.com");
    
    int result = create_user("https://jsonplaceholder.typicode.com/users", &new_user);
    
    if (result == 201) {
        printf("User created successfully!\n");
    } else {
        printf("Failed to create user\n");
    }
    
    return 0;
}
```

## Best Practices

### 1. Memory Management
- Use ownership modes appropriately
- `BEARSSL_HTTPS_COPY` is safest but uses more memory
- `BEARSSL_HTTPS_GET_OWNERSHIP` for large data or when you don't need the data after the request
- `BEARSSL_HTTPS_REFERENCE` only when data lifetime is guaranteed

### 2. Content Types
Always set appropriate `Content-Type` headers:
```c
// For JSON
BearHttpsRequest_add_header(request, "Content-Type", "application/json");

// For form data
BearHttpsRequest_add_header(request, "Content-Type", "application/x-www-form-urlencoded");

// For binary files
BearHttpsRequest_add_header(request, "Content-Type", "application/octet-stream");

// For specific file types
BearHttpsRequest_add_header(request, "Content-Type", "image/jpeg");
BearHttpsRequest_add_header(request, "Content-Type", "text/plain");
```

### 3. Error Handling
Always check for errors and NULL returns:
```c
BearHttpsResponse *response = BearHttpsRequest_fetch(request);

if (BearHttpsResponse_error(response)) {
    printf("Error: %s (Code: %d)\n", 
           BearHttpsResponse_get_error_msg(response),
           BearHttpsResponse_get_error_code(response));
    // Handle error
} else {
    // Check response content
    const char *body = BearHttpsResponse_read_body_str(response);
    if (body != NULL) {
        // Use body safely
    }
}
```

### 4. Large File Uploads
For large files, consider reading in chunks or using streaming approaches to avoid memory issues:
```c
// Example: Check file size before loading
struct stat st;
if (stat("largefile.bin", &st) == 0) {
    if (st.st_size > 100 * 1024 * 1024) { // 100MB
        printf("File too large for memory upload\n");
        // Consider alternative approach
    }
}
```

## Common Issues and Solutions

### Issue: Memory Leaks
**Solution**: Always pair `BearHttpsRequest_free()` and `BearHttpsResponse_free()` calls, and use appropriate ownership modes.

### Issue: JSON Parsing Fails
**Solution**: Always check if `BearHttpsResponse_read_body_json()` returns NULL before using the result.

### Issue: Large File Upload Fails
**Solution**: Check available memory and consider using `BEARSSL_HTTPS_GET_OWNERSHIP` to avoid duplicating large data in memory.

### Issue: Content-Type Not Set
**Solution**: Always set appropriate `Content-Type` headers for your data type.

This tutorial covers all the essential methods for uploading body content with BearHttpsClient. Remember to always check for NULL returns and handle errors appropriately in your production code.
