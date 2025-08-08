# Query Parameters Tutorial

## Introduction

Query parameters are a fundamental part of HTTP requests that allow you to pass additional data to a server through the URL. This tutorial will guide you through handling query parameters using the BearHttpsClient library in C, showing you how to build URLs with query parameters for different use cases.

## Prerequisites

Before starting this tutorial, ensure you have:
- Basic knowledge of C programming
- Understanding of HTTP requests and URLs
- BearHttpsClient library properly set up in your project

## What are Query Parameters?

Query parameters are key-value pairs appended to a URL after a question mark (`?`), separated by ampersands (`&`). They provide a way to send additional information to the server without using the request body.

**URL Structure with Query Parameters:**
```
https://api.example.com/search?q=programming&limit=10&sort=date
```

In this example:
- `q=programming` - search query
- `limit=10` - maximum number of results
- `sort=date` - sorting preference

## Building URLs with Query Parameters

### Simple Example: Single Parameter

```c
#include "BearHttpsClientOne.c"

int main() {
    // Simple search with one parameter
    char url[256];
    const char *search_term = "C programming";
    
    // Build URL with query parameter
    snprintf(url, sizeof(url), "https://api.github.com/search/repositories?q=%s", search_term);
    
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        printf("Status Code: %d\n", BearHttpsResponse_get_status_code(response));
        const char *body = BearHttpsResponse_read_body_str(response);
        if(body != NULL) {
            printf("Response received successfully\n");
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Advanced Example: Multiple Parameters

```c
#include "BearHttpsClientOne.c"

// Helper function to URL encode special characters
void simple_url_encode(const char *input, char *output, size_t output_size) {
    size_t input_len = strlen(input);
    size_t output_pos = 0;
    
    for (size_t i = 0; i < input_len && output_pos < output_size - 1; i++) {
        char c = input[i];
        if (c == ' ') {
            if (output_pos < output_size - 3) {
                strcpy(output + output_pos, "%20");
                output_pos += 3;
            }
        } else if (c == '&') {
            if (output_pos < output_size - 3) {
                strcpy(output + output_pos, "%26");
                output_pos += 3;
            }
        } else if (c == '=') {
            if (output_pos < output_size - 3) {
                strcpy(output + output_pos, "%3D");
                output_pos += 3;
            }
        } else {
            output[output_pos++] = c;
        }
    }
    output[output_pos] = '\0';
}

int main() {
    // Build URL with multiple query parameters
    char url[512];
    char encoded_query[128];
    
    const char *search_query = "machine learning";
    const char *language = "python";
    int per_page = 5;
    const char *sort = "stars";
    
    // Encode the search query
    simple_url_encode(search_query, encoded_query, sizeof(encoded_query));
    
    // Build complete URL with multiple parameters
    snprintf(url, sizeof(url), 
        "https://api.github.com/search/repositories?q=%s+language:%s&per_page=%d&sort=%s",
        encoded_query, language, per_page, sort);
    
    printf("Requesting URL: %s\n", url);
    
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(BearHttpsResponse_error(response)) {
        printf("Error: %s\n", BearHttpsResponse_get_error_msg(response));
    } else {
        printf("Status Code: %d\n", BearHttpsResponse_get_status_code(response));
        
        // Parse JSON response
        const cJSON *json = BearHttpsResponse_read_body_json(response);
        if(json != NULL) {
            const cJSON *total_count = cJSON_GetObjectItem(json, "total_count");
            const cJSON *items = cJSON_GetObjectItem(json, "items");
            
            if(total_count != NULL && cJSON_IsNumber(total_count)) {
                printf("Total repositories found: %d\n", total_count->valueint);
            }
            
            if(items != NULL && cJSON_IsArray(items)) {
                int array_size = cJSON_GetArraySize(items);
                printf("Showing %d repositories:\n\n", array_size);
                
                for(int i = 0; i < array_size; i++) {
                    const cJSON *repo = cJSON_GetArrayItem(items, i);
                    if(repo != NULL) {
                        const cJSON *name = cJSON_GetObjectItem(repo, "name");
                        const cJSON *stars = cJSON_GetObjectItem(repo, "stargazers_count");
                        const cJSON *description = cJSON_GetObjectItem(repo, "description");
                        
                        if(name != NULL && cJSON_IsString(name)) {
                            printf("%d. %s", i + 1, name->valuestring);
                        }
                        if(stars != NULL && cJSON_IsNumber(stars)) {
                            printf(" ⭐ %d", stars->valueint);
                        }
                        if(description != NULL && cJSON_IsString(description)) {
                            printf("\n   %s", description->valuestring);
                        }
                        printf("\n\n");
                    }
                }
            }
        } else {
            const char *body = BearHttpsResponse_read_body_str(response);
            if(body != NULL) {
                printf("Raw response: %s\n", body);
            }
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## Common Query Parameter Use Cases

### 1. Pagination
```c
// Example: Fetch page 2 with 10 items per page
char url[256];
int page = 2;
int limit = 10;
snprintf(url, sizeof(url), "https://api.example.com/users?page=%d&limit=%d", page, limit);
```

### 2. Filtering
```c
// Example: Filter users by status and role
char url[256];
const char *status = "active";
const char *role = "admin";
snprintf(url, sizeof(url), "https://api.example.com/users?status=%s&role=%s", status, role);
```

### 3. Sorting
```c
// Example: Sort results by creation date in descending order
char url[256];
const char *sort_by = "created_at";
const char *order = "desc";
snprintf(url, sizeof(url), "https://api.example.com/posts?sort=%s&order=%s", sort_by, order);
```

### 4. Search with Filters
```c
// Example: Search with multiple filters
char url[512];
char encoded_search[128];
const char *search_term = "web development";
const char *category = "tutorials";
int min_rating = 4;

simple_url_encode(search_term, encoded_search, sizeof(encoded_search));
snprintf(url, sizeof(url), 
    "https://api.example.com/content?search=%s&category=%s&min_rating=%d",
    encoded_search, category, min_rating);
```

## Best Practices

### 1. Always URL Encode Special Characters
Special characters in query parameters must be properly encoded:
- Space becomes `%20`
- `&` becomes `%26`
- `=` becomes `%3D`
- `+` becomes `%2B`

### 2. Handle NULL Values Safely
```c
const cJSON *json = BearHttpsResponse_read_body_json(response);
if(json != NULL) {
    // Safe to process JSON
    const cJSON *item = cJSON_GetObjectItem(json, "key");
    if(item != NULL && cJSON_IsString(item)) {
        printf("Value: %s\n", item->valuestring);
    }
} else {
    printf("Failed to parse JSON response\n");
}
```

### 3. Check Response Status
```c
if(BearHttpsResponse_error(response)) {
    printf("Request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    return 1;
}

int status_code = BearHttpsResponse_get_status_code(response);
if(status_code != 200) {
    printf("HTTP Error: %d\n", status_code);
    return 1;
}
```

### 4. Limit URL Length
URLs have practical length limits (usually around 2048 characters). For very long parameter lists, consider using POST requests instead.

## Real-World Example: Weather API

```c
#include "BearHttpsClientOne.c"

int fetch_weather(const char *city, const char *api_key) {
    char url[512];
    char encoded_city[128];
    
    // URL encode the city name
    simple_url_encode(city, encoded_city, sizeof(encoded_city));
    
    // Build URL with query parameters
    snprintf(url, sizeof(url), 
        "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric",
        encoded_city, api_key);
    
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if(BearHttpsResponse_error(response)) {
        printf("Error fetching weather: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    const cJSON *json = BearHttpsResponse_read_body_json(response);
    if(json != NULL) {
        const cJSON *main = cJSON_GetObjectItem(json, "main");
        const cJSON *weather_array = cJSON_GetObjectItem(json, "weather");
        
        if(main != NULL) {
            const cJSON *temp = cJSON_GetObjectItem(main, "temp");
            const cJSON *humidity = cJSON_GetObjectItem(main, "humidity");
            
            printf("Weather in %s:\n", city);
            if(temp != NULL && cJSON_IsNumber(temp)) {
                printf("Temperature: %.1f°C\n", temp->valuedouble);
            }
            if(humidity != NULL && cJSON_IsNumber(humidity)) {
                printf("Humidity: %d%%\n", humidity->valueint);
            }
        }
        
        if(weather_array != NULL && cJSON_IsArray(weather_array)) {
            const cJSON *weather = cJSON_GetArrayItem(weather_array, 0);
            if(weather != NULL) {
                const cJSON *description = cJSON_GetObjectItem(weather, "description");
                if(description != NULL && cJSON_IsString(description)) {
                    printf("Condition: %s\n", description->valuestring);
                }
            }
        }
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}

int main() {
    const char *api_key = "your_api_key_here";
    const char *city = "New York";
    
    fetch_weather(city, api_key);
    return 0;
}
```

## Conclusion

Query parameters are essential for building flexible and powerful HTTP requests. With BearHttpsClient, you can easily construct URLs with query parameters for various use cases like searching, filtering, pagination, and API configuration. Remember to always:

1. URL encode special characters
2. Check for NULL returns from JSON parsing functions
3. Validate HTTP response status codes
4. Handle errors gracefully

This knowledge will help you effectively consume REST APIs and build robust C applications that interact with web services.
