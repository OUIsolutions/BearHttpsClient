
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end

long private_BearsslHttps_strlen(const char *str){
    long size = 0;
    while(str[size] != '\0'){
        size++;
    }
    return size;
}

char * private_BearsslHttps_strdup(const char *str){
    long size =  private_BearsslHttps_strlen(str);
    char *new_str = (char *)BearsslHttps_allocate(size+1);
    for(long i = 0; i < size; i++){
        new_str[i] = str[i];
    }
    return new_str;
}

char * private_BearsslHttps_strcpy( char *dest,char *str){
    long size =  private_BearsslHttps_strlen(str);
    for(long i = 0; i < size; i++){
        dest[i] = str[i];
    }
    return dest;
}
