# Tutorial: Consuming a REST API with BearHttpsClient

This tutorial will show you how to make HTTP requests to REST APIs using BearHttpsClient. We'll fetch data from a public API and parse the JSON response.

## What We'll Build

We'll create a simple program that:
1. Sends a GET request to a REST API
2. Receives JSON data
3. Extracts and displays specific information

## Step-by-Step Guide

### 1. Include the Library

First, include the BearHttpsClient library in your C program:

```c
#include "BearHttpsClientOne.c"
```

### 2. Create an HTTP Request

```c
int main() {
    // Create a new request to the API endpoint
    BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/posts");
    
    // Tell the server we want JSON data back
    BearHttpsRequest_add_header(request, "Accept", "application/json");
```

**What's happening here?**
- We create a request object pointing to a free testing API
- We add a header to tell the server we prefer JSON format

### 3. Send the Request and Get Response

```c
    // Send the request and get the response
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
```

**What's happening?**
- `BearHttpsRequest_fetch()` sends our request to the server
- It returns a response object containing the server's reply

### 4. Handle Errors

```c
    // Check if something went wrong
    if (BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        // Continue processing if no errors
```

**What's happening?**
- Always check for errors first (network issues, invalid URLs, etc.)
- If there's an error, print the error message and stop

### 5. Check HTTP Status and Process Data

```c
        // Get the HTTP status code (200 = success)
        int status = BearHttpsResponse_get_status_code(response);
        printf("HTTP Status: %d\n", status);

        if (status == 200) {
            // Parse the JSON response
            const cJSON *json = BearHttpsResponse_read_body_json(response);
            
            // Get the first item from the JSON array
            const cJSON *first_post = cJSON_GetArrayItem(json, 0);
            
            // Extract the "title" field from that item
            const cJSON *title = cJSON_GetObjectItem(first_post, "title");
            
            // Make sure it's actually a string before using it
            if (title && cJSON_IsString(title)) {
                printf("First post title: %s\n", title->valuestring);
            }
        }
    }
```

**What's happening?**
- HTTP 200 means "OK" - the request was successful
- We parse the JSON response into a cJSON object
- The API returns an array of posts, so we get the first one (index 0)
- We extract the "title" field and print it
- We verify the title is actually a string before using it

### 6. Clean Up Memory

```c
    // Always free memory when done
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

**Important!** Always free the memory to prevent memory leaks.

## Complete Example

```c
#include "BearHttpsClientOne.c"

int main() {
    // Create request
    BearHttpsRequest *request = newBearHttpsRequest("https://jsonplaceholder.typicode.com/posts");
    BearHttpsRequest_add_header(request, "Accept", "application/json");

    // Send request
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);

    // Handle response
    if (BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        int status = BearHttpsResponse_get_status_code(response);
        printf("HTTP Status: %d\n", status);

        if (status == 200) {
            const cJSON *json = BearHttpsResponse_read_body_json(response);
            const cJSON *first_post = cJSON_GetArrayItem(json, 0);
            const cJSON *title = cJSON_GetObjectItem(first_post, "title");
            if (title && cJSON_IsString(title)) {
                printf("First post title: %s\n", title->valuestring);
            }
        }
    }

    // Clean up
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## What You Learned

- How to create HTTP requests with BearHttpsClient
- How to add headers to requests
- How to handle errors properly
- How to check HTTP status codes
- How to parse JSON responses
- The importance of memory management in C

## Try This Next

- Modify the URL to fetch different data
- Extract different fields from the JSON
- Add error handling for JSON parsing
- Try POST requests with