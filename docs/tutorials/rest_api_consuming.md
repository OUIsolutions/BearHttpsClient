# REST API Consumption Tutorial

## Introduction

REST (Representational State Transfer) APIs are a standardized way for applications to communicate over the internet. This tutorial will guide you through consuming REST APIs using the BearHttpsClient library in C, providing a detailed practical example with a real-world API endpoint.

## Prerequisites

Before starting this tutorial, ensure you have:
- Basic knowledge of C programming
- Understanding of HTTP methods (GET, POST, PUT, DELETE)
- BearHttpsClient library properly set up in your project

## What is a REST API?

A REST API is a web service that follows REST architectural principles. It uses standard HTTP methods to perform operations on resources identified by URLs. The most common HTTP methods are:

- **GET**: Retrieve data from the server
- **POST**: Send data to create a new resource
- **PUT**: Update an existing resource
- **PATCH**: Partially update an existing resource
- **DELETE**: Remove a resource

## Detailed Example: Consuming a Real REST API

We'll use the JSONPlaceholder API (https://jsonplaceholder.typicode.com/), a free fake REST API for testing and prototyping. This example demonstrates fetching user information and their associated posts, showcasing multiple API calls and data processing.

```c
#include "BearHttpsClientOne.c"

// Function to safely extract string from JSON object
const char* safe_json_string(const cJSON *json, const char *key) {
    const cJSON *item = cJSON_GetObjectItem(json, key);
    if (item != NULL && cJSON_IsString(item)) {
        return item->valuestring;
    }
    return "N/A";
}

// Function to safely extract integer from JSON object
int safe_json_int(const cJSON *json, const char *key) {
    const cJSON *item = cJSON_GetObjectItem(json, key);
    if (item != NULL && cJSON_IsNumber(item)) {
        return item->valueint;
    }
    return -1;
}

// Function to fetch and display user information
int fetch_user_info(int user_id) {
    printf("\n=== Fetching User Information (ID: %d) ===\n", user_id);
    
    // Create URL for specific user
    char url[256];
    snprintf(url, sizeof(url), "https://jsonplaceholder.typicode.com/users/%d", user_id);
    
    // Create and configure the request
    BearHttpsRequest *request = newBearHttpsRequest(url);
    if (request == NULL) {
        printf("Failed to create request\n");
        return 0;
    }
    
    // Add headers for better API interaction
    BearHttpsRequest_add_header(request, "User-Agent", "BearHttpsClient-Tutorial/1.0");
    BearHttpsRequest_add_header(request, "Accept", "application/json");
    
    // Execute the request
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    if (response == NULL) {
        printf("Failed to execute request\n");
        BearHttpsRequest_free(request);
        return 0;
    }
    
    // Check for network or request errors
    if (BearHttpsResponse_error(response)) {
        printf("Request failed with error: %s\n", BearHttpsResponse_get_error_msg(response));
        printf("Error code: %d\n", BearHttpsResponse_get_error_code(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    // Check HTTP status code
    int status_code = BearHttpsResponse_get_status_code(response);
    printf("HTTP Status Code: %d\n", status_code);
    
    if (status_code != 200) {
        printf("API returned error status code: %d\n", status_code);
        if (status_code == 404) {
            printf("User with ID %d not found\n", user_id);
        }
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    // Parse the JSON response
    const cJSON *user_json = BearHttpsResponse_read_body_json(response);
    if (user_json == NULL) {
        printf("Failed to parse JSON response\n");
        // Try to read as string to see what we got
        const char *body = BearHttpsResponse_read_body_str(response);
        if (body != NULL) {
            printf("Raw response body: %s\n", body);
        }
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    // Extract user information with proper validation
    printf("\nUser Details:\n");
    printf("- ID: %d\n", safe_json_int(user_json, "id"));
    printf("- Name: %s\n", safe_json_string(user_json, "name"));
    printf("- Username: %s\n", safe_json_string(user_json, "username"));
    printf("- Email: %s\n", safe_json_string(user_json, "email"));
    printf("- Phone: %s\n", safe_json_string(user_json, "phone"));
    printf("- Website: %s\n", safe_json_string(user_json, "website"));
    
    // Extract nested address information
    const cJSON *address = cJSON_GetObjectItem(user_json, "address");
    if (address != NULL && cJSON_IsObject(address)) {
        printf("\nAddress:\n");
        printf("- Street: %s\n", safe_json_string(address, "street"));
        printf("- Suite: %s\n", safe_json_string(address, "suite"));
        printf("- City: %s\n", safe_json_string(address, "city"));
        printf("- Zipcode: %s\n", safe_json_string(address, "zipcode"));
        
        // Extract geo coordinates from nested object
        const cJSON *geo = cJSON_GetObjectItem(address, "geo");
        if (geo != NULL && cJSON_IsObject(geo)) {
            printf("- Coordinates: %s, %s\n", 
                   safe_json_string(geo, "lat"), 
                   safe_json_string(geo, "lng"));
        }
    }
    
    // Extract company information
    const cJSON *company = cJSON_GetObjectItem(user_json, "company");
    if (company != NULL && cJSON_IsObject(company)) {
        printf("\nCompany:\n");
        printf("- Name: %s\n", safe_json_string(company, "name"));
        printf("- Catchphrase: %s\n", safe_json_string(company, "catchPhrase"));
        printf("- Business: %s\n", safe_json_string(company, "bs"));
    }
    
    // Clean up resources
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    
    return 1; // Success
}

// Function to fetch user's posts
int fetch_user_posts(int user_id) {
    printf("\n=== Fetching Posts by User ID: %d ===\n", user_id);
    
    // Create URL for user's posts
    char url[256];
    snprintf(url, sizeof(url), "https://jsonplaceholder.typicode.com/posts?userId=%d", user_id);
    
    // Create and configure the request
    BearHttpsRequest *request = newBearHttpsRequest(url);
    if (request == NULL) {
        printf("Failed to create request\n");
        return 0;
    }
    
    // Add headers
    BearHttpsRequest_add_header(request, "User-Agent", "BearHttpsClient-Tutorial/1.0");
    BearHttpsRequest_add_header(request, "Accept", "application/json");
    
    // Execute the request
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    if (response == NULL) {
        printf("Failed to execute request\n");
        BearHttpsRequest_free(request);
        return 0;
    }
    
    // Check for errors
    if (BearHttpsResponse_error(response)) {
        printf("Request failed with error: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    // Check status code
    int status_code = BearHttpsResponse_get_status_code(response);
    if (status_code != 200) {
        printf("API returned error status code: %d\n", status_code);
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    // Parse JSON array response
    const cJSON *posts_array = BearHttpsResponse_read_body_json(response);
    if (posts_array == NULL) {
        printf("Failed to parse JSON response\n");
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    // Verify it's an array
    if (!cJSON_IsArray(posts_array)) {
        printf("Expected JSON array but got different type\n");
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
    
    // Process the posts
    int posts_count = cJSON_GetArraySize(posts_array);
    printf("Found %d posts:\n\n", posts_count);
    
    for (int i = 0; i < posts_count; i++) {
        const cJSON *post = cJSON_GetArrayItem(posts_array, i);
        if (post != NULL && cJSON_IsObject(post)) {
            int post_id = safe_json_int(post, "id");
            const char *title = safe_json_string(post, "title");
            const char *body = safe_json_string(post, "body");
            
            printf("Post #%d:\n", post_id);
            printf("  Title: %s\n", title);
            printf("  Body: %.100s%s\n", body, strlen(body) > 100 ? "..." : "");
            printf("  ---\n");
        }
    }
    
    // Clean up resources
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    
    return posts_count;
}

int main() {
    printf("REST API Consumption Example\n");
    printf("=============================\n");
    
    // Choose a user ID to demonstrate with
    int user_id = 2;
    
    // Step 1: Fetch user information
    if (!fetch_user_info(user_id)) {
        printf("Failed to fetch user information\n");
        return 1;
    }
    
    // Step 2: Fetch user's posts
    int posts_count = fetch_user_posts(user_id);
    if (posts_count < 0) {
        printf("Failed to fetch user posts\n");
        return 1;
    }
    
    printf("\n=== Summary ===\n");
    printf("Successfully processed user ID %d with %d posts\n", user_id, posts_count);
    
    return 0;
}
```

## Code Explanation

This comprehensive example demonstrates several important concepts when consuming REST APIs:

### 1. Request Creation and Configuration
```c
BearHttpsRequest *request = newBearHttpsRequest(url);
BearHttpsRequest_add_header(request, "User-Agent", "BearHttpsClient-Tutorial/1.0");
BearHttpsRequest_add_header(request, "Accept", "application/json");
```
We create a request object and add appropriate headers. The User-Agent header identifies our application, and Accept header specifies we want JSON responses.

### 2. Error Handling at Multiple Levels
```c
// Network/request errors
if (BearHttpsResponse_error(response)) {
    printf("Request failed with error: %s\n", BearHttpsResponse_get_error_msg(response));
}

// HTTP status code validation
if (status_code != 200) {
    printf("API returned error status code: %d\n", status_code);
}

// JSON parsing validation
if (user_json == NULL) {
    printf("Failed to parse JSON response\n");
}
```
The example shows three levels of error checking: network errors, HTTP status codes, and JSON parsing failures.

### 3. Safe JSON Data Extraction
```c
const char* safe_json_string(const cJSON *json, const char *key) {
    const cJSON *item = cJSON_GetObjectItem(json, key);
    if (item != NULL && cJSON_IsString(item)) {
        return item->valuestring;
    }
    return "N/A";
}
```
Helper functions ensure safe extraction of data from JSON objects, preventing crashes when expected fields are missing or have wrong types.

### 4. Nested JSON Object Processing
```c
const cJSON *address = cJSON_GetObjectItem(user_json, "address");
if (address != NULL && cJSON_IsObject(address)) {
    // Process nested address object
    const cJSON *geo = cJSON_GetObjectItem(address, "geo");
    // Process further nested geo coordinates
}
```
The example handles nested JSON structures commonly found in real APIs.

### 5. Array Processing
```c
if (!cJSON_IsArray(posts_array)) {
    printf("Expected JSON array but got different type\n");
    return 0;
}

int posts_count = cJSON_GetArraySize(posts_array);
for (int i = 0; i < posts_count; i++) {
    const cJSON *post = cJSON_GetArrayItem(posts_array, i);
    // Process each post
}
```
Demonstrates proper handling of JSON arrays with validation and iteration.

### 6. Memory Management
```c
BearHttpsRequest_free(request);
BearHttpsResponse_free(response);
```
Proper cleanup of allocated resources is performed in all code paths, including error conditions.

## Key Learning Points

1. **Always validate responses**: Check for network errors, HTTP status codes, and JSON parsing success
2. **Use helper functions**: Create utility functions for safe data extraction
3. **Handle nested structures**: Real APIs often return complex nested JSON objects
4. **Proper resource cleanup**: Always free allocated memory, even in error cases
5. **Meaningful error messages**: Provide clear feedback about what went wrong
6. **Header management**: Use appropriate headers for better API interaction

This example provides a solid foundation for consuming any REST API using BearHttpsClient, demonstrating production-ready error handling and data processing techniques.

## Common HTTP Status Codes

Understanding HTTP status codes is essential for API consumption:

- **200 OK**: Request successful
- **201 Created**: Resource created successfully
- **400 Bad Request**: Invalid request syntax
- **401 Unauthorized**: Authentication required
- **403 Forbidden**: Access denied
- **404 Not Found**: Resource not found
- **429 Too Many Requests**: Rate limit exceeded
- **500 Internal Server Error**: Server error
- **502 Bad Gateway**: Invalid response from upstream server
- **503 Service Unavailable**: Server temporarily unavailable

## Best Practices

1. **Always check return values**: Never assume API calls will succeed
2. **Handle NULL responses**: All response reading functions can return NULL
3. **Free memory properly**: Always call cleanup functions in all code paths
4. **Use appropriate headers**: Set User-Agent and Accept headers for better API interaction
5. **Implement robust error handling**: Check network errors, status codes, and data validation
6. **Respect rate limits**: Many APIs have request limits
7. **Use HTTPS**: Always prefer secure connections when available
8. **Validate input data**: Sanitize data before sending to APIs
9. **Create helper functions**: Use utility functions for safe data extraction
10. **Handle nested structures**: Real APIs often return complex JSON objects

## Conclusion

This tutorial demonstrated a comprehensive approach to consuming REST APIs using BearHttpsClient. The detailed example showed how to:

- Make multiple related API calls
- Handle complex nested JSON responses
- Implement robust error handling at multiple levels
- Process both single objects and arrays
- Safely extract data with proper validation
- Manage memory correctly in all scenarios

With this foundation, you can confidently integrate any REST API into your C applications using BearHttpsClient. Remember to always read the API documentation for specific endpoints, authentication requirements, and response formats.
