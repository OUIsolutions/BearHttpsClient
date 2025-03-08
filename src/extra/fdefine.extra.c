
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
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

char * private_BearHttps_format_vaarg(const char *expresion, va_list args){

    va_list args_copy;
    va_copy(args_copy, args);
    long required_size = vsnprintf(NULL, 0,expresion,args_copy);
    va_end(args_copy);
    char *buffer = (char*)malloc(sizeof(char) * required_size + 2);
    vsnprintf(buffer,sizeof (char) * required_size+1,expresion,args);
    return buffer;
}