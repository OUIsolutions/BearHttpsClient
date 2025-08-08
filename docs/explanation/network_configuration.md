# Network Configuration

This guide explains how to configure network settings in BearHttpsClient for reliable connections, including DNS providers, SSL certificates, timeouts, and connection optimization.

## Overview

BearHttpsClient provides comprehensive network configuration options to ensure reliable connections across different environments. You can customize DNS resolution, SSL certificate validation, connection timeouts, and optimize performance using known IP addresses.

## DNS Configuration

DNS (Domain Name System) configuration allows you to control how BearHttpsClient resolves domain names to IP addresses. This is essential for reliability, performance, and security.

### Why Configure DNS?

- **Performance**: Faster DNS resolution leads to quicker request completion
- **Reliability**: Multiple DNS providers provide fallback options
- **Security**: Use DNS providers that block malicious domains
- **Privacy**: Choose privacy-focused DNS providers
- **Corporate Networks**: Use internal DNS servers for corporate environments

### Setting Up Custom DNS Providers

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    
    // Configure multiple DNS providers for reliability
    BearHttpsClientDnsProvider providers[] = {
        // Quad9 - Security focused
        {
            .hostname = "dns.quad9.net",
            .route = "/dns-query",
            .ip = "9.9.9.9",
            .port = 5053
        },
        // Google DNS - Fast and reliable
        {
            .hostname = "dns.google.com",
            .route = "/resolve", 
            .ip = "8.8.8.8",
            .port = 443
        },
        // NextDNS - Privacy focused
        {
            .hostname = "dns.nextdns.io",
            .route = "/dns-query",
            .ip = "217.146.9.93", 
            .port = 443
        }
    };
    
    int total_providers = sizeof(providers) / sizeof(BearHttpsClientDnsProvider);
    
    // Apply DNS configuration
    BearHttpsRequest_set_dns_providers(request, providers, total_providers);
    
    printf("Using %d DNS providers for reliability\n", total_providers);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Request successful with custom DNS\n");
        printf("Response: %s\n", BearHttpsResponse_read_body_str(response));
    } else {
        printf("Request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Popular DNS Providers

| Provider | Focus | Primary IP | Hostname |
|----------|-------|------------|----------|
| Google | Speed & reliability | 8.8.8.8 | dns.google.com |
| Cloudflare | Privacy & speed | 1.1.1.1 | cloudflare-dns.com |
| Quad9 | Security & malware blocking | 9.9.9.9 | dns.quad9.net |
| OpenDNS | Parental controls | 208.67.222.222 | doh.opendns.com |
| NextDNS | Customizable privacy | Variable | dns.nextdns.io |

### DNS Configuration Examples

#### Corporate Network Setup
```c
// For corporate environments with internal DNS
BearHttpsClientDnsProvider corporate_dns[] = {
    {
        .hostname = "internal-dns.company.com",
        .route = "/dns-query",
        .ip = "192.168.1.100",    // Internal DNS server
        .port = 53
    },
    {
        .hostname = "dns.google.com",  // Fallback to public DNS
        .route = "/resolve",
        .ip = "8.8.8.8",
        .port = 443
    }
};
```

#### Security-First Setup
```c
// Maximum security with malware blocking
BearHttpsClientDnsProvider security_dns[] = {
    {
        .hostname = "dns.quad9.net",      // Blocks malicious domains
        .route = "/dns-query",
        .ip = "9.9.9.9",
        .port = 5053
    },
    {
        .hostname = "security.cloudflare-dns.com",  // Cloudflare malware blocking
        .route = "/dns-query",
        .ip = "1.1.1.2",
        .port = 443
    }
};
```

#### Performance-Optimized Setup
```c
// Optimized for maximum speed
BearHttpsClientDnsProvider speed_dns[] = {
    {
        .hostname = "dns.google.com",     // Usually fastest
        .route = "/resolve",
        .ip = "8.8.8.8",
        .port = 443
    },
    {
        .hostname = "cloudflare-dns.com", // Very fast alternative
        .route = "/dns-query",
        .ip = "1.1.1.1",
        .port = 443
    }
};
```

## System DNS Configuration

You can configure BearHttpsClient to use your system's default DNS settings instead of custom providers:

### Enabling System DNS

```c
// Enable system DNS resolution BEFORE including the library
#define BEARSSL_USE_GET_ADDRINFO

#include "BearHttpsClientOne.c"

int main() {
    // This will now use your system's DNS settings
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    
    printf("Using system DNS resolution\n");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("System DNS worked successfully\n");
    } else {
        printf("System DNS failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### System DNS Considerations

**Advantages:**
- Uses system settings automatically
- Works with VPNs and corporate networks
- Follows system proxy settings

**Disadvantages:**
- Potential memory leaks on some platforms
- Less control over DNS resolution
- Platform-dependent behavior
- Harder to debug DNS issues

Note: System DNS can cause memory leaks on some platforms. Use with caution in production environments.

## SSL Certificate Configuration

BearHttpsClient uses Mozilla's trusted certificates by default, but you can customize certificate validation for specific requirements:

### Understanding Trust Anchors

Trust anchors are certificate authorities that verify the authenticity of SSL certificates. They act as the foundation of trust for secure connections.

```c
#include "BearHttpsClientOne.c"

// Custom certificate data (example - replace with actual certificate data)
static const unsigned char custom_cert[] = {
    // ... your certificate data here ...
};

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://your-private-server.com");
    
    // Use your custom certificate
    BearHttpsRequest_set_trusted_anchors(request, custom_cert, sizeof(custom_cert));
    
    printf("Using custom SSL certificates\n");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("SSL connection established with custom certificate\n");
    } else {
        printf("SSL failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### When to Use Custom Certificates

- **Internal servers**: Company's private certificates
- **Self-signed certificates**: Testing with local servers
- **Legacy systems**: Old certificates not in Mozilla store
- **High security environments**: Only trust specific certificate authorities
- **Air-gapped networks**: No access to standard certificate stores

### Obtaining Certificate Data

To get certificate data for custom trust anchors:

1. **From BearSSL tools:**
   ```bash
   # Convert PEM certificates to BearSSL format
   ./brssl ta your_certificates.pem > trust_anchors.h
   ```

2. **From Mozilla (default):**
   - BearHttpsClient includes Mozilla's certificates by default
   - Located in `BearSSLTrustAnchors.c`
   - Updated regularly for security

## Known IP Configuration

If you already know a website's IP address, you can skip DNS resolution entirely for improved performance:

### Basic Known IP Setup

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://example.com");
    
    // Pre-configured IP addresses for example.com
    const char *known_ips[] = {
        "93.184.216.34",    // Primary IP
        "93.184.216.35"     // Backup IP
    };
    int known_ips_count = sizeof(known_ips) / sizeof(const char*);
    
    // Tell BearHttpsClient to use these IPs directly
    BearHttpsRequest_set_known_ips(request, known_ips, known_ips_count);
    
    printf("Skipping DNS lookup - using known IPs\n");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Connection established using known IPs\n");
        printf("Response: %s\n", BearHttpsResponse_read_body_str(response));
    } else {
        printf("Connection failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Advanced IP Configuration

```c
#include "BearHttpsClientOne.c"

int make_request_with_known_ips(const char *url, const char **ips, int ip_count) {
    BearHttpsRequest *request = newBearHttpsRequest(url);
    BearHttpsRequest_set_known_ips(request, ips, ip_count);
    
    printf("Connecting to %s with %d known IPs\n", url, ip_count);
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        int status = BearHttpsResponse_get_status_code(response);
        printf("Connected successfully. Status: %d\n", status);
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    } else {
        printf("Connection failed: %s\n", BearHttpsResponse_get_error_msg(response));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 0;
    }
}

int main() {
    // Configure IPs for different services
    const char *google_ips[] = {"142.251.46.238", "142.251.46.174"};
    const char *github_ips[] = {"140.82.113.3", "140.82.113.4"};
    const char *cloudflare_ips[] = {"1.1.1.1", "1.0.0.1"};
    
    // Test multiple services
    make_request_with_known_ips("https://google.com", google_ips, 2);
    make_request_with_known_ips("https://github.com", github_ips, 2);
    make_request_with_known_ips("https://cloudflare.com", cloudflare_ips, 2);
    
    return 0;
}
```

### Finding IP Addresses

You can use various tools to find IP addresses for your services:

- **nslookup.io**: https://www.nslookup.io/ - Simple IP lookup
- **DigitalOcean DNS**: https://www.digitalocean.com/community/tools/dns - Detailed DNS information
- **DNS Checker**: https://dnschecker.org/ - Global IP verification
- **Command line**: `nslookup example.com` - Quick terminal lookup

**Example using command line:**
```bash
# Find IPs for a domain
nslookup example.com

# More detailed information
dig example.com +short
```

## Timeout Configuration

Configure connection and read timeouts to prevent requests from hanging indefinitely:

### Basic Timeout Setup

```c
#include "BearHttpsClientOne.c"

int main() {
    BearHttpsRequest *request = newBearHttpsRequest("https://httpbin.org/delay/2");
    
    // Set connection timeout (how long to wait for connection)
    BearHttpsRequest_set_connection_timeout(request, 10);  // 10 seconds
    
    // Set read timeout (how long to wait for data)
    BearHttpsRequest_set_read_timeout(request, 15);  // 15 seconds
    
    printf("Connection timeout: 10s, Read timeout: 15s\n");
    
    time_t start = time(NULL);
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    time_t end = time(NULL);
    
    printf("Request took %ld seconds\n", end - start);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Request completed within timeout\n");
    } else {
        printf("Request failed (possibly timeout): %s\n", 
               BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### Timeout Recommendations

| Use Case | Connection Timeout | Read Timeout | Reason |
|----------|-------------------|--------------|---------|
| Fast APIs | 5 seconds | 10 seconds | Quick responses expected |
| File downloads | 10 seconds | 300 seconds | Large files take time |
| Real-time data | 3 seconds | 5 seconds | Need quick updates |
| Background sync | 30 seconds | 60 seconds | Can wait longer |
| Mobile applications | 10 seconds | 30 seconds | Balance battery and UX |

## Complete Configuration Examples

### Corporate Network Configuration

```c
#include "BearHttpsClientOne.c"

BearHttpsRequest* create_corporate_request(const char* url) {
    BearHttpsRequest *request = newBearHttpsRequest(url);
    
    // Corporate DNS with fallback
    BearHttpsClientDnsProvider dns_providers[] = {
        {
            .hostname = "dns.corp.company.com",
            .route = "/dns",
            .ip = "10.0.0.100",
            .port = 53
        },
        {
            .hostname = "dns.google.com",  // External fallback
            .route = "/resolve",
            .ip = "8.8.8.8",
            .port = 443
        }
    };
    BearHttpsRequest_set_dns_providers(request, dns_providers, 2);
    
    // Corporate network timeouts
    BearHttpsRequest_set_connection_timeout(request, 30);  // Slower corporate network
    BearHttpsRequest_set_read_timeout(request, 60);
    
    // Corporate headers
    BearHttpsRequest_add_header(request, "X-Corporate-ID", "internal-app-001");
    BearHttpsRequest_add_header(request, "User-Agent", "CorporateApp/2.1");
    
    return request;
}

int main() {
    BearHttpsRequest *request = create_corporate_request("https://internal-api.company.com/status");
    
    printf("Making corporate network request\n");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Corporate API responded successfully\n");
        printf("Status: %d\n", BearHttpsResponse_get_status_code(response));
    } else {
        printf("Corporate request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

### High-Performance Configuration

```c
#include "BearHttpsClientOne.c"

BearHttpsRequest* create_performance_request(const char* url) {
    BearHttpsRequest *request = newBearHttpsRequest(url);
    
    // Fastest DNS providers
    BearHttpsClientDnsProvider fast_dns[] = {
        {
            .hostname = "dns.google.com",
            .route = "/resolve",
            .ip = "8.8.8.8",
            .port = 443
        },
        {
            .hostname = "cloudflare-dns.com",
            .route = "/dns-query",
            .ip = "1.1.1.1",
            .port = 443
        }
    };
    BearHttpsRequest_set_dns_providers(request, fast_dns, 2);
    
    // Aggressive timeouts for speed
    BearHttpsRequest_set_connection_timeout(request, 5);
    BearHttpsRequest_set_read_timeout(request, 10);
    
    // Optimize headers
    BearHttpsRequest_add_header(request, "Connection", "keep-alive");
    BearHttpsRequest_add_header(request, "Accept-Encoding", "gzip, deflate");
    
    return request;
}

int main() {
    // Test multiple fast services
    const char* fast_apis[] = {
        "https://httpbin.org/get",
        "https://api.github.com",
        "https://jsonplaceholder.typicode.com/posts/1"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("Testing fast connection to: %s\n", fast_apis[i]);
        
        BearHttpsRequest *request = create_performance_request(fast_apis[i]);
        
        time_t start = time(NULL);
        BearHttpsResponse *response = BearHttpsRequest_fetch(request);
        time_t end = time(NULL);
        
        if (!BearHttpsResponse_error(response)) {
            printf("Success in %ld seconds\n", end - start);
            printf("Status: %d\n", BearHttpsResponse_get_status_code(response));
        } else {
            printf("Failed: %s\n", BearHttpsResponse_get_error_msg(response));
        }
        
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
    }
    
    return 0;
}
```

### Security-First Configuration

```c
#include "BearHttpsClientOne.c"

BearHttpsRequest* create_secure_request(const char* url) {
    BearHttpsRequest *request = newBearHttpsRequest(url);
    
    // Security-focused DNS (blocks malicious domains)
    BearHttpsClientDnsProvider secure_dns[] = {
        {
            .hostname = "dns.quad9.net",
            .route = "/dns-query",
            .ip = "9.9.9.9",
            .port = 5053
        },
        {
            .hostname = "security.cloudflare-dns.com",
            .route = "/dns-query", 
            .ip = "1.1.1.2",  // Malware blocking
            .port = 443
        }
    };
    BearHttpsRequest_set_dns_providers(request, secure_dns, 2);
    
    // Reasonable timeouts for security checks
    BearHttpsRequest_set_connection_timeout(request, 15);
    BearHttpsRequest_set_read_timeout(request, 30);
    
    // Security headers
    BearHttpsRequest_add_header(request, "X-Requested-With", "BearHttpsClient");
    BearHttpsRequest_add_header(request, "Cache-Control", "no-cache");
    
    return request;
}

int main() {
    BearHttpsRequest *request = create_secure_request("https://httpbin.org/get");
    
    printf("Making security-hardened request\n");
    
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);
    
    if (!BearHttpsResponse_error(response)) {
        printf("Secure connection established\n");
        
        // Check security headers in response
        const char *server = BearHttpsResponse_get_header_value_by_key(response, "Server");
        const char *security = BearHttpsResponse_get_header_value_by_key(response, "X-Frame-Options");
        
        if (server) printf("Server: %s\n", server);
        if (security) printf("Security header found: %s\n", security);
        
    } else {
        printf("Security request failed: %s\n", BearHttpsResponse_get_error_msg(response));
    }
    
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);
    return 0;
}
```

## Common Configuration Mistakes

### Using Too Many DNS Providers

**Incorrect:**
```c
// Too many DNS providers slow things down
BearHttpsClientDnsProvider too_many_dns[10] = { /* ... */ };
BearHttpsRequest_set_dns_providers(request, too_many_dns, 10);
```

**Correct:**
```c
// 2-3 providers is optimal
BearHttpsClientDnsProvider optimal_dns[] = {
    {.hostname = "dns.google.com", .route = "/resolve", .ip = "8.8.8.8", .port = 443},
    {.hostname = "cloudflare-dns.com", .route = "/dns-query", .ip = "1.1.1.1", .port = 443}
};
BearHttpsRequest_set_dns_providers(request, optimal_dns, 2);
```

### Wrong IP Addresses for Known IPs

**Incorrect:**
```c
// Old or incorrect IP addresses
const char *old_ips[] = {"192.168.1.1"};  // This is a local IP!
BearHttpsRequest_set_known_ips(request, old_ips, 1);
```

**Correct:**
```c
// Verify IPs are current and correct
const char *correct_ips[] = {"93.184.216.34"};  // Verified example.com IP
BearHttpsRequest_set_known_ips(request, correct_ips, 1);
```

### Unrealistic Timeouts

**Incorrect:**
```c
// Timeout too short for large requests
BearHttpsRequest_set_connection_timeout(request, 1);  // 1 second is too short!
BearHttpsRequest_set_read_timeout(request, 2);
```

**Correct:**
```c
// Reasonable timeouts
BearHttpsRequest_set_connection_timeout(request, 10);  // 10 seconds to connect
BearHttpsRequest_set_read_timeout(request, 30);        // 30 seconds to read
```

## Configuration Reference

### DNS Configuration
```c
BearHttpsClientDnsProvider dns[] = {
    {.hostname = "dns.google.com", .route = "/resolve", .ip = "8.8.8.8", .port = 443}
};
BearHttpsRequest_set_dns_providers(request, dns, 1);
```

### Known IPs Configuration
```c
const char *ips[] = {"1.2.3.4", "5.6.7.8"};
BearHttpsRequest_set_known_ips(request, ips, 2);
```

### Custom SSL Certificates
```c
BearHttpsRequest_set_trusted_anchors(request, cert_data, cert_size);
```

### Timeout Configuration
```c
BearHttpsRequest_set_connection_timeout(request, 10);  // Connection timeout
BearHttpsRequest_set_read_timeout(request, 30);        // Read timeout
```

### Enable System DNS
```c
#define BEARSSL_USE_GET_ADDRINFO  // Before #include
#include "BearHttpsClientOne.c"
```

## Summary

This guide covered the complete network configuration options available in BearHttpsClient. You learned how to configure DNS providers for reliability and performance, set up custom SSL certificates for secure connections, optimize performance using known IP addresses, and configure appropriate timeouts for different use cases. These configuration options allow you to adapt BearHttpsClient to