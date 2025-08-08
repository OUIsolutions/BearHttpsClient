
### Summary of all Public API Functions 

| Function Name                                                                                                                     | Description                                                            |
| ----------------------------------------------------------------------------------------------------------------------------------| ---------------------------------------------------------------------- |
| **Request Creation Functions**                                                                                                    |                                                                        |
| [newBearHttpsRequest](public_api/newBearHttpsRequest.md)                                                                         | Creates a new HTTP request object with the specified URL              |
| [newBearHttpsRequest_with_url_ownership_config](public_api/newBearHttpsRequest_with_url_ownership_config.md)                   | Creates a new HTTP request object with URL ownership control          |
| [BearHttpsRequest_free](public_api/BearHttpsRequest_free.md)                                                                     | Frees the memory allocated for a request object                       |
| **Request Configuration Functions**                                                                                              |                                                                        |
| [BearHttpsRequest_add_header](public_api/BearHttpsRequest_add_header.md)                                                         | Adds a header to the HTTP request                                      |
| [BearHttpsRequest_add_header_fmt](public_api/BearHttpsRequest_add_header_fmt.md)                                                 | Adds headers to the request using a formatted string                  |
| [BearHttpsRequest_set_chunk_header_read_props](public_api/BearHttpsRequest_set_chunk_header_read_props.md)                     | Configures chunk reading properties for response headers              |
| [BearHttpsRequest_set_dns_providers](public_api/BearHttpsRequest_set_dns_providers.md)                                         | Sets custom DNS providers for resolving hostnames                     |
| [BearHttpsRequest_set_known_ips](public_api/BearHttpsRequest_set_known_ips.md)                                                   | Sets a list of known IP addresses for the request's hostname          |
| [BearHttpsRequest_set_max_redirections](public_api/BearHttpsRequest_set_max_redirections.md)                                   | Sets the maximum number of HTTP redirections to follow                |
| [BearHttpsRequest_set_trusted_anchors](public_api/BearHttpsRequest_set_trusted_anchors.md)                                     | Sets custom trusted anchors (root certificates) for SSL/TLS validation |
| **Request Body/Payload Functions**                                                                                              |                                                                        |
| [BearHttpsRequest_create_cJSONPayloadArray](public_api/BearHttpsRequest_create_cJSONPayloadArray.md)                           | Creates a new cJSON array for building JSON array payloads            |
| [BearHttpsRequest_create_cJSONPayloadObject](public_api/BearHttpsRequest_create_cJSONPayloadObject.md)                         | Creates a new cJSON object for building JSON payloads                 |
| [BearHttpsRequest_send_any](public_api/BearHttpsRequest_send_any.md)                                                             | Sends binary data as the request body (default ownership)             |
| [BearHttpsRequest_send_any_with_ownership_control](public_api/BearHttpsRequest_send_any_with_ownership_control.md)             | Sends binary data as the request body with ownership control          |
| [BearHttpsRequest_send_body_str](public_api/BearHttpsRequest_send_body_str.md)                                                   | Sends a string as the request body (default ownership)                |
| [BearHttpsRequest_send_body_str_with_ownership_control](public_api/BearHttpsRequest_send_body_str_with_ownership_control.md)   | Sends a string as the request body with ownership control             |
| [BearHttpsRequest_send_cJSON](public_api/BearHttpsRequest_send_cJSON.md)                                                         | Sends a cJSON object as the request body (default ownership)          |
| [BearHttpsRequest_send_cJSON_with_ownership_control](public_api/BearHttpsRequest_send_cJSON_with_ownership_control.md)         | Sends a cJSON object as the request body with ownership control       |
| **Request Execution Functions**                                                                                                  |                                                                        |
| [BearHttpsRequest_fetch](public_api/BearHttpsRequest_fetch.md)                                                                   | Executes the HTTP request and returns the response                    |
| **Response Handling Functions**                                                                                                  |                                                                        |
| [BearHttpsResponse_error](public_api/BearHttpsResponse_error.md)                                                                 | Checks if an error occurred during the HTTP request                   |
| [BearHttpsResponse_free](public_api/BearHttpsResponse_free.md)                                                                   | Frees the memory allocated for a response object                      |
| [BearHttpsResponse_get_error_code](public_api/BearHttpsResponse_get_error_code.md)                                             | Gets a numeric error code if an error occurred                        |
| [BearHttpsResponse_get_error_msg](public_api/BearHttpsResponse_get_error_msg.md)                                               | Gets a human-readable error message if an error occurred              |
| [BearHttpsResponse_get_status_code](public_api/BearHttpsResponse_get_status_code.md)                                           | Gets the HTTP status code from the response                           |
| **Response Body Functions**                                                                                                      |                                                                        |
| [BearHttpsResponse_get_body_size](public_api/BearHttpsResponse_get_body_size.md)                                               | Gets the size of the response body in bytes                           |
| [BearHttpsResponse_read_body_str](public_api/BearHttpsResponse_read_body_str.md)                                               | Reads the response body as a null-terminated string                   |
| [BearHttpsResponse_read_body_json](public_api/BearHttpsResponse_read_body_json.md)                                             | Reads and parses the response body as a JSON object                   |
| **Response Header Functions**                                                                                                    |                                                                        |
| [BearHttpsResponse_get_headers_size](public_api/BearHttpsResponse_get_headers_size.md)                                         | Gets the number of headers in the HTTP response                       |
| [BearHttpsResponse_get_header_key_by_index](public_api/BearHttpsResponse_get_header_key_by_index.md)                           | Gets the key (name) of a response header by its index position        |
| [BearHttpsResponse_get_header_value_by_index](public_api/BearHttpsResponse_get_header_value_by_index.md)                       | Gets the value of a response header by its index position             |
| [BearHttpsResponse_get_header_value_by_key](public_api/BearHttpsResponse_get_header_value_by_key.md)                           | Gets the value of a response header by its key (case-sensitive)       |
| [BearHttpsResponse_get_header_value_by_sanitized_key](public_api/BearHttpsResponse_get_header_value_by_sanitized_key.md)       | Gets the value of a response header by its key (case-insensitive)     |