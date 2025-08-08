# Working with HTTP Headers - Beginner's Guide

HTTP headers are key-value pairs that provide additional information about the request or response. This tutorial will show you how to send headers with your requests and read headers from responses using BearHttpsClient.

## Sending Headers with Requests

### Adding Simple Headers

To add headers to your request, use the `BearHttpsRequest_add_header()` function:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/headers");
    
    // Add custom headers
    BearHttpsRequest_add_header(request, "User-Agent", "MyApp/1.0");
    BearHttpsRequest_add_header(request, "Accept", "application/json");
    BearHttpsRequest_add_header(request, "Authorization", "Bearer your-token-here");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        const char *body = BearHttpsResponse_read_body_str(response);
        if(body != NULL) {
            printf("Response: %s\n", body);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Adding Headers with Formatted Values

You can also add headers with formatted values using `BearHttpsRequest_add_header_fmt()`:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/headers");
    
    int user_id = 12345;
    const char *session_token = "abc123xyz";
    
    // Add formatted headers
    BearHttpsRequest_add_header_fmt(request, "X-User-ID", "%d", user_id);
    BearHttpsRequest_add_header_fmt(request, "X-Session", "session_%s", session_token);
    BearHttpsRequest_add_header_fmt(request, "X-Timestamp", "%ld", time(NULL));
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(!BearHttpsResponse_error(response)) {
        const char *body = BearHttpsResponse_read_body_str(response);
        if(body != NULL) {
            printf("Response: %s\n", body);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## Reading Headers from Responses

### Getting All Headers

You can iterate through all response headers:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/response-headers");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(!BearHttpsResponse_error(response)) {
        int headers_count = BearHttpsResponse_get_headers_size(response);
        printf("Response has %d headers:\n", headers_count);
        
        for(int i = 0; i < headers_count; i++) {
            const char *key = BearHttpsResponse_get_header_key_by_index(response, i);
            const char *value = BearHttpsResponse_get_header_value_by_index(response, i);
            
            // ALWAYS check for NULL!
            if(key != NULL && value != NULL) {
                printf("%s: %s\n", key, value);
            }
        }
    } else {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Getting Specific Headers by Name

To get a specific header value by its name:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/json");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(!BearHttpsResponse_error(response)) {
        // Get specific headers - ALWAYS check for NULL!
        const char *content_type = BearHttpsResponse_get_header_value_by_key(response, "Content-Type");
        if(content_type != NULL) {
            printf("Content-Type: %s\n", content_type);
        } else {
            printf("Content-Type header not found\n");
        }
        
        const char *server = BearHttpsResponse_get_header_value_by_key(response, "Server");
        if(server != NULL) {
            printf("Server: %s\n", server);
        } else {
            printf("Server header not found\n");
        }
        
        const char *content_length = BearHttpsResponse_get_header_value_by_key(response, "Content-Length");
        if(content_length != NULL) {
            printf("Content-Length: %s\n", content_length);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Using Sanitized Header Keys

HTTP headers are case-insensitive, so you can use sanitized keys:

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/json");
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(!BearHttpsResponse_error(response)) {
        // These should all work the same way:
        const char *content_type1 = BearHttpsResponse_get_header_value_by_sanitized_key(response, "content-type");
        const char *content_type2 = BearHttpsResponse_get_header_value_by_sanitized_key(response, "CONTENT-TYPE");
        const char *content_type3 = BearHttpsResponse_get_header_value_by_sanitized_key(response, "Content-Type");
        
        if(content_type1 != NULL) {
            printf("Content-Type (lowercase): %s\n", content_type1);
        }
        if(content_type2 != NULL) {
            printf("Content-Type (uppercase): %s\n", content_type2);
        }
        if(content_type3 != NULL) {
            printf("Content-Type (mixed case): %s\n", content_type3);
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## Common Use Cases

### Authentication Headers

```c
// Bearer token authentication
BearHttpsRequest_add_header(request, "Authorization", "Bearer your-jwt-token");

// API key authentication
BearHttpsRequest_add_header(request, "X-API-Key", "your-api-key");

// Basic authentication (base64 encoded username:password)
BearHttpsRequest_add_header(request, "Authorization", "Basic dXNlcjpwYXNz");
```

### Content Type Headers

```c
// For JSON data
BearHttpsRequest_add_header(request, "Content-Type", "application/json");

// For form data
BearHttpsRequest_add_header(request, "Content-Type", "application/x-www-form-urlencoded");

// For file uploads
BearHttpsRequest_add_header(request, "Content-Type", "multipart/form-data");
```

### Custom Application Headers

```c
// Custom tracking headers
BearHttpsRequest_add_header(request, "X-Request-ID", "unique-request-id");
BearHttpsRequest_add_header(request, "X-Client-Version", "1.2.3");
BearHttpsRequest_add_header(request, "X-Platform", "Linux");
```

## Important Notes

⚠️ **CRITICAL**: Always check for NULL when reading headers!
- `BearHttpsResponse_get_header_value_by_key()` can return NULL
- `BearHttpsResponse_get_header_value_by_sanitized_key()` can return NULL
- `BearHttpsResponse_get_header_key_by_index()` can return NULL
- `BearHttpsResponse_get_header_value_by_index()` can return NULL

✅ **Best Practices**:
- Always validate NULL returns before using header values
- Use meaningful header names that follow HTTP conventions
- Don't include sensitive information in custom headers unless necessary
- Remember that header names are case-insensitive

## Complete Example: API Client with Headers

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/posts/1");
    
    // Add common headers
    BearHttpsRequest_add_header(request, "User-Agent", "BearHttpsClient-Tutorial/1.0");
    BearHttpsRequest_add_header(request, "Accept", "application/json");
    BearHttpsRequest_add_header(request, "Accept-Language", "en-US,en;q=0.9");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(!BearHttpsResponse_error(response)) {
        printf("Status Code: %d\n", BearHttpsResponse_get_status_code(response));
        
        // Check response headers
        const char *content_type = BearHttpsResponse_get_header_value_by_key(response, "Content-Type");
        if(content_type != NULL) {
            printf("Response Content-Type: %s\n", content_type);
        }
        
        const char *server = BearHttpsResponse_get_header_value_by_key(response, "Server");
        if(server != NULL) {
            printf("Server: %s\n", server);
        }
        
        // Read the response body
        const char *body = BearHttpsResponse_read_body_str(response);
        if(body != NULL) {
            printf("\nResponse Body:\n%s\n", body);
        }
    } else {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

This tutorial covers the essential aspects of working with HTTP headers in BearHttpsClient. Remember to always check for NULL returns and follow HTTP header conventions for the best results!
