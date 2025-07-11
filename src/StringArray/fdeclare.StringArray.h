//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.typesH.h"
//silver_chain_scope_end
// End the structure with a semicolon
int  privateBearHttpsStringArray_find_position( privateBearHttpsStringArray *self, const char *string);


void privateBearHttpsStringArray_append_getting_ownership( privateBearHttpsStringArray *self, char *string);

void privateBearHttpsStringArray_append( privateBearHttpsStringArray *self, const char *string);

void privateBearHttpsStringArray_pop( privateBearHttpsStringArray *self, int position);

void privateBearHttpsStringArray_merge( privateBearHttpsStringArray *self,  privateBearHttpsStringArray *other);

void privateBearHttpsStringArray_represent( privateBearHttpsStringArray *self);

void privateBearHttpsStringArray_free( privateBearHttpsStringArray *self);


void privateBearHttpsStringArray_set_value( privateBearHttpsStringArray *self, int index, const char *value);

 privateBearHttpsStringArray * newprivateBearHttpsStringArray();

char * privateprivateBearHttpsStringArray_append_if_not_included(privateBearHttpsStringArray *self,char *value);

privateBearHttpsStringArray * privateBearHttpsStringArray_clone(privateBearHttpsStringArray *self);