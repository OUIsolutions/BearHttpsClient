
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

bool private_BearHttps_is_sanitize_key(const char *key,const char *sanitized,int sanitized_size){
    long key_size = private_BearsslHttps_strlen(key);
    int comparation_size = 0;
    for(int i=0; i < key_size;i++){
        char current = key[i];
        if(current == ' ' || current == '\t' || current == '\n' || current == '\r'|| current == '-'|| current == '_'){
            continue;
        }
        if(comparation_size > sanitized_size){
            return false;
        }
        char current_lower = private_BearsslHttps_parse_char_to_lower(current);
        char sanitized_lower = private_BearsslHttps_parse_char_to_lower(sanitized[comparation_size]);

        if(current_lower != sanitized_lower){
            return false;
        }
        comparation_size++;
    }
    if(comparation_size != sanitized_size){
        return false;
    }
    return true;
}
