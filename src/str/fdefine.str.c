
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


long private_BearsslHttps_strlen(const char *str){
    long size = 0;
    while(str[size] != '\0'){
        size++;
    }
    return size;
}
int private_BearsslHttp_strcmp(const char *str1,const char *str2){
    long size1 = private_BearsslHttps_strlen(str1);
    long size2 = private_BearsslHttps_strlen(str2);
    if(size1 != size2){
        return 1;
    }
    for(long i = 0; i < size1; i++){
        if(str1[i] != str2[i]){
            return 1;
        }
    }
    return 0;
}


bool private_BearsslHttps_startswith(const char *str,const char *prefix){
    long size = private_BearsslHttps_strlen(prefix);
    for(long i = 0; i < size; i++){
        if(str[i] != prefix[i]){
            return false;
        }
    }
    return true;
}
char * private_BearsslHttps_strdup(const char *str){
    long size =  private_BearsslHttps_strlen(str);
    char *new_str = (char *)BearsslHttps_allocate(size+1);
    for(long i = 0; i < size; i++){
        new_str[i] = str[i];
    }
    new_str[size] = '\0';
    return new_str;
}

char * private_BearsslHttps_strcpy( char *dest,const char *str){
    long size =  private_BearsslHttps_strlen(str);
    for(long i = 0; i < size; i++){
        dest[i] = str[i];
    }
    return dest;
}
char * private_BearsslHttps_strndup(const char *str,int size){
    char *new_str = (char *)BearsslHttps_allocate(size+1);
    for(int i = 0; i < size; i++){
        new_str[i] = str[i];
    }
    new_str[size] = 0;
    return new_str;
}

int private_BearsslHttps_indexof_from_point(const char *str,char c,int start){
    long size = private_BearsslHttps_strlen(str);
    for(int i = start; i < size; i++){
        if(str[i] == c){
            return i;
        }
    }
    return -1;
}

char  private_BearsslHttps_parse_char_to_lower(char c){
    if(c >= 'A' && c <= 'Z'){
        return c + 32;
    }
    return c;
}