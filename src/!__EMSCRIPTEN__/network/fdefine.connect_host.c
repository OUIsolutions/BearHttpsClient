//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_defineB.h"
//silver_chain_scope_end
#if !defined(__EMSCRIPTEN__)


#if  (!defined(BEARSSL_USSE_GET_ADDRINFO) && !defined(BEARSSL_HTTPS_MOCK_CJSON))
static int private_BearHttps_connect_host(BearHttpsRequest *self, BearHttpsResponse *response, const char *host, int port){
    
    printf("total file descriptors %d\n",BearHttpsRequest_total_open_file_descriptors);
    if(strcmp(host,"localhost")==0){
        return private_BearHttpsRequest_connect_ipv4(response,"0.0.0.0",port,self->connection_timeout);
    }
    for(int i = 0; i < self->known_ips_size;i++){
        const char *ip = self->known_ips[i];
        int sockfd = private_BearHttpsRequest_connect_ipv4_no_error_raise(ip,port,self->connection_timeout);
        if(sockfd < 0){
            continue;
        }
        printf("conectou por um ip conhecido\n");

        return sockfd;
    }
    
    for(int i = 0; i < BEARSSL_DNS_CACHE_SIZE;i++){
        privateBearHttpsDnsCache *cache = &privateBearHttpsDnsCache_itens[i];

        if(private_BearsslHttp_strcmp(cache->host,host) == 0){
            int sockfd = private_BearHttpsRequest_connect_ipv4_no_error_raise(cache->ip,port,self->connection_timeout);
            if(sockfd < 0){
                break;
            }
            printf("conectou por um ip em cache\n");
            return sockfd;
        }
    }
    
    BearHttpsClientDnsProvider *chosen_dns_providers  = self->dns_providers ?  self->dns_providers : privateBearHttpsProviders;
    int chosen_dns_providers_size = self->total_dns_providers ? self->total_dns_providers : privateBearHttpsProvidersSize;

    if(chosen_dns_providers_size == 0){
        BearHttpsResponse_set_error(response,"ERROR: no dns providers\n",BEARSSL_HTTPS_NO_DNS_PROVIDED);
        return -1;
    }

    privateBearHttpsStringArray * already_testted = newprivateBearHttpsStringArray();
    privateBearHttpsStringArray * errors = newprivateBearHttpsStringArray();
    char error_buf[512];

    for(int i = 0; i < chosen_dns_providers_size;i++){
            BearHttpsClientDnsProvider provider = chosen_dns_providers[i];
            BearHttpsRequest *dns_request = newBearHttpsRequest_fmt("https://%s:%d%s?name=%s&type=A",provider.ip,provider.port, provider.route, host); 
           //printf("used url %s\n",dns_request->url);
            dns_request->custom_bear_dns = provider.hostname;

            //these its require, otherwise can generate indirect recursion error if provider.ip its wrong
            dns_request->must_be_ipv4 = true;
            BearHttpsResponse *dns_response = BearHttpsRequest_fetch(dns_request);
            

            if(BearHttpsResponse_error(dns_response)){
                const char *error_msg = BearHttpsResponse_get_error_msg(dns_response);
                snprintf(error_buf, sizeof(error_buf),
                    "ERROR: dns provider request failed | provider: %s:%d (%s) | host: %s | reason: %s",
                    provider.ip, provider.port, provider.hostname ? provider.hostname : "unknown", host,
                    error_msg ? error_msg : "unknown");
                privateBearHttpsStringArray_append(errors, error_buf);
                
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                continue;
            }       
        

            const cJSON * body = BearHttpsResponse_read_body_json(dns_response);
            //printf("leu o body json: %s\n",cJSON_Print(body));
            if(BearHttpsResponse_error(dns_response)){
                const char *message = BearHttpsResponse_get_error_msg(dns_response);
                snprintf(error_buf, sizeof(error_buf),
                    "ERROR: failed to parse dns response body as JSON | provider: %s:%d (%s) | host: %s | reason: %s",
                    provider.ip, provider.port, provider.hostname ? provider.hostname : "unknown", host,
                    message ? message : "unknown");
                privateBearHttpsStringArray_append(errors, error_buf);
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                continue;
            }
      
            cJSON * answer = cJSON_GetObjectItem(body, "Answer");
            if(answer == NULL){
                snprintf(error_buf, sizeof(error_buf),
                    "ERROR: dns response missing 'Answer' field | provider: %s:%d (%s) | host: %s",
                    provider.ip, provider.port, provider.hostname ? provider.hostname : "unknown", host);
                privateBearHttpsStringArray_append(errors, error_buf);
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                continue;
            }
            long size = cJSON_GetArraySize(answer);
            if(size == 0){
                snprintf(error_buf, sizeof(error_buf),
                    "ERROR: dns response 'Answer' array is empty | provider: %s:%d (%s) | host: %s",
                    provider.ip, provider.port, provider.hostname ? provider.hostname : "unknown", host);
                privateBearHttpsStringArray_append(errors, error_buf);
                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                continue;
            }
            for(int j = 0; j < size;j++){
                cJSON * item = cJSON_GetArrayItem(answer,j);
                cJSON * data = cJSON_GetObjectItem(item, "data");
                if(data == NULL){
                    snprintf(error_buf, sizeof(error_buf),
                        "ERROR: dns answer entry[%d] missing 'data' field | provider: %s:%d (%s) | host: %s",
                        j, provider.ip, provider.port, provider.hostname ? provider.hostname : "unknown", host);
                    privateBearHttpsStringArray_append(errors, error_buf);
                    continue;
                }
                const char * ipv4 = cJSON_GetStringValue(data);
                if(ipv4 == NULL){
                    snprintf(error_buf, sizeof(error_buf),
                        "ERROR: dns answer entry[%d] 'data' field is not a valid string | provider: %s:%d (%s) | host: %s",
                        j, provider.ip, provider.port, provider.hostname ? provider.hostname : "unknown", host);
                    privateBearHttpsStringArray_append(errors, error_buf);
                    continue;
                }
                if(privateBearHttpsStringArray_find_position(already_testted, ipv4) != -1){
                    continue;
                }
                privateBearHttpsStringArray_append(already_testted, ipv4);

                int sockfd = private_BearHttpsRequest_connect_ipv4_no_error_raise(ipv4,port,self->connection_timeout);
                if(sockfd < 0){
                    snprintf(error_buf, sizeof(error_buf),
                        "ERROR: failed to connect to resolved ip %s:%d | provider: %s:%d (%s) | host: %s",
                        ipv4, port, provider.ip, provider.port, provider.hostname ? provider.hostname : "unknown", host);
                    privateBearHttpsStringArray_append(errors, error_buf);
                    continue;
                }
                long host_size = private_BearsslHttps_strlen(host);
                long ip_size = private_BearsslHttps_strlen(ipv4);

                if(privateBearHttpsDnsCache_last_free_point >= BEARSSL_DNS_CACHE_SIZE){
                    privateBearHttpsDnsCache_last_free_point = 0;
                }

                if(host_size < BEARSSL_DNS_CACHE_HOST_SIZE && ip_size < BEARSSL_DNS_CACHE_IP_SIZE){
                    
                    
                    privateBearHttpsDnsCache* cache = &privateBearHttpsDnsCache_itens[privateBearHttpsDnsCache_last_free_point];
                    private_BearsslHttps_strcpy( cache->host,host);
                    private_BearsslHttps_strcpy( cache->ip,ipv4);
                    privateBearHttpsDnsCache_last_free_point++; 
                }

                BearHttpsRequest_free(dns_request);
                BearHttpsResponse_free(dns_response);
                privateBearHttpsStringArray_free(already_testted);
                privateBearHttpsStringArray_free(errors);
                return sockfd;
            }
            BearHttpsRequest_free(dns_request);
            BearHttpsResponse_free(dns_response);
    }

     if(errors->size == 0){
         privateBearHttpsStringArray_append(errors, "ERROR: failed to create dns request");
     }

     //join all errors into a single string separated by newlines
     long total_len = 0;
     for(int i = 0; i < errors->size; i++){
         total_len += private_BearsslHttps_strlen(errors->strings[i]);
         if(i < errors->size - 1) total_len += 1; // for '\n'
     }

     char *combined_error = (char*)malloc(total_len + 1);
     combined_error[0] = '\0';
     long offset = 0;
     for(int i = 0; i < errors->size; i++){
         long entry_len = private_BearsslHttps_strlen(errors->strings[i]);
         memcpy(combined_error + offset, errors->strings[i], entry_len);
         offset += entry_len;
         if(i < errors->size - 1){
             combined_error[offset] = '\n';
             offset += 1;
         }
     }
     combined_error[offset] = '\0';

     BearHttpsResponse_set_error(response, combined_error, BEARSSL_HTTPS_FAILT_TO_CREATE_DNS_REQUEST);
     free(combined_error);
     privateBearHttpsStringArray_free(already_testted);
     privateBearHttpsStringArray_free(errors);
     return -1;
}


#endif


#endif