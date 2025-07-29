//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.dep_define.h"
//silver_chain_scope_end

struct privateBearHttpsStringArray * newprivateBearHttpsStringArray(){
    struct privateBearHttpsStringArray *self = ( privateBearHttpsStringArray*)malloc(sizeof(struct privateBearHttpsStringArray));
    self->size = 0;

    self->strings = (char**)malloc(1);

    return self;
}

int privateBearHttpsStringArray_find_position( privateBearHttpsStringArray *self, const char *string){
    for(int i = 0; i < self->size; i++){
        if(strcmp(self->strings[i], string) == 0){
            return i;
        }
    }
    return -1;
}


void privateBearHttpsStringArray_set_value( privateBearHttpsStringArray *self, int index, const char *value){
    if(index < self->size && index >= 0){
        int size = strlen(value);
        self->strings[index] = (char*)realloc(self->strings[index], size + 1);
        self->strings[index][size] = '\0';
        strcpy(self->strings[index], value);
    }
}
void privateBearHttpsStringArray_append_getting_ownership( privateBearHttpsStringArray *self, char *string){
    self->strings =  (char**)realloc(self->strings, (self->size+ 1) * sizeof(char*));
    self->strings[self->size] = string;
    self->size+=1;
}

// Function prototypes
void privateBearHttpsStringArray_append( privateBearHttpsStringArray *self, const  char *string){

    self->strings =  (char**)realloc(self->strings, (self->size+ 1) * sizeof(char*));
    self->strings[self->size] = strdup(string);
    self->size+=1;
}

void privateBearHttpsStringArray_pop( privateBearHttpsStringArray *self, int position){
    free(self->strings[position]);
    for(int i = position; i < self->size -1; i++){
        self->strings[i] = self->strings[i+1];
    }
    self->size-=1;
}

void privateBearHttpsStringArray_merge( privateBearHttpsStringArray *self,  privateBearHttpsStringArray *other){
    for(int i = 0; i < other->size; i++){
        privateBearHttpsStringArray_append(self, other->strings[i]);
    }
}


void privateBearHttpsStringArray_represent( privateBearHttpsStringArray *self){
    for(int i = 0; i < self->size; i++){
        printf("%s\n", self->strings[i]);
    }
}




 privateBearHttpsStringArray * privateBearHttpsStringArray_clone(privateBearHttpsStringArray *self){
    privateBearHttpsStringArray  *clone = newprivateBearHttpsStringArray();
    for(int i = 0; i< self->size; i++){
        privateBearHttpsStringArray_append(clone,self->strings[i]);
    }
    return clone;
}

char * privateprivateBearHttpsStringArray_append_if_not_included(privateBearHttpsStringArray *self,char *value){
    long position=privateBearHttpsStringArray_find_position(self,value);
    if(position != -1){
        free(value);
        return self->strings[position];
    }
    privateBearHttpsStringArray_append_getting_ownership(self,value);
    return value;
}
void privateBearHttpsStringArray_free(struct privateBearHttpsStringArray *self){
    for(int i = 0; i < self->size; i++){
            free(self->strings[i]);
    }

    free(self->strings);
    free(self);
}
