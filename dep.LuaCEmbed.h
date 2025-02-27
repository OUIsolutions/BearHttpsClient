//path: src/one.c

#ifndef LUACEMBED_H
//path: src/imports/imports.fdefine.h
//path: src/imports/imports.fdeclare.h
//path: src/imports/imports.typesD.h
//path: src/imports/imports.typesC.h
//path: src/imports/imports.typesB.h
//path: src/imports/imports.types.h
//path: src/imports/imports.macros.h
//path: src/imports/imports.consts.h
//path: src/imports/imports.dependencies.h
#ifndef privateluacembed_dependencies
#define privateluacembed_dependencies
//path: src/imports/../dependencies.native.h
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif


//path: src/imports/../dependencies/dependencies.all.h
//path: src/imports/../dependencies/UniversalGarbage.h



#ifndef UNIVERSAL_GARBAGE_H


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>





#define UniversalGarbage_create_empty_struct(name,element_type) \
(element_type*)malloc(sizeof(element_type));    \
*name = (element_type){0};


#define UniversalGarbage_cast(value) ((void**)&value)


#define UniversalGarbage_add(garbage,deallocator_callback,value) \
    rawUniversalGarbage_add(garbage,(void*)deallocator_callback,UniversalGarbage_cast(value))

#define UniversalGarbage_add_simple(garbage,value) \
     UniversalGarbage_add(garbage,free,value)


#define UniversalGarbage_add_return(garbage,deallocator_callback,value) \
        UniversalGarbage_add(garbage->return_values,deallocator_callback,value)


#define UniversalGarbage_add_simple_return(garbage,value) \
    UniversalGarbage_add_simple(garbage->return_values,value)



#define  UniversalGarbage_remove(garbage,value) \
    rawUniversalGarbage_remove(garbage,UniversalGarbage_cast(value));


#define  UniversalGarbage_disconnect(garbage,value) \
    rawUniversalGarbage_disconnect(garbage,UniversalGarbage_cast(value));




#define UniversalGarbage_reallocate(garbage,value) \
    rawUniversalGarbage_reallocate(garbage,UniversalGarbage_cast(value))


#define UniversalGarbage_resset(garbage,value) \
    rawUniversalGarbage_resset(garbage,UniversalGarbage_cast(value))






typedef struct privateUniversalGarbageElement{
    void **pointer;
    void (*deallocator_callback)(void *element);
    void *pointed_value;
}privateUniversalGarbageElement;

void private_UniversalGarbageSimpleElement_free_pointed_value(privateUniversalGarbageElement *self);


void private_UniversalGarbageSimpleElement_free(privateUniversalGarbageElement *self);

privateUniversalGarbageElement * private_newUniversalGarbageSimpleElement(void (*dealocator_callback)(void *element), void **pointer);



typedef  struct UniversalGarbage{

    struct UniversalGarbage *return_values;
    privateUniversalGarbageElement **elements;
    int  elements_size;
    bool is_the_root;

}UniversalGarbage;

UniversalGarbage * newUniversalGarbage();

UniversalGarbage * private_new_MainUniversalGarbage();



bool  rawUniversalGarbage_resset(UniversalGarbage *self, void **pointer);

bool  rawUniversalGarbage_remove(UniversalGarbage *self, void **pointer);

bool  rawUniversalGarbage_disconnect(UniversalGarbage *self, void **pointer);

bool rawUniversalGarbage_reallocate(UniversalGarbage *self, void **pointer);

bool  rawUniversalGarbage_add(UniversalGarbage *self,  void *release_callback, void **pointer);

void private_UniversalGarbage_free_all_sub_elements(UniversalGarbage *self);

void UniversalGarbage_free_including_return(UniversalGarbage *self);

void UniversalGarbage_free(UniversalGarbage *self);







privateUniversalGarbageElement * private_newUniversalGarbageSimpleElement(void (*dealocator_callback)(void *element), void **pointer){

    privateUniversalGarbageElement * self = UniversalGarbage_create_empty_struct(
            self,
            privateUniversalGarbageElement
    );
    self->pointer = pointer;
    self->deallocator_callback = dealocator_callback;
    self->pointed_value = *pointer;
    return  self;
}
void private_UniversalGarbageSimpleElement_free_pointed_value(privateUniversalGarbageElement *self){
    if(self->pointed_value){
        self->deallocator_callback(self->pointed_value);
        self->pointed_value = NULL;
    }
}

void private_UniversalGarbageSimpleElement_free(privateUniversalGarbageElement *self){
    private_UniversalGarbageSimpleElement_free_pointed_value(self);
    free(self);
}


UniversalGarbage * private_new_MainUniversalGarbage(){
    UniversalGarbage *self = UniversalGarbage_create_empty_struct(self,UniversalGarbage)
    self->elements = (privateUniversalGarbageElement**)malloc(0);
    self->is_the_root = false;
    return self;
}

UniversalGarbage * newUniversalGarbage(){
    UniversalGarbage *self = UniversalGarbage_create_empty_struct(self,UniversalGarbage)
    self->is_the_root = true;
    self->elements = (privateUniversalGarbageElement**)malloc(0);
    self->return_values =private_new_MainUniversalGarbage();

    return self;
}




bool  rawUniversalGarbage_reallocate(UniversalGarbage *self, void **pointer){

    if(self->is_the_root){

        if(rawUniversalGarbage_reallocate(self->return_values,pointer)){
            return true;
        }
    }


    for(int i = 0; i < self->elements_size; i++){

        privateUniversalGarbageElement *current = self->elements[i];
        bool reallocate = current->pointer == pointer;

        if(reallocate){
            current->pointed_value = *pointer;
            return true;
        }
    }
    return false;
}

bool rawUniversalGarbage_resset(UniversalGarbage *self, void **pointer){

    if(self->is_the_root){
        if(rawUniversalGarbage_resset(self->return_values,pointer)){
            return true;
        }
    }


    for(int i = 0; i < self->elements_size; i++){
        privateUniversalGarbageElement *current = self->elements[i];
        bool resset = current->pointer == pointer;
        if(resset){
            private_UniversalGarbageSimpleElement_free_pointed_value(current);
            current->pointed_value = *pointer;
            return true;
        }
    }
    return  false;

}
bool  rawUniversalGarbage_remove(UniversalGarbage *self, void **pointer){
    if(self->is_the_root){
        if(rawUniversalGarbage_remove(self->return_values,pointer)){
            *pointer = NULL;
            return true;
        }
    }

    for(int i = 0; i < self->elements_size; i++){
        privateUniversalGarbageElement *current = self->elements[i];
        if(current->pointer == pointer){
            private_UniversalGarbageSimpleElement_free(current);
            self->elements_size-=1;
            bool its_not_the_last = i < self->elements_size;
            if(its_not_the_last){
                self->elements[i] = self->elements[self->elements_size];

            }
            *pointer = NULL;
            return  true;
        }
    }
    return  false;
}
bool  rawUniversalGarbage_disconnect(UniversalGarbage *self, void **pointer){
    if(self->is_the_root){
        if(rawUniversalGarbage_disconnect(self->return_values,pointer)){
            return true;
        }
    }

    for(int i = 0; i < self->elements_size; i++){
        privateUniversalGarbageElement *current = self->elements[i];
        if(current->pointer == pointer){
            free(current);
            self->elements_size-=1;
            bool its_not_the_last = i < self->elements_size;
            if(its_not_the_last){
                self->elements[i] = self->elements[self->elements_size];
            }
            return true;
        }
    }
    return  false;



}
bool  rawUniversalGarbage_add(UniversalGarbage *self, void *release_callback, void **pointer){

    if(!pointer){
        return false;
    }


    for(int i = 0; i < self->elements_size; i++){
        privateUniversalGarbageElement *current = self->elements[i];
        if(current->pointer == pointer){
            return false;
        }
    }

    self->elements = (privateUniversalGarbageElement**)realloc(
            self->elements,
            (self->elements_size + 1) * sizeof(privateUniversalGarbageElement*)
    );
    void (*dealocator_callback)(void *element);
#ifdef __cplusplus
    dealocator_callback =reinterpret_cast<void(*)(void*)>(release_callback);
#else
    dealocator_callback = (void*)(void*)release_callback;

#endif



    self->elements[self->elements_size] = private_newUniversalGarbageSimpleElement(dealocator_callback, pointer);
    self->elements_size+=1;
    return  true;
}



void  private_UniversalGarbage_free_all_sub_elements(UniversalGarbage *self){
    for(int i = 0; i < self->elements_size; i++){
        private_UniversalGarbageSimpleElement_free(self->elements[i]);
    }
    free(self->elements);
}

void UniversalGarbage_free_including_return(UniversalGarbage *self){
    private_UniversalGarbage_free_all_sub_elements(self);
    if(self->is_the_root){
        UniversalGarbage_free(self->return_values);
    }
    free(self);
}

void UniversalGarbage_free(UniversalGarbage *self){
    private_UniversalGarbage_free_all_sub_elements(self);

    if(self->is_the_root){

        UniversalGarbage *return_garbage = self->return_values;
        for(int i = 0; i < return_garbage->elements_size; i++){
            free(return_garbage->elements[i]);
        }

        free(return_garbage->elements);
        free(return_garbage);
    }


    free(self);
}




#define UNIVERSAL_GARBAGE_H
#endif



//path: src/imports/../dependencies/lua/onelua.c



#ifndef MAKE_LIB
#ifndef MAKE_LUAC
#ifndef MAKE_LUA
#define MAKE_LUA
#endif
#endif
#endif
#include <math.h>
//path: src/imports/../dependencies/lua/custom_math/custom_math.h
double private_lua_cembed_ldexp(double x, int exp);

double private_lua_cembed_frexp(double x, int *exp);

double private_lua_cembed_asin(double x);

double private_lua_cembed_acos(double x);


double private_lua_cembed_atan2(double y, double x);


double private_lua_cembed_ceil(double x);


double private_lua_cembed_sqrt(double x);

double private_lua_cembed_exp(double x);

double private_lua_cembed_fabs(double x);


double private_lua_cembed_factorial(int n);

double private_lua_cembed_sin(double x);


double private_lua_cembed_cos(double x);


double private_lua_cembed_tan(double x);

double private_lua_cembed_log(double x);

double private_lua_cembed_log2(double x);

double private_lua_embed_log10(double x);

double private_lua_embed_fmod(double x, double y);

double private_lua_embed_pow(double b, double e);

double private_lua_embed_floor(double x);

//path: src/imports/../dependencies/lua/custom_math/custom_math.c
double private_lua_cembed_ldexp(double x, int exp) {
    if (x == 0.0) return x;

    while (exp > 0) {
        x *= 2.0;
        exp--;
    }
    while (exp < 0) {
        x *= 0.5;
        exp++;
    }

    return x;
}

double private_lua_cembed_frexp(double x, int *exp) {
    *exp = 0;
    if (x == 0.0) return x;

    while (x >= 1.0 || x < -1.0) {
        x *= 0.5;
        (*exp)++;
    }
    while (x < 0.5 && x != 0.0) {
        x /= 0.5;
        (*exp)--;
    }

    return x;
}
double private_lua_cembed_asin(double x) {
    if (x <= -1.0 || x >= 1.0) {
        return x / 0.0;  // Retorna NaN para x fora do intervalo [-1, 1]
    }

    return private_lua_cembed_atan2(x, private_lua_cembed_sqrt(1.0 - x * x));
}

double private_lua_cembed_acos(double x) {
    if (x <= -1.0 || x >= 1.0) {
        return x / 0.0;  // Retorna NaN para x fora do intervalo [-1, 1]
    }

    return 1.5707963267948966 - private_lua_cembed_asin(x);
}

double private_lua_cembed_atan2(double y, double x) {
    if (x == 0.0 && y == 0.0) {
        return y / x;  // Retorna NaN para x = 0 e y = 0
    }

    double abs_y = private_lua_cembed_fabs(y);
    double angle;
    if (x >= 0.0) {
        double r = (x - abs_y) / (x + abs_y);
        angle = 0.7853981633974483 - 0.7853981633974483 * r;
    } else {
        double r = (x + abs_y) / (abs_y - x);
        angle = 2.356194490192345 + 0.7853981633974483 * r;
    }
    if (y < 0.0) {
        return -angle;
    } else {
        return angle;
    }
}

double private_lua_cembed_ceil(double x) {
    double f = private_lua_embed_floor(x);
    if (f >= x) {
        return f;
    } else {
        return f + 1.0;
    }
}

double private_lua_cembed_sqrt(double x) {
    if (x < 0.0) {
        return x / 0.0;  // Retorna NaN para x < 0
    }

    double x_half = 0.5 * x;
    long i = *(long*)&x;
    i = 0x5fe6ec85e7de30da - (i >> 1);
    x = *(double*)&i;
    x = x * (1.5 - x_half * x * x);

    return 1.0 / x;
}

double private_lua_cembed_exp(double x) {
    double result = 1.0;
    double term = 1.0;
    int i = 1;

    while (term > 1.0e-15) {
        term *= x / i;
        result += term;
        i++;
    }

    return result;
}

double private_lua_cembed_fabs(double x) {
    return x < 0.0 ? -x : x;
}

double private_lua_cembed_factorial(int n) {
    if (n <= 1) {
        return 1.0;
    }
    double result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double private_lua_cembed_sin(double x) {
    double result = 0.0;
    int n = 0;
    double term = x;
    double current_term = term;
    double term_squared = term * term;

    while (current_term > 1.0e-15) {
        result += current_term;
        n += 2;
        term *= -term_squared / ((n + 1) * (n + 2));
        current_term = term / private_lua_cembed_factorial(n);
    }

    return result;
}

double private_lua_cembed_cos(double x) {
    double result = 0.0;
    int n = 0;
    double term = 1.0;
    double current_term = term;
    double term_squared = term * term;

    while (current_term > 1.0e-15) {
        result += current_term;
        n += 2;
        term *= -term_squared / (n * (n + 1));
        current_term = term / private_lua_cembed_factorial(n);
    }

    return result;
}

double private_lua_cembed_tan(double x) {
    return private_lua_cembed_sin(x) / private_lua_cembed_cos(x);
}

double private_lua_cembed_log(double x) {
    if (x <= 0.0) {
        return -1.0 / 0.0;
    }

    double result = 0.0;
    double term = (x - 1) / (x + 1);
    double current_term = term;
    double term_squared = term * term;
    int n = 1;

    while (current_term > 1.0e-15) {
        result += current_term / n;
        current_term *= term_squared;
        n += 2;
    }

    result *= 2.302585092994046;
    return result;
}

double private_lua_cembed_log2(double x) {
    if (x <= 0.0) {
        return -1.0 / 0.0;
    }

    return private_lua_cembed_log(x) / private_lua_cembed_log(2.0);
}

double private_lua_embed_log10(double x) {
    if (x <= 0.0) {
        return -1.0 / 0.0;
    }

    return private_lua_cembed_log(x) / private_lua_cembed_log(10.0);
}

double private_lua_embed_fmod(double x, double y) {
    while (y != 0.0) {
        while (x >= y) {
            x -= y;
        }
        if (x < y) {
            return x;
        }
    }
    return 0.0 / 0.0;
}

double private_lua_embed_pow(double b, double e) {
    double result = 1.0;
    int i;

    if (e == 0.0) {
        return 1.0;
    }
    if (e < 0.0) {
        b = 1.0 / b;
        e = -e;
    }

    for (i = 0; i < e; i++) {
        result *= b;
    }

    return result;
}

double private_lua_embed_floor(double x) {
    int i = (int)x;

    if (x == (double)i) {
        return x;
    }

    if (x < 0.0) {
        return (double)(i - 1);
    }

    return (double)i;
}



#if 0
#define LUA_USE_LINUX
#define LUA_USE_MACOSX
#define LUA_USE_POSIX
#define LUA_ANSI
#endif




//path: src/imports/../dependencies/lua/lprefix.h


#ifndef lprefix_h
#define lprefix_h



#if !defined(LUA_USE_C89)	

#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE           600
#elif _XOPEN_SOURCE == 0
#undef _XOPEN_SOURCE  
#endif


#if !defined(LUA_32BITS) && !defined(_FILE_OFFSET_BITS)
#define _LARGEFILE_SOURCE       1
#define _FILE_OFFSET_BITS       64
#endif

#endif				



#if defined(_WIN32)	

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS  
#endif

#endif			

#endif




#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif


#define LUA_CORE
#define LUA_LIB
#define ltable_c
#define lvm_c
//path: src/imports/../dependencies/lua/luaconf.h



#ifndef luaconf_h
#define luaconf_h

#include <limits.h>
#include <stddef.h>












#if !defined(LUA_USE_C89) && defined(_WIN32) && !defined(_WIN32_WCE)
#define LUA_USE_WINDOWS  
#endif


#if defined(LUA_USE_WINDOWS)
#define LUA_DL_DLL	
#define LUA_USE_C89	
#endif


#if defined(LUA_USE_LINUX)
#define LUA_USE_POSIX
#define LUA_USE_DLOPEN		
#endif


#if defined(LUA_USE_MACOSX)
#define LUA_USE_POSIX
#define LUA_USE_DLOPEN		
#endif


#if defined(LUA_USE_IOS)
#define LUA_USE_POSIX
#define LUA_USE_DLOPEN
#endif



#define LUAI_IS32INT	((UINT_MAX >> 30) >= 3)










#define LUA_INT_INT		1
#define LUA_INT_LONG		2
#define LUA_INT_LONGLONG	3


#define LUA_FLOAT_FLOAT		1
#define LUA_FLOAT_DOUBLE	2
#define LUA_FLOAT_LONGDOUBLE	3



#define LUA_INT_DEFAULT		LUA_INT_LONGLONG
#define LUA_FLOAT_DEFAULT	LUA_FLOAT_DOUBLE



#define LUA_32BITS	0



#if defined(LUA_USE_C89) && !defined(LUA_USE_WINDOWS)
#define LUA_C89_NUMBERS		1
#else
#define LUA_C89_NUMBERS		0
#endif


#if LUA_32BITS		

#if LUAI_IS32INT  
#define LUA_INT_TYPE	LUA_INT_INT
#else  
#define LUA_INT_TYPE	LUA_INT_LONG
#endif
#define LUA_FLOAT_TYPE	LUA_FLOAT_FLOAT

#elif LUA_C89_NUMBERS	

#define LUA_INT_TYPE	LUA_INT_LONG
#define LUA_FLOAT_TYPE	LUA_FLOAT_DOUBLE

#else		


#define LUA_INT_TYPE	LUA_INT_DEFAULT
#define LUA_FLOAT_TYPE	LUA_FLOAT_DEFAULT

#endif				









#define LUA_PATH_SEP            ";"
#define LUA_PATH_MARK           "?"
#define LUA_EXEC_DIR            "!"




#define LUA_VDIR	LUA_VERSION_MAJOR "." LUA_VERSION_MINOR
#if defined(_WIN32)	

#define LUA_LDIR	"!\\lua\\"
#define LUA_CDIR	"!\\"
#define LUA_SHRDIR	"!\\..\\share\\lua\\" LUA_VDIR "\\"

#if !defined(LUA_PATH_DEFAULT)
#define LUA_PATH_DEFAULT  \
		LUA_LDIR"?.lua;"  LUA_LDIR"?\\init.lua;" \
		LUA_CDIR"?.lua;"  LUA_CDIR"?\\init.lua;" \
		LUA_SHRDIR"?.lua;" LUA_SHRDIR"?\\init.lua;" \
		".\\?.lua;" ".\\?\\init.lua"
#endif

#if !defined(LUA_CPATH_DEFAULT)
#define LUA_CPATH_DEFAULT \
		LUA_CDIR"?.dll;" \
		LUA_CDIR"..\\lib\\lua\\" LUA_VDIR "\\?.dll;" \
		LUA_CDIR"loadall.dll;" ".\\?.dll"
#endif

#else			

#define LUA_ROOT	"/usr/local/"
#define LUA_LDIR	LUA_ROOT "share/lua/" LUA_VDIR "/"
#define LUA_CDIR	LUA_ROOT "lib/lua/" LUA_VDIR "/"

#if !defined(LUA_PATH_DEFAULT)
#define LUA_PATH_DEFAULT  \
		LUA_LDIR"?.lua;"  LUA_LDIR"?/init.lua;" \
		LUA_CDIR"?.lua;"  LUA_CDIR"?/init.lua;" \
		"./?.lua;" "./?/init.lua"
#endif

#if !defined(LUA_CPATH_DEFAULT)
#define LUA_CPATH_DEFAULT \
		LUA_CDIR"?.so;" LUA_CDIR"loadall.so;" "./?.so"
#endif

#endif			



#if !defined(LUA_DIRSEP)

#if defined(_WIN32)
#define LUA_DIRSEP	"\\"
#else
#define LUA_DIRSEP	"/"
#endif

#endif



#define LUA_IGMARK		"-"







#if defined(LUA_BUILD_AS_DLL)	

#if defined(LUA_CORE) || defined(LUA_LIB)	
#define LUA_API __declspec(dllexport)
#else						
#define LUA_API __declspec(dllimport)
#endif						

#else				

#define LUA_API		extern

#endif				



#define LUALIB_API	LUA_API
#define LUAMOD_API	LUA_API



#if defined(__GNUC__) && ((__GNUC__*100 + __GNUC_MINOR__) >= 302) && \
    defined(__ELF__)		
#define LUAI_FUNC	__attribute__((visibility("internal"))) extern
#else				
#define LUAI_FUNC	extern
#endif				

#define LUAI_DDEC(dec)	LUAI_FUNC dec
#define LUAI_DDEF	







#if defined(LUA_COMPAT_5_3)	


#define LUA_COMPAT_MATHLIB


#define LUA_COMPAT_APIINTCASTS



#define LUA_COMPAT_LT_LE



#define lua_strlen(L,i)		lua_rawlen(L, (i))

#define lua_objlen(L,i)		lua_rawlen(L, (i))

#define lua_equal(L,idx1,idx2)		lua_compare(L,(idx1),(idx2),LUA_OPEQ)
#define lua_lessthan(L,idx1,idx2)	lua_compare(L,(idx1),(idx2),LUA_OPLT)

#endif				












#define l_floor(x)		(l_mathop(private_lua_embed_floor)(x))

#define lua_number2str(s,sz,n)  \
	l_sprintf((s), sz, LUA_NUMBER_FMT, (LUAI_UACNUMBER)(n))


#define lua_numbertointeger(n,p) \
  ((n) >= (LUA_NUMBER)(LUA_MININTEGER) && \
   (n) < -(LUA_NUMBER)(LUA_MININTEGER) && \
      (*(p) = (LUA_INTEGER)(n), 1))




#if LUA_FLOAT_TYPE == LUA_FLOAT_FLOAT		

#define LUA_NUMBER	float

#define l_floatatt(n)		(FLT_##n)

#define LUAI_UACNUMBER	double

#define LUA_NUMBER_FRMLEN	""
#define LUA_NUMBER_FMT		"%.7g"

#define l_mathop(op)		op##f

#define lua_str2number(s,p)	strtof((s), (p))


#elif LUA_FLOAT_TYPE == LUA_FLOAT_LONGDOUBLE	

#define LUA_NUMBER	long double

#define l_floatatt(n)		(LDBL_##n)

#define LUAI_UACNUMBER	long double

#define LUA_NUMBER_FRMLEN	"L"
#define LUA_NUMBER_FMT		"%.19Lg"

#define l_mathop(op)		op##l

#define lua_str2number(s,p)	strtold((s), (p))

#elif LUA_FLOAT_TYPE == LUA_FLOAT_DOUBLE	

#define LUA_NUMBER	double

#define l_floatatt(n)		(DBL_##n)

#define LUAI_UACNUMBER	double

#define LUA_NUMBER_FRMLEN	""
#define LUA_NUMBER_FMT		"%.14g"

#define l_mathop(op)		op

#define lua_str2number(s,p)	strtod((s), (p))

#else						

#error "numeric float type not defined"

#endif					








#define LUA_INTEGER_FMT		"%" LUA_INTEGER_FRMLEN "d"

#define LUAI_UACINT		LUA_INTEGER

#define lua_integer2str(s,sz,n)  \
	l_sprintf((s), sz, LUA_INTEGER_FMT, (LUAI_UACINT)(n))


#define LUA_UNSIGNED		unsigned LUAI_UACINT




#if LUA_INT_TYPE == LUA_INT_INT		

#define LUA_INTEGER		int
#define LUA_INTEGER_FRMLEN	""

#define LUA_MAXINTEGER		INT_MAX
#define LUA_MININTEGER		INT_MIN

#define LUA_MAXUNSIGNED		UINT_MAX

#elif LUA_INT_TYPE == LUA_INT_LONG	

#define LUA_INTEGER		long
#define LUA_INTEGER_FRMLEN	"l"

#define LUA_MAXINTEGER		LONG_MAX
#define LUA_MININTEGER		LONG_MIN

#define LUA_MAXUNSIGNED		ULONG_MAX

#elif LUA_INT_TYPE == LUA_INT_LONGLONG	


#if defined(LLONG_MAX)		


#define LUA_INTEGER		long long
#define LUA_INTEGER_FRMLEN	"ll"

#define LUA_MAXINTEGER		LLONG_MAX
#define LUA_MININTEGER		LLONG_MIN

#define LUA_MAXUNSIGNED		ULLONG_MAX

#elif defined(LUA_USE_WINDOWS) 


#define LUA_INTEGER		__int64
#define LUA_INTEGER_FRMLEN	"I64"

#define LUA_MAXINTEGER		_I64_MAX
#define LUA_MININTEGER		_I64_MIN

#define LUA_MAXUNSIGNED		_UI64_MAX

#else				

#error "Compiler does not support 'long long'. Use option '-DLUA_32BITS' \
  or '-DLUA_C89_NUMBERS' (see file 'luaconf.h' for details)"

#endif				

#else				

#error "numeric integer type not defined"

#endif				







#if !defined(LUA_USE_C89)
#define l_sprintf(s,sz,f,i)	snprintf(s,sz,f,i)
#else
#define l_sprintf(s,sz,f,i)	((void)(sz), sprintf(s,f,i))
#endif



#if !defined(LUA_USE_C89)
#define lua_strx2number(s,p)		lua_str2number(s,p)
#endif



#define lua_pointer2str(buff,sz,p)	l_sprintf(buff,sz,"%p",p)



#if !defined(LUA_USE_C89)
#define lua_number2strx(L,b,sz,f,n)  \
	((void)L, l_sprintf(b,sz,f,(LUAI_UACNUMBER)(n)))
#endif



#if defined(LUA_USE_C89) || (defined(HUGE_VAL) && !defined(HUGE_VALF))
#undef l_mathop  
#undef lua_str2number
#define l_mathop(op)		(lua_Number)op  
#define lua_str2number(s,p)	((lua_Number)strtod((s), (p)))
#endif



#define LUA_KCONTEXT	ptrdiff_t

#if !defined(LUA_USE_C89) && defined(__STDC_VERSION__) && \
    __STDC_VERSION__ >= 199901L
#include <stdint.h>
#if defined(INTPTR_MAX)  
#undef LUA_KCONTEXT
#define LUA_KCONTEXT	intptr_t
#endif
#endif



#if !defined(lua_getlocaledecpoint)
#define lua_getlocaledecpoint()		(localeconv()->decimal_point[0])
#endif



#if !defined(luai_likely)

#if defined(__GNUC__) && !defined(LUA_NOBUILTIN)
#define luai_likely(x)		(__builtin_expect(((x) != 0), 1))
#define luai_unlikely(x)	(__builtin_expect(((x) != 0), 0))
#else
#define luai_likely(x)		(x)
#define luai_unlikely(x)	(x)
#endif

#endif


#if defined(LUA_CORE) || defined(LUA_LIB)

#define l_likely(x)	luai_likely(x)
#define l_unlikely(x)	luai_unlikely(x)
#endif














#if defined(LUA_USE_APICHECK)
#include <assert.h>
#define luai_apicheck(l,e)	assert(e)
#endif







#if LUAI_IS32INT
#define LUAI_MAXSTACK		1000000
#else
#define LUAI_MAXSTACK		15000
#endif



#define LUA_EXTRASPACE		(sizeof(void *))



#define LUA_IDSIZE	60



#define LUAL_BUFFERSIZE   ((int)(16 * sizeof(void*) * sizeof(lua_Number)))



#define LUAI_MAXALIGN  lua_Number n; double u; void *s; lua_Integer i; long l















#endif





#undef LUAI_FUNC
#undef LUAI_DDEC
#undef LUAI_DDEF
#define LUAI_FUNC	static
#define LUAI_DDEC(def)	
#define LUAI_DDEF	static


//path: src/imports/../dependencies/lua/lzio.c


#define lzio_c
#define LUA_CORE



#include <string.h>

//path: src/imports/../dependencies/lua/lua.h



#ifndef lua_h
#define lua_h

#include <stdarg.h>
#include <stddef.h>


#define LUA_COPYRIGHT	LUA_RELEASE "  Copyright (C) 1994-2023 Lua.org, PUC-Rio"
#define LUA_AUTHORS	"R. Ierusalimschy, L. H. de Figueiredo, W. Celes"


#define LUA_VERSION_MAJOR_N	5
#define LUA_VERSION_MINOR_N	5
#define LUA_VERSION_RELEASE_N	0

#define LUA_VERSION_NUM  (LUA_VERSION_MAJOR_N * 100 + LUA_VERSION_MINOR_N)
#define LUA_VERSION_RELEASE_NUM  (LUA_VERSION_NUM * 100 + LUA_VERSION_RELEASE_N)







#define LUA_SIGNATURE	"\x1bLua"


#define LUA_MULTRET	(-1)



#define LUA_REGISTRYINDEX	(-LUAI_MAXSTACK - 1000)
#define lua_upvalueindex(i)	(LUA_REGISTRYINDEX - (i))



#define LUA_OK		0
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define LUA_ERRERR	5


typedef struct lua_State lua_State;



#define LUA_TNONE		(-1)

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define LUA_NUMTYPES		9




#define LUA_MINSTACK	20




#define LUA_RIDX_GLOBALS	2
#define LUA_RIDX_MAINTHREAD	3
#define LUA_RIDX_LAST		3



typedef LUA_NUMBER lua_Number;



typedef LUA_INTEGER lua_Integer;


typedef LUA_UNSIGNED lua_Unsigned;


typedef LUA_KCONTEXT lua_KContext;



typedef int (*lua_CFunction) (lua_State *L);


typedef int (*lua_KFunction) (lua_State *L, int status, lua_KContext ctx);



typedef const char * (*lua_Reader) (lua_State *L, void *ud, size_t *sz);

typedef int (*lua_Writer) (lua_State *L, const void *p, size_t sz, void *ud);



typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);



typedef void (*lua_WarnFunction) (void *ud, const char *msg, int tocont);



typedef struct lua_Debug lua_Debug;



typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);




extern const char lua_ident[];



LUA_API lua_State *(lua_newstate) (lua_Alloc f, void *ud, unsigned seed);
LUA_API void       (lua_close) (lua_State *L);
LUA_API lua_State *(lua_newthread) (lua_State *L);
LUA_API int        (lua_closethread) (lua_State *L, lua_State *from);

LUA_API lua_CFunction (lua_atpanic) (lua_State *L, lua_CFunction panicf);


LUA_API lua_Number (lua_version) (lua_State *L);



LUA_API int   (lua_absindex) (lua_State *L, int idx);
LUA_API int   (lua_gettop) (lua_State *L);
LUA_API void  (lua_settop) (lua_State *L, int idx);
LUA_API void  (lua_pushvalue) (lua_State *L, int idx);
LUA_API void  (lua_rotate) (lua_State *L, int idx, int n);
LUA_API void  (lua_copy) (lua_State *L, int fromidx, int toidx);
LUA_API int   (lua_checkstack) (lua_State *L, int n);

LUA_API void  (lua_xmove) (lua_State *from, lua_State *to, int n);




LUA_API int             (lua_isnumber) (lua_State *L, int idx);
LUA_API int             (lua_isstring) (lua_State *L, int idx);
LUA_API int             (lua_iscfunction) (lua_State *L, int idx);
LUA_API int             (lua_isinteger) (lua_State *L, int idx);
LUA_API int             (lua_isuserdata) (lua_State *L, int idx);
LUA_API int             (lua_type) (lua_State *L, int idx);
LUA_API const char     *(lua_typename) (lua_State *L, int tp);

LUA_API lua_Number      (lua_tonumberx) (lua_State *L, int idx, int *isnum);
LUA_API lua_Integer     (lua_tointegerx) (lua_State *L, int idx, int *isnum);
LUA_API int             (lua_toboolean) (lua_State *L, int idx);
LUA_API const char     *(lua_tolstring) (lua_State *L, int idx, size_t *len);
LUA_API lua_Unsigned    (lua_rawlen) (lua_State *L, int idx);
LUA_API lua_CFunction   (lua_tocfunction) (lua_State *L, int idx);
LUA_API void	       *(lua_touserdata) (lua_State *L, int idx);
LUA_API lua_State      *(lua_tothread) (lua_State *L, int idx);
LUA_API const void     *(lua_topointer) (lua_State *L, int idx);




#define LUA_OPADD	0	
#define LUA_OPSUB	1
#define LUA_OPMUL	2
#define LUA_OPMOD	3
#define LUA_OPPOW	4
#define LUA_OPDIV	5
#define LUA_OPIDIV	6
#define LUA_OPBAND	7
#define LUA_OPBOR	8
#define LUA_OPBXOR	9
#define LUA_OPSHL	10
#define LUA_OPSHR	11
#define LUA_OPUNM	12
#define LUA_OPBNOT	13

LUA_API void  (lua_arith) (lua_State *L, int op);

#define LUA_OPEQ	0
#define LUA_OPLT	1
#define LUA_OPLE	2

LUA_API int   (lua_rawequal) (lua_State *L, int idx1, int idx2);
LUA_API int   (lua_compare) (lua_State *L, int idx1, int idx2, int op);



LUA_API void        (lua_pushnil) (lua_State *L);
LUA_API void        (lua_pushnumber) (lua_State *L, lua_Number n);
LUA_API void        (lua_pushinteger) (lua_State *L, lua_Integer n);
LUA_API const char *(lua_pushlstring) (lua_State *L, const char *s, size_t len);
LUA_API const char *(lua_pushextlstring) (lua_State *L,
		const char *s, size_t len, lua_Alloc falloc, void *ud);
LUA_API const char *(lua_pushstring) (lua_State *L, const char *s);
LUA_API const char *(lua_pushvfstring) (lua_State *L, const char *fmt,
                                                      va_list argp);
LUA_API const char *(lua_pushfstring) (lua_State *L, const char *fmt, ...);
LUA_API void  (lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n);
LUA_API void  (lua_pushboolean) (lua_State *L, int b);
LUA_API void  (lua_pushlightuserdata) (lua_State *L, void *p);
LUA_API int   (lua_pushthread) (lua_State *L);



LUA_API int (lua_getglobal) (lua_State *L, const char *name);
LUA_API int (lua_gettable) (lua_State *L, int idx);
LUA_API int (lua_getfield) (lua_State *L, int idx, const char *k);
LUA_API int (lua_geti) (lua_State *L, int idx, lua_Integer n);
LUA_API int (lua_rawget) (lua_State *L, int idx);
LUA_API int (lua_rawgeti) (lua_State *L, int idx, lua_Integer n);
LUA_API int (lua_rawgetp) (lua_State *L, int idx, const void *p);

LUA_API void  (lua_createtable) (lua_State *L, unsigned narr, unsigned nrec);
LUA_API void *(lua_newuserdatauv) (lua_State *L, size_t sz, int nuvalue);
LUA_API int   (lua_getmetatable) (lua_State *L, int objindex);
LUA_API int  (lua_getiuservalue) (lua_State *L, int idx, int n);



LUA_API void  (lua_setglobal) (lua_State *L, const char *name);
LUA_API void  (lua_settable) (lua_State *L, int idx);
LUA_API void  (lua_setfield) (lua_State *L, int idx, const char *k);
LUA_API void  (lua_seti) (lua_State *L, int idx, lua_Integer n);
LUA_API void  (lua_rawset) (lua_State *L, int idx);
LUA_API void  (lua_rawseti) (lua_State *L, int idx, lua_Integer n);
LUA_API void  (lua_rawsetp) (lua_State *L, int idx, const void *p);
LUA_API int   (lua_setmetatable) (lua_State *L, int objindex);
LUA_API int   (lua_setiuservalue) (lua_State *L, int idx, int n);



LUA_API void  (lua_callk) (lua_State *L, int nargs, int nresults,
                           lua_KContext ctx, lua_KFunction k);
#define lua_call(L,n,r)		lua_callk(L, (n), (r), 0, NULL)

LUA_API int   (lua_pcallk) (lua_State *L, int nargs, int nresults, int errfunc,
                            lua_KContext ctx, lua_KFunction k);
#define lua_pcall(L,n,r,f)	lua_pcallk(L, (n), (r), (f), 0, NULL)

LUA_API int   (lua_load) (lua_State *L, lua_Reader reader, void *dt,
                          const char *chunkname, const char *mode);

LUA_API int (lua_dump) (lua_State *L, lua_Writer writer, void *data, int strip);



LUA_API int  (lua_yieldk)     (lua_State *L, int nresults, lua_KContext ctx,
                               lua_KFunction k);
LUA_API int  (lua_resume)     (lua_State *L, lua_State *from, int narg,
                               int *nres);
LUA_API int  (lua_status)     (lua_State *L);
LUA_API int (lua_isyieldable) (lua_State *L);

#define lua_yield(L,n)		lua_yieldk(L, (n), 0, NULL)



LUA_API void (lua_setwarnf) (lua_State *L, lua_WarnFunction f, void *ud);
LUA_API void (lua_warning)  (lua_State *L, const char *msg, int tocont);




#define LUA_GCSTOP		0
#define LUA_GCRESTART		1
#define LUA_GCCOLLECT		2
#define LUA_GCCOUNT		3
#define LUA_GCCOUNTB		4
#define LUA_GCSTEP		5
#define LUA_GCISRUNNING		6
#define LUA_GCGEN		7
#define LUA_GCINC		8
#define LUA_GCPARAM		9




#define LUA_GCPMINORMUL		0  
#define LUA_GCPMAJORMINOR	1  
#define LUA_GCPMINORMAJOR	2  


#define LUA_GCPPAUSE		3  
#define LUA_GCPSTEPMUL		4  
#define LUA_GCPSTEPSIZE		5  


#define LUA_GCPN		6


LUA_API int (lua_gc) (lua_State *L, int what, ...);




LUA_API int   (lua_error) (lua_State *L);

LUA_API int   (lua_next) (lua_State *L, int idx);

LUA_API void  (lua_concat) (lua_State *L, int n);
LUA_API void  (lua_len)    (lua_State *L, int idx);

LUA_API size_t   (lua_stringtonumber) (lua_State *L, const char *s);

LUA_API lua_Alloc (lua_getallocf) (lua_State *L, void **ud);
LUA_API void      (lua_setallocf) (lua_State *L, lua_Alloc f, void *ud);

LUA_API void (lua_toclose) (lua_State *L, int idx);
LUA_API void (lua_closeslot) (lua_State *L, int idx);




#define lua_getextraspace(L)	((void *)((char *)(L) - LUA_EXTRASPACE))

#define lua_tonumber(L,i)	lua_tonumberx(L,(i),NULL)
#define lua_tointeger(L,i)	lua_tointegerx(L,(i),NULL)

#define lua_pop(L,n)		lua_settop(L, -(n)-1)

#define lua_newtable(L)		lua_createtable(L, 0, 0)

#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))

#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)

#define lua_isfunction(L,n)	(lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)	(lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)	(lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)		(lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type(L, (n)) <= 0)

#define lua_pushliteral(L, s)	lua_pushstring(L, "" s)

#define lua_pushglobaltable(L)  \
	((void)lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS))

#define lua_tostring(L,i)	lua_tolstring(L, (i), NULL)


#define lua_insert(L,idx)	lua_rotate(L, (idx), 1)

#define lua_remove(L,idx)	(lua_rotate(L, (idx), -1), lua_pop(L, 1))

#define lua_replace(L,idx)	(lua_copy(L, -1, (idx)), lua_pop(L, 1))





#if defined(LUA_COMPAT_APIINTCASTS)

#define lua_pushunsigned(L,n)	lua_pushinteger(L, (lua_Integer)(n))
#define lua_tounsignedx(L,i,is)	((lua_Unsigned)lua_tointegerx(L,i,is))
#define lua_tounsigned(L,i)	lua_tounsignedx(L,(i),NULL)

#endif

#define lua_newuserdata(L,s)	lua_newuserdatauv(L,s,1)
#define lua_getuservalue(L,idx)	lua_getiuservalue(L,idx,1)
#define lua_setuservalue(L,idx)	lua_setiuservalue(L,idx,1)

#define lua_resetthread(L)	lua_closethread(L,NULL)







#define LUA_HOOKCALL	0
#define LUA_HOOKRET	1
#define LUA_HOOKLINE	2
#define LUA_HOOKCOUNT	3
#define LUA_HOOKTAILCALL 4



#define LUA_MASKCALL	(1 << LUA_HOOKCALL)
#define LUA_MASKRET	(1 << LUA_HOOKRET)
#define LUA_MASKLINE	(1 << LUA_HOOKLINE)
#define LUA_MASKCOUNT	(1 << LUA_HOOKCOUNT)


LUA_API int (lua_getstack) (lua_State *L, int level, lua_Debug *ar);
LUA_API int (lua_getinfo) (lua_State *L, const char *what, lua_Debug *ar);
LUA_API const char *(lua_getlocal) (lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *(lua_setlocal) (lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *(lua_getupvalue) (lua_State *L, int funcindex, int n);
LUA_API const char *(lua_setupvalue) (lua_State *L, int funcindex, int n);

LUA_API void *(lua_upvalueid) (lua_State *L, int fidx, int n);
LUA_API void  (lua_upvaluejoin) (lua_State *L, int fidx1, int n1,
                                               int fidx2, int n2);

LUA_API void (lua_sethook) (lua_State *L, lua_Hook func, int mask, int count);
LUA_API lua_Hook (lua_gethook) (lua_State *L);
LUA_API int (lua_gethookmask) (lua_State *L);
LUA_API int (lua_gethookcount) (lua_State *L);


struct lua_Debug {
  int event;
  const char *name;	
  const char *namewhat;	
  const char *what;	
  const char *source;	
  size_t srclen;	
  int currentline;	
  int linedefined;	
  int lastlinedefined;	
  unsigned char nups;	
  unsigned char nparams;
  char isvararg;        
  char istailcall;	
  unsigned short ftransfer;   
  unsigned short ntransfer;   
  char short_src[LUA_IDSIZE]; 
  
  struct CallInfo *i_ci;  
};




#define LUAI_TOSTRAUX(x)	#x
#define LUAI_TOSTR(x)		LUAI_TOSTRAUX(x)

#define LUA_VERSION_MAJOR	LUAI_TOSTR(LUA_VERSION_MAJOR_N)
#define LUA_VERSION_MINOR	LUAI_TOSTR(LUA_VERSION_MINOR_N)
#define LUA_VERSION_RELEASE	LUAI_TOSTR(LUA_VERSION_RELEASE_N)

#define LUA_VERSION	"Lua " LUA_VERSION_MAJOR "." LUA_VERSION_MINOR
#define LUA_RELEASE	LUA_VERSION "." LUA_VERSION_RELEASE





#endif



//path: src/imports/../dependencies/lua/llimits.h


#ifndef llimits_h
#define llimits_h


#include <limits.h>
#include <stddef.h>







#if defined(LUAI_MEM)		
typedef LUAI_UMEM lu_mem;
typedef LUAI_MEM l_obj;
#elif LUAI_IS32INT	
typedef size_t lu_mem;
typedef ptrdiff_t l_obj;
#else  	
typedef unsigned long lu_mem;
typedef long l_obj;
#endif				

#define MAX_LOBJ  \
	cast(l_obj, (cast(lu_mem, 1) << (sizeof(l_obj) * CHAR_BIT - 1)) - 1)



typedef unsigned char lu_byte;
typedef signed char ls_byte;



#define MAX_SIZET	((size_t)(~(size_t)0))


#define MAX_SIZE	(sizeof(size_t) < sizeof(lua_Integer) ? MAX_SIZET \
                          : (size_t)(LUA_MAXINTEGER))


#define MAX_INT		INT_MAX  



#define log2maxs(t)	cast_int(sizeof(t) * 8 - 2)



#define ispow2(x)	(((x) & ((x) - 1)) == 0)



#define LL(x)   (sizeof(x)/sizeof(char) - 1)



#if !defined(LUA_USE_C89) && defined(__STDC_VERSION__) && \
    __STDC_VERSION__ >= 199901L
#include <stdint.h>
#if defined(UINTPTR_MAX)  
#define L_P2I	uintptr_t
#else  
#define L_P2I	uintmax_t  
#endif
#else  
#define L_P2I	size_t
#endif

#define point2uint(p)	cast_uint((L_P2I)(p) & UINT_MAX)




typedef LUAI_UACNUMBER l_uacNumber;
typedef LUAI_UACINT l_uacInt;



#if defined LUAI_ASSERT
#undef NDEBUG
#include <assert.h>
#define lua_assert(c)           assert(c)
#endif

#if defined(lua_assert)
#define check_exp(c,e)		(lua_assert(c), (e))

#define lua_longassert(c)	((c) ? (void)0 : lua_assert(0))
#else
#define lua_assert(c)		((void)0)
#define check_exp(c,e)		(e)
#define lua_longassert(c)	((void)0)
#endif


#if !defined(luai_apicheck)
#define luai_apicheck(l,e)	((void)l, lua_assert(e))
#endif

#define api_check(l,e,msg)	luai_apicheck(l,(e) && msg)



#if !defined(UNUSED)
#define UNUSED(x)	((void)(x))
#endif



#define cast(t, exp)	((t)(exp))

#define cast_void(i)	cast(void, (i))
#define cast_voidp(i)	cast(void *, (i))
#define cast_num(i)	cast(lua_Number, (i))
#define cast_int(i)	cast(int, (i))
#define cast_uint(i)	cast(unsigned int, (i))
#define cast_byte(i)	cast(lu_byte, (i))
#define cast_uchar(i)	cast(unsigned char, (i))
#define cast_char(i)	cast(char, (i))
#define cast_charp(i)	cast(char *, (i))
#define cast_sizet(i)	cast(size_t, (i))



#if !defined(l_castS2U)
#define l_castS2U(i)	((lua_Unsigned)(i))
#endif


#if !defined(l_castU2S)
#define l_castU2S(i)	((lua_Integer)(i))
#endif



#if !defined(l_noret)

#if defined(__GNUC__)
#define l_noret		void __attribute__((noreturn))
#elif defined(_MSC_VER) && _MSC_VER >= 1200
#define l_noret		void __declspec(noreturn)
#else
#define l_noret		void
#endif

#endif



#if !defined(LUA_USE_C89)
#define l_inline	inline
#elif defined(__GNUC__)
#define l_inline	__inline__
#else
#define l_inline	
#endif

#define l_sinline	static l_inline



#if LUAI_IS32INT
typedef unsigned int l_uint32;
#else
typedef unsigned long l_uint32;
#endif

typedef l_uint32 Instruction;




#if !defined(LUAI_MAXSHORTLEN)
#define LUAI_MAXSHORTLEN	40
#endif



#if !defined(MINSTRTABSIZE)
#define MINSTRTABSIZE	128
#endif



#if !defined(STRCACHE_N)
#define STRCACHE_N		53
#define STRCACHE_M		2
#endif



#if !defined(LUA_MINBUFFER)
#define LUA_MINBUFFER	32
#endif



#if !defined(LUAI_MAXCCALLS)
#define LUAI_MAXCCALLS		200
#endif



#if !defined(lua_lock)
#define lua_lock(L)	((void) 0)
#define lua_unlock(L)	((void) 0)
#endif


#if !defined(luai_threadyield)
#define luai_threadyield(L)	{lua_unlock(L); lua_lock(L);}
#endif



#if !defined(luai_userstateopen)
#define luai_userstateopen(L)		((void)L)
#endif

#if !defined(luai_userstateclose)
#define luai_userstateclose(L)		((void)L)
#endif

#if !defined(luai_userstatethread)
#define luai_userstatethread(L,L1)	((void)L)
#endif

#if !defined(luai_userstatefree)
#define luai_userstatefree(L,L1)	((void)L)
#endif

#if !defined(luai_userstateresume)
#define luai_userstateresume(L,n)	((void)L)
#endif

#if !defined(luai_userstateyield)
#define luai_userstateyield(L,n)	((void)L)
#endif






#if !defined(luai_numidiv)
#define luai_numidiv(L,a,b)     ((void)L, l_floor(luai_numdiv(L,a,b)))
#endif


#if !defined(luai_numdiv)
#define luai_numdiv(L,a,b)      ((a)/(b))
#endif


#if !defined(luai_nummod)
#define luai_nummod(L,a,b,m)  \
  { (void)L; (m) = l_mathop(private_lua_embed_fmod)(a,b); \
    if (((m) > 0) ? (b) < 0 : ((m) < 0 && (b) > 0)) (m) += (b); }
#endif


#if !defined(luai_numpow)
#define luai_numpow(L,a,b)  \
  ((void)L, (b == 2) ? (a)*(a) : l_mathop(private_lua_embed_pow)(a,b))
#endif


#if !defined(luai_numadd)
#define luai_numadd(L,a,b)      ((a)+(b))
#define luai_numsub(L,a,b)      ((a)-(b))
#define luai_nummul(L,a,b)      ((a)*(b))
#define luai_numunm(L,a)        (-(a))
#define luai_numeq(a,b)         ((a)==(b))
#define luai_numlt(a,b)         ((a)<(b))
#define luai_numle(a,b)         ((a)<=(b))
#define luai_numgt(a,b)         ((a)>(b))
#define luai_numge(a,b)         ((a)>=(b))
#define luai_numisnan(a)        (!luai_numeq((a), (a)))
#endif






#if !defined(HARDSTACKTESTS)
#define condmovestack(L,pre,pos)	((void)0)
#else

#define condmovestack(L,pre,pos)  \
  { int sz_ = stacksize(L); pre; luaD_reallocstack((L), sz_, 0); pos; }
#endif

#if !defined(HARDMEMTESTS)
#define condchangemem(L,pre,pos)	((void)0)
#else
#define condchangemem(L,pre,pos)  \
	{ if (gcrunning(G(L))) { pre; luaC_fullgc(L, 0); pos; } }
#endif

#endif


//path: src/imports/../dependencies/lua/lmem.h


#ifndef lmem_h
#define lmem_h


#include <stddef.h>







#define luaM_error(L)	luaD_throw(L, LUA_ERRMEM)



#define luaM_testsize(n,e)  \
	(sizeof(n) >= sizeof(size_t) && cast_sizet((n)) + 1 > MAX_SIZET/(e))

#define luaM_checksize(L,n,e)  \
	(luaM_testsize(n,e) ? luaM_toobig(L) : cast_void(0))



#define luaM_limitN(n,t)  \
  ((cast_sizet(n) <= MAX_SIZET/sizeof(t)) ? (n) :  \
     cast_uint((MAX_SIZET/sizeof(t))))



#define luaM_reallocvchar(L,b,on,n)  \
  cast_charp(luaM_saferealloc_(L, (b), (on)*sizeof(char), (n)*sizeof(char)))

#define luaM_freemem(L, b, s)	luaM_free_(L, (b), (s))
#define luaM_free(L, b)		luaM_free_(L, (b), sizeof(*(b)))
#define luaM_freearray(L, b, n)   luaM_free_(L, (b), (n)*sizeof(*(b)))

#define luaM_new(L,t)		cast(t*, luaM_malloc_(L, sizeof(t), 0))
#define luaM_newvector(L,n,t)	cast(t*, luaM_malloc_(L, (n)*sizeof(t), 0))
#define luaM_newvectorchecked(L,n,t) \
  (luaM_checksize(L,n,sizeof(t)), luaM_newvector(L,n,t))

#define luaM_newobject(L,tag,s)	luaM_malloc_(L, (s), tag)

#define luaM_newblock(L, size)	luaM_newvector(L, size, char)

#define luaM_growvector(L,v,nelems,size,t,limit,e) \
	((v)=cast(t *, luaM_growaux_(L,v,nelems,&(size),sizeof(t), \
                         luaM_limitN(limit,t),e)))

#define luaM_reallocvector(L, v,oldn,n,t) \
   (cast(t *, luaM_realloc_(L, v, cast_sizet(oldn) * sizeof(t), \
                                  cast_sizet(n) * sizeof(t))))

#define luaM_shrinkvector(L,v,size,fs,t) \
   ((v)=cast(t *, luaM_shrinkvector_(L, v, &(size), fs, sizeof(t))))

LUAI_FUNC l_noret luaM_toobig (lua_State *L);


LUAI_FUNC void *luaM_realloc_ (lua_State *L, void *block, size_t oldsize,
                                                          size_t size);
LUAI_FUNC void *luaM_saferealloc_ (lua_State *L, void *block, size_t oldsize,
                                                              size_t size);
LUAI_FUNC void luaM_free_ (lua_State *L, void *block, size_t osize);
LUAI_FUNC void *luaM_growaux_ (lua_State *L, void *block, int nelems,
                               int *size, int size_elem, int limit,
                               const char *what);
LUAI_FUNC void *luaM_shrinkvector_ (lua_State *L, void *block, int *nelem,
                                    int final_n, int size_elem);
LUAI_FUNC void *luaM_malloc_ (lua_State *L, size_t size, int tag);

#endif



//path: src/imports/../dependencies/lua/lstate.h


#ifndef lstate_h
#define lstate_h






typedef struct CallInfo CallInfo;


//path: src/imports/../dependencies/lua/lobject.h



#ifndef lobject_h
#define lobject_h


#include <stdarg.h>









#define LUA_TUPVAL	LUA_NUMTYPES  
#define LUA_TPROTO	(LUA_NUMTYPES+1)  
#define LUA_TDEADKEY	(LUA_NUMTYPES+2)  




#define LUA_TOTALTYPES		(LUA_TPROTO + 2)





#define makevariant(t,v)	((t) | ((v) << 4))




typedef union Value {
  struct GCObject *gc;    
  void *p;         
  lua_CFunction f; 
  lua_Integer i;   
  lua_Number n;    
  
  lu_byte ub;
} Value;




#define TValuefields	Value value_; lu_byte tt_

typedef struct TValue {
  TValuefields;
} TValue;


#define val_(o)		((o)->value_)
#define valraw(o)	(val_(o))



#define rawtt(o)	((o)->tt_)


#define novariant(t)	((t) & 0x0F)


#define withvariant(t)	((t) & 0x3F)
#define ttypetag(o)	withvariant(rawtt(o))


#define ttype(o)	(novariant(rawtt(o)))



#define checktag(o,t)		(rawtt(o) == (t))
#define checktype(o,t)		(ttype(o) == (t))





#define righttt(obj)		(ttypetag(obj) == gcvalue(obj)->tt)


#define checkliveness(L,obj) \
	((void)L, lua_longassert(!iscollectable(obj) || \
		(righttt(obj) && (L == NULL || !isdead(G(L),gcvalue(obj))))))





#define settt_(o,t)	((o)->tt_=(t))



#define setobj(L,obj1,obj2) \
	{ TValue *io1=(obj1); const TValue *io2=(obj2); \
          io1->value_ = io2->value_; settt_(io1, io2->tt_); \
	  checkliveness(L,io1); lua_assert(!isnonstrictnil(io1)); }




#define setobjs2s(L,o1,o2)	setobj(L,s2v(o1),s2v(o2))

#define setobj2s(L,o1,o2)	setobj(L,s2v(o1),o2)

#define setobjt2t	setobj

#define setobj2n	setobj

#define setobj2t	setobj



typedef union StackValue {
  TValue val;
  struct {
    TValuefields;
    unsigned short delta;
  } tbclist;
} StackValue;



typedef StackValue *StkId;



typedef union {
  StkId p;  
  ptrdiff_t offset;  
} StkIdRel;



#define s2v(o)	(&(o)->val)






#define LUA_VNIL	makevariant(LUA_TNIL, 0)


#define LUA_VEMPTY	makevariant(LUA_TNIL, 1)


#define LUA_VABSTKEY	makevariant(LUA_TNIL, 2)


#define LUA_VNOTABLE    makevariant(LUA_TNIL, 3)



#define ttisnil(v)		checktype((v), LUA_TNIL)


#define tagisempty(tag)		(novariant(tag) == LUA_TNIL)



#define ttisstrictnil(o)	checktag((o), LUA_VNIL)


#define setnilvalue(obj) settt_(obj, LUA_VNIL)


#define isabstkey(v)		checktag((v), LUA_VABSTKEY)



#define isnonstrictnil(v)	(ttisnil(v) && !ttisstrictnil(v))



#define isempty(v)		ttisnil(v)



#define ABSTKEYCONSTANT		{NULL}, LUA_VABSTKEY



#define setempty(v)		settt_(v, LUA_VEMPTY)









#define LUA_VFALSE	makevariant(LUA_TBOOLEAN, 0)
#define LUA_VTRUE	makevariant(LUA_TBOOLEAN, 1)

#define ttisboolean(o)		checktype((o), LUA_TBOOLEAN)
#define ttisfalse(o)		checktag((o), LUA_VFALSE)
#define ttistrue(o)		checktag((o), LUA_VTRUE)


#define l_isfalse(o)	(ttisfalse(o) || ttisnil(o))
#define tagisfalse(t)	((t) == LUA_VFALSE || novariant(t) == LUA_TNIL)



#define setbfvalue(obj)		settt_(obj, LUA_VFALSE)
#define setbtvalue(obj)		settt_(obj, LUA_VTRUE)






#define LUA_VTHREAD		makevariant(LUA_TTHREAD, 0)

#define ttisthread(o)		checktag((o), ctb(LUA_VTHREAD))

#define thvalue(o)	check_exp(ttisthread(o), gco2th(val_(o).gc))

#define setthvalue(L,obj,x) \
  { TValue *io = (obj); lua_State *x_ = (x); \
    val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VTHREAD)); \
    checkliveness(L,io); }

#define setthvalue2s(L,o,t)	setthvalue(L,s2v(o),t)







#define CommonHeader	struct GCObject *next; lu_byte tt; lu_byte marked



typedef struct GCObject {
  CommonHeader;
} GCObject;



#define BIT_ISCOLLECTABLE	(1 << 6)

#define iscollectable(o)	(rawtt(o) & BIT_ISCOLLECTABLE)


#define ctb(t)			((t) | BIT_ISCOLLECTABLE)

#define gcvalue(o)	check_exp(iscollectable(o), val_(o).gc)

#define gcvalueraw(v)	((v).gc)

#define setgcovalue(L,obj,x) \
  { TValue *io = (obj); GCObject *i_g=(x); \
    val_(io).gc = i_g; settt_(io, ctb(i_g->tt)); }







#define LUA_VNUMINT	makevariant(LUA_TNUMBER, 0)  
#define LUA_VNUMFLT	makevariant(LUA_TNUMBER, 1)  

#define ttisnumber(o)		checktype((o), LUA_TNUMBER)
#define ttisfloat(o)		checktag((o), LUA_VNUMFLT)
#define ttisinteger(o)		checktag((o), LUA_VNUMINT)

#define nvalue(o)	check_exp(ttisnumber(o), \
	(ttisinteger(o) ? cast_num(ivalue(o)) : fltvalue(o)))
#define fltvalue(o)	check_exp(ttisfloat(o), val_(o).n)
#define ivalue(o)	check_exp(ttisinteger(o), val_(o).i)

#define fltvalueraw(v)	((v).n)
#define ivalueraw(v)	((v).i)

#define setfltvalue(obj,x) \
  { TValue *io=(obj); val_(io).n=(x); settt_(io, LUA_VNUMFLT); }

#define chgfltvalue(obj,x) \
  { TValue *io=(obj); lua_assert(ttisfloat(io)); val_(io).n=(x); }

#define setivalue(obj,x) \
  { TValue *io=(obj); val_(io).i=(x); settt_(io, LUA_VNUMINT); }

#define chgivalue(obj,x) \
  { TValue *io=(obj); lua_assert(ttisinteger(io)); val_(io).i=(x); }







#define LUA_VSHRSTR	makevariant(LUA_TSTRING, 0)  
#define LUA_VLNGSTR	makevariant(LUA_TSTRING, 1)  

#define ttisstring(o)		checktype((o), LUA_TSTRING)
#define ttisshrstring(o)	checktag((o), ctb(LUA_VSHRSTR))
#define ttislngstring(o)	checktag((o), ctb(LUA_VLNGSTR))

#define tsvalueraw(v)	(gco2ts((v).gc))

#define tsvalue(o)	check_exp(ttisstring(o), gco2ts(val_(o).gc))

#define setsvalue(L,obj,x) \
  { TValue *io = (obj); TString *x_ = (x); \
    val_(io).gc = obj2gco(x_); settt_(io, ctb(x_->tt)); \
    checkliveness(L,io); }


#define setsvalue2s(L,o,s)	setsvalue(L,s2v(o),s)


#define setsvalue2n	setsvalue



#define LSTRREG		-1  
#define LSTRFIX		-2  
#define LSTRMEM		-3  



typedef struct TString {
  CommonHeader;
  lu_byte extra;  
  ls_byte shrlen;  
  unsigned int hash;
  union {
    size_t lnglen;  
    struct TString *hnext;  
  } u;
  char *contents;  
  lua_Alloc falloc;  
  void *ud;  
} TString;


#define strisshr(ts)	((ts)->shrlen >= 0)



#define rawgetshrstr(ts)  (cast_charp(&(ts)->contents))
#define getshrstr(ts)	check_exp(strisshr(ts), rawgetshrstr(ts))
#define getlngstr(ts)	check_exp(!strisshr(ts), (ts)->contents)
#define getstr(ts) 	(strisshr(ts) ? rawgetshrstr(ts) : (ts)->contents)



#define tsslen(ts)  \
	(strisshr(ts) ? cast_uint((ts)->shrlen) : (ts)->u.lnglen)


#define getlstr(ts, len)  \
	(strisshr(ts) \
	? (cast_void((len) = (ts)->shrlen), rawgetshrstr(ts)) \
	: (cast_void((len) = (ts)->u.lnglen), (ts)->contents))








#define LUA_VLIGHTUSERDATA	makevariant(LUA_TLIGHTUSERDATA, 0)

#define LUA_VUSERDATA		makevariant(LUA_TUSERDATA, 0)

#define ttislightuserdata(o)	checktag((o), LUA_VLIGHTUSERDATA)
#define ttisfulluserdata(o)	checktag((o), ctb(LUA_VUSERDATA))

#define pvalue(o)	check_exp(ttislightuserdata(o), val_(o).p)
#define uvalue(o)	check_exp(ttisfulluserdata(o), gco2u(val_(o).gc))

#define pvalueraw(v)	((v).p)

#define setpvalue(obj,x) \
  { TValue *io=(obj); val_(io).p=(x); settt_(io, LUA_VLIGHTUSERDATA); }

#define setuvalue(L,obj,x) \
  { TValue *io = (obj); Udata *x_ = (x); \
    val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VUSERDATA)); \
    checkliveness(L,io); }



typedef union UValue {
  TValue uv;
  LUAI_MAXALIGN;  
} UValue;



typedef struct Udata {
  CommonHeader;
  unsigned short nuvalue;  
  size_t len;  
  struct Table *metatable;
  GCObject *gclist;
  UValue uv[1];  
} Udata;



typedef struct Udata0 {
  CommonHeader;
  unsigned short nuvalue;  
  size_t len;  
  struct Table *metatable;
  union {LUAI_MAXALIGN;} bindata;
} Udata0;



#define udatamemoffset(nuv) \
	((nuv) == 0 ? offsetof(Udata0, bindata)  \
                    : offsetof(Udata, uv) + (sizeof(UValue) * (nuv)))


#define getudatamem(u)	(cast_charp(u) + udatamemoffset((u)->nuvalue))


#define sizeudata(nuv,nb)	(udatamemoffset(nuv) + (nb))






#define LUA_VPROTO	makevariant(LUA_TPROTO, 0)



typedef struct Upvaldesc {
  TString *name;  
  lu_byte instack;  
  lu_byte idx;  
  lu_byte kind;  
} Upvaldesc;



typedef struct LocVar {
  TString *varname;
  int startpc;  
  int endpc;    
} LocVar;



typedef struct AbsLineInfo {
  int pc;
  int line;
} AbsLineInfo;



#define PF_ISVARARG	1
#define PF_FIXED	2  



typedef struct Proto {
  CommonHeader;
  lu_byte numparams;  
  lu_byte flag;
  lu_byte maxstacksize;  
  int sizeupvalues;  
  int sizek;  
  int sizecode;
  int sizelineinfo;
  int sizep;  
  int sizelocvars;
  int sizeabslineinfo;  
  int linedefined;  
  int lastlinedefined;  
  TValue *k;  
  Instruction *code;  
  struct Proto **p;  
  Upvaldesc *upvalues;  
  ls_byte *lineinfo;  
  AbsLineInfo *abslineinfo;  
  LocVar *locvars;  
  TString  *source;  
  GCObject *gclist;
} Proto;






#define LUA_VUPVAL	makevariant(LUA_TUPVAL, 0)



#define LUA_VLCL	makevariant(LUA_TFUNCTION, 0)  
#define LUA_VLCF	makevariant(LUA_TFUNCTION, 1)  
#define LUA_VCCL	makevariant(LUA_TFUNCTION, 2)  

#define ttisfunction(o)		checktype(o, LUA_TFUNCTION)
#define ttisLclosure(o)		checktag((o), ctb(LUA_VLCL))
#define ttislcf(o)		checktag((o), LUA_VLCF)
#define ttisCclosure(o)		checktag((o), ctb(LUA_VCCL))
#define ttisclosure(o)         (ttisLclosure(o) || ttisCclosure(o))


#define isLfunction(o)	ttisLclosure(o)

#define clvalue(o)	check_exp(ttisclosure(o), gco2cl(val_(o).gc))
#define clLvalue(o)	check_exp(ttisLclosure(o), gco2lcl(val_(o).gc))
#define fvalue(o)	check_exp(ttislcf(o), val_(o).f)
#define clCvalue(o)	check_exp(ttisCclosure(o), gco2ccl(val_(o).gc))

#define fvalueraw(v)	((v).f)

#define setclLvalue(L,obj,x) \
  { TValue *io = (obj); LClosure *x_ = (x); \
    val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VLCL)); \
    checkliveness(L,io); }

#define setclLvalue2s(L,o,cl)	setclLvalue(L,s2v(o),cl)

#define setfvalue(obj,x) \
  { TValue *io=(obj); val_(io).f=(x); settt_(io, LUA_VLCF); }

#define setclCvalue(L,obj,x) \
  { TValue *io = (obj); CClosure *x_ = (x); \
    val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VCCL)); \
    checkliveness(L,io); }



typedef struct UpVal {
  CommonHeader;
  union {
    TValue *p;  
    ptrdiff_t offset;  
  } v;
  union {
    struct {  
      struct UpVal *next;  
      struct UpVal **previous;
    } open;
    TValue value;  
  } u;
} UpVal;



#define ClosureHeader \
	CommonHeader; lu_byte nupvalues; GCObject *gclist

typedef struct CClosure {
  ClosureHeader;
  lua_CFunction f;
  TValue upvalue[1];  
} CClosure;


typedef struct LClosure {
  ClosureHeader;
  struct Proto *p;
  UpVal *upvals[1];  
} LClosure;


typedef union Closure {
  CClosure c;
  LClosure l;
} Closure;


#define getproto(o)	(clLvalue(o)->p)






#define LUA_VTABLE	makevariant(LUA_TTABLE, 0)

#define ttistable(o)		checktag((o), ctb(LUA_VTABLE))

#define hvalue(o)	check_exp(ttistable(o), gco2t(val_(o).gc))

#define sethvalue(L,obj,x) \
  { TValue *io = (obj); Table *x_ = (x); \
    val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VTABLE)); \
    checkliveness(L,io); }

#define sethvalue2s(L,o,h)	sethvalue(L,s2v(o),h)



typedef union Node {
  struct NodeKey {
    TValuefields;  
    lu_byte key_tt;  
    int next;  
    Value key_val;  
  } u;
  TValue i_val;  
} Node;



#define setnodekey(L,node,obj) \
	{ Node *n_=(node); const TValue *io_=(obj); \
	  n_->u.key_val = io_->value_; n_->u.key_tt = io_->tt_; \
	  checkliveness(L,io_); }



#define getnodekey(L,obj,node) \
	{ TValue *io_=(obj); const Node *n_=(node); \
	  io_->value_ = n_->u.key_val; io_->tt_ = n_->u.key_tt; \
	  checkliveness(L,io_); }




#define BITRAS		(1 << 7)
#define isrealasize(t)		(!((t)->flags & BITRAS))
#define setrealasize(t)		((t)->flags &= cast_byte(~BITRAS))
#define setnorealasize(t)	((t)->flags |= BITRAS)


typedef struct ArrayCell ArrayCell;


typedef struct Table {
  CommonHeader;
  lu_byte flags;  
  lu_byte lsizenode;  
  unsigned int alimit;  
  ArrayCell *array;  
  Node *node;
  struct Table *metatable;
  GCObject *gclist;
} Table;



#define keytt(node)		((node)->u.key_tt)
#define keyval(node)		((node)->u.key_val)

#define keyisnil(node)		(keytt(node) == LUA_TNIL)
#define keyisinteger(node)	(keytt(node) == LUA_VNUMINT)
#define keyival(node)		(keyval(node).i)
#define keyisshrstr(node)	(keytt(node) == ctb(LUA_VSHRSTR))
#define keystrval(node)		(gco2ts(keyval(node).gc))

#define setnilkey(node)		(keytt(node) = LUA_TNIL)

#define keyiscollectable(n)	(keytt(n) & BIT_ISCOLLECTABLE)

#define gckey(n)	(keyval(n).gc)
#define gckeyN(n)	(keyiscollectable(n) ? gckey(n) : NULL)



#define setdeadkey(node)	(keytt(node) = LUA_TDEADKEY)
#define keyisdead(node)		(keytt(node) == LUA_TDEADKEY)






#define lmod(s,size) \
	(check_exp((size&(size-1))==0, (cast_int((s) & ((size)-1)))))


#define twoto(x)	(1<<(x))
#define sizenode(t)	(twoto((t)->lsizenode))



#define UTF8BUFFSZ	8

LUAI_FUNC int luaO_utf8esc (char *buff, unsigned long x);
LUAI_FUNC int luaO_ceillog2 (unsigned int x);
LUAI_FUNC unsigned int luaO_codeparam (unsigned int p);
LUAI_FUNC l_obj luaO_applyparam (unsigned int p, l_obj x);

LUAI_FUNC int luaO_rawarith (lua_State *L, int op, const TValue *p1,
                             const TValue *p2, TValue *res);
LUAI_FUNC void luaO_arith (lua_State *L, int op, const TValue *p1,
                           const TValue *p2, StkId res);
LUAI_FUNC size_t luaO_str2num (const char *s, TValue *o);
LUAI_FUNC int luaO_hexavalue (int c);
LUAI_FUNC void luaO_tostring (lua_State *L, TValue *obj);
LUAI_FUNC const char *luaO_pushvfstring (lua_State *L, const char *fmt,
                                                       va_list argp);
LUAI_FUNC const char *luaO_pushfstring (lua_State *L, const char *fmt, ...);
LUAI_FUNC void luaO_chunkid (char *out, const char *source, size_t srclen);


#endif



//path: src/imports/../dependencies/lua/ltm.h


#ifndef ltm_h
#define ltm_h







typedef enum {
  TM_INDEX,
  TM_NEWINDEX,
  TM_GC,
  TM_MODE,
  TM_LEN,
  TM_EQ,  
  TM_ADD,
  TM_SUB,
  TM_MUL,
  TM_MOD,
  TM_POW,
  TM_DIV,
  TM_IDIV,
  TM_BAND,
  TM_BOR,
  TM_BXOR,
  TM_SHL,
  TM_SHR,
  TM_UNM,
  TM_BNOT,
  TM_LT,
  TM_LE,
  TM_CONCAT,
  TM_CALL,
  TM_CLOSE,
  TM_N		
} TMS;



#define maskflags	(~(~0u << (TM_EQ + 1)))



#define notm(tm)	ttisnil(tm)

#define checknoTM(mt,e)	((mt) == NULL || (mt)->flags & (1u<<(e)))

#define gfasttm(g,mt,e)  \
  (checknoTM(mt, e) ? NULL : luaT_gettm(mt, e, (g)->tmname[e]))

#define fasttm(l,mt,e)	gfasttm(G(l), mt, e)

#define ttypename(x)	luaT_typenames_[(x) + 1]

LUAI_DDEC(const char *const luaT_typenames_[LUA_TOTALTYPES];)


LUAI_FUNC const char *luaT_objtypename (lua_State *L, const TValue *o);

LUAI_FUNC const TValue *luaT_gettm (Table *events, TMS event, TString *ename);
LUAI_FUNC const TValue *luaT_gettmbyobj (lua_State *L, const TValue *o,
                                                       TMS event);
LUAI_FUNC void luaT_init (lua_State *L);

LUAI_FUNC void luaT_callTM (lua_State *L, const TValue *f, const TValue *p1,
                            const TValue *p2, const TValue *p3);
LUAI_FUNC int luaT_callTMres (lua_State *L, const TValue *f,
                              const TValue *p1, const TValue *p2, StkId p3);
LUAI_FUNC void luaT_trybinTM (lua_State *L, const TValue *p1, const TValue *p2,
                              StkId res, TMS event);
LUAI_FUNC void luaT_tryconcatTM (lua_State *L);
LUAI_FUNC void luaT_trybinassocTM (lua_State *L, const TValue *p1,
       const TValue *p2, int inv, StkId res, TMS event);
LUAI_FUNC void luaT_trybiniTM (lua_State *L, const TValue *p1, lua_Integer i2,
                               int inv, StkId res, TMS event);
LUAI_FUNC int luaT_callorderTM (lua_State *L, const TValue *p1,
                                const TValue *p2, TMS event);
LUAI_FUNC int luaT_callorderiTM (lua_State *L, const TValue *p1, int v2,
                                 int inv, int isfloat, TMS event);

LUAI_FUNC void luaT_adjustvarargs (lua_State *L, int nfixparams,
                                   struct CallInfo *ci, const Proto *p);
LUAI_FUNC void luaT_getvarargs (lua_State *L, struct CallInfo *ci,
                                              StkId where, int wanted);


#endif


//path: src/imports/../dependencies/lua/lzio.h



#ifndef lzio_h
#define lzio_h








#define EOZ	(-1)			

typedef struct Zio ZIO;

#define zgetc(z)  (((z)->n--)>0 ?  cast_uchar(*(z)->p++) : luaZ_fill(z))


typedef struct Mbuffer {
  char *buffer;
  size_t n;
  size_t buffsize;
} Mbuffer;

#define luaZ_initbuffer(L, buff) ((buff)->buffer = NULL, (buff)->buffsize = 0)

#define luaZ_buffer(buff)	((buff)->buffer)
#define luaZ_sizebuffer(buff)	((buff)->buffsize)
#define luaZ_bufflen(buff)	((buff)->n)

#define luaZ_buffremove(buff,i)	((buff)->n -= (i))
#define luaZ_resetbuffer(buff) ((buff)->n = 0)


#define luaZ_resizebuffer(L, buff, size) \
	((buff)->buffer = luaM_reallocvchar(L, (buff)->buffer, \
				(buff)->buffsize, size), \
	(buff)->buffsize = size)

#define luaZ_freebuffer(L, buff)	luaZ_resizebuffer(L, buff, 0)


LUAI_FUNC void luaZ_init (lua_State *L, ZIO *z, lua_Reader reader,
                                        void *data);
LUAI_FUNC size_t luaZ_read (ZIO* z, void *b, size_t n);	

LUAI_FUNC const void *luaZ_getaddr (ZIO* z, size_t n);




struct Zio {
  size_t n;			
  const char *p;		
  lua_Reader reader;		
  void *data;			
  lua_State *L;			
};


LUAI_FUNC int luaZ_fill (ZIO *z);

#endif














#define yieldable(L)		(((L)->nCcalls & 0xffff0000) == 0)


#define getCcalls(L)	((L)->nCcalls & 0xffff)



#define incnny(L)	((L)->nCcalls += 0x10000)


#define decnny(L)	((L)->nCcalls -= 0x10000)


#define nyci	(0x10000 | 1)




struct lua_longjmp;  



#if !defined(l_signalT)
#include <signal.h>
#define l_signalT	sig_atomic_t
#endif



#define EXTRA_STACK   5


#define BASIC_STACK_SIZE        (2*LUA_MINSTACK)

#define stacksize(th)	cast_int((th)->stack_last.p - (th)->stack.p)



#define KGC_INC		0	
#define KGC_GENMINOR	1	
#define KGC_GENMAJOR	2	


typedef struct stringtable {
  TString **hash;  
  int nuse;  
  int size;  
} stringtable;



struct CallInfo {
  StkIdRel func;  
  StkIdRel	top;  
  struct CallInfo *previous, *next;  
  union {
    struct {  
      const Instruction *savedpc;
      volatile l_signalT trap;  
      int nextraargs;  
    } l;
    struct {  
      lua_KFunction k;  
      ptrdiff_t old_errfunc;
      lua_KContext ctx;  
    } c;
  } u;
  union {
    int funcidx;  
    int nyield;  
    int nres;  
    struct {  
      unsigned short ftransfer;  
      unsigned short ntransfer;  
    } transferinfo;
  } u2;
  short nresults;  
  unsigned short callstatus;
};



#define CIST_OAH	(1<<0)	
#define CIST_C		(1<<1)	
#define CIST_FRESH	(1<<2)	
#define CIST_HOOKED	(1<<3)	
#define CIST_YPCALL	(1<<4)	
#define CIST_TAIL	(1<<5)	
#define CIST_HOOKYIELD	(1<<6)	
#define CIST_FIN	(1<<7)	
#define CIST_TRAN	(1<<8)	
#define CIST_CLSRET	(1<<9)  

#define CIST_RECST	10
#if defined(LUA_COMPAT_LT_LE)
#define CIST_LEQ	(1<<13)  
#endif



#define getcistrecst(ci)     (((ci)->callstatus >> CIST_RECST) & 7)
#define setcistrecst(ci,st)  \
  check_exp(((st) & 7) == (st),     \
            ((ci)->callstatus = ((ci)->callstatus & ~(7 << CIST_RECST))  \
                                                  | ((st) << CIST_RECST)))



#define isLua(ci)	(!((ci)->callstatus & CIST_C))


#define isLuacode(ci)	(!((ci)->callstatus & (CIST_C | CIST_HOOKED)))


#define setoah(st,v)	((st) = ((st) & ~CIST_OAH) | (v))
#define getoah(st)	((st) & CIST_OAH)



typedef struct global_State {
  lua_Alloc frealloc;  
  void *ud;         
  lu_mem totalbytes;  
  l_obj totalobjs;  
  l_obj GCdebt;  
  l_obj marked;  
  l_obj GCmajorminor;  
  stringtable strt;  
  TValue l_registry;
  TValue nilvalue;  
  unsigned int seed;  
  lu_byte gcparams[LUA_GCPN];
  lu_byte currentwhite;
  lu_byte gcstate;  
  lu_byte gckind;  
  lu_byte gcstopem;  
  lu_byte gcstp;  
  lu_byte gcemergency;  
  GCObject *allgc;  
  GCObject **sweepgc;  
  GCObject *finobj;  
  GCObject *gray;  
  GCObject *grayagain;  
  GCObject *weak;  
  GCObject *ephemeron;  
  GCObject *allweak;  
  GCObject *tobefnz;  
  GCObject *fixedgc;  
  
  GCObject *survival;  
  GCObject *old1;  
  GCObject *reallyold;  
  GCObject *firstold1;  
  GCObject *finobjsur;  
  GCObject *finobjold1;  
  GCObject *finobjrold;  
  struct lua_State *twups;  
  lua_CFunction panic;  
  struct lua_State *mainthread;
  TString *memerrmsg;  
  TString *tmname[TM_N];  
  struct Table *mt[LUA_NUMTYPES];  
  TString *strcache[STRCACHE_N][STRCACHE_M];  
  lua_WarnFunction warnf;  
  void *ud_warn;         
} global_State;



struct lua_State {
  CommonHeader;
  lu_byte status;
  lu_byte allowhook;
  unsigned short nci;  
  StkIdRel top;  
  global_State *l_G;
  CallInfo *ci;  
  StkIdRel stack_last;  
  StkIdRel stack;  
  UpVal *openupval;  
  StkIdRel tbclist;  
  GCObject *gclist;
  struct lua_State *twups;  
  struct lua_longjmp *errorJmp;  
  CallInfo base_ci;  
  volatile lua_Hook hook;
  ptrdiff_t errfunc;  
  l_uint32 nCcalls;  
  int oldpc;  
  int basehookcount;
  int hookcount;
  volatile l_signalT hookmask;
};


#define G(L)	(L->l_G)


#define completestate(g)	ttisnil(&g->nilvalue)



union GCUnion {
  GCObject gc;  
  struct TString ts;
  struct Udata u;
  union Closure cl;
  struct Table h;
  struct Proto p;
  struct lua_State th;  
  struct UpVal upv;
};



#define cast_u(o)	cast(union GCUnion *, (o))


#define gco2ts(o)  \
	check_exp(novariant((o)->tt) == LUA_TSTRING, &((cast_u(o))->ts))
#define gco2u(o)  check_exp((o)->tt == LUA_VUSERDATA, &((cast_u(o))->u))
#define gco2lcl(o)  check_exp((o)->tt == LUA_VLCL, &((cast_u(o))->cl.l))
#define gco2ccl(o)  check_exp((o)->tt == LUA_VCCL, &((cast_u(o))->cl.c))
#define gco2cl(o)  \
	check_exp(novariant((o)->tt) == LUA_TFUNCTION, &((cast_u(o))->cl))
#define gco2t(o)  check_exp((o)->tt == LUA_VTABLE, &((cast_u(o))->h))
#define gco2p(o)  check_exp((o)->tt == LUA_VPROTO, &((cast_u(o))->p))
#define gco2th(o)  check_exp((o)->tt == LUA_VTHREAD, &((cast_u(o))->th))
#define gco2upv(o)	check_exp((o)->tt == LUA_VUPVAL, &((cast_u(o))->upv))



#define obj2gco(v)	check_exp((v)->tt >= LUA_TSTRING, &(cast_u(v)->gc))



#define gettotalobjs(g)	((g)->totalobjs - (g)->GCdebt)


LUAI_FUNC void luaE_setdebt (global_State *g, l_obj debt);
LUAI_FUNC void luaE_freethread (lua_State *L, lua_State *L1);
LUAI_FUNC CallInfo *luaE_extendCI (lua_State *L);
LUAI_FUNC void luaE_shrinkCI (lua_State *L);
LUAI_FUNC void luaE_checkcstack (lua_State *L);
LUAI_FUNC void luaE_incCstack (lua_State *L);
LUAI_FUNC void luaE_warning (lua_State *L, const char *msg, int tocont);
LUAI_FUNC void luaE_warnerror (lua_State *L, const char *where);
LUAI_FUNC int luaE_resetthread (lua_State *L, int status);


#endif







int luaZ_fill (ZIO *z) {
  size_t size;
  lua_State *L = z->L;
  const char *buff;
  lua_unlock(L);
  buff = z->reader(L, z->data, &size);
  lua_lock(L);
  if (buff == NULL || size == 0)
    return EOZ;
  z->n = size - 1;  
  z->p = buff;
  return cast_uchar(*(z->p++));
}


void luaZ_init (lua_State *L, ZIO *z, lua_Reader reader, void *data) {
  z->L = L;
  z->reader = reader;
  z->data = data;
  z->n = 0;
  z->p = NULL;
}




static int checkbuffer (ZIO *z) {
  if (z->n == 0) {  
    if (luaZ_fill(z) == EOZ)  
      return 0;  
    else {
      z->n++;  
      z->p--;
    }
  }
  return 1;  
}


size_t luaZ_read (ZIO *z, void *b, size_t n) {
  while (n) {
    size_t m;
    if (!checkbuffer(z))
      return n;  
    m = (n <= z->n) ? n : z->n;  
    memcpy(b, z->p, m);
    z->n -= m;
    z->p += m;
    b = (char *)b + m;
    n -= m;
  }
  return 0;
}


const void *luaZ_getaddr (ZIO* z, size_t n) {
  const void *res;
  if (!checkbuffer(z))
    return NULL;  
  if (z->n < n)  
    return NULL;  
  res = z->p;  
  z->n -= n;  
  z->p += n;
  return res;
}


//path: src/imports/../dependencies/lua/lctype.c


#define lctype_c
#define LUA_CORE





//path: src/imports/../dependencies/lua/lctype.h


#ifndef lctype_h
#define lctype_h







#if !defined(LUA_USE_CTYPE)

#if 'A' == 65 && '0' == 48

#define LUA_USE_CTYPE	0
#else

#define LUA_USE_CTYPE	1
#endif

#endif


#if !LUA_USE_CTYPE	

#include <limits.h>





#define ALPHABIT	0
#define DIGITBIT	1
#define PRINTBIT	2
#define SPACEBIT	3
#define XDIGITBIT	4


#define MASK(B)		(1 << (B))



#define testprop(c,p)	(luai_ctype_[(c)+1] & (p))


#define lislalpha(c)	testprop(c, MASK(ALPHABIT))
#define lislalnum(c)	testprop(c, (MASK(ALPHABIT) | MASK(DIGITBIT)))
#define lisdigit(c)	testprop(c, MASK(DIGITBIT))
#define lisspace(c)	testprop(c, MASK(SPACEBIT))
#define lisprint(c)	testprop(c, MASK(PRINTBIT))
#define lisxdigit(c)	testprop(c, MASK(XDIGITBIT))



#define ltolower(c)  \
  check_exp(('A' <= (c) && (c) <= 'Z') || (c) == ((c) | ('A' ^ 'a')),  \
            (c) | ('A' ^ 'a'))



LUAI_DDEC(const lu_byte luai_ctype_[UCHAR_MAX + 2];)


#else			



#include <ctype.h>


#define lislalpha(c)	(isalpha(c) || (c) == '_')
#define lislalnum(c)	(isalnum(c) || (c) == '_')
#define lisdigit(c)	(isdigit(c))
#define lisspace(c)	(isspace(c))
#define lisprint(c)	(isprint(c))
#define lisxdigit(c)	(isxdigit(c))

#define ltolower(c)	(tolower(c))

#endif			

#endif




#if !LUA_USE_CTYPE	

#include <limits.h>


#if defined (LUA_UCID)		

#define NONA		0x01
#else
#define NONA		0x00	
#endif


LUAI_DDEF const lu_byte luai_ctype_[UCHAR_MAX + 2] = {
  0x00,  
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,	
  0x00,  0x08,  0x08,  0x08,  0x08,  0x08,  0x00,  0x00,
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,	
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
  0x0c,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,	
  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,
  0x16,  0x16,  0x16,  0x16,  0x16,  0x16,  0x16,  0x16,	
  0x16,  0x16,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,
  0x04,  0x15,  0x15,  0x15,  0x15,  0x15,  0x15,  0x05,	
  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,
  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,	
  0x05,  0x05,  0x05,  0x04,  0x04,  0x04,  0x04,  0x05,
  0x04,  0x15,  0x15,  0x15,  0x15,  0x15,  0x15,  0x05,	
  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,
  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,	
  0x05,  0x05,  0x05,  0x04,  0x04,  0x04,  0x04,  0x00,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  0x00,  0x00,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  0x00,  0x00,  0x00,	
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00
};

#endif			

//path: src/imports/../dependencies/lua/lopcodes.c


#define lopcodes_c
#define LUA_CORE





//path: src/imports/../dependencies/lua/lopcodes.h


#ifndef lopcodes_h
#define lopcodes_h








enum OpMode {iABC, iABx, iAsBx, iAx, isJ};  



#define SIZE_C		8
#define SIZE_B		8
#define SIZE_Bx		(SIZE_C + SIZE_B + 1)
#define SIZE_A		8
#define SIZE_Ax		(SIZE_Bx + SIZE_A)
#define SIZE_sJ		(SIZE_Bx + SIZE_A)

#define SIZE_OP		7

#define POS_OP		0

#define POS_A		(POS_OP + SIZE_OP)
#define POS_k		(POS_A + SIZE_A)
#define POS_B		(POS_k + 1)
#define POS_C		(POS_B + SIZE_B)

#define POS_Bx		POS_k

#define POS_Ax		POS_A

#define POS_sJ		POS_A





#define L_INTHASBITS(b)		((UINT_MAX >> ((b) - 1)) >= 1)


#if L_INTHASBITS(SIZE_Bx)
#define MAXARG_Bx	((1<<SIZE_Bx)-1)
#else
#define MAXARG_Bx	MAX_INT
#endif

#define OFFSET_sBx	(MAXARG_Bx>>1)         


#if L_INTHASBITS(SIZE_Ax)
#define MAXARG_Ax	((1<<SIZE_Ax)-1)
#else
#define MAXARG_Ax	MAX_INT
#endif

#if L_INTHASBITS(SIZE_sJ)
#define MAXARG_sJ	((1 << SIZE_sJ) - 1)
#else
#define MAXARG_sJ	MAX_INT
#endif

#define OFFSET_sJ	(MAXARG_sJ >> 1)


#define MAXARG_A	((1<<SIZE_A)-1)
#define MAXARG_B	((1<<SIZE_B)-1)
#define MAXARG_C	((1<<SIZE_C)-1)
#define OFFSET_sC	(MAXARG_C >> 1)

#define int2sC(i)	((i) + OFFSET_sC)
#define sC2int(i)	((i) - OFFSET_sC)



#define MASK1(n,p)	((~((~(Instruction)0)<<(n)))<<(p))


#define MASK0(n,p)	(~MASK1(n,p))



#define GET_OPCODE(i)	(cast(OpCode, ((i)>>POS_OP) & MASK1(SIZE_OP,0)))
#define SET_OPCODE(i,o)	((i) = (((i)&MASK0(SIZE_OP,POS_OP)) | \
		((cast(Instruction, o)<<POS_OP)&MASK1(SIZE_OP,POS_OP))))

#define checkopm(i,m)	(getOpMode(GET_OPCODE(i)) == m)


#define getarg(i,pos,size)	(cast_int(((i)>>(pos)) & MASK1(size,0)))
#define setarg(i,v,pos,size)	((i) = (((i)&MASK0(size,pos)) | \
                ((cast(Instruction, v)<<pos)&MASK1(size,pos))))

#define GETARG_A(i)	getarg(i, POS_A, SIZE_A)
#define SETARG_A(i,v)	setarg(i, v, POS_A, SIZE_A)

#define GETARG_B(i)	check_exp(checkopm(i, iABC), getarg(i, POS_B, SIZE_B))
#define GETARG_sB(i)	sC2int(GETARG_B(i))
#define SETARG_B(i,v)	setarg(i, v, POS_B, SIZE_B)

#define GETARG_C(i)	check_exp(checkopm(i, iABC), getarg(i, POS_C, SIZE_C))
#define GETARG_sC(i)	sC2int(GETARG_C(i))
#define SETARG_C(i,v)	setarg(i, v, POS_C, SIZE_C)

#define TESTARG_k(i)	check_exp(checkopm(i, iABC), (cast_int(((i) & (1u << POS_k)))))
#define GETARG_k(i)	check_exp(checkopm(i, iABC), getarg(i, POS_k, 1))
#define SETARG_k(i,v)	setarg(i, v, POS_k, 1)

#define GETARG_Bx(i)	check_exp(checkopm(i, iABx), getarg(i, POS_Bx, SIZE_Bx))
#define SETARG_Bx(i,v)	setarg(i, v, POS_Bx, SIZE_Bx)

#define GETARG_Ax(i)	check_exp(checkopm(i, iAx), getarg(i, POS_Ax, SIZE_Ax))
#define SETARG_Ax(i,v)	setarg(i, v, POS_Ax, SIZE_Ax)

#define GETARG_sBx(i)  \
	check_exp(checkopm(i, iAsBx), getarg(i, POS_Bx, SIZE_Bx) - OFFSET_sBx)
#define SETARG_sBx(i,b)	SETARG_Bx((i),cast_uint((b)+OFFSET_sBx))

#define GETARG_sJ(i)  \
	check_exp(checkopm(i, isJ), getarg(i, POS_sJ, SIZE_sJ) - OFFSET_sJ)
#define SETARG_sJ(i,j) \
	setarg(i, cast_uint((j)+OFFSET_sJ), POS_sJ, SIZE_sJ)


#define CREATE_ABCk(o,a,b,c,k)	((cast(Instruction, o)<<POS_OP) \
			| (cast(Instruction, a)<<POS_A) \
			| (cast(Instruction, b)<<POS_B) \
			| (cast(Instruction, c)<<POS_C) \
			| (cast(Instruction, k)<<POS_k))

#define CREATE_ABx(o,a,bc)	((cast(Instruction, o)<<POS_OP) \
			| (cast(Instruction, a)<<POS_A) \
			| (cast(Instruction, bc)<<POS_Bx))

#define CREATE_Ax(o,a)		((cast(Instruction, o)<<POS_OP) \
			| (cast(Instruction, a)<<POS_Ax))

#define CREATE_sJ(o,j,k)	((cast(Instruction, o) << POS_OP) \
			| (cast(Instruction, j) << POS_sJ) \
			| (cast(Instruction, k) << POS_k))


#if !defined(MAXINDEXRK)  
#define MAXINDEXRK	MAXARG_B
#endif



#define NO_REG		MAXARG_A







typedef enum {

OP_MOVE,
OP_LOADI,
OP_LOADF,
OP_LOADK,
OP_LOADKX,
OP_LOADFALSE,
OP_LFALSESKIP,
OP_LOADTRUE,
OP_LOADNIL,
OP_GETUPVAL,
OP_SETUPVAL,

OP_GETTABUP,
OP_GETTABLE,
OP_GETI,
OP_GETFIELD,

OP_SETTABUP,
OP_SETTABLE,
OP_SETI,
OP_SETFIELD,

OP_NEWTABLE,

OP_SELF,

OP_ADDI,

OP_ADDK,
OP_SUBK,
OP_MULK,
OP_MODK,
OP_POWK,
OP_DIVK,
OP_IDIVK,

OP_BANDK,
OP_BORK,
OP_BXORK,

OP_SHRI,
OP_SHLI,

OP_ADD,
OP_SUB,
OP_MUL,
OP_MOD,
OP_POW,
OP_DIV,
OP_IDIV,

OP_BAND,
OP_BOR,
OP_BXOR,
OP_SHL,
OP_SHR,

OP_MMBIN,
OP_MMBINI,
OP_MMBINK,

OP_UNM,
OP_BNOT,
OP_NOT,
OP_LEN,

OP_CONCAT,

OP_CLOSE,
OP_TBC,
OP_JMP,
OP_EQ,
OP_LT,
OP_LE,

OP_EQK,
OP_EQI,
OP_LTI,
OP_LEI,
OP_GTI,
OP_GEI,

OP_TEST,
OP_TESTSET,

OP_CALL,
OP_TAILCALL,

OP_RETURN,
OP_RETURN0,
OP_RETURN1,

OP_FORLOOP,
OP_FORPREP,

OP_TFORPREP,
OP_TFORCALL,
OP_TFORLOOP,

OP_SETLIST,

OP_CLOSURE,

OP_VARARG,

OP_VARARGPREP,

OP_EXTRAARG
} OpCode;


#define NUM_OPCODES	((int)(OP_EXTRAARG) + 1)








LUAI_DDEC(const lu_byte luaP_opmodes[NUM_OPCODES];)

#define getOpMode(m)	(cast(enum OpMode, luaP_opmodes[m] & 7))
#define testAMode(m)	(luaP_opmodes[m] & (1 << 3))
#define testTMode(m)	(luaP_opmodes[m] & (1 << 4))
#define testITMode(m)	(luaP_opmodes[m] & (1 << 5))
#define testOTMode(m)	(luaP_opmodes[m] & (1 << 6))
#define testMMMode(m)	(luaP_opmodes[m] & (1 << 7))


#define isOT(i)  \
	((testOTMode(GET_OPCODE(i)) && GETARG_C(i) == 0) || \
          GET_OPCODE(i) == OP_TAILCALL)


#define isIT(i)		(testITMode(GET_OPCODE(i)) && GETARG_B(i) == 0)

#define opmode(mm,ot,it,t,a,m)  \
    (((mm) << 7) | ((ot) << 6) | ((it) << 5) | ((t) << 4) | ((a) << 3) | (m))



#define LFIELDS_PER_FLUSH	50

#endif






LUAI_DDEF const lu_byte luaP_opmodes[NUM_OPCODES] = {

  opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iAsBx)		
 ,opmode(0, 0, 0, 0, 1, iAsBx)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(1, 0, 0, 0, 0, iABC)		
 ,opmode(1, 0, 0, 0, 0, iABC)		
 ,opmode(1, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, isJ)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 1, iABC)		
 ,opmode(0, 1, 1, 0, 1, iABC)		
 ,opmode(0, 1, 1, 0, 1, iABC)		
 ,opmode(0, 0, 1, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 0, 0, 0, iABx)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 1, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 1, 0, 0, 1, iABC)		
 ,opmode(0, 0, 1, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 0, iAx)		
};



//path: src/imports/../dependencies/lua/lmem.c


#define lmem_c
#define LUA_CORE





#include <stddef.h>




//path: src/imports/../dependencies/lua/ldebug.h


#ifndef ldebug_h
#define ldebug_h






#define pcRel(pc, p)	(cast_int((pc) - (p)->code) - 1)



#define ci_func(ci)		(clLvalue(s2v((ci)->func.p)))


#define resethookcount(L)	(L->hookcount = L->basehookcount)


#define ABSLINEINFO	(-0x80)



#if !defined(MAXIWTHABS)
#define MAXIWTHABS	128
#endif


LUAI_FUNC int luaG_getfuncline (const Proto *f, int pc);
LUAI_FUNC const char *luaG_findlocal (lua_State *L, CallInfo *ci, int n,
                                                    StkId *pos);
LUAI_FUNC l_noret luaG_typeerror (lua_State *L, const TValue *o,
                                                const char *opname);
LUAI_FUNC l_noret luaG_callerror (lua_State *L, const TValue *o);
LUAI_FUNC l_noret luaG_forerror (lua_State *L, const TValue *o,
                                               const char *what);
LUAI_FUNC l_noret luaG_concaterror (lua_State *L, const TValue *p1,
                                                  const TValue *p2);
LUAI_FUNC l_noret luaG_opinterror (lua_State *L, const TValue *p1,
                                                 const TValue *p2,
                                                 const char *msg);
LUAI_FUNC l_noret luaG_tointerror (lua_State *L, const TValue *p1,
                                                 const TValue *p2);
LUAI_FUNC l_noret luaG_ordererror (lua_State *L, const TValue *p1,
                                                 const TValue *p2);
LUAI_FUNC l_noret luaG_runerror (lua_State *L, const char *fmt, ...);
LUAI_FUNC const char *luaG_addinfo (lua_State *L, const char *msg,
                                                  TString *src, int line);
LUAI_FUNC l_noret luaG_errormsg (lua_State *L);
LUAI_FUNC int luaG_traceexec (lua_State *L, const Instruction *pc);
LUAI_FUNC int luaG_tracecall (lua_State *L);


#endif


//path: src/imports/../dependencies/lua/ldo.h


#ifndef ldo_h
#define ldo_h













#define luaD_checkstackaux(L,n,pre,pos)  \
	if (l_unlikely(L->stack_last.p - L->top.p <= (n))) \
	  { pre; luaD_growstack(L, n, 1); pos; } \
        else { condmovestack(L,pre,pos); }


#define luaD_checkstack(L,n)	luaD_checkstackaux(L,n,(void)0,(void)0)



#define savestack(L,pt)		(cast_charp(pt) - cast_charp(L->stack.p))
#define restorestack(L,n)	cast(StkId, cast_charp(L->stack.p) + (n))



#define checkstackp(L,n,p)  \
  luaD_checkstackaux(L, n, \
    ptrdiff_t t__ = savestack(L, p),   \
    p = restorestack(L, t__))  




typedef void (*Pfunc) (lua_State *L, void *ud);

LUAI_FUNC void luaD_seterrorobj (lua_State *L, int errcode, StkId oldtop);
LUAI_FUNC int luaD_protectedparser (lua_State *L, ZIO *z, const char *name,
                                                  const char *mode);
LUAI_FUNC void luaD_hook (lua_State *L, int event, int line,
                                        int fTransfer, int nTransfer);
LUAI_FUNC void luaD_hookcall (lua_State *L, CallInfo *ci);
LUAI_FUNC int luaD_pretailcall (lua_State *L, CallInfo *ci, StkId func,
                                              int narg1, int delta);
LUAI_FUNC CallInfo *luaD_precall (lua_State *L, StkId func, int nResults);
LUAI_FUNC void luaD_call (lua_State *L, StkId func, int nResults);
LUAI_FUNC void luaD_callnoyield (lua_State *L, StkId func, int nResults);
LUAI_FUNC int luaD_closeprotected (lua_State *L, ptrdiff_t level, int status);
LUAI_FUNC int luaD_pcall (lua_State *L, Pfunc func, void *u,
                                        ptrdiff_t oldtop, ptrdiff_t ef);
LUAI_FUNC void luaD_poscall (lua_State *L, CallInfo *ci, int nres);
LUAI_FUNC int luaD_reallocstack (lua_State *L, int newsize, int raiseerror);
LUAI_FUNC int luaD_growstack (lua_State *L, int n, int raiseerror);
LUAI_FUNC void luaD_shrinkstack (lua_State *L);
LUAI_FUNC void luaD_inctop (lua_State *L);

LUAI_FUNC l_noret luaD_throw (lua_State *L, int errcode);
LUAI_FUNC int luaD_rawrunprotected (lua_State *L, Pfunc f, void *ud);

#endif



//path: src/imports/../dependencies/lua/lgc.h


#ifndef lgc_h
#define lgc_h


#include <stddef.h>











#define GCSpropagate	0
#define GCSenteratomic	1
#define GCSatomic	2
#define GCSswpallgc	3
#define GCSswpfinobj	4
#define GCSswptobefnz	5
#define GCSswpend	6
#define GCScallfin	7
#define GCSpause	8


#define issweepphase(g)  \
	(GCSswpallgc <= (g)->gcstate && (g)->gcstate <= GCSswpend)




#define keepinvariant(g)	((g)->gcstate <= GCSatomic)



#define resetbits(x,m)		((x) &= cast_byte(~(m)))
#define setbits(x,m)		((x) |= (m))
#define testbits(x,m)		((x) & (m))
#define bitmask(b)		(1<<(b))
#define bit2mask(b1,b2)		(bitmask(b1) | bitmask(b2))
#define l_setbit(x,b)		setbits(x, bitmask(b))
#define resetbit(x,b)		resetbits(x, bitmask(b))
#define testbit(x,b)		testbits(x, bitmask(b))



#define WHITE0BIT	3  
#define WHITE1BIT	4  
#define BLACKBIT	5  
#define FINALIZEDBIT	6  

#define TESTBIT		7



#define WHITEBITS	bit2mask(WHITE0BIT, WHITE1BIT)


#define iswhite(x)      testbits((x)->marked, WHITEBITS)
#define isblack(x)      testbit((x)->marked, BLACKBIT)
#define isgray(x)    \
	(!testbits((x)->marked, WHITEBITS | bitmask(BLACKBIT)))

#define tofinalize(x)	testbit((x)->marked, FINALIZEDBIT)

#define otherwhite(g)	((g)->currentwhite ^ WHITEBITS)
#define isdeadm(ow,m)	((m) & (ow))
#define isdead(g,v)	isdeadm(otherwhite(g), (v)->marked)

#define changewhite(x)	((x)->marked ^= WHITEBITS)
#define nw2black(x)  \
	check_exp(!iswhite(x), l_setbit((x)->marked, BLACKBIT))

#define luaC_white(g)	cast_byte((g)->currentwhite & WHITEBITS)



#define G_NEW		0	
#define G_SURVIVAL	1	
#define G_OLD0		2	
#define G_OLD1		3	
#define G_OLD		4	
#define G_TOUCHED1	5	
#define G_TOUCHED2	6	

#define AGEBITS		7  

#define getage(o)	((o)->marked & AGEBITS)
#define setage(o,a)  ((o)->marked = cast_byte(((o)->marked & (~AGEBITS)) | a))
#define isold(o)	(getage(o) > G_SURVIVAL)








#define LUAI_MINORMAJOR         100


#define LUAI_MAJORMINOR         50


#define LUAI_GENMINORMUL         25





#define LUAI_GCPAUSE    200


#define LUAI_GCMUL      200


#define LUAI_GCSTEPSIZE	250


#define setgcparam(g,p,v)  (g->gcparams[LUA_GCP##p] = luaO_codeparam(v))
#define applygcparam(g,p,x)  luaO_applyparam(g->gcparams[LUA_GCP##p], x)


#define GCSTPUSR	1  
#define GCSTPGC		2  
#define GCSTPCLS	4  
#define gcrunning(g)	((g)->gcstp == 0)



#define luaC_condGC(L,pre,pos) \
	{ if (G(L)->GCdebt <= 0) { pre; luaC_step(L); pos;}; \
	  condchangemem(L,pre,pos); }


#define luaC_checkGC(L)		luaC_condGC(L,(void)0,(void)0)


#define luaC_objbarrier(L,p,o) (  \
	(isblack(p) && iswhite(o)) ? \
	luaC_barrier_(L,obj2gco(p),obj2gco(o)) : cast_void(0))

#define luaC_barrier(L,p,v) (  \
	iscollectable(v) ? luaC_objbarrier(L,p,gcvalue(v)) : cast_void(0))

#define luaC_objbarrierback(L,p,o) (  \
	(isblack(p) && iswhite(o)) ? luaC_barrierback_(L,p) : cast_void(0))

#define luaC_barrierback(L,p,v) (  \
	iscollectable(v) ? luaC_objbarrierback(L, p, gcvalue(v)) : cast_void(0))

LUAI_FUNC void luaC_fix (lua_State *L, GCObject *o);
LUAI_FUNC void luaC_freeallobjects (lua_State *L);
LUAI_FUNC void luaC_step (lua_State *L);
LUAI_FUNC void luaC_runtilstate (lua_State *L, int state, int fast);
LUAI_FUNC void luaC_fullgc (lua_State *L, int isemergency);
LUAI_FUNC GCObject *luaC_newobj (lua_State *L, int tt, size_t sz);
LUAI_FUNC GCObject *luaC_newobjdt (lua_State *L, int tt, size_t sz,
                                                 size_t offset);
LUAI_FUNC void luaC_barrier_ (lua_State *L, GCObject *o, GCObject *v);
LUAI_FUNC void luaC_barrierback_ (lua_State *L, GCObject *o);
LUAI_FUNC void luaC_checkfinalizer (lua_State *L, GCObject *o, Table *mt);
LUAI_FUNC void luaC_changemode (lua_State *L, int newmode);


#endif















#define callfrealloc(g,block,os,ns)    ((*g->frealloc)(g->ud, block, os, ns))



#define cantryagain(g)	(completestate(g) && !g->gcstopem)




#if defined(EMERGENCYGCTESTS)

static void *firsttry (global_State *g, void *block, size_t os, size_t ns) {
  if (ns > 0 && cantryagain(g))
    return NULL;  
  else  
    return callfrealloc(g, block, os, ns);
}
#else
#define firsttry(g,block,os,ns)    callfrealloc(g, block, os, ns)
#endif








#define MINSIZEARRAY	4


void *luaM_growaux_ (lua_State *L, void *block, int nelems, int *psize,
                     int size_elems, int limit, const char *what) {
  void *newblock;
  int size = *psize;
  if (nelems + 1 <= size)  
    return block;  
  if (size >= limit / 2) {  
    if (l_unlikely(size >= limit))  
      luaG_runerror(L, "too many %s (limit is %d)", what, limit);
    size = limit;  
  }
  else {
    size *= 2;
    if (size < MINSIZEARRAY)
      size = MINSIZEARRAY;  
  }
  lua_assert(nelems + 1 <= size && size <= limit);
  
  newblock = luaM_saferealloc_(L, block, cast_sizet(*psize) * size_elems,
                                         cast_sizet(size) * size_elems);
  *psize = size;  
  return newblock;
}



void *luaM_shrinkvector_ (lua_State *L, void *block, int *size,
                          int final_n, int size_elem) {
  void *newblock;
  size_t oldsize = cast_sizet((*size) * size_elem);
  size_t newsize = cast_sizet(final_n * size_elem);
  lua_assert(newsize <= oldsize);
  newblock = luaM_saferealloc_(L, block, oldsize, newsize);
  *size = final_n;
  return newblock;
}




l_noret luaM_toobig (lua_State *L) {
  luaG_runerror(L, "memory allocation private_lua_error: block too big");
}



void luaM_free_ (lua_State *L, void *block, size_t osize) {
  global_State *g = G(L);
  lua_assert((osize == 0) == (block == NULL));
  callfrealloc(g, block, osize, 0);
  g->totalbytes -= osize;
}



static void *tryagain (lua_State *L, void *block,
                       size_t osize, size_t nsize) {
  global_State *g = G(L);
  if (cantryagain(g)) {
    luaC_fullgc(L, 1);  
    return callfrealloc(g, block, osize, nsize);  
  }
  else return NULL;  
}



void *luaM_realloc_ (lua_State *L, void *block, size_t osize, size_t nsize) {
  void *newblock;
  global_State *g = G(L);
  lua_assert((osize == 0) == (block == NULL));
  newblock = firsttry(g, block, osize, nsize);
  if (l_unlikely(newblock == NULL && nsize > 0)) {
    newblock = tryagain(L, block, osize, nsize);
    if (newblock == NULL)  
      return NULL;  
  }
  lua_assert((nsize == 0) == (newblock == NULL));
  g->totalbytes += nsize - osize;
  return newblock;
}


void *luaM_saferealloc_ (lua_State *L, void *block, size_t osize,
                                                    size_t nsize) {
  void *newblock = luaM_realloc_(L, block, osize, nsize);
  if (l_unlikely(newblock == NULL && nsize > 0))  
    luaM_error(L);
  return newblock;
}


void *luaM_malloc_ (lua_State *L, size_t size, int tag) {
  if (size == 0)
    return NULL;  
  else {
    global_State *g = G(L);
    void *newblock = firsttry(g, NULL, tag, size);
    if (l_unlikely(newblock == NULL)) {
      newblock = tryagain(L, NULL, tag, size);
      if (newblock == NULL)
        luaM_error(L);
    }
    g->totalbytes += size;
    return newblock;
  }
}


//path: src/imports/../dependencies/lua/lundump.c


#define lundump_c
#define LUA_CORE





#include <limits.h>
#include <string.h>








//path: src/imports/../dependencies/lua/lfunc.h


#ifndef lfunc_h
#define lfunc_h






#define sizeCclosure(n)	(cast_int(offsetof(CClosure, upvalue)) + \
                         cast_int(sizeof(TValue)) * (n))

#define sizeLclosure(n)	(cast_int(offsetof(LClosure, upvals)) + \
                         cast_int(sizeof(TValue *)) * (n))



#define isintwups(L)	(L->twups != L)



#define MAXUPVAL	255


#define upisopen(up)	((up)->v.p != &(up)->u.value)


#define uplevel(up)	check_exp(upisopen(up), cast(StkId, (up)->v.p))



#define MAXMISS		10




#define CLOSEKTOP	(-1)


LUAI_FUNC Proto *luaF_newproto (lua_State *L);
LUAI_FUNC CClosure *luaF_newCclosure (lua_State *L, int nupvals);
LUAI_FUNC LClosure *luaF_newLclosure (lua_State *L, int nupvals);
LUAI_FUNC void luaF_initupvals (lua_State *L, LClosure *cl);
LUAI_FUNC UpVal *luaF_findupval (lua_State *L, StkId level);
LUAI_FUNC void luaF_newtbcupval (lua_State *L, StkId level);
LUAI_FUNC void luaF_closeupval (lua_State *L, StkId level);
LUAI_FUNC StkId luaF_close (lua_State *L, StkId level, int status, int yy);
LUAI_FUNC void luaF_unlinkupval (UpVal *uv);
LUAI_FUNC void luaF_freeproto (lua_State *L, Proto *f);
LUAI_FUNC const char *luaF_getlocalname (const Proto *func, int local_number,
                                         int pc);


#endif






//path: src/imports/../dependencies/lua/lstring.h


#ifndef lstring_h
#define lstring_h










#define MEMERRMSG       "not enough memory"



#define sizestrshr(l)  \
	(offsetof(TString, contents) + ((l) + 1) * sizeof(char))


#define luaS_newliteral(L, s)	(luaS_newlstr(L, "" s, \
                                 (sizeof(s)/sizeof(char))-1))



#define isreserved(s)	((s)->tt == LUA_VSHRSTR && (s)->extra > 0)



#define eqshrstr(a,b)	check_exp((a)->tt == LUA_VSHRSTR, (a) == (b))


LUAI_FUNC unsigned luaS_hash (const char *str, size_t l, unsigned seed);
LUAI_FUNC unsigned luaS_hashlongstr (TString *ts);
LUAI_FUNC int luaS_eqlngstr (TString *a, TString *b);
LUAI_FUNC void luaS_resize (lua_State *L, int newsize);
LUAI_FUNC void luaS_clearcache (global_State *g);
LUAI_FUNC void luaS_init (lua_State *L);
LUAI_FUNC void luaS_remove (lua_State *L, TString *ts);
LUAI_FUNC Udata *luaS_newudata (lua_State *L, size_t s, int nuvalue);
LUAI_FUNC TString *luaS_newlstr (lua_State *L, const char *str, size_t l);
LUAI_FUNC TString *luaS_new (lua_State *L, const char *str);
LUAI_FUNC TString *luaS_createlngstrobj (lua_State *L, size_t l);
LUAI_FUNC TString *luaS_newextlstr (lua_State *L,
		const char *s, size_t len, lua_Alloc falloc, void *ud);
LUAI_FUNC size_t luaS_sizelngstr (size_t len, int kind);

#endif


//path: src/imports/../dependencies/lua/ltable.h


#ifndef ltable_h
#define ltable_h





#define gnode(t,i)	(&(t)->node[i])
#define gval(n)		(&(n)->i_val)
#define gnext(n)	((n)->u.next)



#define invalidateTMcache(t)	((t)->flags &= ~maskflags)




#define BITDUMMY		(1 << 6)
#define NOTBITDUMMY		cast_byte(~BITDUMMY)
#define isdummy(t)		((t)->flags & BITDUMMY)

#define setnodummy(t)		((t)->flags &= NOTBITDUMMY)
#define setdummy(t)		((t)->flags |= BITDUMMY)




#define allocsizenode(t)	(isdummy(t) ? 0 : sizenode(t))



#define nodefromval(v)	cast(Node *, (v))



#define luaH_fastgeti(t,k,res,tag) \
  { Table *h = t; lua_Unsigned u = l_castS2U(k); \
    if ((u - 1u < h->alimit)) { \
      tag = *getArrTag(h,(u)-1u); \
      if (!tagisempty(tag)) { farr2val(h, u, tag, res); }} \
    else { tag = luaH_getint(h, u, res); }}


#define luaH_fastseti(t,k,val,hres) \
  { Table *h = t; lua_Unsigned u = l_castS2U(k); \
    if ((u - 1u < h->alimit)) { \
      lu_byte *tag = getArrTag(h,(u)-1u); \
      if (tagisempty(*tag)) hres = ~cast_int(u); \
      else { fval2arr(h, u, tag, val); hres = HOK; }} \
    else { hres = luaH_psetint(h, u, val); }}



#define HOK		0
#define HNOTFOUND	1
#define HNOTATABLE	2
#define HFIRSTNODE	3





#define NM      cast_uint(sizeof(Value))

struct ArrayCell {
  lu_byte tag[NM];
  Value value[NM];
};



#define getArrTag(t,k)	(&(t)->array[(k)/NM].tag[(k)%NM])


#define getArrVal(t,k)	(&(t)->array[(k)/NM].value[(k)%NM])



#define arr2obj(h,k,val)  \
  ((val)->tt_ = *getArrTag(h,(k)-1u), (val)->value_ = *getArrVal(h,(k)-1u))

#define obj2arr(h,k,val)  \
  (*getArrTag(h,(k)-1u) = (val)->tt_, *getArrVal(h,(k)-1u) = (val)->value_)



#define farr2val(h,k,tag,res)  \
  ((res)->tt_ = tag, (res)->value_ = *getArrVal(h,(k)-1u))

#define fval2arr(h,k,tag,val)  \
  (*tag = (val)->tt_, *getArrVal(h,(k)-1u) = (val)->value_)


LUAI_FUNC int luaH_get (Table *t, const TValue *key, TValue *res);
LUAI_FUNC int luaH_getshortstr (Table *t, TString *key, TValue *res);
LUAI_FUNC int luaH_getstr (Table *t, TString *key, TValue *res);
LUAI_FUNC int luaH_getint (Table *t, lua_Integer key, TValue *res);


LUAI_FUNC const TValue *luaH_Hgetshortstr (Table *t, TString *key);

LUAI_FUNC TString *luaH_getstrkey (Table *t, TString *key);

LUAI_FUNC int luaH_psetint (Table *t, lua_Integer key, TValue *val);
LUAI_FUNC int luaH_psetshortstr (Table *t, TString *key, TValue *val);
LUAI_FUNC int luaH_psetstr (Table *t, TString *key, TValue *val);
LUAI_FUNC int luaH_pset (Table *t, const TValue *key, TValue *val);

LUAI_FUNC void luaH_setint (lua_State *L, Table *t, lua_Integer key,
                                                    TValue *value);
LUAI_FUNC void luaH_set (lua_State *L, Table *t, const TValue *key,
                                                 TValue *value);

LUAI_FUNC void luaH_finishset (lua_State *L, Table *t, const TValue *key,
                                              TValue *value, int hres);
LUAI_FUNC Table *luaH_new (lua_State *L);
LUAI_FUNC void luaH_resize (lua_State *L, Table *t, unsigned nasize,
                                                    unsigned nhsize);
LUAI_FUNC void luaH_resizearray (lua_State *L, Table *t, unsigned nasize);
LUAI_FUNC void luaH_free (lua_State *L, Table *t);
LUAI_FUNC int luaH_next (lua_State *L, Table *t, StkId key);
LUAI_FUNC lua_Unsigned luaH_getn (Table *t);
LUAI_FUNC unsigned luaH_realasize (const Table *t);


#if defined(LUA_DEBUG)
LUAI_FUNC Node *luaH_mainposition (const Table *t, const TValue *key);
#endif


#endif


//path: src/imports/../dependencies/lua/lundump.h


#ifndef lundump_h
#define lundump_h

#include <limits.h>










#define LUAC_DATA	"\x19\x93\r\n\x1a\n"

#define LUAC_INT	0x5678
#define LUAC_NUM	cast_num(370.5)


#define LUAC_VERSION	(LUA_VERSION_MAJOR_N*16+LUA_VERSION_MINOR_N)

#define LUAC_FORMAT	0	



LUAI_FUNC LClosure* luaU_undump (lua_State* L, ZIO* Z, const char* name,
                                               int fixed);


LUAI_FUNC int luaU_dump (lua_State* L, const Proto* f, lua_Writer w,
                         void* data, int strip);

#endif






#if !defined(luai_verifycode)
#define luai_verifycode(L,f)  
#endif


typedef struct {
  lua_State *L;
  ZIO *Z;
  const char *name;
  Table *h;  
  lu_mem offset;  
  lua_Integer nstr;  
  lu_byte fixed;  
} LoadState;


static l_noret private_lua_error (LoadState *S, const char *why) {
  luaO_pushfstring(S->L, "%s: bad binary format (%s)", S->name, why);
  luaD_throw(S->L, LUA_ERRSYNTAX);
}



#define loadVector(S,b,n)	loadBlock(S,b,(n)*sizeof((b)[0]))

static void loadBlock (LoadState *S, void *b, size_t size) {
  if (luaZ_read(S->Z, b, size) != 0)
      private_lua_error(S, "truncated chunk");
  S->offset += size;
}


static void loadAlign (LoadState *S, int align) {
  int padding = align - (S->offset % align);
  if (padding < align) {  
    lua_Integer paddingContent;
    loadBlock(S, &paddingContent, padding);
    lua_assert(S->offset % align == 0);
  }
}


#define getaddr(S,n,t)	cast(t *, getaddr_(S,(n) * sizeof(t)))

static const void *getaddr_ (LoadState *S, size_t size) {
  const void *block = luaZ_getaddr(S->Z, size);
  S->offset += size;
  if (block == NULL)
      private_lua_error(S, "truncated fixed buffer");
  return block;
}


#define loadVar(S,x)		loadVector(S,&x,1)


static lu_byte loadByte (LoadState *S) {
  int b = zgetc(S->Z);
  if (b == EOZ)
      private_lua_error(S, "truncated chunk");
  S->offset++;
  return cast_byte(b);
}


static size_t loadVarint (LoadState *S, size_t limit) {
  size_t x = 0;
  int b;
  limit >>= 7;
  do {
    b = loadByte(S);
    if (x > limit)
        private_lua_error(S, "integer overflow");
    x = (x << 7) | (b & 0x7f);
  } while ((b & 0x80) != 0);
  return x;
}


static size_t loadSize (LoadState *S) {
  return loadVarint(S, MAX_SIZET);
}


static int loadInt (LoadState *S) {
  return cast_int(loadVarint(S, cast_sizet(INT_MAX)));
}


static lua_Number loadNumber (LoadState *S) {
  lua_Number x;
  loadVar(S, x);
  return x;
}


static lua_Integer loadInteger (LoadState *S) {
  lua_Integer x;
  loadVar(S, x);
  return x;
}



static void loadString (LoadState *S, Proto *p, TString **sl) {
  lua_State *L = S->L;
  TString *ts;
  TValue sv;
  size_t size = loadSize(S);
  if (size == 0) {  
    lua_assert(*sl == NULL);  
    return;
  }
  else if (size == 1) {  
    lua_Integer idx = cast(lua_Integer, loadSize(S));  
    TValue stv;
    luaH_getint(S->h, idx, &stv);  
    *sl = ts = tsvalue(&stv);
    luaC_objbarrier(L, p, ts);
    return;  
  }
  else if ((size -= 2) <= LUAI_MAXSHORTLEN) {  
    char buff[LUAI_MAXSHORTLEN + 1];  
    loadVector(S, buff, size + 1);  
    *sl = ts = luaS_newlstr(L, buff, size);  
    luaC_objbarrier(L, p, ts);
  }
  else if (S->fixed) {  
    const char *s = getaddr(S, size + 1, char);  
    *sl = ts = luaS_newextlstr(L, s, size, NULL, NULL);
    luaC_objbarrier(L, p, ts);
  }
  else {  
    *sl = ts = luaS_createlngstrobj(L, size);  
    luaC_objbarrier(L, p, ts);
    loadVector(S, getlngstr(ts), size + 1);  
  }
  
  S->nstr++;
  setsvalue(L, &sv, ts);
  luaH_setint(L, S->h, S->nstr, &sv);
  luaC_objbarrierback(L, obj2gco(S->h), ts);
}


static void loadCode (LoadState *S, Proto *f) {
  int n = loadInt(S);
  loadAlign(S, sizeof(f->code[0]));
  if (S->fixed) {
    f->code = getaddr(S, n, Instruction);
    f->sizecode = n;
  }
  else {
    f->code = luaM_newvectorchecked(S->L, n, Instruction);
    f->sizecode = n;
    loadVector(S, f->code, n);
  }
}


static void loadFunction(LoadState *S, Proto *f);


static void loadConstants (LoadState *S, Proto *f) {
  int i;
  int n = loadInt(S);
  f->k = luaM_newvectorchecked(S->L, n, TValue);
  f->sizek = n;
  for (i = 0; i < n; i++)
    setnilvalue(&f->k[i]);
  for (i = 0; i < n; i++) {
    TValue *o = &f->k[i];
    int t = loadByte(S);
    switch (t) {
      case LUA_VNIL:
        setnilvalue(o);
        break;
      case LUA_VFALSE:
        setbfvalue(o);
        break;
      case LUA_VTRUE:
        setbtvalue(o);
        break;
      case LUA_VNUMFLT:
        setfltvalue(o, loadNumber(S));
        break;
      case LUA_VNUMINT:
        setivalue(o, loadInteger(S));
        break;
      case LUA_VSHRSTR:
      case LUA_VLNGSTR: {
        lua_assert(f->source == NULL);
        loadString(S, f, &f->source);  
        if (f->source == NULL)
            private_lua_error(S, "bad format for constant string");
        setsvalue2n(S->L, o, f->source);  
        f->source = NULL;
        break;
      }
      default: lua_assert(0);
    }
  }
}


static void loadProtos (LoadState *S, Proto *f) {
  int i;
  int n = loadInt(S);
  f->p = luaM_newvectorchecked(S->L, n, Proto *);
  f->sizep = n;
  for (i = 0; i < n; i++)
    f->p[i] = NULL;
  for (i = 0; i < n; i++) {
    f->p[i] = luaF_newproto(S->L);
    luaC_objbarrier(S->L, f, f->p[i]);
    loadFunction(S, f->p[i]);
  }
}



static void loadUpvalues (LoadState *S, Proto *f) {
  int i, n;
  n = loadInt(S);
  f->upvalues = luaM_newvectorchecked(S->L, n, Upvaldesc);
  f->sizeupvalues = n;
  for (i = 0; i < n; i++)  
    f->upvalues[i].name = NULL;
  for (i = 0; i < n; i++) {  
    f->upvalues[i].instack = loadByte(S);
    f->upvalues[i].idx = loadByte(S);
    f->upvalues[i].kind = loadByte(S);
  }
}


static void loadDebug (LoadState *S, Proto *f) {
  int i, n;
  n = loadInt(S);
  if (S->fixed) {
    f->lineinfo = getaddr(S, n, ls_byte);
    f->sizelineinfo = n;
  }
  else {
    f->lineinfo = luaM_newvectorchecked(S->L, n, ls_byte);
    f->sizelineinfo = n;
    loadVector(S, f->lineinfo, n);
  }
  n = loadInt(S);
  if (n > 0) {
    loadAlign(S, sizeof(int));
    if (S->fixed) {
      f->abslineinfo = getaddr(S, n, AbsLineInfo);
      f->sizeabslineinfo = n;
    }
    else {
      f->abslineinfo = luaM_newvectorchecked(S->L, n, AbsLineInfo);
      f->sizeabslineinfo = n;
      loadVector(S, f->abslineinfo, n);
    }
  }
  n = loadInt(S);
  f->locvars = luaM_newvectorchecked(S->L, n, LocVar);
  f->sizelocvars = n;
  for (i = 0; i < n; i++)
    f->locvars[i].varname = NULL;
  for (i = 0; i < n; i++) {
    loadString(S, f, &f->locvars[i].varname);
    f->locvars[i].startpc = loadInt(S);
    f->locvars[i].endpc = loadInt(S);
  }
  n = loadInt(S);
  if (n != 0)  
    n = f->sizeupvalues;  
  for (i = 0; i < n; i++)
    loadString(S, f, &f->upvalues[i].name);
}


static void loadFunction (LoadState *S, Proto *f) {
  f->linedefined = loadInt(S);
  f->lastlinedefined = loadInt(S);
  f->numparams = loadByte(S);
  f->flag = loadByte(S) & PF_ISVARARG;  
  if (S->fixed)
    f->flag |= PF_FIXED;  
  f->maxstacksize = loadByte(S);
  loadCode(S, f);
  loadConstants(S, f);
  loadUpvalues(S, f);
  loadProtos(S, f);
  loadString(S, f, &f->source);
  loadDebug(S, f);
}


static void checkliteral (LoadState *S, const char *s, const char *msg) {
  char buff[sizeof(LUA_SIGNATURE) + sizeof(LUAC_DATA)]; 
  size_t len = strlen(s);
  loadVector(S, buff, len);
  if (memcmp(s, buff, len) != 0)
      private_lua_error(S, msg);
}


static void fchecksize (LoadState *S, size_t size, const char *tname) {
  if (loadByte(S) != size)
      private_lua_error(S, luaO_pushfstring(S->L, "%s size mismatch", tname));
}


#define checksize(S,t)	fchecksize(S,sizeof(t),#t)

static void checkHeader (LoadState *S) {
  
  checkliteral(S, &LUA_SIGNATURE[1], "not a binary chunk");
  if (loadByte(S) != LUAC_VERSION)
      private_lua_error(S, "version mismatch");
  if (loadByte(S) != LUAC_FORMAT)
      private_lua_error(S, "format mismatch");
  checkliteral(S, LUAC_DATA, "corrupted chunk");
  checksize(S, Instruction);
  checksize(S, lua_Integer);
  checksize(S, lua_Number);
  if (loadInteger(S) != LUAC_INT)
      private_lua_error(S, "integer format mismatch");
  if (loadNumber(S) != LUAC_NUM)
      private_lua_error(S, "float format mismatch");
}



LClosure *luaU_undump (lua_State *L, ZIO *Z, const char *name, int fixed) {
  LoadState S;
  LClosure *cl;
  if (*name == '@' || *name == '=')
    S.name = name + 1;
  else if (*name == LUA_SIGNATURE[0])
    S.name = "binary string";
  else
    S.name = name;
  S.L = L;
  S.Z = Z;
  S.fixed = fixed;
  S.offset = 1;  
  checkHeader(&S);
  cl = luaF_newLclosure(L, loadByte(&S));
  setclLvalue2s(L, L->top.p, cl);
  luaD_inctop(L);
  S.h = luaH_new(L);  
  S.nstr = 0;
  sethvalue2s(L, L->top.p, S.h);  
  luaD_inctop(L);
  cl->p = luaF_newproto(L);
  luaC_objbarrier(L, cl, cl->p);
  loadFunction(&S, cl->p);
  lua_assert(cl->nupvalues == cl->p->sizeupvalues);
  luai_verifycode(L, cl->p);
  L->top.p--;  
  return cl;
}



//path: src/imports/../dependencies/lua/ldump.c


#define ldump_c
#define LUA_CORE





#include <limits.h>
#include <stddef.h>
















typedef struct {
  lua_State *L;
  lua_Writer writer;
  void *data;
  lu_mem offset;  
  int strip;
  int status;
  Table *h;  
  lua_Integer nstr;  
} DumpState;



#define dumpVector(D,v,n)	dumpBlock(D,v,(n)*sizeof((v)[0]))

#define dumpLiteral(D, s)	dumpBlock(D,s,sizeof(s) - sizeof(char))



static void dumpBlock (DumpState *D, const void *b, size_t size) {
  if (D->status == 0) {  
    lua_unlock(D->L);
    D->status = (*D->writer)(D->L, b, size, D->data);
    lua_lock(D->L);
    D->offset += size;
  }
}



static void dumpAlign (DumpState *D, int align) {
  int padding = align - (D->offset % align);
  if (padding < align) {  
    static lua_Integer paddingContent = 0;
    lua_assert(cast_uint(align) <= sizeof(lua_Integer));
    dumpBlock(D, &paddingContent, padding);
  }
  lua_assert(D->offset % align == 0);
}


#define dumpVar(D,x)		dumpVector(D,&x,1)


static void dumpByte (DumpState *D, int y) {
  lu_byte x = (lu_byte)y;
  dumpVar(D, x);
}



#define DIBS    ((sizeof(size_t) * CHAR_BIT + 6) / 7)


static void dumpVarint (DumpState *D, size_t x) {
  lu_byte buff[DIBS];
  int n = 1;
  buff[DIBS - 1] = x & 0x7f;  
  while ((x >>= 7) != 0)  
    buff[DIBS - (++n)] = (x & 0x7f) | 0x80;
  dumpVector(D, buff + DIBS - n, n);
}


static void dumpSize (DumpState *D, size_t sz) {
  dumpVarint(D, sz);
}

static void dumpInt (DumpState *D, int x) {
  lua_assert(x >= 0);
  dumpVarint(D, cast(size_t, x));
}


static void dumpNumber (DumpState *D, lua_Number x) {
  dumpVar(D, x);
}


static void dumpInteger (DumpState *D, lua_Integer x) {
  dumpVar(D, x);
}



static void dumpString (DumpState *D, TString *ts) {
  if (ts == NULL)
    dumpSize(D, 0);
  else {
    TValue idx;
    int tag = luaH_getstr(D->h, ts, &idx);
    if (!tagisempty(tag)) {  
      dumpSize(D, 1);  
      dumpSize(D, cast_sizet(ivalue(&idx)));  
    }
    else {  
      TValue key, value;  
      size_t size;
      const char *s = getlstr(ts, size);
      dumpSize(D, size + 2);
      dumpVector(D, s, size + 1);  
      D->nstr++;  
      setsvalue(D->L, &key, ts);  
      setivalue(&value, D->nstr);  
      luaH_set(D->L, D->h, &key, &value);  
      
    }
  }
}


static void dumpCode (DumpState *D, const Proto *f) {
  dumpInt(D, f->sizecode);
  dumpAlign(D, sizeof(f->code[0]));
  lua_assert(f->code != NULL);
  dumpVector(D, f->code, f->sizecode);
}


static void dumpFunction (DumpState *D, const Proto *f);

static void dumpConstants (DumpState *D, const Proto *f) {
  int i;
  int n = f->sizek;
  dumpInt(D, n);
  for (i = 0; i < n; i++) {
    const TValue *o = &f->k[i];
    int tt = ttypetag(o);
    dumpByte(D, tt);
    switch (tt) {
      case LUA_VNUMFLT:
        dumpNumber(D, fltvalue(o));
        break;
      case LUA_VNUMINT:
        dumpInteger(D, ivalue(o));
        break;
      case LUA_VSHRSTR:
      case LUA_VLNGSTR:
        dumpString(D, tsvalue(o));
        break;
      default:
        lua_assert(tt == LUA_VNIL || tt == LUA_VFALSE || tt == LUA_VTRUE);
    }
  }
}


static void dumpProtos (DumpState *D, const Proto *f) {
  int i;
  int n = f->sizep;
  dumpInt(D, n);
  for (i = 0; i < n; i++)
    dumpFunction(D, f->p[i]);
}


static void dumpUpvalues (DumpState *D, const Proto *f) {
  int i, n = f->sizeupvalues;
  dumpInt(D, n);
  for (i = 0; i < n; i++) {
    dumpByte(D, f->upvalues[i].instack);
    dumpByte(D, f->upvalues[i].idx);
    dumpByte(D, f->upvalues[i].kind);
  }
}


static void dumpDebug (DumpState *D, const Proto *f) {
  int i, n;
  n = (D->strip) ? 0 : f->sizelineinfo;
  dumpInt(D, n);
  if (f->lineinfo != NULL)
    dumpVector(D, f->lineinfo, n);
  n = (D->strip) ? 0 : f->sizeabslineinfo;
  dumpInt(D, n);
  if (n > 0) {
    
    dumpAlign(D, sizeof(int));
    dumpVector(D, f->abslineinfo, n);
  }
  n = (D->strip) ? 0 : f->sizelocvars;
  dumpInt(D, n);
  for (i = 0; i < n; i++) {
    dumpString(D, f->locvars[i].varname);
    dumpInt(D, f->locvars[i].startpc);
    dumpInt(D, f->locvars[i].endpc);
  }
  n = (D->strip) ? 0 : f->sizeupvalues;
  dumpInt(D, n);
  for (i = 0; i < n; i++)
    dumpString(D, f->upvalues[i].name);
}


static void dumpFunction (DumpState *D, const Proto *f) {
  dumpInt(D, f->linedefined);
  dumpInt(D, f->lastlinedefined);
  dumpByte(D, f->numparams);
  dumpByte(D, f->flag);
  dumpByte(D, f->maxstacksize);
  dumpCode(D, f);
  dumpConstants(D, f);
  dumpUpvalues(D, f);
  dumpProtos(D, f);
  dumpString(D, D->strip ? NULL : f->source);
  dumpDebug(D, f);
}


static void dumpHeader (DumpState *D) {
  dumpLiteral(D, LUA_SIGNATURE);
  dumpByte(D, LUAC_VERSION);
  dumpByte(D, LUAC_FORMAT);
  dumpLiteral(D, LUAC_DATA);
  dumpByte(D, sizeof(Instruction));
  dumpByte(D, sizeof(lua_Integer));
  dumpByte(D, sizeof(lua_Number));
  dumpInteger(D, LUAC_INT);
  dumpNumber(D, LUAC_NUM);
}



int luaU_dump (lua_State *L, const Proto *f, lua_Writer w, void *data,
               int strip) {
  DumpState D;
  D.h = luaH_new(L);  
  sethvalue2s(L, L->top.p, D.h);  
  L->top.p++;
  D.L = L;
  D.writer = w;
  D.offset = 0;
  D.data = data;
  D.strip = strip;
  D.status = 0;
  D.nstr = 0;
  dumpHeader(&D);
  dumpByte(&D, f->sizeupvalues);
  dumpFunction(&D, f);
  dumpBlock(&D, NULL, 0);  
  return D.status;
}



//path: src/imports/../dependencies/lua/lstate.c


#define lstate_c
#define LUA_CORE





#include <stddef.h>
#include <string.h>




//path: src/imports/../dependencies/lua/lapi.h


#ifndef lapi_h
#define lapi_h









#define api_incr_top(L)  \
    (L->top.p++, api_check(L, L->top.p <= L->ci->top.p, "stack overflow"))



#define adjustresults(L,nres) \
    { if ((nres) <= LUA_MULTRET && L->ci->top.p < L->top.p) \
	L->ci->top.p = L->top.p; }



#define api_checknelems(L,n) \
       api_check(L, (n) < (L->top.p - L->ci->func.p), \
                         "not enough elements in the stack")



#define api_checkpop(L,n) \
	api_check(L, (n) < L->top.p - L->ci->func.p &&  \
                     L->tbclist.p < L->top.p - (n), \
			  "not enough free elements in the stack")




#define hastocloseCfunc(n)	((n) < LUA_MULTRET)


#define codeNresults(n)		(-(n) - 3)
#define decodeNresults(n)	(-(n) - 3)

#endif










//path: src/imports/../dependencies/lua/llex.h


#ifndef llex_h
#define llex_h

#include <limits.h>








#define FIRST_RESERVED	(UCHAR_MAX + 1)


#if !defined(LUA_ENV)
#define LUA_ENV		"_ENV"
#endif



enum RESERVED {
  
  TK_AND = FIRST_RESERVED, TK_BREAK,
  TK_DO, TK_ELSE, TK_ELSEIF, TK_END, TK_FALSE, TK_FOR, TK_FUNCTION,
  TK_GOTO, TK_IF, TK_IN, TK_LOCAL, TK_NIL, TK_NOT, TK_OR, TK_REPEAT,
  TK_RETURN, TK_THEN, TK_TRUE, TK_UNTIL, TK_WHILE,
  
  TK_IDIV, TK_CONCAT, TK_DOTS, TK_EQ, TK_GE, TK_LE, TK_NE,
  TK_SHL, TK_SHR,
  TK_DBCOLON, TK_EOS,
  TK_FLT, TK_INT, TK_NAME, TK_STRING
};


#define NUM_RESERVED	(cast_int(TK_WHILE-FIRST_RESERVED + 1))


typedef union {
  lua_Number r;
  lua_Integer i;
  TString *ts;
} SemInfo;  


typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;



typedef struct LexState {
  int current;  
  int linenumber;  
  int lastline;  
  Token t;  
  Token lookahead;  
  struct FuncState *fs;  
  struct lua_State *L;
  ZIO *z;  
  Mbuffer *buff;  
  Table *h;  
  struct Dyndata *dyd;  
  TString *source;  
  TString *envn;  
} LexState;


LUAI_FUNC void luaX_init (lua_State *L);
LUAI_FUNC void luaX_setinput (lua_State *L, LexState *ls, ZIO *z,
                              TString *source, int firstchar);
LUAI_FUNC TString *luaX_newstring (LexState *ls, const char *str, size_t l);
LUAI_FUNC void luaX_next (LexState *ls);
LUAI_FUNC int luaX_lookahead (LexState *ls);
LUAI_FUNC l_noret luaX_syntaxerror (LexState *ls, const char *s);
LUAI_FUNC const char *luaX_token2str (LexState *ls, int token);


#endif
















typedef struct LX {
  lu_byte extra_[LUA_EXTRASPACE];
  lua_State l;
} LX;



typedef struct LG {
  LX l;
  global_State g;
} LG;



#define fromstate(L)	(cast(LX *, cast(lu_byte *, (L)) - offsetof(LX, l)))



void luaE_setdebt (global_State *g, l_obj debt) {
  l_obj tb = gettotalobjs(g);
  lua_assert(tb > 0);
  if (debt > MAX_LOBJ - tb)
    debt = MAX_LOBJ - tb;  
  g->totalobjs = tb + debt;
  g->GCdebt = debt;
}


CallInfo *luaE_extendCI (lua_State *L) {
  CallInfo *ci;
  lua_assert(L->ci->next == NULL);
  ci = luaM_new(L, CallInfo);
  lua_assert(L->ci->next == NULL);
  L->ci->next = ci;
  ci->previous = L->ci;
  ci->next = NULL;
  ci->u.l.trap = 0;
  L->nci++;
  return ci;
}



static void freeCI (lua_State *L) {
  CallInfo *ci = L->ci;
  CallInfo *next = ci->next;
  ci->next = NULL;
  while ((ci = next) != NULL) {
    next = ci->next;
    luaM_free(L, ci);
    L->nci--;
  }
}



void luaE_shrinkCI (lua_State *L) {
  CallInfo *ci = L->ci->next;  
  CallInfo *next;
  if (ci == NULL)
    return;  
  while ((next = ci->next) != NULL) {  
    CallInfo *next2 = next->next;  
    ci->next = next2;  
    L->nci--;
    luaM_free(L, next);  
    if (next2 == NULL)
      break;  
    else {
      next2->previous = ci;
      ci = next2;  
    }
  }
}



void luaE_checkcstack (lua_State *L) {
  if (getCcalls(L) == LUAI_MAXCCALLS)
    luaG_runerror(L, "C stack overflow");
  else if (getCcalls(L) >= (LUAI_MAXCCALLS / 10 * 11))
    luaD_throw(L, LUA_ERRERR);  
}


LUAI_FUNC void luaE_incCstack (lua_State *L) {
  L->nCcalls++;
  if (l_unlikely(getCcalls(L) >= LUAI_MAXCCALLS))
    luaE_checkcstack(L);
}


static void stack_init (lua_State *L1, lua_State *L) {
  int i; CallInfo *ci;
  
  L1->stack.p = luaM_newvector(L, BASIC_STACK_SIZE + EXTRA_STACK, StackValue);
  L1->tbclist.p = L1->stack.p;
  for (i = 0; i < BASIC_STACK_SIZE + EXTRA_STACK; i++)
    setnilvalue(s2v(L1->stack.p + i));  
  L1->top.p = L1->stack.p;
  L1->stack_last.p = L1->stack.p + BASIC_STACK_SIZE;
  
  ci = &L1->base_ci;
  ci->next = ci->previous = NULL;
  ci->callstatus = CIST_C;
  ci->func.p = L1->top.p;
  ci->u.c.k = NULL;
  ci->nresults = 0;
  setnilvalue(s2v(L1->top.p));  
  L1->top.p++;
  ci->top.p = L1->top.p + LUA_MINSTACK;
  L1->ci = ci;
}


static void freestack (lua_State *L) {
  if (L->stack.p == NULL)
    return;  
  L->ci = &L->base_ci;  
  freeCI(L);
  lua_assert(L->nci == 0);
  luaM_freearray(L, L->stack.p, stacksize(L) + EXTRA_STACK);  
}



static void init_registry (lua_State *L, global_State *g) {
  
  TValue aux;
  Table *registry = luaH_new(L);
  sethvalue(L, &g->l_registry, registry);
  luaH_resize(L, registry, LUA_RIDX_LAST, 0);
  
  setbfvalue(&aux);
  luaH_setint(L, registry, 1, &aux);
  
  setthvalue(L, &aux, L);
  luaH_setint(L, registry, LUA_RIDX_MAINTHREAD, &aux);
  
  sethvalue(L, &aux, luaH_new(L));
  luaH_setint(L, registry, LUA_RIDX_GLOBALS, &aux);
}



static void f_luaopen (lua_State *L, void *ud) {
  global_State *g = G(L);
  UNUSED(ud);
  stack_init(L, L);  
  init_registry(L, g);
  luaS_init(L);
  luaT_init(L);
  luaX_init(L);
  g->gcstp = 0;  
  setnilvalue(&g->nilvalue);  
  luai_userstateopen(L);
}



static void preinit_thread (lua_State *L, global_State *g) {
  G(L) = g;
  L->stack.p = NULL;
  L->ci = NULL;
  L->nci = 0;
  L->twups = L;  
  L->nCcalls = 0;
  L->errorJmp = NULL;
  L->hook = NULL;
  L->hookmask = 0;
  L->basehookcount = 0;
  L->allowhook = 1;
  resethookcount(L);
  L->openupval = NULL;
  L->status = LUA_OK;
  L->errfunc = 0;
  L->oldpc = 0;
}


static void close_state (lua_State *L) {
  global_State *g = G(L);
  if (!completestate(g))  
    luaC_freeallobjects(L);  
  else {  
    L->ci = &L->base_ci;  
    luaD_closeprotected(L, 1, LUA_OK);  
    luaC_freeallobjects(L);  
    luai_userstateclose(L);
  }
  luaM_freearray(L, G(L)->strt.hash, G(L)->strt.size);
  freestack(L);
  lua_assert(g->totalbytes == sizeof(LG));
  lua_assert(gettotalobjs(g) == 1);
  (*g->frealloc)(g->ud, fromstate(L), sizeof(LG), 0);  
}


LUA_API lua_State *lua_newthread (lua_State *L) {
  global_State *g = G(L);
  GCObject *o;
  lua_State *L1;
  lua_lock(L);
  luaC_checkGC(L);
  
  o = luaC_newobjdt(L, LUA_TTHREAD, sizeof(LX), offsetof(LX, l));
  L1 = gco2th(o);
  
  setthvalue2s(L, L->top.p, L1);
  api_incr_top(L);
  preinit_thread(L1, g);
  L1->hookmask = L->hookmask;
  L1->basehookcount = L->basehookcount;
  L1->hook = L->hook;
  resethookcount(L1);
  
  memcpy(lua_getextraspace(L1), lua_getextraspace(g->mainthread),
         LUA_EXTRASPACE);
  luai_userstatethread(L, L1);
  stack_init(L1, L);  
  lua_unlock(L);
  return L1;
}


void luaE_freethread (lua_State *L, lua_State *L1) {
  LX *l = fromstate(L1);
  luaF_closeupval(L1, L1->stack.p);  
  lua_assert(L1->openupval == NULL);
  luai_userstatefree(L, L1);
  freestack(L1);
  luaM_free(L, l);
}


int luaE_resetthread (lua_State *L, int status) {
  CallInfo *ci = L->ci = &L->base_ci;  
  setnilvalue(s2v(L->stack.p));  
  ci->func.p = L->stack.p;
  ci->callstatus = CIST_C;
  if (status == LUA_YIELD)
    status = LUA_OK;
  L->status = LUA_OK;  
  status = luaD_closeprotected(L, 1, status);
  if (status != LUA_OK)  
    luaD_seterrorobj(L, status, L->stack.p + 1);
  else
    L->top.p = L->stack.p + 1;
  ci->top.p = L->top.p + LUA_MINSTACK;
  luaD_reallocstack(L, cast_int(ci->top.p - L->stack.p), 0);
  return status;
}


LUA_API int lua_closethread (lua_State *L, lua_State *from) {
  int status;
  lua_lock(L);
  L->nCcalls = (from) ? getCcalls(from) : 0;
  status = luaE_resetthread(L, L->status);
  lua_unlock(L);
  return status;
}


LUA_API lua_State *lua_newstate (lua_Alloc f, void *ud, unsigned seed) {
  int i;
  lua_State *L;
  global_State *g;
  LG *l = cast(LG *, (*f)(ud, NULL, LUA_TTHREAD, sizeof(LG)));
  if (l == NULL) return NULL;
  L = &l->l.l;
  g = &l->g;
  L->tt = LUA_VTHREAD;
  g->currentwhite = bitmask(WHITE0BIT);
  L->marked = luaC_white(g);
  preinit_thread(L, g);
  g->allgc = obj2gco(L);  
  L->next = NULL;
  incnny(L);  
  g->frealloc = f;
  g->ud = ud;
  g->warnf = NULL;
  g->ud_warn = NULL;
  g->mainthread = L;
  g->seed = seed;
  g->gcstp = GCSTPGC;  
  g->strt.size = g->strt.nuse = 0;
  g->strt.hash = NULL;
  setnilvalue(&g->l_registry);
  g->panic = NULL;
  g->gcstate = GCSpause;
  g->gckind = KGC_INC;
  g->gcstopem = 0;
  g->gcemergency = 0;
  g->finobj = g->tobefnz = g->fixedgc = NULL;
  g->firstold1 = g->survival = g->old1 = g->reallyold = NULL;
  g->finobjsur = g->finobjold1 = g->finobjrold = NULL;
  g->sweepgc = NULL;
  g->gray = g->grayagain = NULL;
  g->weak = g->ephemeron = g->allweak = NULL;
  g->twups = NULL;
  g->totalbytes = sizeof(LG);
  g->totalobjs = 1;
  g->marked = 0;
  g->GCdebt = 0;
  setivalue(&g->nilvalue, 0);  
  setgcparam(g, PAUSE, LUAI_GCPAUSE);
  setgcparam(g, STEPMUL, LUAI_GCMUL);
  setgcparam(g, STEPSIZE, LUAI_GCSTEPSIZE);
  setgcparam(g, MINORMUL, LUAI_GENMINORMUL);
  setgcparam(g, MINORMAJOR, LUAI_MINORMAJOR);
  setgcparam(g, MAJORMINOR, LUAI_MAJORMINOR);
  for (i=0; i < LUA_NUMTYPES; i++) g->mt[i] = NULL;
  if (luaD_rawrunprotected(L, f_luaopen, NULL) != LUA_OK) {
    
    close_state(L);
    L = NULL;
  }
  return L;
}


LUA_API void lua_close (lua_State *L) {
  lua_lock(L);
  L = G(L)->mainthread;  
  close_state(L);
}


void luaE_warning (lua_State *L, const char *msg, int tocont) {
  lua_WarnFunction wf = G(L)->warnf;
  if (wf != NULL)
    wf(G(L)->ud_warn, msg, tocont);
}



void luaE_warnerror (lua_State *L, const char *where) {
  TValue *errobj = s2v(L->top.p - 1);  
  const char *msg = (ttisstring(errobj))
                  ? getstr(tsvalue(errobj))
                  : "private_lua_error object is not a string";
  
  luaE_warning(L, "private_lua_error in ", 1);
  luaE_warning(L, where, 1);
  luaE_warning(L, " (", 1);
  luaE_warning(L, msg, 1);
  luaE_warning(L, ")", 0);
}



//path: src/imports/../dependencies/lua/lgc.c


#define lgc_c
#define LUA_CORE




#include <string.h>






























#define NFIXED		(TM_N + NUM_RESERVED + 2)



#define GCSWEEPMAX	20



#define maskcolors	(bitmask(BLACKBIT) | WHITEBITS)


#define maskgcbits      (maskcolors | AGEBITS)



#define makewhite(g,x)	\
  (x->marked = cast_byte((x->marked & ~maskcolors) | luaC_white(g)))


#define set2gray(x)	resetbits(x->marked, maskcolors)



#define set2black(x)  \
  (x->marked = cast_byte((x->marked & ~WHITEBITS) | bitmask(BLACKBIT)))


#define valiswhite(x)   (iscollectable(x) && iswhite(gcvalue(x)))

#define keyiswhite(n)   (keyiscollectable(n) && iswhite(gckey(n)))



#define gcvalueN(o)     (iscollectable(o) ? gcvalue(o) : NULL)



#define gcvalarr(t,i)  \
	((*getArrTag(t,i) & BIT_ISCOLLECTABLE) ? getArrVal(t,i)->gc : NULL)


#define markvalue(g,o) { checkliveness(g->mainthread,o); \
  if (valiswhite(o)) reallymarkobject(g,gcvalue(o)); }

#define markkey(g, n)	{ if keyiswhite(n) reallymarkobject(g,gckey(n)); }

#define markobject(g,t)	{ if (iswhite(t)) reallymarkobject(g, obj2gco(t)); }


#define markobjectN(g,t)	{ if (t) markobject(g,t); }


static void reallymarkobject (global_State *g, GCObject *o);
static l_obj atomic (lua_State *L);
static void entersweep (lua_State *L);






#define gnodelast(h)	gnode(h, cast_sizet(sizenode(h)))


static GCObject **getgclist (GCObject *o) {
  switch (o->tt) {
    case LUA_VTABLE: return &gco2t(o)->gclist;
    case LUA_VLCL: return &gco2lcl(o)->gclist;
    case LUA_VCCL: return &gco2ccl(o)->gclist;
    case LUA_VTHREAD: return &gco2th(o)->gclist;
    case LUA_VPROTO: return &gco2p(o)->gclist;
    case LUA_VUSERDATA: {
      Udata *u = gco2u(o);
      lua_assert(u->nuvalue > 0);
      return &u->gclist;
    }
    default: lua_assert(0); return 0;
  }
}



#define linkgclist(o,p)	linkgclist_(obj2gco(o), &(o)->gclist, &(p))

static void linkgclist_ (GCObject *o, GCObject **pnext, GCObject **list) {
  lua_assert(!isgray(o));  
  *pnext = *list;
  *list = o;
  set2gray(o);  
}



#define linkobjgclist(o,p) linkgclist_(obj2gco(o), getgclist(o), &(p))




static void clearkey (Node *n) {
  lua_assert(isempty(gval(n)));
  if (keyiscollectable(n))
    setdeadkey(n);  
}



static int iscleared (global_State *g, const GCObject *o) {
  if (o == NULL) return 0;  
  else if (novariant(o->tt) == LUA_TSTRING) {
    markobject(g, o);  
    return 0;
  }
  else return iswhite(o);
}



void luaC_barrier_ (lua_State *L, GCObject *o, GCObject *v) {
  global_State *g = G(L);
  lua_assert(isblack(o) && iswhite(v) && !isdead(g, v) && !isdead(g, o));
  if (keepinvariant(g)) {  
    reallymarkobject(g, v);  
    if (isold(o)) {
      lua_assert(!isold(v));  
      setage(v, G_OLD0);  
    }
  }
  else {  
    lua_assert(issweepphase(g));
    if (g->gckind != KGC_GENMINOR)  
      makewhite(g, o);  
  }
}



void luaC_barrierback_ (lua_State *L, GCObject *o) {
  global_State *g = G(L);
  lua_assert(isblack(o) && !isdead(g, o));
  lua_assert((g->gckind != KGC_GENMINOR)
          || (isold(o) && getage(o) != G_TOUCHED1));
  if (getage(o) == G_TOUCHED2)  
    set2gray(o);  
  else  
    linkobjgclist(o, g->grayagain);
  if (isold(o))  
    setage(o, G_TOUCHED1);  
}


void luaC_fix (lua_State *L, GCObject *o) {
  global_State *g = G(L);
  lua_assert(g->allgc == o);  
  set2gray(o);  
  setage(o, G_OLD);  
  g->allgc = o->next;  
  o->next = g->fixedgc;  
  g->fixedgc = o;
}



GCObject *luaC_newobjdt (lua_State *L, int tt, size_t sz, size_t offset) {
  global_State *g = G(L);
  char *p = cast_charp(luaM_newobject(L, novariant(tt), sz));
  GCObject *o = cast(GCObject *, p + offset);
  g->GCdebt--;
  o->marked = luaC_white(g);
  o->tt = tt;
  o->next = g->allgc;
  g->allgc = o;
  return o;
}



GCObject *luaC_newobj (lua_State *L, int tt, size_t sz) {
  return luaC_newobjdt(L, tt, sz, 0);
}









static void reallymarkobject (global_State *g, GCObject *o) {
  g->marked++;
  switch (o->tt) {
    case LUA_VSHRSTR:
    case LUA_VLNGSTR: {
      set2black(o);  
      break;
    }
    case LUA_VUPVAL: {
      UpVal *uv = gco2upv(o);
      if (upisopen(uv))
        set2gray(uv);  
      else
        set2black(uv);  
      markvalue(g, uv->v.p);  
      break;
    }
    case LUA_VUSERDATA: {
      Udata *u = gco2u(o);
      if (u->nuvalue == 0) {  
        markobjectN(g, u->metatable);  
        set2black(u);  
        break;
      }
      
    }  
    case LUA_VLCL: case LUA_VCCL: case LUA_VTABLE:
    case LUA_VTHREAD: case LUA_VPROTO: {
      linkobjgclist(o, g->gray);  
      break;
    }
    default: lua_assert(0); break;
  }
}



static void markmt (global_State *g) {
  int i;
  for (i=0; i < LUA_NUMTYPES; i++)
    markobjectN(g, g->mt[i]);
}



static l_obj markbeingfnz (global_State *g) {
  GCObject *o;
  l_obj count = 0;
  for (o = g->tobefnz; o != NULL; o = o->next) {
    count++;
    markobject(g, o);
  }
  return count;
}



static l_obj remarkupvals (global_State *g) {
  l_obj work = 0;
  lua_State *thread;
  lua_State **p = &g->twups;
  while ((thread = *p) != NULL) {
    if (!iswhite(thread) && thread->openupval != NULL)
      p = &thread->twups;  
    else {  
      UpVal *uv;
      lua_assert(!isold(thread) || thread->openupval == NULL);
      *p = thread->twups;  
      thread->twups = thread;  
      for (uv = thread->openupval; uv != NULL; uv = uv->u.open.next) {
        lua_assert(getage(uv) <= getage(thread));
        if (!iswhite(uv)) {  
          lua_assert(upisopen(uv) && isgray(uv));
          markvalue(g, uv->v.p);  
        }
      }
    }
    work++;
  }
  return work;
}


static void cleargraylists (global_State *g) {
  g->gray = g->grayagain = NULL;
  g->weak = g->allweak = g->ephemeron = NULL;
}



static void restartcollection (global_State *g) {
  cleargraylists(g);
  g->marked = NFIXED;
  markobject(g, g->mainthread);
  markvalue(g, &g->l_registry);
  markmt(g);
  markbeingfnz(g);  
}








static void genlink (global_State *g, GCObject *o) {
  lua_assert(isblack(o));
  if (getage(o) == G_TOUCHED1) {  
    linkobjgclist(o, g->grayagain);  
  }  
  else if (getage(o) == G_TOUCHED2)
    setage(o, G_OLD);  
}



static void traverseweakvalue (global_State *g, Table *h) {
  Node *n, *limit = gnodelast(h);
  
  int hasclears = (h->alimit > 0);
  for (n = gnode(h, 0); n < limit; n++) {  
    if (isempty(gval(n)))  
      clearkey(n);  
    else {
      lua_assert(!keyisnil(n));
      markkey(g, n);
      if (!hasclears && iscleared(g, gcvalueN(gval(n))))  
        hasclears = 1;  
    }
  }
  if (g->gcstate == GCSatomic && hasclears)
    linkgclist(h, g->weak);  
  else
    linkgclist(h, g->grayagain);  
}



static int traversearray (global_State *g, Table *h) {
  unsigned asize = luaH_realasize(h);
  int marked = 0;  
  unsigned i;
  for (i = 0; i < asize; i++) {
    GCObject *o = gcvalarr(h, i);
    if (o != NULL && iswhite(o)) {
      marked = 1;
      reallymarkobject(g, o);
    }
  }
  return marked;
}



static int traverseephemeron (global_State *g, Table *h, int inv) {
  int hasclears = 0;  
  int hasww = 0;  
  unsigned int i;
  unsigned int nsize = sizenode(h);
  int marked = traversearray(g, h);  
  
  for (i = 0; i < nsize; i++) {
    Node *n = inv ? gnode(h, nsize - 1 - i) : gnode(h, i);
    if (isempty(gval(n)))  
      clearkey(n);  
    else if (iscleared(g, gckeyN(n))) {  
      hasclears = 1;  
      if (valiswhite(gval(n)))  
        hasww = 1;  
    }
    else if (valiswhite(gval(n))) {  
      marked = 1;
      reallymarkobject(g, gcvalue(gval(n)));  
    }
  }
  
  if (g->gcstate == GCSpropagate)
    linkgclist(h, g->grayagain);  
  else if (hasww)  
    linkgclist(h, g->ephemeron);  
  else if (hasclears)  
    linkgclist(h, g->allweak);  
  else
    genlink(g, obj2gco(h));  
  return marked;
}


static void traversestrongtable (global_State *g, Table *h) {
  Node *n, *limit = gnodelast(h);
  traversearray(g, h);
  for (n = gnode(h, 0); n < limit; n++) {  
    if (isempty(gval(n)))  
      clearkey(n);  
    else {
      lua_assert(!keyisnil(n));
      markkey(g, n);
      markvalue(g, gval(n));
    }
  }
  genlink(g, obj2gco(h));
}


static void traversetable (global_State *g, Table *h) {
  const char *weakkey, *weakvalue;
  const TValue *mode = gfasttm(g, h->metatable, TM_MODE);
  TString *smode;
  markobjectN(g, h->metatable);
  if (mode && ttisshrstring(mode) &&  
      (cast_void(smode = tsvalue(mode)),
       cast_void(weakkey = strchr(getshrstr(smode), 'k')),
       cast_void(weakvalue = strchr(getshrstr(smode), 'v')),
       (weakkey || weakvalue))) {  
    if (!weakkey)  
      traverseweakvalue(g, h);
    else if (!weakvalue)  
      traverseephemeron(g, h, 0);
    else  
      linkgclist(h, g->allweak);  
  }
  else  
    traversestrongtable(g, h);
}


static void traverseudata (global_State *g, Udata *u) {
  int i;
  markobjectN(g, u->metatable);  
  for (i = 0; i < u->nuvalue; i++)
    markvalue(g, &u->uv[i].uv);
  genlink(g, obj2gco(u));
}



static void traverseproto (global_State *g, Proto *f) {
  int i;
  markobjectN(g, f->source);
  for (i = 0; i < f->sizek; i++)  
    markvalue(g, &f->k[i]);
  for (i = 0; i < f->sizeupvalues; i++)  
    markobjectN(g, f->upvalues[i].name);
  for (i = 0; i < f->sizep; i++)  
    markobjectN(g, f->p[i]);
  for (i = 0; i < f->sizelocvars; i++)  
    markobjectN(g, f->locvars[i].varname);
}


static void traverseCclosure (global_State *g, CClosure *cl) {
  int i;
  for (i = 0; i < cl->nupvalues; i++)  
    markvalue(g, &cl->upvalue[i]);
}


static void traverseLclosure (global_State *g, LClosure *cl) {
  int i;
  markobjectN(g, cl->p);  
  for (i = 0; i < cl->nupvalues; i++) {  
    UpVal *uv = cl->upvals[i];
    markobjectN(g, uv);  
  }
}



static void traversethread (global_State *g, lua_State *th) {
  UpVal *uv;
  StkId o = th->stack.p;
  if (isold(th) || g->gcstate == GCSpropagate)
    linkgclist(th, g->grayagain);  
  if (o == NULL)
    return;  
  lua_assert(g->gcstate == GCSatomic ||
             th->openupval == NULL || isintwups(th));
  for (; o < th->top.p; o++)  
    markvalue(g, s2v(o));
  for (uv = th->openupval; uv != NULL; uv = uv->u.open.next)
    markobject(g, uv);  
  if (g->gcstate == GCSatomic) {  
    if (!g->gcemergency)
      luaD_shrinkstack(th); 
    for (o = th->top.p; o < th->stack_last.p + EXTRA_STACK; o++)
      setnilvalue(s2v(o));  
    
    if (!isintwups(th) && th->openupval != NULL) {
      th->twups = g->twups;  
      g->twups = th;
    }
  }
}



static void propagatemark (global_State *g) {
  GCObject *o = g->gray;
  nw2black(o);
  g->gray = *getgclist(o);  
  switch (o->tt) {
    case LUA_VTABLE: traversetable(g, gco2t(o)); break;
    case LUA_VUSERDATA: traverseudata(g, gco2u(o)); break;
    case LUA_VLCL: traverseLclosure(g, gco2lcl(o)); break;
    case LUA_VCCL: traverseCclosure(g, gco2ccl(o)); break;
    case LUA_VPROTO: traverseproto(g, gco2p(o)); break;
    case LUA_VTHREAD: traversethread(g, gco2th(o)); break;
    default: lua_assert(0);
  }
}


static l_obj propagateall (global_State *g) {
  l_obj work = 0;
  while (g->gray) {
    propagatemark(g);
    work++;
  }
  return work;
}



static l_obj convergeephemerons (global_State *g) {
  int changed;
  l_obj work = 0;
  int dir = 0;
  do {
    GCObject *w;
    GCObject *next = g->ephemeron;  
    g->ephemeron = NULL;  
    changed = 0;
    while ((w = next) != NULL) {  
      Table *h = gco2t(w);
      next = h->gclist;  
      nw2black(h);  
      if (traverseephemeron(g, h, dir)) {  
        propagateall(g);  
        changed = 1;  
      }
      work++;
    }
    dir = !dir;  
  } while (changed);  
  return work;
}








static l_obj clearbykeys (global_State *g, GCObject *l) {
  l_obj work = 0;
  for (; l; l = gco2t(l)->gclist) {
    Table *h = gco2t(l);
    Node *limit = gnodelast(h);
    Node *n;
    for (n = gnode(h, 0); n < limit; n++) {
      if (iscleared(g, gckeyN(n)))  
        setempty(gval(n));  
      if (isempty(gval(n)))  
        clearkey(n);  
    }
    work++;
  }
  return work;
}



static l_obj clearbyvalues (global_State *g, GCObject *l, GCObject *f) {
  l_obj work = 0;
  for (; l != f; l = gco2t(l)->gclist) {
    Table *h = gco2t(l);
    Node *n, *limit = gnodelast(h);
    unsigned int i;
    unsigned int asize = luaH_realasize(h);
    for (i = 0; i < asize; i++) {
      GCObject *o = gcvalarr(h, i);
      if (iscleared(g, o))  
        *getArrTag(h, i) = LUA_VEMPTY;  
    }
    for (n = gnode(h, 0); n < limit; n++) {
      if (iscleared(g, gcvalueN(gval(n))))  
        setempty(gval(n));  
      if (isempty(gval(n)))  
        clearkey(n);  
    }
    work++;
  }
  return work;
}


static void freeupval (lua_State *L, UpVal *uv) {
  if (upisopen(uv))
    luaF_unlinkupval(uv);
  luaM_free(L, uv);
}


static void freeobj (lua_State *L, GCObject *o) {
  G(L)->totalobjs--;
  switch (o->tt) {
    case LUA_VPROTO:
      luaF_freeproto(L, gco2p(o));
      break;
    case LUA_VUPVAL:
      freeupval(L, gco2upv(o));
      break;
    case LUA_VLCL: {
      LClosure *cl = gco2lcl(o);
      luaM_freemem(L, cl, sizeLclosure(cl->nupvalues));
      break;
    }
    case LUA_VCCL: {
      CClosure *cl = gco2ccl(o);
      luaM_freemem(L, cl, sizeCclosure(cl->nupvalues));
      break;
    }
    case LUA_VTABLE:
      luaH_free(L, gco2t(o));
      break;
    case LUA_VTHREAD:
      luaE_freethread(L, gco2th(o));
      break;
    case LUA_VUSERDATA: {
      Udata *u = gco2u(o);
      luaM_freemem(L, o, sizeudata(u->nuvalue, u->len));
      break;
    }
    case LUA_VSHRSTR: {
      TString *ts = gco2ts(o);
      luaS_remove(L, ts);  
      luaM_freemem(L, ts, sizestrshr(ts->shrlen));
      break;
    }
    case LUA_VLNGSTR: {
      TString *ts = gco2ts(o);
      if (ts->shrlen == LSTRMEM)  
        (*ts->falloc)(ts->ud, ts->contents, ts->u.lnglen + 1, 0);
      luaM_freemem(L, ts, luaS_sizelngstr(ts->u.lnglen, ts->shrlen));
      break;
    }
    default: lua_assert(0);
  }
}



static GCObject **sweeplist (lua_State *L, GCObject **p, l_obj countin) {
  global_State *g = G(L);
  int ow = otherwhite(g);
  l_obj i;
  int white = luaC_white(g);  
  for (i = 0; *p != NULL && i < countin; i++) {
    GCObject *curr = *p;
    int marked = curr->marked;
    if (isdeadm(ow, marked)) {  
      *p = curr->next;  
      freeobj(L, curr);  
    }
    else {  
      curr->marked = cast_byte((marked & ~maskgcbits) | white | G_NEW);
      p = &curr->next;  
    }
  }
  return (*p == NULL) ? NULL : p;
}



static GCObject **sweeptolive (lua_State *L, GCObject **p) {
  GCObject **old = p;
  do {
    p = sweeplist(L, p, 1);
  } while (p == old);
  return p;
}







static void checkSizes (lua_State *L, global_State *g) {
  if (!g->gcemergency) {
    if (g->strt.nuse < g->strt.size / 4)  
      luaS_resize(L, g->strt.size / 2);
  }
}



static GCObject *udata2finalize (global_State *g) {
  GCObject *o = g->tobefnz;  
  lua_assert(tofinalize(o));
  g->tobefnz = o->next;  
  o->next = g->allgc;  
  g->allgc = o;
  resetbit(o->marked, FINALIZEDBIT);  
  if (issweepphase(g))
    makewhite(g, o);  
  else if (getage(o) == G_OLD1)
    g->firstold1 = o;  
  return o;
}


static void dothecall (lua_State *L, void *ud) {
  UNUSED(ud);
  luaD_callnoyield(L, L->top.p - 2, 0);
}


static void GCTM (lua_State *L) {
  global_State *g = G(L);
  const TValue *tm;
  TValue v;
  lua_assert(!g->gcemergency);
  setgcovalue(L, &v, udata2finalize(g));
  tm = luaT_gettmbyobj(L, &v, TM_GC);
  if (!notm(tm)) {  
    int status;
    lu_byte oldah = L->allowhook;
    int oldgcstp  = g->gcstp;
    g->gcstp |= GCSTPGC;  
    L->allowhook = 0;  
    setobj2s(L, L->top.p++, tm);  
    setobj2s(L, L->top.p++, &v);  
    L->ci->callstatus |= CIST_FIN;  
    status = luaD_pcall(L, dothecall, NULL, savestack(L, L->top.p - 2), 0);
    L->ci->callstatus &= ~CIST_FIN;  
    L->allowhook = oldah;  
    g->gcstp = oldgcstp;  
    if (l_unlikely(status != LUA_OK)) {  
      luaE_warnerror(L, "__gc");
      L->top.p--;  
    }
  }
}



static void callallpendingfinalizers (lua_State *L) {
  global_State *g = G(L);
  while (g->tobefnz)
    GCTM(L);
}



static GCObject **findlast (GCObject **p) {
  while (*p != NULL)
    p = &(*p)->next;
  return p;
}



static void separatetobefnz (global_State *g, int all) {
  GCObject *curr;
  GCObject **p = &g->finobj;
  GCObject **lastnext = findlast(&g->tobefnz);
  while ((curr = *p) != g->finobjold1) {  
    lua_assert(tofinalize(curr));
    if (!(iswhite(curr) || all))  
      p = &curr->next;  
    else {
      if (curr == g->finobjsur)  
        g->finobjsur = curr->next;  
      *p = curr->next;  
      curr->next = *lastnext;  
      *lastnext = curr;
      lastnext = &curr->next;
    }
  }
}



static void checkpointer (GCObject **p, GCObject *o) {
  if (o == *p)
    *p = o->next;
}



static void correctpointers (global_State *g, GCObject *o) {
  checkpointer(&g->survival, o);
  checkpointer(&g->old1, o);
  checkpointer(&g->reallyold, o);
  checkpointer(&g->firstold1, o);
}



void luaC_checkfinalizer (lua_State *L, GCObject *o, Table *mt) {
  global_State *g = G(L);
  if (tofinalize(o) ||                 
      gfasttm(g, mt, TM_GC) == NULL ||    
      (g->gcstp & GCSTPCLS))                   
    return;  
  else {  
    GCObject **p;
    if (issweepphase(g)) {
      makewhite(g, o);  
      if (g->sweepgc == &o->next)  
        g->sweepgc = sweeptolive(L, g->sweepgc);  
    }
    else
      correctpointers(g, o);
    
    for (p = &g->allgc; *p != o; p = &(*p)->next) {  }
    *p = o->next;  
    o->next = g->finobj;  
    g->finobj = o;
    l_setbit(o->marked, FINALIZEDBIT);  
  }
}








static void setpause (global_State *g) {
  l_obj threshold = applygcparam(g, PAUSE, g->marked);
  l_obj debt = threshold - gettotalobjs(g);
  if (debt < 0) debt = 0;
  luaE_setdebt(g, debt);
}



static void sweep2old (lua_State *L, GCObject **p) {
  GCObject *curr;
  global_State *g = G(L);
  while ((curr = *p) != NULL) {
    if (iswhite(curr)) {  
      lua_assert(isdead(g, curr));
      *p = curr->next;  
      freeobj(L, curr);  
    }
    else {  
      setage(curr, G_OLD);
      if (curr->tt == LUA_VTHREAD) {  
        lua_State *th = gco2th(curr);
        linkgclist(th, g->grayagain);  
      }
      else if (curr->tt == LUA_VUPVAL && upisopen(gco2upv(curr)))
        set2gray(curr);  
      else  
        nw2black(curr);
      p = &curr->next;  
    }
  }
}



static GCObject **sweepgen (lua_State *L, global_State *g, GCObject **p,
                            GCObject *limit, GCObject **pfirstold1,
                            l_obj *paddedold) {
  static const lu_byte nextage[] = {
    G_SURVIVAL,  
    G_OLD1,      
    G_OLD1,      
    G_OLD,       
    G_OLD,       
    G_TOUCHED1,  
    G_TOUCHED2   
  };
  l_obj addedold = 0;
  int white = luaC_white(g);
  GCObject *curr;
  while ((curr = *p) != limit) {
    if (iswhite(curr)) {  
      lua_assert(!isold(curr) && isdead(g, curr));
      *p = curr->next;  
      freeobj(L, curr);  
    }
    else {  
      int age = getage(curr);
      if (age == G_NEW) {  
        int marked = curr->marked & ~maskgcbits;  
        curr->marked = cast_byte(marked | G_SURVIVAL | white);
      }
      else {  
        lua_assert(age != G_OLD1);  
        setage(curr, nextage[age]);
        if (getage(curr) == G_OLD1) {
          addedold++;  
          if (*pfirstold1 == NULL)
            *pfirstold1 = curr;  
        }
      }
      p = &curr->next;  
    }
  }
  *paddedold += addedold;
  return p;
}



static GCObject **correctgraylist (GCObject **p) {
  GCObject *curr;
  while ((curr = *p) != NULL) {
    GCObject **next = getgclist(curr);
    if (iswhite(curr))
      goto remove;  
    else if (getage(curr) == G_TOUCHED1) {  
      lua_assert(isgray(curr));
      nw2black(curr);  
      setage(curr, G_TOUCHED2);
      goto remain;  
    }
    else if (curr->tt == LUA_VTHREAD) {
      lua_assert(isgray(curr));
      goto remain;  
    }
    else {  
      lua_assert(isold(curr));  
      if (getage(curr) == G_TOUCHED2)  
        setage(curr, G_OLD);  
      nw2black(curr);  
      goto remove;
    }
    remove: *p = *next; continue;
    remain: p = next; continue;
  }
  return p;
}



static void correctgraylists (global_State *g) {
  GCObject **list = correctgraylist(&g->grayagain);
  *list = g->weak; g->weak = NULL;
  list = correctgraylist(list);
  *list = g->allweak; g->allweak = NULL;
  list = correctgraylist(list);
  *list = g->ephemeron; g->ephemeron = NULL;
  correctgraylist(list);
}



static void markold (global_State *g, GCObject *from, GCObject *to) {
  GCObject *p;
  for (p = from; p != to; p = p->next) {
    if (getage(p) == G_OLD1) {
      lua_assert(!iswhite(p));
      setage(p, G_OLD);  
      if (isblack(p))
        reallymarkobject(g, p);
    }
  }
}



static void finishgencycle (lua_State *L, global_State *g) {
  correctgraylists(g);
  checkSizes(L, g);
  g->gcstate = GCSpropagate;  
  if (!g->gcemergency)
    callallpendingfinalizers(L);
}



static void minor2inc (lua_State *L, global_State *g, int kind) {
  g->GCmajorminor = g->marked;  
  g->gckind = kind;
  g->reallyold = g->old1 = g->survival = NULL;
  g->finobjrold = g->finobjold1 = g->finobjsur = NULL;
  entersweep(L);  
  
  luaE_setdebt(g, applygcparam(g, STEPSIZE, 100));
}



static int checkminormajor (global_State *g, l_obj addedold1) {
  l_obj step = applygcparam(g, MINORMUL, g->GCmajorminor);
  l_obj limit = applygcparam(g, MINORMAJOR, g->GCmajorminor);
  return (addedold1 >= (step >> 1) || g->marked >= limit);
}


static void youngcollection (lua_State *L, global_State *g) {
  l_obj addedold1 = 0;
  l_obj marked = g->marked;  
  GCObject **psurvival;  
  GCObject *dummy;  
  lua_assert(g->gcstate == GCSpropagate);
  if (g->firstold1) {  
    markold(g, g->firstold1, g->reallyold);  
    g->firstold1 = NULL;  
  }
  markold(g, g->finobj, g->finobjrold);
  markold(g, g->tobefnz, NULL);

  atomic(L);  

  
  g->gcstate = GCSswpallgc;
  psurvival = sweepgen(L, g, &g->allgc, g->survival, &g->firstold1, &addedold1);
  
  sweepgen(L, g, psurvival, g->old1, &g->firstold1, &addedold1);
  g->reallyold = g->old1;
  g->old1 = *psurvival;  
  g->survival = g->allgc;  

  
  dummy = NULL;  
  psurvival = sweepgen(L, g, &g->finobj, g->finobjsur, &dummy, &addedold1);
  
  sweepgen(L, g, psurvival, g->finobjold1, &dummy, &addedold1);
  g->finobjrold = g->finobjold1;
  g->finobjold1 = *psurvival;  
  g->finobjsur = g->finobj;  

  sweepgen(L, g, &g->tobefnz, NULL, &dummy, &addedold1);

  
  g->marked = marked + addedold1;

  
  if (checkminormajor(g, addedold1)) {
    minor2inc(L, g, KGC_GENMAJOR);  
    g->marked = 0;  
  }
  else
    finishgencycle(L, g);  
}



static void atomic2gen (lua_State *L, global_State *g) {
  cleargraylists(g);
  
  g->gcstate = GCSswpallgc;
  sweep2old(L, &g->allgc);
  
  g->reallyold = g->old1 = g->survival = g->allgc;
  g->firstold1 = NULL;  

  
  sweep2old(L, &g->finobj);
  g->finobjrold = g->finobjold1 = g->finobjsur = g->finobj;

  sweep2old(L, &g->tobefnz);

  g->gckind = KGC_GENMINOR;
  g->GCmajorminor = g->marked;  
  g->marked = 0;  
  finishgencycle(L, g);
}



static void setminordebt (global_State *g) {
  luaE_setdebt(g, applygcparam(g, MINORMUL, g->GCmajorminor));
}



static void entergen (lua_State *L, global_State *g) {
  luaC_runtilstate(L, GCSpause, 1);  
  luaC_runtilstate(L, GCSpropagate, 1);  
  atomic(L);  
  atomic2gen(L, g);
  setminordebt(g);  
}



void luaC_changemode (lua_State *L, int newmode) {
  global_State *g = G(L);
  if (g->gckind == KGC_GENMAJOR)  
    g->gckind = KGC_INC;  
  if (newmode != g->gckind) {  
    if (newmode == KGC_INC)  
      minor2inc(L, g, KGC_INC);  
    else {
      lua_assert(newmode == KGC_GENMINOR);
      entergen(L, g);
    }
  }
}



static void fullgen (lua_State *L, global_State *g) {
  minor2inc(L, g, KGC_INC);
  entergen(L, g);
}



static int checkmajorminor (lua_State *L, global_State *g) {
  if (g->gckind == KGC_GENMAJOR) {  
    l_obj numobjs = gettotalobjs(g);
    l_obj addedobjs = numobjs - g->GCmajorminor;
    l_obj limit = applygcparam(g, MAJORMINOR, addedobjs);
    l_obj tobecollected = numobjs - g->marked;
    if (tobecollected > limit) {
      atomic2gen(L, g);  
      setminordebt(g);
      return 0;  
    }
  }
  g->GCmajorminor = g->marked;  
  return 1;  
}








static void entersweep (lua_State *L) {
  global_State *g = G(L);
  g->gcstate = GCSswpallgc;
  lua_assert(g->sweepgc == NULL);
  g->sweepgc = sweeptolive(L, &g->allgc);
}



static void deletelist (lua_State *L, GCObject *p, GCObject *limit) {
  while (p != limit) {
    GCObject *next = p->next;
    freeobj(L, p);
    p = next;
  }
}



void luaC_freeallobjects (lua_State *L) {
  global_State *g = G(L);
  g->gcstp = GCSTPCLS;  
  luaC_changemode(L, KGC_INC);
  separatetobefnz(g, 1);  
  lua_assert(g->finobj == NULL);
  callallpendingfinalizers(L);
  deletelist(L, g->allgc, obj2gco(g->mainthread));
  lua_assert(g->finobj == NULL);  
  deletelist(L, g->fixedgc, NULL);  
  lua_assert(g->strt.nuse == 0);
}


static l_obj atomic (lua_State *L) {
  l_obj work = 0;
  global_State *g = G(L);
  GCObject *origweak, *origall;
  GCObject *grayagain = g->grayagain;  
  g->grayagain = NULL;
  lua_assert(g->ephemeron == NULL && g->weak == NULL);
  lua_assert(!iswhite(g->mainthread));
  g->gcstate = GCSatomic;
  markobject(g, L);  
  
  markvalue(g, &g->l_registry);
  markmt(g);  
  work += propagateall(g);  
  
  work += remarkupvals(g);
  work += propagateall(g);  
  g->gray = grayagain;
  work += propagateall(g);  
  work += convergeephemerons(g);
  
  
  work += clearbyvalues(g, g->weak, NULL);
  work += clearbyvalues(g, g->allweak, NULL);
  origweak = g->weak; origall = g->allweak;
  separatetobefnz(g, 0);  
  work += markbeingfnz(g);  
  work += propagateall(g);  
  work += convergeephemerons(g);
  
  
  work += clearbykeys(g, g->ephemeron);  
  work += clearbykeys(g, g->allweak);  
  
  work += clearbyvalues(g, g->weak, origweak);
  work += clearbyvalues(g, g->allweak, origall);
  luaS_clearcache(g);
  g->currentwhite = cast_byte(otherwhite(g));  
  lua_assert(g->gray == NULL);
  return work;
}



static void sweepstep (lua_State *L, global_State *g,
                       int nextstate, GCObject **nextlist, int fast) {
  if (g->sweepgc)
    g->sweepgc = sweeplist(L, g->sweepgc, fast ? MAX_LOBJ : GCSWEEPMAX);
  else {  
    g->gcstate = nextstate;
    g->sweepgc = nextlist;
  }
}



static l_obj singlestep (lua_State *L, int fast) {
  global_State *g = G(L);
  l_obj work;
  lua_assert(!g->gcstopem);  
  g->gcstopem = 1;  
  switch (g->gcstate) {
    case GCSpause: {
      restartcollection(g);
      g->gcstate = GCSpropagate;
      work = 1;
      break;
    }
    case GCSpropagate: {
      if (fast || g->gray == NULL) {
        g->gcstate = GCSenteratomic;  
        work = 0;
      }
      else {
        propagatemark(g);  
        work = 1;
      }
      break;
    }
    case GCSenteratomic: {
      work = atomic(L);
      if (checkmajorminor(L, g))
        entersweep(L);
      break;
    }
    case GCSswpallgc: {  
      sweepstep(L, g, GCSswpfinobj, &g->finobj, fast);
      work = GCSWEEPMAX;
      break;
    }
    case GCSswpfinobj: {  
      sweepstep(L, g, GCSswptobefnz, &g->tobefnz, fast);
      work = GCSWEEPMAX;
      break;
    }
    case GCSswptobefnz: {  
      sweepstep(L, g, GCSswpend, NULL, fast);
      work = GCSWEEPMAX;
      break;
    }
    case GCSswpend: {  
      checkSizes(L, g);
      g->gcstate = GCScallfin;
      work = 0;
      break;
    }
    case GCScallfin: {  
      if (g->tobefnz && !g->gcemergency) {
        g->gcstopem = 0;  
        GCTM(L);  
        work = 1;
      }
      else {  
        g->gcstate = GCSpause;  
        work = 0;
      }
      break;
    }
    default: lua_assert(0); return 0;
  }
  g->gcstopem = 0;
  return work;
}



void luaC_runtilstate (lua_State *L, int state, int fast) {
  global_State *g = G(L);
  lua_assert(g->gckind == KGC_INC);
  while (state != g->gcstate)
    singlestep(L, fast);
}




static void incstep (lua_State *L, global_State *g) {
  l_obj stepsize = applygcparam(g, STEPSIZE, 100);
  l_obj work2do = applygcparam(g, STEPMUL, stepsize);
  int fast = 0;
  if (work2do == 0) {  
    work2do = MAX_LOBJ;  
    fast = 1;
  }
  do {  
    l_obj work = singlestep(L, fast);  
    if (g->gckind == KGC_GENMINOR)  
      return;  
    work2do -= work;
  } while (work2do > 0 && g->gcstate != GCSpause);
  if (g->gcstate == GCSpause)
    setpause(g);  
  else
    luaE_setdebt(g, stepsize);
}


void luaC_step (lua_State *L) {
  global_State *g = G(L);
  lua_assert(!g->gcemergency);
  if (!gcrunning(g))  
    luaE_setdebt(g, 2000);
  else {
    switch (g->gckind) {
      case KGC_INC: case KGC_GENMAJOR:
        incstep(L, g);
        break;
      case KGC_GENMINOR:
        youngcollection(L, g);
        setminordebt(g);
        break;
    }
  }
}



static void fullinc (lua_State *L, global_State *g) {
  if (keepinvariant(g))  
    entersweep(L); 
  
  luaC_runtilstate(L, GCSpause, 1);
  luaC_runtilstate(L, GCScallfin, 1);  
  
  lua_assert(g->marked == gettotalobjs(g));
  luaC_runtilstate(L, GCSpause, 1);  
  setpause(g);
}



void luaC_fullgc (lua_State *L, int isemergency) {
  global_State *g = G(L);
  lua_assert(!g->gcemergency);
  g->gcemergency = isemergency;  
  switch (g->gckind) {
    case KGC_GENMINOR: fullgen(L, g); break;
    case KGC_INC: fullinc(L, g); break;
    case KGC_GENMAJOR:
      g->gckind = KGC_INC;
      fullinc(L, g);
      g->gckind = KGC_GENMAJOR;
      break;
  }
  g->gcemergency = 0;
}






//path: src/imports/../dependencies/lua/llex.c


#define llex_c
#define LUA_CORE





#include <locale.h>
#include <string.h>
















//path: src/imports/../dependencies/lua/lparser.h


#ifndef lparser_h
#define lparser_h












typedef enum {
  VVOID,  
  VNIL,  
  VTRUE,  
  VFALSE,  
  VK,  
  VKFLT,  
  VKINT,  
  VKSTR,  
  VNONRELOC,  
  VLOCAL,  
  VUPVAL,  
  VCONST,  
  VINDEXED,  
  VINDEXUP,  
  VINDEXI, 
  VINDEXSTR, 
  VJMP,  
  VRELOC,  
  VCALL,  
  VVARARG  
} expkind;


#define vkisvar(k)	(VLOCAL <= (k) && (k) <= VINDEXSTR)
#define vkisindexed(k)	(VINDEXED <= (k) && (k) <= VINDEXSTR)


typedef struct expdesc {
  expkind k;
  union {
    lua_Integer ival;    
    lua_Number nval;  
    TString *strval;  
    int info;  
    struct {  
      short idx;  
      lu_byte t;  
    } ind;
    struct {  
      lu_byte ridx;  
      unsigned short vidx;  
    } var;
  } u;
  int t;  
  int f;  
} expdesc;



#define VDKREG		0   
#define RDKCONST	1   
#define RDKTOCLOSE	2   
#define RDKCTC		3   


typedef union Vardesc {
  struct {
    TValuefields;  
    lu_byte kind;
    lu_byte ridx;  
    short pidx;  
    TString *name;  
  } vd;
  TValue k;  
} Vardesc;




typedef struct Labeldesc {
  TString *name;  
  int pc;  
  int line;  
  lu_byte nactvar;  
  lu_byte close;  
} Labeldesc;



typedef struct Labellist {
  Labeldesc *arr;  
  int n;  
  int size;  
} Labellist;



typedef struct Dyndata {
  struct {  
    Vardesc *arr;
    int n;
    int size;
  } actvar;
  Labellist gt;  
  Labellist label;   
} Dyndata;



struct BlockCnt;  



typedef struct FuncState {
  Proto *f;  
  struct FuncState *prev;  
  struct LexState *ls;  
  struct BlockCnt *bl;  
  int pc;  
  int lasttarget;   
  int previousline;  
  int nk;  
  int np;  
  int nabslineinfo;  
  int firstlocal;  
  int firstlabel;  
  short ndebugvars;  
  lu_byte nactvar;  
  lu_byte nups;  
  lu_byte freereg;  
  lu_byte iwthabs;  
  lu_byte needclose;  
} FuncState;


LUAI_FUNC int luaY_nvarstack (FuncState *fs);
LUAI_FUNC LClosure *luaY_parser (lua_State *L, ZIO *z, Mbuffer *buff,
                                 Dyndata *dyd, const char *name, int firstchar);


#endif













#define next(ls)	(ls->current = zgetc(ls->z))



#define currIsNewline(ls)	(ls->current == '\n' || ls->current == '\r')



static const char *const luaX_tokens [] = {
    "and", "break", "do", "else", "elseif",
    "end", "false", "for", "function", "goto", "if",
    "in", "local", "nil", "not", "or", "repeat",
    "return", "then", "true", "until", "while",
    "//", "..", "...", "==", ">=", "<=", "~=",
    "<<", ">>", "::", "<eof>",
    "<number>", "<integer>", "<name>", "<string>"
};


#define save_and_next(ls) (save(ls, ls->current), next(ls))


static l_noret lexerror (LexState *ls, const char *msg, int token);


static void save (LexState *ls, int c) {
  Mbuffer *b = ls->buff;
  if (luaZ_bufflen(b) + 1 > luaZ_sizebuffer(b)) {
    size_t newsize;
    if (luaZ_sizebuffer(b) >= MAX_SIZE/2)
      lexerror(ls, "lexical element too long", 0);
    newsize = luaZ_sizebuffer(b) * 2;
    luaZ_resizebuffer(ls->L, b, newsize);
  }
  b->buffer[luaZ_bufflen(b)++] = cast_char(c);
}


void luaX_init (lua_State *L) {
  int i;
  TString *e = luaS_newliteral(L, LUA_ENV);  
  luaC_fix(L, obj2gco(e));  
  for (i=0; i<NUM_RESERVED; i++) {
    TString *ts = luaS_new(L, luaX_tokens[i]);
    luaC_fix(L, obj2gco(ts));  
    ts->extra = cast_byte(i+1);  
  }
}


const char *luaX_token2str (LexState *ls, int token) {
  if (token < FIRST_RESERVED) {  
    if (lisprint(token))
      return luaO_pushfstring(ls->L, "'%c'", token);
    else  
      return luaO_pushfstring(ls->L, "'<\\%d>'", token);
  }
  else {
    const char *s = luaX_tokens[token - FIRST_RESERVED];
    if (token < TK_EOS)  
      return luaO_pushfstring(ls->L, "'%s'", s);
    else  
      return s;
  }
}


static const char *txtToken (LexState *ls, int token) {
  switch (token) {
    case TK_NAME: case TK_STRING:
    case TK_FLT: case TK_INT:
      save(ls, '\0');
      return luaO_pushfstring(ls->L, "'%s'", luaZ_buffer(ls->buff));
    default:
      return luaX_token2str(ls, token);
  }
}


static l_noret lexerror (LexState *ls, const char *msg, int token) {
  msg = luaG_addinfo(ls->L, msg, ls->source, ls->linenumber);
  if (token)
    luaO_pushfstring(ls->L, "%s near %s", msg, txtToken(ls, token));
  luaD_throw(ls->L, LUA_ERRSYNTAX);
}


l_noret luaX_syntaxerror (LexState *ls, const char *msg) {
  lexerror(ls, msg, ls->t.token);
}



TString *luaX_newstring (LexState *ls, const char *str, size_t l) {
  lua_State *L = ls->L;
  TString *ts = luaS_newlstr(L, str, l);  
  TString *oldts = luaH_getstrkey(ls->h, ts);
  if (oldts != NULL)  
    return oldts;  
  else {  
    TValue *stv = s2v(L->top.p++);  
    setsvalue(L, stv, ts);  
    luaH_set(L, ls->h, stv, stv);  
    
    luaC_checkGC(L);
    L->top.p--;  
  }
  return ts;
}



static void inclinenumber (LexState *ls) {
  int old = ls->current;
  lua_assert(currIsNewline(ls));
  next(ls);  
  if (currIsNewline(ls) && ls->current != old)
    next(ls);  
  if (++ls->linenumber >= MAX_INT)
    lexerror(ls, "chunk has too many lines", 0);
}


void luaX_setinput (lua_State *L, LexState *ls, ZIO *z, TString *source,
                    int firstchar) {
  ls->t.token = 0;
  ls->L = L;
  ls->current = firstchar;
  ls->lookahead.token = TK_EOS;  
  ls->z = z;
  ls->fs = NULL;
  ls->linenumber = 1;
  ls->lastline = 1;
  ls->source = source;
  ls->envn = luaS_newliteral(L, LUA_ENV);  
  luaZ_resizebuffer(ls->L, ls->buff, LUA_MINBUFFER);  
}






static int check_next1 (LexState *ls, int c) {
  if (ls->current == c) {
    next(ls);
    return 1;
  }
  else return 0;
}



static int check_next2 (LexState *ls, const char *set) {
  lua_assert(set[2] == '\0');
  if (ls->current == set[0] || ls->current == set[1]) {
    save_and_next(ls);
    return 1;
  }
  else return 0;
}




static int read_numeral (LexState *ls, SemInfo *seminfo) {
  TValue obj;
  const char *expo = "Ee";
  int first = ls->current;
  lua_assert(lisdigit(ls->current));
  save_and_next(ls);
  if (first == '0' && check_next2(ls, "xX"))  
    expo = "Pp";
  for (;;) {
    if (check_next2(ls, expo))  
      check_next2(ls, "-+");  
    else if (lisxdigit(ls->current) || ls->current == '.')  
      save_and_next(ls);
    else break;
  }
  if (lislalpha(ls->current))  
    save_and_next(ls);  
  save(ls, '\0');
  if (luaO_str2num(luaZ_buffer(ls->buff), &obj) == 0)  
    lexerror(ls, "malformed number", TK_FLT);
  if (ttisinteger(&obj)) {
    seminfo->i = ivalue(&obj);
    return TK_INT;
  }
  else {
    lua_assert(ttisfloat(&obj));
    seminfo->r = fltvalue(&obj);
    return TK_FLT;
  }
}



static size_t skip_sep (LexState *ls) {
  size_t count = 0;
  int s = ls->current;
  lua_assert(s == '[' || s == ']');
  save_and_next(ls);
  while (ls->current == '=') {
    save_and_next(ls);
    count++;
  }
  return (ls->current == s) ? count + 2
         : (count == 0) ? 1
         : 0;
}


static void read_long_string (LexState *ls, SemInfo *seminfo, size_t sep) {
  int line = ls->linenumber;  
  save_and_next(ls);  
  if (currIsNewline(ls))  
    inclinenumber(ls);  
  for (;;) {
    switch (ls->current) {
      case EOZ: {  
        const char *what = (seminfo ? "string" : "comment");
        const char *msg = luaO_pushfstring(ls->L,
                     "unfinished long %s (starting at line %d)", what, line);
        lexerror(ls, msg, TK_EOS);
        break;  
      }
      case ']': {
        if (skip_sep(ls) == sep) {
          save_and_next(ls);  
          goto endloop;
        }
        break;
      }
      case '\n': case '\r': {
        save(ls, '\n');
        inclinenumber(ls);
        if (!seminfo) luaZ_resetbuffer(ls->buff);  
        break;
      }
      default: {
        if (seminfo) save_and_next(ls);
        else next(ls);
      }
    }
  } endloop:
  if (seminfo)
    seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + sep,
                                     luaZ_bufflen(ls->buff) - 2 * sep);
}


static void esccheck (LexState *ls, int c, const char *msg) {
  if (!c) {
    if (ls->current != EOZ)
      save_and_next(ls);  
    lexerror(ls, msg, TK_STRING);
  }
}


static int gethexa (LexState *ls) {
  save_and_next(ls);
  esccheck (ls, lisxdigit(ls->current), "hexadecimal digit expected");
  return luaO_hexavalue(ls->current);
}


static int readhexaesc (LexState *ls) {
  int r = gethexa(ls);
  r = (r << 4) + gethexa(ls);
  luaZ_buffremove(ls->buff, 2);  
  return r;
}


static unsigned long readutf8esc (LexState *ls) {
  unsigned long r;
  int i = 4;  
  save_and_next(ls);  
  esccheck(ls, ls->current == '{', "missing '{'");
  r = gethexa(ls);  
  while (cast_void(save_and_next(ls)), lisxdigit(ls->current)) {
    i++;
    esccheck(ls, r <= (0x7FFFFFFFu >> 4), "UTF-8 value too large");
    r = (r << 4) + luaO_hexavalue(ls->current);
  }
  esccheck(ls, ls->current == '}', "missing '}'");
  next(ls);  
  luaZ_buffremove(ls->buff, i);  
  return r;
}


static void utf8esc (LexState *ls) {
  char buff[UTF8BUFFSZ];
  int n = luaO_utf8esc(buff, readutf8esc(ls));
  for (; n > 0; n--)  
    save(ls, buff[UTF8BUFFSZ - n]);
}


static int readdecesc (LexState *ls) {
  int i;
  int r = 0;  
  for (i = 0; i < 3 && lisdigit(ls->current); i++) {  
    r = 10*r + ls->current - '0';
    save_and_next(ls);
  }
  esccheck(ls, r <= UCHAR_MAX, "decimal escape too large");
  luaZ_buffremove(ls->buff, i);  
  return r;
}


static void read_string (LexState *ls, int del, SemInfo *seminfo) {
  save_and_next(ls);  
  while (ls->current != del) {
    switch (ls->current) {
      case EOZ:
        lexerror(ls, "unfinished string", TK_EOS);
        break;  
      case '\n':
      case '\r':
        lexerror(ls, "unfinished string", TK_STRING);
        break;  
      case '\\': {  
        int c;  
        save_and_next(ls);  
        switch (ls->current) {
          case 'a': c = '\a'; goto read_save;
          case 'b': c = '\b'; goto read_save;
          case 'f': c = '\f'; goto read_save;
          case 'n': c = '\n'; goto read_save;
          case 'r': c = '\r'; goto read_save;
          case 't': c = '\t'; goto read_save;
          case 'v': c = '\v'; goto read_save;
          case 'x': c = readhexaesc(ls); goto read_save;
          case 'u': utf8esc(ls);  goto no_save;
          case '\n': case '\r':
            inclinenumber(ls); c = '\n'; goto only_save;
          case '\\': case '\"': case '\'':
            c = ls->current; goto read_save;
          case EOZ: goto no_save;  /* will raise an error next loop */
          case 'z': {  /* zap following span of spaces */
            luaZ_buffremove(ls->buff, 1);  /* remove '\\' */
            next(ls);  /* skip the 'z' */
            while (lisspace(ls->current)) {
              if (currIsNewline(ls)) inclinenumber(ls);
              else next(ls);
            }
            goto no_save;
          }
          default: {
            esccheck(ls, lisdigit(ls->current), "invalid escape sequence");
            c = readdecesc(ls);  /* digital escape '\ddd' */
            goto only_save;
          }
        }
       read_save:
         next(ls);
         /* go through */
       only_save:
         luaZ_buffremove(ls->buff, 1);  /* remove '\\' */
         save(ls, c);
         /* go through */
       no_save: break;
      }
      default:
        save_and_next(ls);
    }
  }
  save_and_next(ls);  /* skip delimiter */
  seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + 1,
                                   luaZ_bufflen(ls->buff) - 2);
}


static int llex (LexState *ls, SemInfo *seminfo) {
  luaZ_resetbuffer(ls->buff);
  for (;;) {
    switch (ls->current) {
      case '\n': case '\r': {  /* line breaks */
        inclinenumber(ls);
        break;
      }
      case ' ': case '\f': case '\t': case '\v': {  /* spaces */
        next(ls);
        break;
      }
      case '-': {  /* '-' or '--' (comment) */
        next(ls);
        if (ls->current != '-') return '-';
        /* else is a comment */
        next(ls);
        if (ls->current == '[') {  /* long comment? */
          size_t sep = skip_sep(ls);
          luaZ_resetbuffer(ls->buff);  /* 'skip_sep' may dirty the buffer */
          if (sep >= 2) {
            read_long_string(ls, NULL, sep);  /* skip long comment */
            luaZ_resetbuffer(ls->buff);  /* previous call may dirty the buff. */
            break;
          }
        }
        /* else short comment */
        while (!currIsNewline(ls) && ls->current != EOZ)
          next(ls);  /* skip until end of line (or end of file) */
        break;
      }
      case '[': {  /* long string or simply '[' */
        size_t sep = skip_sep(ls);
        if (sep >= 2) {
          read_long_string(ls, seminfo, sep);
          return TK_STRING;
        }
        else if (sep == 0)  /* '[=...' missing second bracket? */
          lexerror(ls, "invalid long string delimiter", TK_STRING);
        return '[';
      }
      case '=': {
        next(ls);
        if (check_next1(ls, '=')) return TK_EQ;  /* '==' */
        else return '=';
      }
      case '<': {
        next(ls);
        if (check_next1(ls, '=')) return TK_LE;  /* '<=' */
        else if (check_next1(ls, '<')) return TK_SHL;  /* '<<' */
        else return '<';
      }
      case '>': {
        next(ls);
        if (check_next1(ls, '=')) return TK_GE;  /* '>=' */
        else if (check_next1(ls, '>')) return TK_SHR;  /* '>>' */
        else return '>';
      }
      case '/': {
        next(ls);
        if (check_next1(ls, '/')) return TK_IDIV;  /* '//' */
        else return '/';
      }
      case '~': {
        next(ls);
        if (check_next1(ls, '=')) return TK_NE;  /* '~=' */
        else return '~';
      }
      case ':': {
        next(ls);
        if (check_next1(ls, ':')) return TK_DBCOLON;  /* '::' */
        else return ':';
      }
      case '"': case '\'': {  
        read_string(ls, ls->current, seminfo);
        return TK_STRING;
      }
      case '.': {  
        save_and_next(ls);
        if (check_next1(ls, '.')) {
          if (check_next1(ls, '.'))
            return TK_DOTS;   
          else return TK_CONCAT;   
        }
        else if (!lisdigit(ls->current)) return '.';
        else return read_numeral(ls, seminfo);
      }
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9': {
        return read_numeral(ls, seminfo);
      }
      case EOZ: {
        return TK_EOS;
      }
      default: {
        if (lislalpha(ls->current)) {  
          TString *ts;
          do {
            save_and_next(ls);
          } while (lislalnum(ls->current));
          ts = luaX_newstring(ls, luaZ_buffer(ls->buff),
                                  luaZ_bufflen(ls->buff));
          seminfo->ts = ts;
          if (isreserved(ts))  
            return ts->extra - 1 + FIRST_RESERVED;
          else {
            return TK_NAME;
          }
        }
        else {  
          int c = ls->current;
          next(ls);
          return c;
        }
      }
    }
  }
}


void luaX_next (LexState *ls) {
  ls->lastline = ls->linenumber;
  if (ls->lookahead.token != TK_EOS) {  
    ls->t = ls->lookahead;  
    ls->lookahead.token = TK_EOS;  
  }
  else
    ls->t.token = llex(ls, &ls->t.seminfo);  
}


int luaX_lookahead (LexState *ls) {
  lua_assert(ls->lookahead.token == TK_EOS);
  ls->lookahead.token = llex(ls, &ls->lookahead.seminfo);
  return ls->lookahead.token;
}



//path: src/imports/../dependencies/lua/lcode.c


#define lcode_c
#define LUA_CORE





#include <float.h>
#include <limits.h>
#include <stdlib.h>




//path: src/imports/../dependencies/lua/lcode.h


#ifndef lcode_h
#define lcode_h












#define NO_JUMP (-1)



typedef enum BinOpr {
  
  OPR_ADD, OPR_SUB, OPR_MUL, OPR_MOD, OPR_POW,
  OPR_DIV, OPR_IDIV,
  
  OPR_BAND, OPR_BOR, OPR_BXOR,
  OPR_SHL, OPR_SHR,
  
  OPR_CONCAT,
  
  OPR_EQ, OPR_LT, OPR_LE,
  OPR_NE, OPR_GT, OPR_GE,
  
  OPR_AND, OPR_OR,
  OPR_NOBINOPR
} BinOpr;



#define foldbinop(op)	((op) <= OPR_SHR)


#define luaK_codeABC(fs,o,a,b,c)	luaK_codeABCk(fs,o,a,b,c,0)


typedef enum UnOpr { OPR_MINUS, OPR_BNOT, OPR_NOT, OPR_LEN, OPR_NOUNOPR } UnOpr;



#define getinstruction(fs,e)	((fs)->f->code[(e)->u.info])


#define luaK_setmultret(fs,e)	luaK_setreturns(fs, e, LUA_MULTRET)

#define luaK_jumpto(fs,t)	luaK_patchlist(fs, luaK_jump(fs), t)

LUAI_FUNC int luaK_code (FuncState *fs, Instruction i);
LUAI_FUNC int luaK_codeABx (FuncState *fs, OpCode o, int A, unsigned Bx);
LUAI_FUNC int luaK_codeABCk (FuncState *fs, OpCode o, int A,
                                            int B, int C, int k);
LUAI_FUNC int luaK_exp2const (FuncState *fs, const expdesc *e, TValue *v);
LUAI_FUNC void luaK_fixline (FuncState *fs, int line);
LUAI_FUNC void luaK_nil (FuncState *fs, int from, int n);
LUAI_FUNC void luaK_reserveregs (FuncState *fs, int n);
LUAI_FUNC void luaK_checkstack (FuncState *fs, int n);
LUAI_FUNC void luaK_int (FuncState *fs, int reg, lua_Integer n);
LUAI_FUNC void luaK_dischargevars (FuncState *fs, expdesc *e);
LUAI_FUNC int luaK_exp2anyreg (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_exp2anyregup (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_exp2nextreg (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_exp2val (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_self (FuncState *fs, expdesc *e, expdesc *key);
LUAI_FUNC void luaK_indexed (FuncState *fs, expdesc *t, expdesc *k);
LUAI_FUNC void luaK_goiftrue (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_goiffalse (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_storevar (FuncState *fs, expdesc *var, expdesc *e);
LUAI_FUNC void luaK_setreturns (FuncState *fs, expdesc *e, int nresults);
LUAI_FUNC void luaK_setoneret (FuncState *fs, expdesc *e);
LUAI_FUNC int luaK_jump (FuncState *fs);
LUAI_FUNC void luaK_ret (FuncState *fs, int first, int nret);
LUAI_FUNC void luaK_patchlist (FuncState *fs, int list, int target);
LUAI_FUNC void luaK_patchtohere (FuncState *fs, int list);
LUAI_FUNC void luaK_concat (FuncState *fs, int *l1, int l2);
LUAI_FUNC int luaK_getlabel (FuncState *fs);
LUAI_FUNC void luaK_prefix (FuncState *fs, UnOpr op, expdesc *v, int line);
LUAI_FUNC void luaK_infix (FuncState *fs, BinOpr op, expdesc *v);
LUAI_FUNC void luaK_posfix (FuncState *fs, BinOpr op, expdesc *v1,
                            expdesc *v2, int line);
LUAI_FUNC void luaK_settablesize (FuncState *fs, int pc,
                                  int ra, int asize, int hsize);
LUAI_FUNC void luaK_setlist (FuncState *fs, int base, int nelems, int tostore);
LUAI_FUNC void luaK_finish (FuncState *fs);
LUAI_FUNC l_noret luaK_semerror (LexState *ls, const char *msg);


#endif






















//path: src/imports/../dependencies/lua/lvm.h


#ifndef lvm_h
#define lvm_h










#if !defined(LUA_NOCVTN2S)
#define cvt2str(o)	ttisnumber(o)
#else
#define cvt2str(o)	0	
#endif


#if !defined(LUA_NOCVTS2N)
#define cvt2num(o)	ttisstring(o)
#else
#define cvt2num(o)	0	
#endif



#if !defined(LUA_FLOORN2I)
#define LUA_FLOORN2I		F2Ieq
#endif



typedef enum {
  F2Ieq,     
  F2Ifloor,  
  F2Iceil    
} F2Imod;



#define tonumber(o,n) \
	(ttisfloat(o) ? (*(n) = fltvalue(o), 1) : luaV_tonumber_(o,n))



#define tonumberns(o,n) \
	(ttisfloat(o) ? ((n) = fltvalue(o), 1) : \
	(ttisinteger(o) ? ((n) = cast_num(ivalue(o)), 1) : 0))



#define tointeger(o,i) \
  (l_likely(ttisinteger(o)) ? (*(i) = ivalue(o), 1) \
                          : luaV_tointeger(o,i,LUA_FLOORN2I))



#define tointegerns(o,i) \
  (l_likely(ttisinteger(o)) ? (*(i) = ivalue(o), 1) \
                          : luaV_tointegerns(o,i,LUA_FLOORN2I))


#define intop(op,v1,v2) l_castU2S(l_castS2U(v1) op l_castS2U(v2))

#define luaV_rawequalobj(t1,t2)		luaV_equalobj(NULL,t1,t2)



#define luaV_fastget(t,k,res,f, tag) \
  (tag = (!ttistable(t) ? LUA_VNOTABLE : f(hvalue(t), k, res)))



#define luaV_fastgeti(t,k,res,tag) \
  if (!ttistable(t)) tag = LUA_VNOTABLE; \
  else { luaH_fastgeti(hvalue(t), k, res, tag); }


#define luaV_fastset(t,k,val,hres,f) \
  (hres = (!ttistable(t) ? HNOTATABLE : f(hvalue(t), k, val)))

#define luaV_fastseti(t,k,val,hres) \
  if (!ttistable(t)) hres = HNOTATABLE; \
  else { luaH_fastseti(hvalue(t), k, val, hres); }



#define luaV_finishfastset(L,t,v)	luaC_barrierback(L, gcvalue(t), v)



#define luaV_shiftr(x,y)	luaV_shiftl(x,intop(-, 0, y))



LUAI_FUNC int luaV_equalobj (lua_State *L, const TValue *t1, const TValue *t2);
LUAI_FUNC int luaV_lessthan (lua_State *L, const TValue *l, const TValue *r);
LUAI_FUNC int luaV_lessequal (lua_State *L, const TValue *l, const TValue *r);
LUAI_FUNC int luaV_tonumber_ (const TValue *obj, lua_Number *n);
LUAI_FUNC int luaV_tointeger (const TValue *obj, lua_Integer *p, F2Imod mode);
LUAI_FUNC int luaV_tointegerns (const TValue *obj, lua_Integer *p,
                                F2Imod mode);
LUAI_FUNC int luaV_flttointeger (lua_Number n, lua_Integer *p, F2Imod mode);
LUAI_FUNC int luaV_finishget (lua_State *L, const TValue *t, TValue *key,
                                            StkId val, int tag);
LUAI_FUNC void luaV_finishset (lua_State *L, const TValue *t, TValue *key,
                                             TValue *val, int aux);
LUAI_FUNC void luaV_finishOp (lua_State *L);
LUAI_FUNC void luaV_execute (lua_State *L, CallInfo *ci);
LUAI_FUNC void luaV_concat (lua_State *L, int total);
LUAI_FUNC lua_Integer luaV_idiv (lua_State *L, lua_Integer x, lua_Integer y);
LUAI_FUNC lua_Integer luaV_mod (lua_State *L, lua_Integer x, lua_Integer y);
LUAI_FUNC lua_Number luaV_modf (lua_State *L, lua_Number x, lua_Number y);
LUAI_FUNC lua_Integer luaV_shiftl (lua_Integer x, lua_Integer y);
LUAI_FUNC void luaV_objlen (lua_State *L, StkId ra, const TValue *rb);

#endif





#define MAXREGS		255


#define hasjumps(e)	((e)->t != (e)->f)


static int codesJ (FuncState *fs, OpCode o, int sj, int k);




l_noret luaK_semerror (LexState *ls, const char *msg) {
  ls->t.token = 0;  
  luaX_syntaxerror(ls, msg);
}



static int tonumeral (const expdesc *e, TValue *v) {
  if (hasjumps(e))
    return 0;  
  switch (e->k) {
    case VKINT:
      if (v) setivalue(v, e->u.ival);
      return 1;
    case VKFLT:
      if (v) setfltvalue(v, e->u.nval);
      return 1;
    default: return 0;
  }
}



static TValue *const2val (FuncState *fs, const expdesc *e) {
  lua_assert(e->k == VCONST);
  return &fs->ls->dyd->actvar.arr[e->u.info].k;
}



int luaK_exp2const (FuncState *fs, const expdesc *e, TValue *v) {
  if (hasjumps(e))
    return 0;  
  switch (e->k) {
    case VFALSE:
      setbfvalue(v);
      return 1;
    case VTRUE:
      setbtvalue(v);
      return 1;
    case VNIL:
      setnilvalue(v);
      return 1;
    case VKSTR: {
      setsvalue(fs->ls->L, v, e->u.strval);
      return 1;
    }
    case VCONST: {
      setobj(fs->ls->L, v, const2val(fs, e));
      return 1;
    }
    default: return tonumeral(e, v);
  }
}



static Instruction *previousinstruction (FuncState *fs) {
  static const Instruction invalidinstruction = ~(Instruction)0;
  if (fs->pc > fs->lasttarget)
    return &fs->f->code[fs->pc - 1];  
  else
    return cast(Instruction*, &invalidinstruction);
}



void luaK_nil (FuncState *fs, int from, int n) {
  int l = from + n - 1;  
  Instruction *previous = previousinstruction(fs);
  if (GET_OPCODE(*previous) == OP_LOADNIL) {  
    int pfrom = GETARG_A(*previous);  
    int pl = pfrom + GETARG_B(*previous);
    if ((pfrom <= from && from <= pl + 1) ||
        (from <= pfrom && pfrom <= l + 1)) {  
      if (pfrom < from) from = pfrom;  
      if (pl > l) l = pl;  
      SETARG_A(*previous, from);
      SETARG_B(*previous, l - from);
      return;
    }  
  }
  luaK_codeABC(fs, OP_LOADNIL, from, n - 1, 0);  
}



static int getjump (FuncState *fs, int pc) {
  int offset = GETARG_sJ(fs->f->code[pc]);
  if (offset == NO_JUMP)  
    return NO_JUMP;  
  else
    return (pc+1)+offset;  
}



static void fixjump (FuncState *fs, int pc, int dest) {
  Instruction *jmp = &fs->f->code[pc];
  int offset = dest - (pc + 1);
  lua_assert(dest != NO_JUMP);
  if (!(-OFFSET_sJ <= offset && offset <= MAXARG_sJ - OFFSET_sJ))
    luaX_syntaxerror(fs->ls, "control structure too long");
  lua_assert(GET_OPCODE(*jmp) == OP_JMP);
  SETARG_sJ(*jmp, offset);
}



void luaK_concat (FuncState *fs, int *l1, int l2) {
  if (l2 == NO_JUMP) return;  
  else if (*l1 == NO_JUMP)  
    *l1 = l2;  
  else {
    int list = *l1;
    int next;
    while ((next = getjump(fs, list)) != NO_JUMP)  
      list = next;
    fixjump(fs, list, l2);  
  }
}



int luaK_jump (FuncState *fs) {
  return codesJ(fs, OP_JMP, NO_JUMP, 0);
}



void luaK_ret (FuncState *fs, int first, int nret) {
  OpCode op;
  switch (nret) {
    case 0: op = OP_RETURN0; break;
    case 1: op = OP_RETURN1; break;
    default: op = OP_RETURN; break;
  }
  luaK_codeABC(fs, op, first, nret + 1, 0);
}



static int condjump (FuncState *fs, OpCode op, int A, int B, int C, int k) {
  luaK_codeABCk(fs, op, A, B, C, k);
  return luaK_jump(fs);
}



int luaK_getlabel (FuncState *fs) {
  fs->lasttarget = fs->pc;
  return fs->pc;
}



static Instruction *getjumpcontrol (FuncState *fs, int pc) {
  Instruction *pi = &fs->f->code[pc];
  if (pc >= 1 && testTMode(GET_OPCODE(*(pi-1))))
    return pi-1;
  else
    return pi;
}



static int patchtestreg (FuncState *fs, int node, int reg) {
  Instruction *i = getjumpcontrol(fs, node);
  if (GET_OPCODE(*i) != OP_TESTSET)
    return 0;  
  if (reg != NO_REG && reg != GETARG_B(*i))
    SETARG_A(*i, reg);
  else {
     
    *i = CREATE_ABCk(OP_TEST, GETARG_B(*i), 0, 0, GETARG_k(*i));
  }
  return 1;
}



static void removevalues (FuncState *fs, int list) {
  for (; list != NO_JUMP; list = getjump(fs, list))
      patchtestreg(fs, list, NO_REG);
}



static void patchlistaux (FuncState *fs, int list, int vtarget, int reg,
                          int dtarget) {
  while (list != NO_JUMP) {
    int next = getjump(fs, list);
    if (patchtestreg(fs, list, reg))
      fixjump(fs, list, vtarget);
    else
      fixjump(fs, list, dtarget);  
    list = next;
  }
}



void luaK_patchlist (FuncState *fs, int list, int target) {
  lua_assert(target <= fs->pc);
  patchlistaux(fs, list, target, NO_REG, target);
}


void luaK_patchtohere (FuncState *fs, int list) {
  int hr = luaK_getlabel(fs);  
  luaK_patchlist(fs, list, hr);
}



#define LIMLINEDIFF	0x80



static void savelineinfo (FuncState *fs, Proto *f, int line) {
  int linedif = line - fs->previousline;
  int pc = fs->pc - 1;  
  if (abs(linedif) >= LIMLINEDIFF || fs->iwthabs++ >= MAXIWTHABS) {
    luaM_growvector(fs->ls->L, f->abslineinfo, fs->nabslineinfo,
                    f->sizeabslineinfo, AbsLineInfo, MAX_INT, "lines");
    f->abslineinfo[fs->nabslineinfo].pc = pc;
    f->abslineinfo[fs->nabslineinfo++].line = line;
    linedif = ABSLINEINFO;  
    fs->iwthabs = 1;  
  }
  luaM_growvector(fs->ls->L, f->lineinfo, pc, f->sizelineinfo, ls_byte,
                  MAX_INT, "opcodes");
  f->lineinfo[pc] = linedif;
  fs->previousline = line;  
}



static void removelastlineinfo (FuncState *fs) {
  Proto *f = fs->f;
  int pc = fs->pc - 1;  
  if (f->lineinfo[pc] != ABSLINEINFO) {  
    fs->previousline -= f->lineinfo[pc];  
    fs->iwthabs--;  
  }
  else {  
    lua_assert(f->abslineinfo[fs->nabslineinfo - 1].pc == pc);
    fs->nabslineinfo--;  
    fs->iwthabs = MAXIWTHABS + 1;  
  }
}



static void removelastinstruction (FuncState *fs) {
  removelastlineinfo(fs);
  fs->pc--;
}



int luaK_code (FuncState *fs, Instruction i) {
  Proto *f = fs->f;
  
  luaM_growvector(fs->ls->L, f->code, fs->pc, f->sizecode, Instruction,
                  MAX_INT, "opcodes");
  f->code[fs->pc++] = i;
  savelineinfo(fs, f, fs->ls->lastline);
  return fs->pc - 1;  
}



int luaK_codeABCk (FuncState *fs, OpCode o, int a, int b, int c, int k) {
  lua_assert(getOpMode(o) == iABC);
  lua_assert(a <= MAXARG_A && b <= MAXARG_B &&
             c <= MAXARG_C && (k & ~1) == 0);
  return luaK_code(fs, CREATE_ABCk(o, a, b, c, k));
}



int luaK_codeABx (FuncState *fs, OpCode o, int a, unsigned int bc) {
  lua_assert(getOpMode(o) == iABx);
  lua_assert(a <= MAXARG_A && bc <= MAXARG_Bx);
  return luaK_code(fs, CREATE_ABx(o, a, bc));
}



static int codeAsBx (FuncState *fs, OpCode o, int a, int bc) {
  unsigned int b = bc + OFFSET_sBx;
  lua_assert(getOpMode(o) == iAsBx);
  lua_assert(a <= MAXARG_A && b <= MAXARG_Bx);
  return luaK_code(fs, CREATE_ABx(o, a, b));
}



static int codesJ (FuncState *fs, OpCode o, int sj, int k) {
  unsigned int j = sj + OFFSET_sJ;
  lua_assert(getOpMode(o) == isJ);
  lua_assert(j <= MAXARG_sJ && (k & ~1) == 0);
  return luaK_code(fs, CREATE_sJ(o, j, k));
}



static int codeextraarg (FuncState *fs, int a) {
  lua_assert(a <= MAXARG_Ax);
  return luaK_code(fs, CREATE_Ax(OP_EXTRAARG, a));
}



static int luaK_codek (FuncState *fs, int reg, int k) {
  if (k <= MAXARG_Bx)
    return luaK_codeABx(fs, OP_LOADK, reg, k);
  else {
    int p = luaK_codeABx(fs, OP_LOADKX, reg, 0);
    codeextraarg(fs, k);
    return p;
  }
}



void luaK_checkstack (FuncState *fs, int n) {
  int newstack = fs->freereg + n;
  if (newstack > fs->f->maxstacksize) {
    if (newstack >= MAXREGS)
      luaX_syntaxerror(fs->ls,
        "function or expression needs too many registers");
    fs->f->maxstacksize = cast_byte(newstack);
  }
}



void luaK_reserveregs (FuncState *fs, int n) {
  luaK_checkstack(fs, n);
  fs->freereg += n;
}



static void freereg (FuncState *fs, int reg) {
  if (reg >= luaY_nvarstack(fs)) {
    fs->freereg--;
    lua_assert(reg == fs->freereg);
  }
}



static void freeregs (FuncState *fs, int r1, int r2) {
  if (r1 > r2) {
    freereg(fs, r1);
    freereg(fs, r2);
  }
  else {
    freereg(fs, r2);
    freereg(fs, r1);
  }
}



static void freeexp (FuncState *fs, expdesc *e) {
  if (e->k == VNONRELOC)
    freereg(fs, e->u.info);
}



static void freeexps (FuncState *fs, expdesc *e1, expdesc *e2) {
  int r1 = (e1->k == VNONRELOC) ? e1->u.info : -1;
  int r2 = (e2->k == VNONRELOC) ? e2->u.info : -1;
  freeregs(fs, r1, r2);
}



static int addk (FuncState *fs, TValue *key, TValue *v) {
  TValue val;
  lua_State *L = fs->ls->L;
  Proto *f = fs->f;
  int tag = luaH_get(fs->ls->h, key, &val);  
  int k, oldsize;
  if (tag == LUA_VNUMINT) {  
    k = cast_int(ivalue(&val));
    
    if (k < fs->nk && ttypetag(&f->k[k]) == ttypetag(v) &&
                      luaV_rawequalobj(&f->k[k], v))
      return k;  
  }
  
  oldsize = f->sizek;
  k = fs->nk;
  
  setivalue(&val, k);
  luaH_set(L, fs->ls->h, key, &val);
  luaM_growvector(L, f->k, k, f->sizek, TValue, MAXARG_Ax, "constants");
  while (oldsize < f->sizek) setnilvalue(&f->k[oldsize++]);
  setobj(L, &f->k[k], v);
  fs->nk++;
  luaC_barrier(L, f, v);
  return k;
}



static int stringK (FuncState *fs, TString *s) {
  TValue o;
  setsvalue(fs->ls->L, &o, s);
  return addk(fs, &o, &o);  
}



static int luaK_intK (FuncState *fs, lua_Integer n) {
  TValue o;
  setivalue(&o, n);
  return addk(fs, &o, &o);  
}


static int luaK_numberK (FuncState *fs, lua_Number r) {
  TValue o;
  lua_Integer ik;
  setfltvalue(&o, r);
  if (!luaV_flttointeger(r, &ik, F2Ieq))  
    return addk(fs, &o, &o);  
  else {  
    const int nbm = l_floatatt(MANT_DIG);
    const lua_Number q = l_mathop(private_lua_cembed_ldexp)(l_mathop(1.0), -nbm + 1);
    const lua_Number k = (ik == 0) ? q : r + r*q;  
    TValue kv;
    setfltvalue(&kv, k);
    
    lua_assert(!luaV_flttointeger(k, &ik, F2Ieq) ||
                l_mathop(fabs)(r) >= l_mathop(1e6));
    return addk(fs, &kv, &o);
  }
}



static int boolF (FuncState *fs) {
  TValue o;
  setbfvalue(&o);
  return addk(fs, &o, &o);  
}



static int boolT (FuncState *fs) {
  TValue o;
  setbtvalue(&o);
  return addk(fs, &o, &o);  
}



static int nilK (FuncState *fs) {
  TValue k, v;
  setnilvalue(&v);
  
  sethvalue(fs->ls->L, &k, fs->ls->h);
  return addk(fs, &k, &v);
}



static int fitsC (lua_Integer i) {
  return (l_castS2U(i) + OFFSET_sC <= cast_uint(MAXARG_C));
}



static int fitsBx (lua_Integer i) {
  return (-OFFSET_sBx <= i && i <= MAXARG_Bx - OFFSET_sBx);
}


void luaK_int (FuncState *fs, int reg, lua_Integer i) {
  if (fitsBx(i))
    codeAsBx(fs, OP_LOADI, reg, cast_int(i));
  else
    luaK_codek(fs, reg, luaK_intK(fs, i));
}


static void luaK_float (FuncState *fs, int reg, lua_Number f) {
  lua_Integer fi;
  if (luaV_flttointeger(f, &fi, F2Ieq) && fitsBx(fi))
    codeAsBx(fs, OP_LOADF, reg, cast_int(fi));
  else
    luaK_codek(fs, reg, luaK_numberK(fs, f));
}



static void const2exp (TValue *v, expdesc *e) {
  switch (ttypetag(v)) {
    case LUA_VNUMINT:
      e->k = VKINT; e->u.ival = ivalue(v);
      break;
    case LUA_VNUMFLT:
      e->k = VKFLT; e->u.nval = fltvalue(v);
      break;
    case LUA_VFALSE:
      e->k = VFALSE;
      break;
    case LUA_VTRUE:
      e->k = VTRUE;
      break;
    case LUA_VNIL:
      e->k = VNIL;
      break;
    case LUA_VSHRSTR:  case LUA_VLNGSTR:
      e->k = VKSTR; e->u.strval = tsvalue(v);
      break;
    default: lua_assert(0);
  }
}



void luaK_setreturns (FuncState *fs, expdesc *e, int nresults) {
  Instruction *pc = &getinstruction(fs, e);
  if (e->k == VCALL)  
    SETARG_C(*pc, nresults + 1);
  else {
    lua_assert(e->k == VVARARG);
    SETARG_C(*pc, nresults + 1);
    SETARG_A(*pc, fs->freereg);
    luaK_reserveregs(fs, 1);
  }
}



static void str2K (FuncState *fs, expdesc *e) {
  lua_assert(e->k == VKSTR);
  e->u.info = stringK(fs, e->u.strval);
  e->k = VK;
}



void luaK_setoneret (FuncState *fs, expdesc *e) {
  if (e->k == VCALL) {  
    
    lua_assert(GETARG_C(getinstruction(fs, e)) == 2);
    e->k = VNONRELOC;  
    e->u.info = GETARG_A(getinstruction(fs, e));
  }
  else if (e->k == VVARARG) {
    SETARG_C(getinstruction(fs, e), 2);
    e->k = VRELOC;  
  }
}



void luaK_dischargevars (FuncState *fs, expdesc *e) {
  switch (e->k) {
    case VCONST: {
      const2exp(const2val(fs, e), e);
      break;
    }
    case VLOCAL: {  
      e->u.info = e->u.var.ridx;
      e->k = VNONRELOC;  
      break;
    }
    case VUPVAL: {  
      e->u.info = luaK_codeABC(fs, OP_GETUPVAL, 0, e->u.info, 0);
      e->k = VRELOC;
      break;
    }
    case VINDEXUP: {
      e->u.info = luaK_codeABC(fs, OP_GETTABUP, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOC;
      break;
    }
    case VINDEXI: {
      freereg(fs, e->u.ind.t);
      e->u.info = luaK_codeABC(fs, OP_GETI, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOC;
      break;
    }
    case VINDEXSTR: {
      freereg(fs, e->u.ind.t);
      e->u.info = luaK_codeABC(fs, OP_GETFIELD, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOC;
      break;
    }
    case VINDEXED: {
      freeregs(fs, e->u.ind.t, e->u.ind.idx);
      e->u.info = luaK_codeABC(fs, OP_GETTABLE, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOC;
      break;
    }
    case VVARARG: case VCALL: {
      luaK_setoneret(fs, e);
      break;
    }
    default: break;  
  }
}



static void discharge2reg (FuncState *fs, expdesc *e, int reg) {
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VNIL: {
      luaK_nil(fs, reg, 1);
      break;
    }
    case VFALSE: {
      luaK_codeABC(fs, OP_LOADFALSE, reg, 0, 0);
      break;
    }
    case VTRUE: {
      luaK_codeABC(fs, OP_LOADTRUE, reg, 0, 0);
      break;
    }
    case VKSTR: {
      str2K(fs, e);
    }  
    case VK: {
      luaK_codek(fs, reg, e->u.info);
      break;
    }
    case VKFLT: {
      luaK_float(fs, reg, e->u.nval);
      break;
    }
    case VKINT: {
      luaK_int(fs, reg, e->u.ival);
      break;
    }
    case VRELOC: {
      Instruction *pc = &getinstruction(fs, e);
      SETARG_A(*pc, reg);  
      break;
    }
    case VNONRELOC: {
      if (reg != e->u.info)
        luaK_codeABC(fs, OP_MOVE, reg, e->u.info, 0);
      break;
    }
    default: {
      lua_assert(e->k == VJMP);
      return;  
    }
  }
  e->u.info = reg;
  e->k = VNONRELOC;
}



static void discharge2anyreg (FuncState *fs, expdesc *e) {
  if (e->k != VNONRELOC) {  
    luaK_reserveregs(fs, 1);  
    discharge2reg(fs, e, fs->freereg-1);  
  }
}


static int code_loadbool (FuncState *fs, int A, OpCode op) {
  luaK_getlabel(fs);  
  return luaK_codeABC(fs, op, A, 0, 0);
}



static int need_value (FuncState *fs, int list) {
  for (; list != NO_JUMP; list = getjump(fs, list)) {
    Instruction i = *getjumpcontrol(fs, list);
    if (GET_OPCODE(i) != OP_TESTSET) return 1;
  }
  return 0;  
}



static void exp2reg (FuncState *fs, expdesc *e, int reg) {
  discharge2reg(fs, e, reg);
  if (e->k == VJMP)  
    luaK_concat(fs, &e->t, e->u.info);  
  if (hasjumps(e)) {
    int final;  
    int p_f = NO_JUMP;  
    int p_t = NO_JUMP;  
    if (need_value(fs, e->t) || need_value(fs, e->f)) {
      int fj = (e->k == VJMP) ? NO_JUMP : luaK_jump(fs);
      p_f = code_loadbool(fs, reg, OP_LFALSESKIP);  
      p_t = code_loadbool(fs, reg, OP_LOADTRUE);
      
      luaK_patchtohere(fs, fj);
    }
    final = luaK_getlabel(fs);
    patchlistaux(fs, e->f, final, reg, p_f);
    patchlistaux(fs, e->t, final, reg, p_t);
  }
  e->f = e->t = NO_JUMP;
  e->u.info = reg;
  e->k = VNONRELOC;
}



void luaK_exp2nextreg (FuncState *fs, expdesc *e) {
  luaK_dischargevars(fs, e);
  freeexp(fs, e);
  luaK_reserveregs(fs, 1);
  exp2reg(fs, e, fs->freereg - 1);
}



int luaK_exp2anyreg (FuncState *fs, expdesc *e) {
  luaK_dischargevars(fs, e);
  if (e->k == VNONRELOC) {  
    if (!hasjumps(e))  
      return e->u.info;  
    if (e->u.info >= luaY_nvarstack(fs)) {  
      exp2reg(fs, e, e->u.info);  
      return e->u.info;
    }
    
  }
  luaK_exp2nextreg(fs, e);  
  return e->u.info;
}



void luaK_exp2anyregup (FuncState *fs, expdesc *e) {
  if (e->k != VUPVAL || hasjumps(e))
    luaK_exp2anyreg(fs, e);
}



void luaK_exp2val (FuncState *fs, expdesc *e) {
  if (hasjumps(e))
    luaK_exp2anyreg(fs, e);
  else
    luaK_dischargevars(fs, e);
}



static int luaK_exp2K (FuncState *fs, expdesc *e) {
  if (!hasjumps(e)) {
    int info;
    switch (e->k) {  
      case VTRUE: info = boolT(fs); break;
      case VFALSE: info = boolF(fs); break;
      case VNIL: info = nilK(fs); break;
      case VKINT: info = luaK_intK(fs, e->u.ival); break;
      case VKFLT: info = luaK_numberK(fs, e->u.nval); break;
      case VKSTR: info = stringK(fs, e->u.strval); break;
      case VK: info = e->u.info; break;
      default: return 0;  
    }
    if (info <= MAXINDEXRK) {  
      e->k = VK;  
      e->u.info = info;
      return 1;
    }
  }
  
  return 0;
}



static int exp2RK (FuncState *fs, expdesc *e) {
  if (luaK_exp2K(fs, e))
    return 1;
  else {  
    luaK_exp2anyreg(fs, e);
    return 0;
  }
}


static void codeABRK (FuncState *fs, OpCode o, int a, int b,
                      expdesc *ec) {
  int k = exp2RK(fs, ec);
  luaK_codeABCk(fs, o, a, b, ec->u.info, k);
}



void luaK_storevar (FuncState *fs, expdesc *var, expdesc *ex) {
  switch (var->k) {
    case VLOCAL: {
      freeexp(fs, ex);
      exp2reg(fs, ex, var->u.var.ridx);  
      return;
    }
    case VUPVAL: {
      int e = luaK_exp2anyreg(fs, ex);
      luaK_codeABC(fs, OP_SETUPVAL, e, var->u.info, 0);
      break;
    }
    case VINDEXUP: {
      codeABRK(fs, OP_SETTABUP, var->u.ind.t, var->u.ind.idx, ex);
      break;
    }
    case VINDEXI: {
      codeABRK(fs, OP_SETI, var->u.ind.t, var->u.ind.idx, ex);
      break;
    }
    case VINDEXSTR: {
      codeABRK(fs, OP_SETFIELD, var->u.ind.t, var->u.ind.idx, ex);
      break;
    }
    case VINDEXED: {
      codeABRK(fs, OP_SETTABLE, var->u.ind.t, var->u.ind.idx, ex);
      break;
    }
    default: lua_assert(0);  
  }
  freeexp(fs, ex);
}



void luaK_self (FuncState *fs, expdesc *e, expdesc *key) {
  int ereg;
  luaK_exp2anyreg(fs, e);
  ereg = e->u.info;  
  freeexp(fs, e);
  e->u.info = fs->freereg;  
  e->k = VNONRELOC;  
  luaK_reserveregs(fs, 2);  
  codeABRK(fs, OP_SELF, e->u.info, ereg, key);
  freeexp(fs, key);
}



static void negatecondition (FuncState *fs, expdesc *e) {
  Instruction *pc = getjumpcontrol(fs, e->u.info);
  lua_assert(testTMode(GET_OPCODE(*pc)) && GET_OPCODE(*pc) != OP_TESTSET &&
                                           GET_OPCODE(*pc) != OP_TEST);
  SETARG_k(*pc, (GETARG_k(*pc) ^ 1));
}



static int jumponcond (FuncState *fs, expdesc *e, int cond) {
  if (e->k == VRELOC) {
    Instruction ie = getinstruction(fs, e);
    if (GET_OPCODE(ie) == OP_NOT) {
      removelastinstruction(fs);  
      return condjump(fs, OP_TEST, GETARG_B(ie), 0, 0, !cond);
    }
    
  }
  discharge2anyreg(fs, e);
  freeexp(fs, e);
  return condjump(fs, OP_TESTSET, NO_REG, e->u.info, 0, cond);
}



void luaK_goiftrue (FuncState *fs, expdesc *e) {
  int pc;  
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VJMP: {  
      negatecondition(fs, e);  
      pc = e->u.info;  
      break;
    }
    case VK: case VKFLT: case VKINT: case VKSTR: case VTRUE: {
      pc = NO_JUMP;  
      break;
    }
    default: {
      pc = jumponcond(fs, e, 0);  
      break;
    }
  }
  luaK_concat(fs, &e->f, pc);  
  luaK_patchtohere(fs, e->t);  
  e->t = NO_JUMP;
}



void luaK_goiffalse (FuncState *fs, expdesc *e) {
  int pc;  
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VJMP: {
      pc = e->u.info;  
      break;
    }
    case VNIL: case VFALSE: {
      pc = NO_JUMP;  
      break;
    }
    default: {
      pc = jumponcond(fs, e, 1);  
      break;
    }
  }
  luaK_concat(fs, &e->t, pc);  
  luaK_patchtohere(fs, e->f);  
  e->f = NO_JUMP;
}



static void codenot (FuncState *fs, expdesc *e) {
  switch (e->k) {
    case VNIL: case VFALSE: {
      e->k = VTRUE;  
      break;
    }
    case VK: case VKFLT: case VKINT: case VKSTR: case VTRUE: {
      e->k = VFALSE;  
      break;
    }
    case VJMP: {
      negatecondition(fs, e);
      break;
    }
    case VRELOC:
    case VNONRELOC: {
      discharge2anyreg(fs, e);
      freeexp(fs, e);
      e->u.info = luaK_codeABC(fs, OP_NOT, 0, e->u.info, 0);
      e->k = VRELOC;
      break;
    }
    default: lua_assert(0);  
  }
  
  { int temp = e->f; e->f = e->t; e->t = temp; }
  removevalues(fs, e->f);  
  removevalues(fs, e->t);
}



static int isKstr (FuncState *fs, expdesc *e) {
  return (e->k == VK && !hasjumps(e) && e->u.info <= MAXARG_B &&
          ttisshrstring(&fs->f->k[e->u.info]));
}


static int isKint (expdesc *e) {
  return (e->k == VKINT && !hasjumps(e));
}



static int isCint (expdesc *e) {
  return isKint(e) && (l_castS2U(e->u.ival) <= l_castS2U(MAXARG_C));
}



static int isSCint (expdesc *e) {
  return isKint(e) && fitsC(e->u.ival);
}



static int isSCnumber (expdesc *e, int *pi, int *isfloat) {
  lua_Integer i;
  if (e->k == VKINT)
    i = e->u.ival;
  else if (e->k == VKFLT && luaV_flttointeger(e->u.nval, &i, F2Ieq))
    *isfloat = 1;
  else
    return 0;  
  if (!hasjumps(e) && fitsC(i)) {
    *pi = int2sC(cast_int(i));
    return 1;
  }
  else
    return 0;
}



void luaK_indexed (FuncState *fs, expdesc *t, expdesc *k) {
  if (k->k == VKSTR)
    str2K(fs, k);
  lua_assert(!hasjumps(t) &&
             (t->k == VLOCAL || t->k == VNONRELOC || t->k == VUPVAL));
  if (t->k == VUPVAL && !isKstr(fs, k))  
    luaK_exp2anyreg(fs, t);  
  if (t->k == VUPVAL) {
    lua_assert(isKstr(fs, k));
    t->u.ind.t = t->u.info;  
    t->u.ind.idx = k->u.info;  
    t->k = VINDEXUP;
  }
  else {
    
    t->u.ind.t = (t->k == VLOCAL) ? t->u.var.ridx: t->u.info;
    if (isKstr(fs, k)) {
      t->u.ind.idx = k->u.info;  
      t->k = VINDEXSTR;
    }
    else if (isCint(k)) {
      t->u.ind.idx = cast_int(k->u.ival);  
      t->k = VINDEXI;
    }
    else {
      t->u.ind.idx = luaK_exp2anyreg(fs, k);  
      t->k = VINDEXED;
    }
  }
}



static int validop (int op, TValue *v1, TValue *v2) {
  switch (op) {
    case LUA_OPBAND: case LUA_OPBOR: case LUA_OPBXOR:
    case LUA_OPSHL: case LUA_OPSHR: case LUA_OPBNOT: {  
      lua_Integer i;
      return (luaV_tointegerns(v1, &i, LUA_FLOORN2I) &&
              luaV_tointegerns(v2, &i, LUA_FLOORN2I));
    }
    case LUA_OPDIV: case LUA_OPIDIV: case LUA_OPMOD:  
      return (nvalue(v2) != 0);
    default: return 1;  
  }
}



static int constfolding (FuncState *fs, int op, expdesc *e1,
                                        const expdesc *e2) {
  TValue v1, v2, res;
  if (!tonumeral(e1, &v1) || !tonumeral(e2, &v2) || !validop(op, &v1, &v2))
    return 0;  
  luaO_rawarith(fs->ls->L, op, &v1, &v2, &res);  
  if (ttisinteger(&res)) {
    e1->k = VKINT;
    e1->u.ival = ivalue(&res);
  }
  else {  
    lua_Number n = fltvalue(&res);
    if (luai_numisnan(n) || n == 0)
      return 0;
    e1->k = VKFLT;
    e1->u.nval = n;
  }
  return 1;
}



l_sinline OpCode binopr2op (BinOpr opr, BinOpr baser, OpCode base) {
  lua_assert(baser <= opr &&
            ((baser == OPR_ADD && opr <= OPR_SHR) ||
             (baser == OPR_LT && opr <= OPR_LE)));
  return cast(OpCode, (cast_int(opr) - cast_int(baser)) + cast_int(base));
}



l_sinline OpCode unopr2op (UnOpr opr) {
  return cast(OpCode, (cast_int(opr) - cast_int(OPR_MINUS)) +
                                       cast_int(OP_UNM));
}



l_sinline TMS binopr2TM (BinOpr opr) {
  lua_assert(OPR_ADD <= opr && opr <= OPR_SHR);
  return cast(TMS, (cast_int(opr) - cast_int(OPR_ADD)) + cast_int(TM_ADD));
}



static void codeunexpval (FuncState *fs, OpCode op, expdesc *e, int line) {
  int r = luaK_exp2anyreg(fs, e);  
  freeexp(fs, e);
  e->u.info = luaK_codeABC(fs, op, 0, r, 0);  
  e->k = VRELOC;  
  luaK_fixline(fs, line);
}



static void finishbinexpval (FuncState *fs, expdesc *e1, expdesc *e2,
                             OpCode op, int v2, int flip, int line,
                             OpCode mmop, TMS event) {
  int v1 = luaK_exp2anyreg(fs, e1);
  int pc = luaK_codeABCk(fs, op, 0, v1, v2, 0);
  freeexps(fs, e1, e2);
  e1->u.info = pc;
  e1->k = VRELOC;  
  luaK_fixline(fs, line);
  luaK_codeABCk(fs, mmop, v1, v2, event, flip);  
  luaK_fixline(fs, line);
}



static void codebinexpval (FuncState *fs, BinOpr opr,
                           expdesc *e1, expdesc *e2, int line) {
  OpCode op = binopr2op(opr, OPR_ADD, OP_ADD);
  int v2 = luaK_exp2anyreg(fs, e2);  
  
  lua_assert((VNIL <= e1->k && e1->k <= VKSTR) ||
             e1->k == VNONRELOC || e1->k == VRELOC);
  lua_assert(OP_ADD <= op && op <= OP_SHR);
  finishbinexpval(fs, e1, e2, op, v2, 0, line, OP_MMBIN, binopr2TM(opr));
}



static void codebini (FuncState *fs, OpCode op,
                       expdesc *e1, expdesc *e2, int flip, int line,
                       TMS event) {
  int v2 = int2sC(cast_int(e2->u.ival));  
  lua_assert(e2->k == VKINT);
  finishbinexpval(fs, e1, e2, op, v2, flip, line, OP_MMBINI, event);
}



static void codebinK (FuncState *fs, BinOpr opr,
                      expdesc *e1, expdesc *e2, int flip, int line) {
  TMS event = binopr2TM(opr);
  int v2 = e2->u.info;  
  OpCode op = binopr2op(opr, OPR_ADD, OP_ADDK);
  finishbinexpval(fs, e1, e2, op, v2, flip, line, OP_MMBINK, event);
}



static int finishbinexpneg (FuncState *fs, expdesc *e1, expdesc *e2,
                             OpCode op, int line, TMS event) {
  if (!isKint(e2))
    return 0;  
  else {
    lua_Integer i2 = e2->u.ival;
    if (!(fitsC(i2) && fitsC(-i2)))
      return 0;  
    else {  
      int v2 = cast_int(i2);
      finishbinexpval(fs, e1, e2, op, int2sC(-v2), 0, line, OP_MMBINI, event);
      
      SETARG_B(fs->f->code[fs->pc - 1], int2sC(v2));
      return 1;  
    }
  }
}


static void swapexps (expdesc *e1, expdesc *e2) {
  expdesc temp = *e1; *e1 = *e2; *e2 = temp;  
}



static void codebinNoK (FuncState *fs, BinOpr opr,
                        expdesc *e1, expdesc *e2, int flip, int line) {
  if (flip)
    swapexps(e1, e2);  
  codebinexpval(fs, opr, e1, e2, line);  
}



static void codearith (FuncState *fs, BinOpr opr,
                       expdesc *e1, expdesc *e2, int flip, int line) {
  if (tonumeral(e2, NULL) && luaK_exp2K(fs, e2))  
    codebinK(fs, opr, e1, e2, flip, line);
  else  
    codebinNoK(fs, opr, e1, e2, flip, line);
}



static void codecommutative (FuncState *fs, BinOpr op,
                             expdesc *e1, expdesc *e2, int line) {
  int flip = 0;
  if (tonumeral(e1, NULL)) {  
    swapexps(e1, e2);  
    flip = 1;
  }
  if (op == OPR_ADD && isSCint(e2))  
    codebini(fs, OP_ADDI, e1, e2, flip, line, TM_ADD);
  else
    codearith(fs, op, e1, e2, flip, line);
}



static void codebitwise (FuncState *fs, BinOpr opr,
                         expdesc *e1, expdesc *e2, int line) {
  int flip = 0;
  if (e1->k == VKINT) {
    swapexps(e1, e2);  
    flip = 1;
  }
  if (e2->k == VKINT && luaK_exp2K(fs, e2))  
    codebinK(fs, opr, e1, e2, flip, line);
  else  
    codebinNoK(fs, opr, e1, e2, flip, line);
}



static void codeorder (FuncState *fs, BinOpr opr, expdesc *e1, expdesc *e2) {
  int r1, r2;
  int im;
  int isfloat = 0;
  OpCode op;
  if (isSCnumber(e2, &im, &isfloat)) {
    
    r1 = luaK_exp2anyreg(fs, e1);
    r2 = im;
    op = binopr2op(opr, OPR_LT, OP_LTI);
  }
  else if (isSCnumber(e1, &im, &isfloat)) {
    
    r1 = luaK_exp2anyreg(fs, e2);
    r2 = im;
    op = binopr2op(opr, OPR_LT, OP_GTI);
  }
  else {  
    r1 = luaK_exp2anyreg(fs, e1);
    r2 = luaK_exp2anyreg(fs, e2);
    op = binopr2op(opr, OPR_LT, OP_LT);
  }
  freeexps(fs, e1, e2);
  e1->u.info = condjump(fs, op, r1, r2, isfloat, 1);
  e1->k = VJMP;
}



static void codeeq (FuncState *fs, BinOpr opr, expdesc *e1, expdesc *e2) {
  int r1, r2;
  int im;
  int isfloat = 0;  
  OpCode op;
  if (e1->k != VNONRELOC) {
    lua_assert(e1->k == VK || e1->k == VKINT || e1->k == VKFLT);
    swapexps(e1, e2);
  }
  r1 = luaK_exp2anyreg(fs, e1);  
  if (isSCnumber(e2, &im, &isfloat)) {
    op = OP_EQI;
    r2 = im;  
  }
  else if (exp2RK(fs, e2)) {  
    op = OP_EQK;
    r2 = e2->u.info;  
  }
  else {
    op = OP_EQ;  
    r2 = luaK_exp2anyreg(fs, e2);
  }
  freeexps(fs, e1, e2);
  e1->u.info = condjump(fs, op, r1, r2, isfloat, (opr == OPR_EQ));
  e1->k = VJMP;
}



void luaK_prefix (FuncState *fs, UnOpr opr, expdesc *e, int line) {
  static const expdesc ef = {VKINT, {0}, NO_JUMP, NO_JUMP};
  luaK_dischargevars(fs, e);
  switch (opr) {
    case OPR_MINUS: case OPR_BNOT:  
      if (constfolding(fs, opr + LUA_OPUNM, e, &ef))
        break;
       
    case OPR_LEN:
      codeunexpval(fs, unopr2op(opr), e, line);
      break;
    case OPR_NOT: codenot(fs, e); break;
    default: lua_assert(0);
  }
}



void luaK_infix (FuncState *fs, BinOpr op, expdesc *v) {
  luaK_dischargevars(fs, v);
  switch (op) {
    case OPR_AND: {
      luaK_goiftrue(fs, v);  
      break;
    }
    case OPR_OR: {
      luaK_goiffalse(fs, v);  
      break;
    }
    case OPR_CONCAT: {
      luaK_exp2nextreg(fs, v);  
      break;
    }
    case OPR_ADD: case OPR_SUB:
    case OPR_MUL: case OPR_DIV: case OPR_IDIV:
    case OPR_MOD: case OPR_POW:
    case OPR_BAND: case OPR_BOR: case OPR_BXOR:
    case OPR_SHL: case OPR_SHR: {
      if (!tonumeral(v, NULL))
        luaK_exp2anyreg(fs, v);
      
      break;
    }
    case OPR_EQ: case OPR_NE: {
      if (!tonumeral(v, NULL))
        exp2RK(fs, v);
      
      break;
    }
    case OPR_LT: case OPR_LE:
    case OPR_GT: case OPR_GE: {
      int dummy, dummy2;
      if (!isSCnumber(v, &dummy, &dummy2))
        luaK_exp2anyreg(fs, v);
      
      break;
    }
    default: lua_assert(0);
  }
}


static void codeconcat (FuncState *fs, expdesc *e1, expdesc *e2, int line) {
  Instruction *ie2 = previousinstruction(fs);
  if (GET_OPCODE(*ie2) == OP_CONCAT) {  
    int n = GETARG_B(*ie2);  
    lua_assert(e1->u.info + 1 == GETARG_A(*ie2));
    freeexp(fs, e2);
    SETARG_A(*ie2, e1->u.info);  
    SETARG_B(*ie2, n + 1);  
  }
  else {  
    luaK_codeABC(fs, OP_CONCAT, e1->u.info, 2, 0);  
    freeexp(fs, e2);
    luaK_fixline(fs, line);
  }
}



void luaK_posfix (FuncState *fs, BinOpr opr,
                  expdesc *e1, expdesc *e2, int line) {
  luaK_dischargevars(fs, e2);
  if (foldbinop(opr) && constfolding(fs, opr + LUA_OPADD, e1, e2))
    return;  
  switch (opr) {
    case OPR_AND: {
      lua_assert(e1->t == NO_JUMP);  
      luaK_concat(fs, &e2->f, e1->f);
      *e1 = *e2;
      break;
    }
    case OPR_OR: {
      lua_assert(e1->f == NO_JUMP);  
      luaK_concat(fs, &e2->t, e1->t);
      *e1 = *e2;
      break;
    }
    case OPR_CONCAT: {  
      luaK_exp2nextreg(fs, e2);
      codeconcat(fs, e1, e2, line);
      break;
    }
    case OPR_ADD: case OPR_MUL: {
      codecommutative(fs, opr, e1, e2, line);
      break;
    }
    case OPR_SUB: {
      if (finishbinexpneg(fs, e1, e2, OP_ADDI, line, TM_SUB))
        break; 
      
    }  
    case OPR_DIV: case OPR_IDIV: case OPR_MOD: case OPR_POW: {
      codearith(fs, opr, e1, e2, 0, line);
      break;
    }
    case OPR_BAND: case OPR_BOR: case OPR_BXOR: {
      codebitwise(fs, opr, e1, e2, line);
      break;
    }
    case OPR_SHL: {
      if (isSCint(e1)) {
        swapexps(e1, e2);
        codebini(fs, OP_SHLI, e1, e2, 1, line, TM_SHL);  
      }
      else if (finishbinexpneg(fs, e1, e2, OP_SHRI, line, TM_SHL)) {
        ;
      }
      else  
       codebinexpval(fs, opr, e1, e2, line);
      break;
    }
    case OPR_SHR: {
      if (isSCint(e2))
        codebini(fs, OP_SHRI, e1, e2, 0, line, TM_SHR);  
      else  
        codebinexpval(fs, opr, e1, e2, line);
      break;
    }
    case OPR_EQ: case OPR_NE: {
      codeeq(fs, opr, e1, e2);
      break;
    }
    case OPR_GT: case OPR_GE: {
      
      swapexps(e1, e2);
      opr = cast(BinOpr, (opr - OPR_GT) + OPR_LT);
    }  
    case OPR_LT: case OPR_LE: {
      codeorder(fs, opr, e1, e2);
      break;
    }
    default: lua_assert(0);
  }
}



void luaK_fixline (FuncState *fs, int line) {
  removelastlineinfo(fs);
  savelineinfo(fs, fs->f, line);
}


void luaK_settablesize (FuncState *fs, int pc, int ra, int asize, int hsize) {
  Instruction *inst = &fs->f->code[pc];
  int rb = (hsize != 0) ? luaO_ceillog2(hsize) + 1 : 0;  
  int extra = asize / (MAXARG_C + 1);  
  int rc = asize % (MAXARG_C + 1);  
  int k = (extra > 0);  
  *inst = CREATE_ABCk(OP_NEWTABLE, ra, rb, rc, k);
  *(inst + 1) = CREATE_Ax(OP_EXTRAARG, extra);
}



void luaK_setlist (FuncState *fs, int base, int nelems, int tostore) {
  lua_assert(tostore != 0 && tostore <= LFIELDS_PER_FLUSH);
  if (tostore == LUA_MULTRET)
    tostore = 0;
  if (nelems <= MAXARG_C)
    luaK_codeABC(fs, OP_SETLIST, base, tostore, nelems);
  else {
    int extra = nelems / (MAXARG_C + 1);
    nelems %= (MAXARG_C + 1);
    luaK_codeABCk(fs, OP_SETLIST, base, tostore, nelems, 1);
    codeextraarg(fs, extra);
  }
  fs->freereg = base + 1;  
}



static int finaltarget (Instruction *code, int i) {
  int count;
  for (count = 0; count < 100; count++) {  
    Instruction pc = code[i];
    if (GET_OPCODE(pc) != OP_JMP)
      break;
     else
       i += GETARG_sJ(pc) + 1;
  }
  return i;
}



void luaK_finish (FuncState *fs) {
  int i;
  Proto *p = fs->f;
  for (i = 0; i < fs->pc; i++) {
    Instruction *pc = &p->code[i];
    lua_assert(i == 0 || isOT(*(pc - 1)) == isIT(*pc));
    switch (GET_OPCODE(*pc)) {
      case OP_RETURN0: case OP_RETURN1: {
        if (!(fs->needclose || (p->flag & PF_ISVARARG)))
          break;  
        
        SET_OPCODE(*pc, OP_RETURN);
      }  
      case OP_RETURN: case OP_TAILCALL: {
        if (fs->needclose)
          SETARG_k(*pc, 1);  
        if (p->flag & PF_ISVARARG)
          SETARG_C(*pc, p->numparams + 1);  
        break;
      }
      case OP_JMP: {
        int target = finaltarget(p->code, i);
        fixjump(fs, i, target);
        break;
      }
      default: break;
    }
  }
}


//path: src/imports/../dependencies/lua/lparser.c


#define lparser_c
#define LUA_CORE





#include <limits.h>
#include <string.h>
































#define MAXVARS		200


#define hasmultret(k)		((k) == VCALL || (k) == VVARARG)



#define eqstr(a,b)	((a) == (b))



typedef struct BlockCnt {
  struct BlockCnt *previous;  
  int firstlabel;  
  int firstgoto;  
  lu_byte nactvar;  
  lu_byte upval;  
  lu_byte isloop;  
  lu_byte insidetbc;  
} BlockCnt;




static void statement (LexState *ls);
static void expr (LexState *ls, expdesc *v);


static l_noret error_expected (LexState *ls, int token) {
  luaX_syntaxerror(ls,
      luaO_pushfstring(ls->L, "%s expected", luaX_token2str(ls, token)));
}


static l_noret errorlimit (FuncState *fs, int limit, const char *what) {
  lua_State *L = fs->ls->L;
  const char *msg;
  int line = fs->f->linedefined;
  const char *where = (line == 0)
                      ? "main function"
                      : luaO_pushfstring(L, "function at line %d", line);
  msg = luaO_pushfstring(L, "too many %s (limit is %d) in %s",
                             what, limit, where);
  luaX_syntaxerror(fs->ls, msg);
}


static void checklimit (FuncState *fs, int v, int l, const char *what) {
  if (v > l) errorlimit(fs, l, what);
}



static int testnext (LexState *ls, int c) {
  if (ls->t.token == c) {
    luaX_next(ls);
    return 1;
  }
  else return 0;
}



static void check (LexState *ls, int c) {
  if (ls->t.token != c)
    error_expected(ls, c);
}



static void checknext (LexState *ls, int c) {
  check(ls, c);
  luaX_next(ls);
}


#define check_condition(ls,c,msg)	{ if (!(c)) luaX_syntaxerror(ls, msg); }



static void check_match (LexState *ls, int what, int who, int where) {
  if (l_unlikely(!testnext(ls, what))) {
    if (where == ls->linenumber)  
      error_expected(ls, what);  
    else {
      luaX_syntaxerror(ls, luaO_pushfstring(ls->L,
             "%s expected (to close %s at line %d)",
              luaX_token2str(ls, what), luaX_token2str(ls, who), where));
    }
  }
}


static TString *str_checkname (LexState *ls) {
  TString *ts;
  check(ls, TK_NAME);
  ts = ls->t.seminfo.ts;
  luaX_next(ls);
  return ts;
}


static void init_exp (expdesc *e, expkind k, int i) {
  e->f = e->t = NO_JUMP;
  e->k = k;
  e->u.info = i;
}


static void codestring (expdesc *e, TString *s) {
  e->f = e->t = NO_JUMP;
  e->k = VKSTR;
  e->u.strval = s;
}


static void codename (LexState *ls, expdesc *e) {
  codestring(e, str_checkname(ls));
}



static int registerlocalvar (LexState *ls, FuncState *fs, TString *varname) {
  Proto *f = fs->f;
  int oldsize = f->sizelocvars;
  luaM_growvector(ls->L, f->locvars, fs->ndebugvars, f->sizelocvars,
                  LocVar, SHRT_MAX, "local variables");
  while (oldsize < f->sizelocvars)
    f->locvars[oldsize++].varname = NULL;
  f->locvars[fs->ndebugvars].varname = varname;
  f->locvars[fs->ndebugvars].startpc = fs->pc;
  luaC_objbarrier(ls->L, f, varname);
  return fs->ndebugvars++;
}



static int new_localvarkind (LexState *ls, TString *name, int kind) {
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Dyndata *dyd = ls->dyd;
  Vardesc *var;
  checklimit(fs, dyd->actvar.n + 1 - fs->firstlocal,
                 MAXVARS, "local variables");
  luaM_growvector(L, dyd->actvar.arr, dyd->actvar.n + 1,
                  dyd->actvar.size, Vardesc, USHRT_MAX, "local variables");
  var = &dyd->actvar.arr[dyd->actvar.n++];
  var->vd.kind = kind;  
  var->vd.name = name;
  return dyd->actvar.n - 1 - fs->firstlocal;
}



static int new_localvar (LexState *ls, TString *name) {
  return new_localvarkind(ls, name, VDKREG);
}

#define new_localvarliteral(ls,v) \
    new_localvar(ls,  \
      luaX_newstring(ls, "" v, (sizeof(v)/sizeof(char)) - 1));




static Vardesc *getlocalvardesc (FuncState *fs, int vidx) {
  return &fs->ls->dyd->actvar.arr[fs->firstlocal + vidx];
}



static int reglevel (FuncState *fs, int nvar) {
  while (nvar-- > 0) {
    Vardesc *vd = getlocalvardesc(fs, nvar);  
    if (vd->vd.kind != RDKCTC)  
      return vd->vd.ridx + 1;
  }
  return 0;  
}



int luaY_nvarstack (FuncState *fs) {
  return reglevel(fs, fs->nactvar);
}



static LocVar *localdebuginfo (FuncState *fs, int vidx) {
  Vardesc *vd = getlocalvardesc(fs,  vidx);
  if (vd->vd.kind == RDKCTC)
    return NULL;  
  else {
    int idx = vd->vd.pidx;
    lua_assert(idx < fs->ndebugvars);
    return &fs->f->locvars[idx];
  }
}



static void init_var (FuncState *fs, expdesc *e, int vidx) {
  e->f = e->t = NO_JUMP;
  e->k = VLOCAL;
  e->u.var.vidx = vidx;
  e->u.var.ridx = getlocalvardesc(fs, vidx)->vd.ridx;
}



static void check_readonly (LexState *ls, expdesc *e) {
  FuncState *fs = ls->fs;
  TString *varname = NULL;  
  switch (e->k) {
    case VCONST: {
      varname = ls->dyd->actvar.arr[e->u.info].vd.name;
      break;
    }
    case VLOCAL: {
      Vardesc *vardesc = getlocalvardesc(fs, e->u.var.vidx);
      if (vardesc->vd.kind != VDKREG)  
        varname = vardesc->vd.name;
      break;
    }
    case VUPVAL: {
      Upvaldesc *up = &fs->f->upvalues[e->u.info];
      if (up->kind != VDKREG)
        varname = up->name;
      break;
    }
    default:
      return;  
  }
  if (varname) {
    const char *msg = luaO_pushfstring(ls->L,
       "attempt to assign to const variable '%s'", getstr(varname));
    luaK_semerror(ls, msg);  
  }
}



static void adjustlocalvars (LexState *ls, int nvars) {
  FuncState *fs = ls->fs;
  int reglevel = luaY_nvarstack(fs);
  int i;
  for (i = 0; i < nvars; i++) {
    int vidx = fs->nactvar++;
    Vardesc *var = getlocalvardesc(fs, vidx);
    var->vd.ridx = reglevel++;
    var->vd.pidx = registerlocalvar(ls, fs, var->vd.name);
  }
}



static void removevars (FuncState *fs, int tolevel) {
  fs->ls->dyd->actvar.n -= (fs->nactvar - tolevel);
  while (fs->nactvar > tolevel) {
    LocVar *var = localdebuginfo(fs, --fs->nactvar);
    if (var)  
      var->endpc = fs->pc;
  }
}



static int searchupvalue (FuncState *fs, TString *name) {
  int i;
  Upvaldesc *up = fs->f->upvalues;
  for (i = 0; i < fs->nups; i++) {
    if (eqstr(up[i].name, name)) return i;
  }
  return -1;  
}


static Upvaldesc *allocupvalue (FuncState *fs) {
  Proto *f = fs->f;
  int oldsize = f->sizeupvalues;
  checklimit(fs, fs->nups + 1, MAXUPVAL, "upvalues");
  luaM_growvector(fs->ls->L, f->upvalues, fs->nups, f->sizeupvalues,
                  Upvaldesc, MAXUPVAL, "upvalues");
  while (oldsize < f->sizeupvalues)
    f->upvalues[oldsize++].name = NULL;
  return &f->upvalues[fs->nups++];
}


static int newupvalue (FuncState *fs, TString *name, expdesc *v) {
  Upvaldesc *up = allocupvalue(fs);
  FuncState *prev = fs->prev;
  if (v->k == VLOCAL) {
    up->instack = 1;
    up->idx = v->u.var.ridx;
    up->kind = getlocalvardesc(prev, v->u.var.vidx)->vd.kind;
    lua_assert(eqstr(name, getlocalvardesc(prev, v->u.var.vidx)->vd.name));
  }
  else {
    up->instack = 0;
    up->idx = cast_byte(v->u.info);
    up->kind = prev->f->upvalues[v->u.info].kind;
    lua_assert(eqstr(name, prev->f->upvalues[v->u.info].name));
  }
  up->name = name;
  luaC_objbarrier(fs->ls->L, fs->f, name);
  return fs->nups - 1;
}



static int searchvar (FuncState *fs, TString *n, expdesc *var) {
  int i;
  for (i = cast_int(fs->nactvar) - 1; i >= 0; i--) {
    Vardesc *vd = getlocalvardesc(fs, i);
    if (eqstr(n, vd->vd.name)) {  
      if (vd->vd.kind == RDKCTC)  
        init_exp(var, VCONST, fs->firstlocal + i);
      else  
        init_var(fs, var, i);
      return var->k;
    }
  }
  return -1;  
}



static void markupval (FuncState *fs, int level) {
  BlockCnt *bl = fs->bl;
  while (bl->nactvar > level)
    bl = bl->previous;
  bl->upval = 1;
  fs->needclose = 1;
}



static void marktobeclosed (FuncState *fs) {
  BlockCnt *bl = fs->bl;
  bl->upval = 1;
  bl->insidetbc = 1;
  fs->needclose = 1;
}



static void singlevaraux (FuncState *fs, TString *n, expdesc *var, int base) {
  if (fs == NULL)  
    init_exp(var, VVOID, 0);  
  else {
    int v = searchvar(fs, n, var);  
    if (v >= 0) {  
      if (v == VLOCAL && !base)
        markupval(fs, var->u.var.vidx);  
    }
    else {  
      int idx = searchupvalue(fs, n);  
      if (idx < 0) {  
        singlevaraux(fs->prev, n, var, 0);  
        if (var->k == VLOCAL || var->k == VUPVAL)  
          idx  = newupvalue(fs, n, var);  
        else  
          return;  
      }
      init_exp(var, VUPVAL, idx);  
    }
  }
}



static void singlevar (LexState *ls, expdesc *var) {
  TString *varname = str_checkname(ls);
  FuncState *fs = ls->fs;
  singlevaraux(fs, varname, var, 1);
  if (var->k == VVOID) {  
    expdesc key;
    singlevaraux(fs, ls->envn, var, 1);  
    lua_assert(var->k != VVOID);  
    luaK_exp2anyregup(fs, var);  
    codestring(&key, varname);  
    luaK_indexed(fs, var, &key);  
  }
}



static void adjust_assign (LexState *ls, int nvars, int nexps, expdesc *e) {
  FuncState *fs = ls->fs;
  int needed = nvars - nexps;  
  if (hasmultret(e->k)) {  
    int extra = needed + 1;  
    if (extra < 0)
      extra = 0;
    luaK_setreturns(fs, e, extra);  
  }
  else {
    if (e->k != VVOID)  
      luaK_exp2nextreg(fs, e);  
    if (needed > 0)  
      luaK_nil(fs, fs->freereg, needed);  
  }
  if (needed > 0)
    luaK_reserveregs(fs, needed);  
  else  
    fs->freereg += needed;  
}


#define enterlevel(ls)	luaE_incCstack(ls->L)


#define leavelevel(ls) ((ls)->L->nCcalls--)



static l_noret jumpscopeerror (LexState *ls, Labeldesc *gt) {
  TString *tsname = getlocalvardesc(ls->fs, gt->nactvar)->vd.name;
  const char *varname = getstr(tsname);
  const char *msg = "<goto %s> at line %d jumps into the scope of local '%s'";
  msg = luaO_pushfstring(ls->L, msg, getstr(gt->name), gt->line, varname);
  luaK_semerror(ls, msg);  
}



static void solvegoto (LexState *ls, int g, Labeldesc *label) {
  int i;
  Labellist *gl = &ls->dyd->gt;  
  Labeldesc *gt = &gl->arr[g];  
  lua_assert(eqstr(gt->name, label->name));
  if (l_unlikely(gt->nactvar < label->nactvar))  
    jumpscopeerror(ls, gt);
  luaK_patchlist(ls->fs, gt->pc, label->pc);
  for (i = g; i < gl->n - 1; i++)  
    gl->arr[i] = gl->arr[i + 1];
  gl->n--;
}



static Labeldesc *findlabel (LexState *ls, TString *name) {
  int i;
  Dyndata *dyd = ls->dyd;
  
  for (i = ls->fs->firstlabel; i < dyd->label.n; i++) {
    Labeldesc *lb = &dyd->label.arr[i];
    if (eqstr(lb->name, name))  
      return lb;
  }
  return NULL;  
}



static int newlabelentry (LexState *ls, Labellist *l, TString *name,
                          int line, int pc) {
  int n = l->n;
  luaM_growvector(ls->L, l->arr, n, l->size,
                  Labeldesc, SHRT_MAX, "labels/gotos");
  l->arr[n].name = name;
  l->arr[n].line = line;
  l->arr[n].nactvar = ls->fs->nactvar;
  l->arr[n].close = 0;
  l->arr[n].pc = pc;
  l->n = n + 1;
  return n;
}


static int newgotoentry (LexState *ls, TString *name, int line, int pc) {
  return newlabelentry(ls, &ls->dyd->gt, name, line, pc);
}



static int solvegotos (LexState *ls, Labeldesc *lb) {
  Labellist *gl = &ls->dyd->gt;
  int i = ls->fs->bl->firstgoto;
  int needsclose = 0;
  while (i < gl->n) {
    if (eqstr(gl->arr[i].name, lb->name)) {
      needsclose |= gl->arr[i].close;
      solvegoto(ls, i, lb);  
    }
    else
      i++;
  }
  return needsclose;
}



static int createlabel (LexState *ls, TString *name, int line,
                        int last) {
  FuncState *fs = ls->fs;
  Labellist *ll = &ls->dyd->label;
  int l = newlabelentry(ls, ll, name, line, luaK_getlabel(fs));
  if (last) {  
    
    ll->arr[l].nactvar = fs->bl->nactvar;
  }
  if (solvegotos(ls, &ll->arr[l])) {  
    luaK_codeABC(fs, OP_CLOSE, luaY_nvarstack(fs), 0, 0);
    return 1;
  }
  return 0;
}



static void movegotosout (FuncState *fs, BlockCnt *bl) {
  int i;
  Labellist *gl = &fs->ls->dyd->gt;
  
  for (i = bl->firstgoto; i < gl->n; i++) {  
    Labeldesc *gt = &gl->arr[i];
    
    if (reglevel(fs, gt->nactvar) > reglevel(fs, bl->nactvar))
      gt->close |= bl->upval;  
    gt->nactvar = bl->nactvar;  
  }
}


static void enterblock (FuncState *fs, BlockCnt *bl, lu_byte isloop) {
  bl->isloop = isloop;
  bl->nactvar = fs->nactvar;
  bl->firstlabel = fs->ls->dyd->label.n;
  bl->firstgoto = fs->ls->dyd->gt.n;
  bl->upval = 0;
  bl->insidetbc = (fs->bl != NULL && fs->bl->insidetbc);
  bl->previous = fs->bl;
  fs->bl = bl;
  lua_assert(fs->freereg == luaY_nvarstack(fs));
}



static l_noret undefgoto (LexState *ls, Labeldesc *gt) {
  const char *msg;
  if (eqstr(gt->name, luaS_newliteral(ls->L, "break"))) {
    msg = "break outside loop at line %d";
    msg = luaO_pushfstring(ls->L, msg, gt->line);
  }
  else {
    msg = "no visible label '%s' for <goto> at line %d";
    msg = luaO_pushfstring(ls->L, msg, getstr(gt->name), gt->line);
  }
  luaK_semerror(ls, msg);
}


static void leaveblock (FuncState *fs) {
  BlockCnt *bl = fs->bl;
  LexState *ls = fs->ls;
  int hasclose = 0;
  int stklevel = reglevel(fs, bl->nactvar);  
  removevars(fs, bl->nactvar);  
  lua_assert(bl->nactvar == fs->nactvar);  
  if (bl->isloop)  
    hasclose = createlabel(ls, luaS_newliteral(ls->L, "break"), 0, 0);
  if (!hasclose && bl->previous && bl->upval)  
    luaK_codeABC(fs, OP_CLOSE, stklevel, 0, 0);
  fs->freereg = stklevel;  
  ls->dyd->label.n = bl->firstlabel;  
  fs->bl = bl->previous;  
  if (bl->previous)  
    movegotosout(fs, bl);  
  else {
    if (bl->firstgoto < ls->dyd->gt.n)  
      undefgoto(ls, &ls->dyd->gt.arr[bl->firstgoto]);  
  }
}



static Proto *addprototype (LexState *ls) {
  Proto *clp;
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f;  
  if (fs->np >= f->sizep) {
    int oldsize = f->sizep;
    luaM_growvector(L, f->p, fs->np, f->sizep, Proto *, MAXARG_Bx, "functions");
    while (oldsize < f->sizep)
      f->p[oldsize++] = NULL;
  }
  f->p[fs->np++] = clp = luaF_newproto(L);
  luaC_objbarrier(L, f, clp);
  return clp;
}



static void codeclosure (LexState *ls, expdesc *v) {
  FuncState *fs = ls->fs->prev;
  init_exp(v, VRELOC, luaK_codeABx(fs, OP_CLOSURE, 0, fs->np - 1));
  luaK_exp2nextreg(fs, v);  
}


static void open_func (LexState *ls, FuncState *fs, BlockCnt *bl) {
  Proto *f = fs->f;
  fs->prev = ls->fs;  
  fs->ls = ls;
  ls->fs = fs;
  fs->pc = 0;
  fs->previousline = f->linedefined;
  fs->iwthabs = 0;
  fs->lasttarget = 0;
  fs->freereg = 0;
  fs->nk = 0;
  fs->nabslineinfo = 0;
  fs->np = 0;
  fs->nups = 0;
  fs->ndebugvars = 0;
  fs->nactvar = 0;
  fs->needclose = 0;
  fs->firstlocal = ls->dyd->actvar.n;
  fs->firstlabel = ls->dyd->label.n;
  fs->bl = NULL;
  f->source = ls->source;
  luaC_objbarrier(ls->L, f, f->source);
  f->maxstacksize = 2;  
  enterblock(fs, bl, 0);
}


static void close_func (LexState *ls) {
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  luaK_ret(fs, luaY_nvarstack(fs), 0);  
  leaveblock(fs);
  lua_assert(fs->bl == NULL);
  luaK_finish(fs);
  luaM_shrinkvector(L, f->code, f->sizecode, fs->pc, Instruction);
  luaM_shrinkvector(L, f->lineinfo, f->sizelineinfo, fs->pc, ls_byte);
  luaM_shrinkvector(L, f->abslineinfo, f->sizeabslineinfo,
                       fs->nabslineinfo, AbsLineInfo);
  luaM_shrinkvector(L, f->k, f->sizek, fs->nk, TValue);
  luaM_shrinkvector(L, f->p, f->sizep, fs->np, Proto *);
  luaM_shrinkvector(L, f->locvars, f->sizelocvars, fs->ndebugvars, LocVar);
  luaM_shrinkvector(L, f->upvalues, f->sizeupvalues, fs->nups, Upvaldesc);
  ls->fs = fs->prev;
  luaC_checkGC(L);
}









static int block_follow (LexState *ls, int withuntil) {
  switch (ls->t.token) {
    case TK_ELSE: case TK_ELSEIF:
    case TK_END: case TK_EOS:
      return 1;
    case TK_UNTIL: return withuntil;
    default: return 0;
  }
}


static void statlist (LexState *ls) {
  
  while (!block_follow(ls, 1)) {
    if (ls->t.token == TK_RETURN) {
      statement(ls);
      return;  
    }
    statement(ls);
  }
}


static void fieldsel (LexState *ls, expdesc *v) {
  
  FuncState *fs = ls->fs;
  expdesc key;
  luaK_exp2anyregup(fs, v);
  luaX_next(ls);  
  codename(ls, &key);
  luaK_indexed(fs, v, &key);
}


static void yindex (LexState *ls, expdesc *v) {
  
  luaX_next(ls);  
  expr(ls, v);
  luaK_exp2val(ls->fs, v);
  checknext(ls, ']');
}





typedef struct ConsControl {
  expdesc v;  
  expdesc *t;  
  int nh;  
  int na;  
  int tostore;  
} ConsControl;


static void recfield (LexState *ls, ConsControl *cc) {
  
  FuncState *fs = ls->fs;
  int reg = ls->fs->freereg;
  expdesc tab, key, val;
  if (ls->t.token == TK_NAME) {
    checklimit(fs, cc->nh, MAX_INT, "items in a constructor");
    codename(ls, &key);
  }
  else  
    yindex(ls, &key);
  cc->nh++;
  checknext(ls, '=');
  tab = *cc->t;
  luaK_indexed(fs, &tab, &key);
  expr(ls, &val);
  luaK_storevar(fs, &tab, &val);
  fs->freereg = reg;  
}


static void closelistfield (FuncState *fs, ConsControl *cc) {
  if (cc->v.k == VVOID) return;  
  luaK_exp2nextreg(fs, &cc->v);
  cc->v.k = VVOID;
  if (cc->tostore == LFIELDS_PER_FLUSH) {
    luaK_setlist(fs, cc->t->u.info, cc->na, cc->tostore);  
    cc->na += cc->tostore;
    cc->tostore = 0;  
  }
}


static void lastlistfield (FuncState *fs, ConsControl *cc) {
  if (cc->tostore == 0) return;
  if (hasmultret(cc->v.k)) {
    luaK_setmultret(fs, &cc->v);
    luaK_setlist(fs, cc->t->u.info, cc->na, LUA_MULTRET);
    cc->na--;  
  }
  else {
    if (cc->v.k != VVOID)
      luaK_exp2nextreg(fs, &cc->v);
    luaK_setlist(fs, cc->t->u.info, cc->na, cc->tostore);
  }
  cc->na += cc->tostore;
}


static void listfield (LexState *ls, ConsControl *cc) {
  
  expr(ls, &cc->v);
  cc->tostore++;
}


static void field (LexState *ls, ConsControl *cc) {
  
  switch(ls->t.token) {
    case TK_NAME: {  
      if (luaX_lookahead(ls) != '=')  
        listfield(ls, cc);
      else
        recfield(ls, cc);
      break;
    }
    case '[': {
      recfield(ls, cc);
      break;
    }
    default: {
      listfield(ls, cc);
      break;
    }
  }
}


static void constructor (LexState *ls, expdesc *t) {
  
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  int pc = luaK_codeABC(fs, OP_NEWTABLE, 0, 0, 0);
  ConsControl cc;
  luaK_code(fs, 0);  
  cc.na = cc.nh = cc.tostore = 0;
  cc.t = t;
  init_exp(t, VNONRELOC, fs->freereg);  
  luaK_reserveregs(fs, 1);
  init_exp(&cc.v, VVOID, 0);  
  checknext(ls, '{');
  do {
    lua_assert(cc.v.k == VVOID || cc.tostore > 0);
    if (ls->t.token == '}') break;
    closelistfield(fs, &cc);
    field(ls, &cc);
  } while (testnext(ls, ',') || testnext(ls, ';'));
  check_match(ls, '}', '{', line);
  lastlistfield(fs, &cc);
  luaK_settablesize(fs, pc, t->u.info, cc.na, cc.nh);
}




static void setvararg (FuncState *fs, int nparams) {
  fs->f->flag |= PF_ISVARARG;
  luaK_codeABC(fs, OP_VARARGPREP, nparams, 0, 0);
}


static void parlist (LexState *ls) {
  
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  int nparams = 0;
  int isvararg = 0;
  if (ls->t.token != ')') {  
    do {
      switch (ls->t.token) {
        case TK_NAME: {
          new_localvar(ls, str_checkname(ls));
          nparams++;
          break;
        }
        case TK_DOTS: {
          luaX_next(ls);
          isvararg = 1;
          break;
        }
        default: luaX_syntaxerror(ls, "<name> or '...' expected");
      }
    } while (!isvararg && testnext(ls, ','));
  }
  adjustlocalvars(ls, nparams);
  f->numparams = cast_byte(fs->nactvar);
  if (isvararg)
    setvararg(fs, f->numparams);  
  luaK_reserveregs(fs, fs->nactvar);  
}


static void body (LexState *ls, expdesc *e, int ismethod, int line) {
  
  FuncState new_fs;
  BlockCnt bl;
  new_fs.f = addprototype(ls);
  new_fs.f->linedefined = line;
  open_func(ls, &new_fs, &bl);
  checknext(ls, '(');
  if (ismethod) {
    new_localvarliteral(ls, "self");  
    adjustlocalvars(ls, 1);
  }
  parlist(ls);
  checknext(ls, ')');
  statlist(ls);
  new_fs.f->lastlinedefined = ls->linenumber;
  check_match(ls, TK_END, TK_FUNCTION, line);
  codeclosure(ls, e);
  close_func(ls);
}


static int explist (LexState *ls, expdesc *v) {
  
  int n = 1;  
  expr(ls, v);
  while (testnext(ls, ',')) {
    luaK_exp2nextreg(ls->fs, v);
    expr(ls, v);
    n++;
  }
  return n;
}


static void funcargs (LexState *ls, expdesc *f) {
  FuncState *fs = ls->fs;
  expdesc args;
  int base, nparams;
  int line = ls->linenumber;
  switch (ls->t.token) {
    case '(': {  
      luaX_next(ls);
      if (ls->t.token == ')')  
        args.k = VVOID;
      else {
        explist(ls, &args);
        if (hasmultret(args.k))
          luaK_setmultret(fs, &args);
      }
      check_match(ls, ')', '(', line);
      break;
    }
    case '{': {  
      constructor(ls, &args);
      break;
    }
    case TK_STRING: {  
      codestring(&args, ls->t.seminfo.ts);
      luaX_next(ls);  
      break;
    }
    default: {
      luaX_syntaxerror(ls, "function arguments expected");
    }
  }
  lua_assert(f->k == VNONRELOC);
  base = f->u.info;  
  if (hasmultret(args.k))
    nparams = LUA_MULTRET;  
  else {
    if (args.k != VVOID)
      luaK_exp2nextreg(fs, &args);  
    nparams = fs->freereg - (base+1);
  }
  init_exp(f, VCALL, luaK_codeABC(fs, OP_CALL, base, nparams+1, 2));
  luaK_fixline(fs, line);
  fs->freereg = base+1;  
}







static void primaryexp (LexState *ls, expdesc *v) {
  
  switch (ls->t.token) {
    case '(': {
      int line = ls->linenumber;
      luaX_next(ls);
      expr(ls, v);
      check_match(ls, ')', '(', line);
      luaK_dischargevars(ls->fs, v);
      return;
    }
    case TK_NAME: {
      singlevar(ls, v);
      return;
    }
    default: {
      luaX_syntaxerror(ls, "unexpected symbol");
    }
  }
}


static void suffixedexp (LexState *ls, expdesc *v) {
  
  FuncState *fs = ls->fs;
  primaryexp(ls, v);
  for (;;) {
    switch (ls->t.token) {
      case '.': {  
        fieldsel(ls, v);
        break;
      }
      case '[': {  
        expdesc key;
        luaK_exp2anyregup(fs, v);
        yindex(ls, &key);
        luaK_indexed(fs, v, &key);
        break;
      }
      case ':': {  
        expdesc key;
        luaX_next(ls);
        codename(ls, &key);
        luaK_self(fs, v, &key);
        funcargs(ls, v);
        break;
      }
      case '(': case TK_STRING: case '{': {  
        luaK_exp2nextreg(fs, v);
        funcargs(ls, v);
        break;
      }
      default: return;
    }
  }
}


static void simpleexp (LexState *ls, expdesc *v) {
  
  switch (ls->t.token) {
    case TK_FLT: {
      init_exp(v, VKFLT, 0);
      v->u.nval = ls->t.seminfo.r;
      break;
    }
    case TK_INT: {
      init_exp(v, VKINT, 0);
      v->u.ival = ls->t.seminfo.i;
      break;
    }
    case TK_STRING: {
      codestring(v, ls->t.seminfo.ts);
      break;
    }
    case TK_NIL: {
      init_exp(v, VNIL, 0);
      break;
    }
    case TK_TRUE: {
      init_exp(v, VTRUE, 0);
      break;
    }
    case TK_FALSE: {
      init_exp(v, VFALSE, 0);
      break;
    }
    case TK_DOTS: {  
      FuncState *fs = ls->fs;
      check_condition(ls, fs->f->flag & PF_ISVARARG,
                      "cannot use '...' outside a vararg function");
      init_exp(v, VVARARG, luaK_codeABC(fs, OP_VARARG, 0, 0, 1));
      break;
    }
    case '{': {  
      constructor(ls, v);
      return;
    }
    case TK_FUNCTION: {
      luaX_next(ls);
      body(ls, v, 0, ls->linenumber);
      return;
    }
    default: {
      suffixedexp(ls, v);
      return;
    }
  }
  luaX_next(ls);
}


static UnOpr getunopr (int op) {
  switch (op) {
    case TK_NOT: return OPR_NOT;
    case '-': return OPR_MINUS;
    case '~': return OPR_BNOT;
    case '#': return OPR_LEN;
    default: return OPR_NOUNOPR;
  }
}


static BinOpr getbinopr (int op) {
  switch (op) {
    case '+': return OPR_ADD;
    case '-': return OPR_SUB;
    case '*': return OPR_MUL;
    case '%': return OPR_MOD;
    case '^': return OPR_POW;
    case '/': return OPR_DIV;
    case TK_IDIV: return OPR_IDIV;
    case '&': return OPR_BAND;
    case '|': return OPR_BOR;
    case '~': return OPR_BXOR;
    case TK_SHL: return OPR_SHL;
    case TK_SHR: return OPR_SHR;
    case TK_CONCAT: return OPR_CONCAT;
    case TK_NE: return OPR_NE;
    case TK_EQ: return OPR_EQ;
    case '<': return OPR_LT;
    case TK_LE: return OPR_LE;
    case '>': return OPR_GT;
    case TK_GE: return OPR_GE;
    case TK_AND: return OPR_AND;
    case TK_OR: return OPR_OR;
    default: return OPR_NOBINOPR;
  }
}



static const struct {
  lu_byte left;  
  lu_byte right; 
} priority[] = {  
   {10, 10}, {10, 10},           
   {11, 11}, {11, 11},           
   {14, 13},                  
   {11, 11}, {11, 11},           
   {6, 6}, {4, 4}, {5, 5},   
   {7, 7}, {7, 7},           
   {9, 8},                   
   {3, 3}, {3, 3}, {3, 3},   
   {3, 3}, {3, 3}, {3, 3},   
   {2, 2}, {1, 1}            
};

#define UNARY_PRIORITY	12  



static BinOpr subexpr (LexState *ls, expdesc *v, int limit) {
  BinOpr op;
  UnOpr uop;
  enterlevel(ls);
  uop = getunopr(ls->t.token);
  if (uop != OPR_NOUNOPR) {  
    int line = ls->linenumber;
    luaX_next(ls);  
    subexpr(ls, v, UNARY_PRIORITY);
    luaK_prefix(ls->fs, uop, v, line);
  }
  else simpleexp(ls, v);
  
  op = getbinopr(ls->t.token);
  while (op != OPR_NOBINOPR && priority[op].left > limit) {
    expdesc v2;
    BinOpr nextop;
    int line = ls->linenumber;
    luaX_next(ls);  
    luaK_infix(ls->fs, op, v);
    
    nextop = subexpr(ls, &v2, priority[op].right);
    luaK_posfix(ls->fs, op, v, &v2, line);
    op = nextop;
  }
  leavelevel(ls);
  return op;  
}


static void expr (LexState *ls, expdesc *v) {
  subexpr(ls, v, 0);
}








static void block (LexState *ls) {
  
  FuncState *fs = ls->fs;
  BlockCnt bl;
  enterblock(fs, &bl, 0);
  statlist(ls);
  leaveblock(fs);
}



struct LHS_assign {
  struct LHS_assign *prev;
  expdesc v;  
};



static void check_conflict (LexState *ls, struct LHS_assign *lh, expdesc *v) {
  FuncState *fs = ls->fs;
  int extra = fs->freereg;  
  int conflict = 0;
  for (; lh; lh = lh->prev) {  
    if (vkisindexed(lh->v.k)) {  
      if (lh->v.k == VINDEXUP) {  
        if (v->k == VUPVAL && lh->v.u.ind.t == v->u.info) {
          conflict = 1;  
          lh->v.k = VINDEXSTR;
          lh->v.u.ind.t = extra;  
        }
      }
      else {  
        if (v->k == VLOCAL && lh->v.u.ind.t == v->u.var.ridx) {
          conflict = 1;  
          lh->v.u.ind.t = extra;  
        }
        
        if (lh->v.k == VINDEXED && v->k == VLOCAL &&
            lh->v.u.ind.idx == v->u.var.ridx) {
          conflict = 1;
          lh->v.u.ind.idx = extra;  
        }
      }
    }
  }
  if (conflict) {
    
    if (v->k == VLOCAL)
      luaK_codeABC(fs, OP_MOVE, extra, v->u.var.ridx, 0);
    else
      luaK_codeABC(fs, OP_GETUPVAL, extra, v->u.info, 0);
    luaK_reserveregs(fs, 1);
  }
}


static void restassign (LexState *ls, struct LHS_assign *lh, int nvars) {
  expdesc e;
  check_condition(ls, vkisvar(lh->v.k), "syntax private_lua_error");
  check_readonly(ls, &lh->v);
  if (testnext(ls, ',')) {  
    struct LHS_assign nv;
    nv.prev = lh;
    suffixedexp(ls, &nv.v);
    if (!vkisindexed(nv.v.k))
      check_conflict(ls, lh, &nv.v);
    enterlevel(ls);  
    restassign(ls, &nv, nvars+1);
    leavelevel(ls);
  }
  else {  
    int nexps;
    checknext(ls, '=');
    nexps = explist(ls, &e);
    if (nexps != nvars)
      adjust_assign(ls, nvars, nexps, &e);
    else {
      luaK_setoneret(ls->fs, &e);  
      luaK_storevar(ls->fs, &lh->v, &e);
      return;  
    }
  }
  init_exp(&e, VNONRELOC, ls->fs->freereg-1);  
  luaK_storevar(ls->fs, &lh->v, &e);
}


static int cond (LexState *ls) {
  
  expdesc v;
  expr(ls, &v);  
  if (v.k == VNIL) v.k = VFALSE;  
  luaK_goiftrue(ls->fs, &v);
  return v.f;
}


static void gotostat (LexState *ls) {
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  TString *name = str_checkname(ls);  
  Labeldesc *lb = findlabel(ls, name);
  if (lb == NULL)  
    
    newgotoentry(ls, name, line, luaK_jump(fs));
  else {  
    
    int lblevel = reglevel(fs, lb->nactvar);  
    if (luaY_nvarstack(fs) > lblevel)  
      luaK_codeABC(fs, OP_CLOSE, lblevel, 0, 0);
    
    luaK_patchlist(fs, luaK_jump(fs), lb->pc);
  }
}



static void breakstat (LexState *ls) {
  int line = ls->linenumber;
  luaX_next(ls);  
  newgotoentry(ls, luaS_newliteral(ls->L, "break"), line, luaK_jump(ls->fs));
}



static void checkrepeated (LexState *ls, TString *name) {
  Labeldesc *lb = findlabel(ls, name);
  if (l_unlikely(lb != NULL)) {  
    const char *msg = "label '%s' already defined on line %d";
    msg = luaO_pushfstring(ls->L, msg, getstr(name), lb->line);
    luaK_semerror(ls, msg);  
  }
}


static void labelstat (LexState *ls, TString *name, int line) {
  
  checknext(ls, TK_DBCOLON);  
  while (ls->t.token == ';' || ls->t.token == TK_DBCOLON)
    statement(ls);  
  checkrepeated(ls, name);  
  createlabel(ls, name, line, block_follow(ls, 0));
}


static void whilestat (LexState *ls, int line) {
  
  FuncState *fs = ls->fs;
  int whileinit;
  int condexit;
  BlockCnt bl;
  luaX_next(ls);  
  whileinit = luaK_getlabel(fs);
  condexit = cond(ls);
  enterblock(fs, &bl, 1);
  checknext(ls, TK_DO);
  block(ls);
  luaK_jumpto(fs, whileinit);
  check_match(ls, TK_END, TK_WHILE, line);
  leaveblock(fs);
  luaK_patchtohere(fs, condexit);  
}


static void repeatstat (LexState *ls, int line) {
  
  int condexit;
  FuncState *fs = ls->fs;
  int repeat_init = luaK_getlabel(fs);
  BlockCnt bl1, bl2;
  enterblock(fs, &bl1, 1);  
  enterblock(fs, &bl2, 0);  
  luaX_next(ls);  
  statlist(ls);
  check_match(ls, TK_UNTIL, TK_REPEAT, line);
  condexit = cond(ls);  
  leaveblock(fs);  
  if (bl2.upval) {  
    int exit = luaK_jump(fs);  
    luaK_patchtohere(fs, condexit);  
    luaK_codeABC(fs, OP_CLOSE, reglevel(fs, bl2.nactvar), 0, 0);
    condexit = luaK_jump(fs);  
    luaK_patchtohere(fs, exit);  
  }
  luaK_patchlist(fs, condexit, repeat_init);  
  leaveblock(fs);  
}



static void exp1 (LexState *ls) {
  expdesc e;
  expr(ls, &e);
  luaK_exp2nextreg(ls->fs, &e);
  lua_assert(e.k == VNONRELOC);
}



static void fixforjump (FuncState *fs, int pc, int dest, int back) {
  Instruction *jmp = &fs->f->code[pc];
  int offset = dest - (pc + 1);
  if (back)
    offset = -offset;
  if (l_unlikely(offset > MAXARG_Bx))
    luaX_syntaxerror(fs->ls, "control structure too long");
  SETARG_Bx(*jmp, offset);
}



static void forbody (LexState *ls, int base, int line, int nvars, int isgen) {
  
  static const OpCode forprep[2] = {OP_FORPREP, OP_TFORPREP};
  static const OpCode forloop[2] = {OP_FORLOOP, OP_TFORLOOP};
  BlockCnt bl;
  FuncState *fs = ls->fs;
  int prep, endfor;
  checknext(ls, TK_DO);
  prep = luaK_codeABx(fs, forprep[isgen], base, 0);
  fs->freereg--;  
  enterblock(fs, &bl, 0);  
  adjustlocalvars(ls, nvars);
  luaK_reserveregs(fs, nvars);
  block(ls);
  leaveblock(fs);  
  fixforjump(fs, prep, luaK_getlabel(fs), 0);
  if (isgen) {  
    luaK_codeABC(fs, OP_TFORCALL, base, 0, nvars);
    luaK_fixline(fs, line);
  }
  endfor = luaK_codeABx(fs, forloop[isgen], base, 0);
  fixforjump(fs, endfor, prep + 1, 1);
  luaK_fixline(fs, line);
}


static void fornum (LexState *ls, TString *varname, int line) {
  
  FuncState *fs = ls->fs;
  int base = fs->freereg;
  new_localvarliteral(ls, "(for state)");
  new_localvarliteral(ls, "(for state)");
  new_localvarkind(ls, varname, RDKCONST);  
  checknext(ls, '=');
  exp1(ls);  
  checknext(ls, ',');
  exp1(ls);  
  if (testnext(ls, ','))
    exp1(ls);  
  else {  
    luaK_int(fs, fs->freereg, 1);
    luaK_reserveregs(fs, 1);
  }
  adjustlocalvars(ls, 2);  
  forbody(ls, base, line, 1, 0);
}


static void forlist (LexState *ls, TString *indexname) {
  
  FuncState *fs = ls->fs;
  expdesc e;
  int nvars = 4;  
  int line;
  int base = fs->freereg;
  
  new_localvarliteral(ls, "(for state)");  
  new_localvarliteral(ls, "(for state)");  
  new_localvarliteral(ls, "(for state)");  
  new_localvarkind(ls, indexname, RDKCONST);  
  
  while (testnext(ls, ',')) {
    new_localvar(ls, str_checkname(ls));
    nvars++;
  }
  checknext(ls, TK_IN);
  line = ls->linenumber;
  adjust_assign(ls, 4, explist(ls, &e), &e);
  adjustlocalvars(ls, 3);  
  marktobeclosed(fs);  
  luaK_checkstack(fs, 2);  
  forbody(ls, base, line, nvars - 3, 1);
}


static void forstat (LexState *ls, int line) {
  
  FuncState *fs = ls->fs;
  TString *varname;
  BlockCnt bl;
  enterblock(fs, &bl, 1);  
  luaX_next(ls);  
  varname = str_checkname(ls);  
  switch (ls->t.token) {
    case '=': fornum(ls, varname, line); break;
    case ',': case TK_IN: forlist(ls, varname); break;
    default: luaX_syntaxerror(ls, "'=' or 'in' expected");
  }
  check_match(ls, TK_END, TK_FOR, line);
  leaveblock(fs);  
}


static void test_then_block (LexState *ls, int *escapelist) {
  
  BlockCnt bl;
  FuncState *fs = ls->fs;
  expdesc v;
  int jf;  
  luaX_next(ls);  
  expr(ls, &v);  
  checknext(ls, TK_THEN);
  if (ls->t.token == TK_BREAK) {  
    int line = ls->linenumber;
    luaK_goiffalse(ls->fs, &v);  
    luaX_next(ls);  
    enterblock(fs, &bl, 0);  
    newgotoentry(ls, luaS_newliteral(ls->L, "break"), line, v.t);
    while (testnext(ls, ';')) {}  
    if (block_follow(ls, 0)) {  
      leaveblock(fs);
      return;  
    }
    else  
      jf = luaK_jump(fs);
  }
  else {  
    luaK_goiftrue(ls->fs, &v);  
    enterblock(fs, &bl, 0);
    jf = v.f;
  }
  statlist(ls);  
  leaveblock(fs);
  if (ls->t.token == TK_ELSE ||
      ls->t.token == TK_ELSEIF)  
    luaK_concat(fs, escapelist, luaK_jump(fs));  
  luaK_patchtohere(fs, jf);
}


static void ifstat (LexState *ls, int line) {
  
  FuncState *fs = ls->fs;
  int escapelist = NO_JUMP;  
  test_then_block(ls, &escapelist);  
  while (ls->t.token == TK_ELSEIF)
    test_then_block(ls, &escapelist);  
  if (testnext(ls, TK_ELSE))
    block(ls);  
  check_match(ls, TK_END, TK_IF, line);
  luaK_patchtohere(fs, escapelist);  
}


static void localfunc (LexState *ls) {
  expdesc b;
  FuncState *fs = ls->fs;
  int fvar = fs->nactvar;  
  new_localvar(ls, str_checkname(ls));  
  adjustlocalvars(ls, 1);  
  body(ls, &b, 0, ls->linenumber);  
  
  localdebuginfo(fs, fvar)->startpc = fs->pc;
}


static int getlocalattribute (LexState *ls) {
  
  if (testnext(ls, '<')) {
    TString *ts = str_checkname(ls);
    const char *attr = getstr(ts);
    checknext(ls, '>');
    if (strcmp(attr, "const") == 0)
      return RDKCONST;  
    else if (strcmp(attr, "close") == 0)
      return RDKTOCLOSE;  
    else
      luaK_semerror(ls,
        luaO_pushfstring(ls->L, "unknown attribute '%s'", attr));
  }
  return VDKREG;  
}


static void checktoclose (FuncState *fs, int level) {
  if (level != -1) {  
    marktobeclosed(fs);
    luaK_codeABC(fs, OP_TBC, reglevel(fs, level), 0, 0);
  }
}


static void localstat (LexState *ls) {
  
  FuncState *fs = ls->fs;
  int toclose = -1;  
  Vardesc *var;  
  int vidx;  
  int nvars = 0;
  int nexps;
  expdesc e;
  do {
    TString *vname = str_checkname(ls);
    int kind = getlocalattribute(ls);
    vidx = new_localvarkind(ls, vname, kind);
    if (kind == RDKTOCLOSE) {  
      if (toclose != -1)  
        luaK_semerror(ls, "multiple to-be-closed variables in local list");
      toclose = fs->nactvar + nvars;
    }
    nvars++;
  } while (testnext(ls, ','));
  if (testnext(ls, '='))
    nexps = explist(ls, &e);
  else {
    e.k = VVOID;
    nexps = 0;
  }
  var = getlocalvardesc(fs, vidx);  
  if (nvars == nexps &&  
      var->vd.kind == RDKCONST &&  
      luaK_exp2const(fs, &e, &var->k)) {  
    var->vd.kind = RDKCTC;  
    adjustlocalvars(ls, nvars - 1);  
    fs->nactvar++;  
  }
  else {
    adjust_assign(ls, nvars, nexps, &e);
    adjustlocalvars(ls, nvars);
  }
  checktoclose(fs, toclose);
}


static int funcname (LexState *ls, expdesc *v) {
  
  int ismethod = 0;
  singlevar(ls, v);
  while (ls->t.token == '.')
    fieldsel(ls, v);
  if (ls->t.token == ':') {
    ismethod = 1;
    fieldsel(ls, v);
  }
  return ismethod;
}


static void funcstat (LexState *ls, int line) {
  
  int ismethod;
  expdesc v, b;
  luaX_next(ls);  
  ismethod = funcname(ls, &v);
  body(ls, &b, ismethod, line);
  check_readonly(ls, &v);
  luaK_storevar(ls->fs, &v, &b);
  luaK_fixline(ls->fs, line);  
}


static void exprstat (LexState *ls) {
  
  FuncState *fs = ls->fs;
  struct LHS_assign v;
  suffixedexp(ls, &v.v);
  if (ls->t.token == '=' || ls->t.token == ',') { 
    v.prev = NULL;
    restassign(ls, &v, 1);
  }
  else {  
    Instruction *inst;
    check_condition(ls, v.v.k == VCALL, "syntax private_lua_error");
    inst = &getinstruction(fs, &v.v);
    SETARG_C(*inst, 1);  
  }
}


static void retstat (LexState *ls) {
  
  FuncState *fs = ls->fs;
  expdesc e;
  int nret;  
  int first = luaY_nvarstack(fs);  
  if (block_follow(ls, 1) || ls->t.token == ';')
    nret = 0;  
  else {
    nret = explist(ls, &e);  
    if (hasmultret(e.k)) {
      luaK_setmultret(fs, &e);
      if (e.k == VCALL && nret == 1 && !fs->bl->insidetbc) {  
        SET_OPCODE(getinstruction(fs,&e), OP_TAILCALL);
        lua_assert(GETARG_A(getinstruction(fs,&e)) == luaY_nvarstack(fs));
      }
      nret = LUA_MULTRET;  
    }
    else {
      if (nret == 1)  
        first = luaK_exp2anyreg(fs, &e);  
      else {  
        luaK_exp2nextreg(fs, &e);
        lua_assert(nret == fs->freereg - first);
      }
    }
  }
  luaK_ret(fs, first, nret);
  testnext(ls, ';');  
}


static void statement (LexState *ls) {
  int line = ls->linenumber;  
  enterlevel(ls);
  switch (ls->t.token) {
    case ';': {  
      luaX_next(ls);  
      break;
    }
    case TK_IF: {  
      ifstat(ls, line);
      break;
    }
    case TK_WHILE: {  
      whilestat(ls, line);
      break;
    }
    case TK_DO: {  
      luaX_next(ls);  
      block(ls);
      check_match(ls, TK_END, TK_DO, line);
      break;
    }
    case TK_FOR: {  
      forstat(ls, line);
      break;
    }
    case TK_REPEAT: {  
      repeatstat(ls, line);
      break;
    }
    case TK_FUNCTION: {  
      funcstat(ls, line);
      break;
    }
    case TK_LOCAL: {  
      luaX_next(ls);  
      if (testnext(ls, TK_FUNCTION))  
        localfunc(ls);
      else
        localstat(ls);
      break;
    }
    case TK_DBCOLON: {  
      luaX_next(ls);  
      labelstat(ls, str_checkname(ls), line);
      break;
    }
    case TK_RETURN: {  
      luaX_next(ls);  
      retstat(ls);
      break;
    }
    case TK_BREAK: {  
      breakstat(ls);
      break;
    }
    case TK_GOTO: {  
      luaX_next(ls);  
      gotostat(ls);
      break;
    }
    default: {  
      exprstat(ls);
      break;
    }
  }
  lua_assert(ls->fs->f->maxstacksize >= ls->fs->freereg &&
             ls->fs->freereg >= luaY_nvarstack(ls->fs));
  ls->fs->freereg = luaY_nvarstack(ls->fs);  
  leavelevel(ls);
}





static void mainfunc (LexState *ls, FuncState *fs) {
  BlockCnt bl;
  Upvaldesc *env;
  open_func(ls, fs, &bl);
  setvararg(fs, 0);  
  env = allocupvalue(fs);  
  env->instack = 1;
  env->idx = 0;
  env->kind = VDKREG;
  env->name = ls->envn;
  luaC_objbarrier(ls->L, fs->f, env->name);
  luaX_next(ls);  
  statlist(ls);  
  check(ls, TK_EOS);
  close_func(ls);
}


LClosure *luaY_parser (lua_State *L, ZIO *z, Mbuffer *buff,
                       Dyndata *dyd, const char *name, int firstchar) {
  LexState lexstate;
  FuncState funcstate;
  LClosure *cl = luaF_newLclosure(L, 1);  
  setclLvalue2s(L, L->top.p, cl);  
  luaD_inctop(L);
  lexstate.h = luaH_new(L);  
  sethvalue2s(L, L->top.p, lexstate.h);  
  luaD_inctop(L);
  funcstate.f = cl->p = luaF_newproto(L);
  luaC_objbarrier(L, cl, cl->p);
  funcstate.f->source = luaS_new(L, name);  
  luaC_objbarrier(L, funcstate.f, funcstate.f->source);
  lexstate.buff = buff;
  lexstate.dyd = dyd;
  dyd->actvar.n = dyd->gt.n = dyd->label.n = 0;
  luaX_setinput(L, &lexstate, z, funcstate.f->source, firstchar);
  mainfunc(&lexstate, &funcstate);
  lua_assert(!funcstate.prev && funcstate.nups == 1 && !lexstate.fs);
  
  lua_assert(dyd->actvar.n == 0 && dyd->gt.n == 0 && dyd->label.n == 0);
  L->top.p--;  
  return cl;  
}



//path: src/imports/../dependencies/lua/ldebug.c


#define ldebug_c
#define LUA_CORE





#include <stdarg.h>
#include <stddef.h>
#include <string.h>































#define noLuaClosure(f)		((f) == NULL || (f)->c.tt == LUA_VCCL)


static const char *funcnamefromcall (lua_State *L, CallInfo *ci,
                                                   const char **name);


static int currentpc (CallInfo *ci) {
  lua_assert(isLua(ci));
  return pcRel(ci->u.l.savedpc, ci_func(ci)->p);
}



static int getbaseline (const Proto *f, int pc, int *basepc) {
  if (f->sizeabslineinfo == 0 || pc < f->abslineinfo[0].pc) {
    *basepc = -1;  
    return f->linedefined;
  }
  else {
    int i = cast_uint(pc) / MAXIWTHABS - 1;  
    
    lua_assert(i < 0 ||
              (i < f->sizeabslineinfo && f->abslineinfo[i].pc <= pc));
    while (i + 1 < f->sizeabslineinfo && pc >= f->abslineinfo[i + 1].pc)
      i++;  
    *basepc = f->abslineinfo[i].pc;
    return f->abslineinfo[i].line;
  }
}



int luaG_getfuncline (const Proto *f, int pc) {
  if (f->lineinfo == NULL)  
    return -1;
  else {
    int basepc;
    int baseline = getbaseline(f, pc, &basepc);
    while (basepc++ < pc) {  
      lua_assert(f->lineinfo[basepc] != ABSLINEINFO);
      baseline += f->lineinfo[basepc];  
    }
    return baseline;
  }
}


static int getcurrentline (CallInfo *ci) {
  return luaG_getfuncline(ci_func(ci)->p, currentpc(ci));
}



static void settraps (CallInfo *ci) {
  for (; ci != NULL; ci = ci->previous)
    if (isLua(ci))
      ci->u.l.trap = 1;
}



LUA_API void lua_sethook (lua_State *L, lua_Hook func, int mask, int count) {
  if (func == NULL || mask == 0) {  
    mask = 0;
    func = NULL;
  }
  L->hook = func;
  L->basehookcount = count;
  resethookcount(L);
  L->hookmask = cast_byte(mask);
  if (mask)
    settraps(L->ci);  
}


LUA_API lua_Hook lua_gethook (lua_State *L) {
  return L->hook;
}


LUA_API int lua_gethookmask (lua_State *L) {
  return L->hookmask;
}


LUA_API int lua_gethookcount (lua_State *L) {
  return L->basehookcount;
}


LUA_API int lua_getstack (lua_State *L, int level, lua_Debug *ar) {
  int status;
  CallInfo *ci;
  if (level < 0) return 0;  
  lua_lock(L);
  for (ci = L->ci; level > 0 && ci != &L->base_ci; ci = ci->previous)
    level--;
  if (level == 0 && ci != &L->base_ci) {  
    status = 1;
    ar->i_ci = ci;
  }
  else status = 0;  
  lua_unlock(L);
  return status;
}


static const char *upvalname (const Proto *p, int uv) {
  TString *s = check_exp(uv < p->sizeupvalues, p->upvalues[uv].name);
  if (s == NULL) return "?";
  else return getstr(s);
}


static const char *findvararg (CallInfo *ci, int n, StkId *pos) {
  if (clLvalue(s2v(ci->func.p))->p->flag & PF_ISVARARG) {
    int nextra = ci->u.l.nextraargs;
    if (n >= -nextra) {  
      *pos = ci->func.p - nextra - (n + 1);
      return "(vararg)";  
    }
  }
  return NULL;  
}


const char *luaG_findlocal (lua_State *L, CallInfo *ci, int n, StkId *pos) {
  StkId base = ci->func.p + 1;
  const char *name = NULL;
  if (isLua(ci)) {
    if (n < 0)  
      return findvararg(ci, n, pos);
    else
      name = luaF_getlocalname(ci_func(ci)->p, n, currentpc(ci));
  }
  if (name == NULL) {  
    StkId limit = (ci == L->ci) ? L->top.p : ci->next->func.p;
    if (limit - base >= n && n > 0) {  
      
      name = isLua(ci) ? "(temporary)" : "(C temporary)";
    }
    else
      return NULL;  
  }
  if (pos)
    *pos = base + (n - 1);
  return name;
}


LUA_API const char *lua_getlocal (lua_State *L, const lua_Debug *ar, int n) {
  const char *name;
  lua_lock(L);
  if (ar == NULL) {  
    if (!isLfunction(s2v(L->top.p - 1)))  
      name = NULL;
    else  
      name = luaF_getlocalname(clLvalue(s2v(L->top.p - 1))->p, n, 0);
  }
  else {  
    StkId pos = NULL;  
    name = luaG_findlocal(L, ar->i_ci, n, &pos);
    if (name) {
      setobjs2s(L, L->top.p, pos);
      api_incr_top(L);
    }
  }
  lua_unlock(L);
  return name;
}


LUA_API const char *lua_setlocal (lua_State *L, const lua_Debug *ar, int n) {
  StkId pos = NULL;  
  const char *name;
  lua_lock(L);
  name = luaG_findlocal(L, ar->i_ci, n, &pos);
  if (name) {
    api_checkpop(L, 1);
    setobjs2s(L, pos, L->top.p - 1);
    L->top.p--;  
  }
  lua_unlock(L);
  return name;
}


static void funcinfo (lua_Debug *ar, Closure *cl) {
  if (noLuaClosure(cl)) {
    ar->source = "=[C]";
    ar->srclen = LL("=[C]");
    ar->linedefined = -1;
    ar->lastlinedefined = -1;
    ar->what = "C";
  }
  else {
    const Proto *p = cl->l.p;
    if (p->source) {
      ar->source = getlstr(p->source, ar->srclen);
    }
    else {
      ar->source = "=?";
      ar->srclen = LL("=?");
    }
    ar->linedefined = p->linedefined;
    ar->lastlinedefined = p->lastlinedefined;
    ar->what = (ar->linedefined == 0) ? "main" : "Lua";
  }
  luaO_chunkid(ar->short_src, ar->source, ar->srclen);
}


static int nextline (const Proto *p, int currentline, int pc) {
  if (p->lineinfo[pc] != ABSLINEINFO)
    return currentline + p->lineinfo[pc];
  else
    return luaG_getfuncline(p, pc);
}


static void collectvalidlines (lua_State *L, Closure *f) {
  if (noLuaClosure(f)) {
    setnilvalue(s2v(L->top.p));
    api_incr_top(L);
  }
  else {
    int i;
    TValue v;
    const Proto *p = f->l.p;
    int currentline = p->linedefined;
    Table *t = luaH_new(L);  
    sethvalue2s(L, L->top.p, t);  
    api_incr_top(L);
    setbtvalue(&v);  
    if (!(p->flag & PF_ISVARARG))  
      i = 0;  
    else {  
      lua_assert(GET_OPCODE(p->code[0]) == OP_VARARGPREP);
      currentline = nextline(p, currentline, 0);
      i = 1;  
    }
    for (; i < p->sizelineinfo; i++) {  
      currentline = nextline(p, currentline, i);  
      luaH_setint(L, t, currentline, &v);  
    }
  }
}


static const char *getfuncname (lua_State *L, CallInfo *ci, const char **name) {
  
  if (ci != NULL && !(ci->callstatus & CIST_TAIL))
    return funcnamefromcall(L, ci->previous, name);
  else return NULL;  
}


static int auxgetinfo (lua_State *L, const char *what, lua_Debug *ar,
                       Closure *f, CallInfo *ci) {
  int status = 1;
  for (; *what; what++) {
    switch (*what) {
      case 'S': {
        funcinfo(ar, f);
        break;
      }
      case 'l': {
        ar->currentline = (ci && isLua(ci)) ? getcurrentline(ci) : -1;
        break;
      }
      case 'u': {
        ar->nups = (f == NULL) ? 0 : f->c.nupvalues;
        if (noLuaClosure(f)) {
          ar->isvararg = 1;
          ar->nparams = 0;
        }
        else {
          ar->isvararg = f->l.p->flag & PF_ISVARARG;
          ar->nparams = f->l.p->numparams;
        }
        break;
      }
      case 't': {
        ar->istailcall = (ci) ? ci->callstatus & CIST_TAIL : 0;
        break;
      }
      case 'n': {
        ar->namewhat = getfuncname(L, ci, &ar->name);
        if (ar->namewhat == NULL) {
          ar->namewhat = "";  
          ar->name = NULL;
        }
        break;
      }
      case 'r': {
        if (ci == NULL || !(ci->callstatus & CIST_TRAN))
          ar->ftransfer = ar->ntransfer = 0;
        else {
          ar->ftransfer = ci->u2.transferinfo.ftransfer;
          ar->ntransfer = ci->u2.transferinfo.ntransfer;
        }
        break;
      }
      case 'L':
      case 'f':  
        break;
      default: status = 0;  
    }
  }
  return status;
}


LUA_API int lua_getinfo (lua_State *L, const char *what, lua_Debug *ar) {
  int status;
  Closure *cl;
  CallInfo *ci;
  TValue *func;
  lua_lock(L);
  if (*what == '>') {
    ci = NULL;
    func = s2v(L->top.p - 1);
    api_check(L, ttisfunction(func), "function expected");
    what++;  
    L->top.p--;  
  }
  else {
    ci = ar->i_ci;
    func = s2v(ci->func.p);
    lua_assert(ttisfunction(func));
  }
  cl = ttisclosure(func) ? clvalue(func) : NULL;
  status = auxgetinfo(L, what, ar, cl, ci);
  if (strchr(what, 'f')) {
    setobj2s(L, L->top.p, func);
    api_incr_top(L);
  }
  if (strchr(what, 'L'))
    collectvalidlines(L, cl);
  lua_unlock(L);
  return status;
}





static int filterpc (int pc, int jmptarget) {
  if (pc < jmptarget)  
    return -1;  
  else return pc;  
}



static int findsetreg (const Proto *p, int lastpc, int reg) {
  int pc;
  int setreg = -1;  
  int jmptarget = 0;  
  if (testMMMode(GET_OPCODE(p->code[lastpc])))
    lastpc--;  
  for (pc = 0; pc < lastpc; pc++) {
    Instruction i = p->code[pc];
    OpCode op = GET_OPCODE(i);
    int a = GETARG_A(i);
    int change;  
    switch (op) {
      case OP_LOADNIL: {  
        int b = GETARG_B(i);
        change = (a <= reg && reg <= a + b);
        break;
      }
      case OP_TFORCALL: {  
        change = (reg >= a + 2);
        break;
      }
      case OP_CALL:
      case OP_TAILCALL: {  
        change = (reg >= a);
        break;
      }
      case OP_JMP: {  
        int b = GETARG_sJ(i);
        int dest = pc + 1 + b;
        
        if (dest <= lastpc && dest > jmptarget)
          jmptarget = dest;  
        change = 0;
        break;
      }
      default:  
        change = (testAMode(op) && reg == a);
        break;
    }
    if (change)
      setreg = filterpc(pc, jmptarget);
  }
  return setreg;
}



static const char *kname (const Proto *p, int index, const char **name) {
  TValue *kvalue = &p->k[index];
  if (ttisstring(kvalue)) {
    *name = getstr(tsvalue(kvalue));
    return "constant";
  }
  else {
    *name = "?";
    return NULL;
  }
}


static const char *basicgetobjname (const Proto *p, int *ppc, int reg,
                                    const char **name) {
  int pc = *ppc;
  *name = luaF_getlocalname(p, reg + 1, pc);
  if (*name)  
    return "local";
  
  *ppc = pc = findsetreg(p, pc, reg);
  if (pc != -1) {  
    Instruction i = p->code[pc];
    OpCode op = GET_OPCODE(i);
    switch (op) {
      case OP_MOVE: {
        int b = GETARG_B(i);  
        if (b < GETARG_A(i))
          return basicgetobjname(p, ppc, b, name);  
        break;
      }
      case OP_GETUPVAL: {
        *name = upvalname(p, GETARG_B(i));
        return "upvalue";
      }
      case OP_LOADK: return kname(p, GETARG_Bx(i), name);
      case OP_LOADKX: return kname(p, GETARG_Ax(p->code[pc + 1]), name);
      default: break;
    }
  }
  return NULL;  
}



static void rname (const Proto *p, int pc, int c, const char **name) {
  const char *what = basicgetobjname(p, &pc, c, name); 
  if (!(what && *what == 'c'))  
    *name = "?";
}



static void rkname (const Proto *p, int pc, Instruction i, const char **name) {
  int c = GETARG_C(i);  
  if (GETARG_k(i))  
    kname(p, c, name);
  else  
    rname(p, pc, c, name);
}



static const char *isEnv (const Proto *p, int pc, Instruction i, int isup) {
  int t = GETARG_B(i);  
  const char *name;  
  if (isup)  
    name = upvalname(p, t);
  else  
    basicgetobjname(p, &pc, t, &name);
  return (name && strcmp(name, LUA_ENV) == 0) ? "global" : "field";
}



static const char *getobjname (const Proto *p, int lastpc, int reg,
                               const char **name) {
  const char *kind = basicgetobjname(p, &lastpc, reg, name);
  if (kind != NULL)
    return kind;
  else if (lastpc != -1) {  
    Instruction i = p->code[lastpc];
    OpCode op = GET_OPCODE(i);
    switch (op) {
      case OP_GETTABUP: {
        int k = GETARG_C(i);  
        kname(p, k, name);
        return isEnv(p, lastpc, i, 1);
      }
      case OP_GETTABLE: {
        int k = GETARG_C(i);  
        rname(p, lastpc, k, name);
        return isEnv(p, lastpc, i, 0);
      }
      case OP_GETI: {
        *name = "integer index";
        return "field";
      }
      case OP_GETFIELD: {
        int k = GETARG_C(i);  
        kname(p, k, name);
        return isEnv(p, lastpc, i, 0);
      }
      case OP_SELF: {
        rkname(p, lastpc, i, name);
        return "method";
      }
      default: break;  
    }
  }
  return NULL;  
}



static const char *funcnamefromcode (lua_State *L, const Proto *p,
                                     int pc, const char **name) {
  TMS tm = (TMS)0;  
  Instruction i = p->code[pc];  
  switch (GET_OPCODE(i)) {
    case OP_CALL:
    case OP_TAILCALL:
      return getobjname(p, pc, GETARG_A(i), name);  
    case OP_TFORCALL: {  
      *name = "for iterator";
       return "for iterator";
    }
    
    case OP_SELF: case OP_GETTABUP: case OP_GETTABLE:
    case OP_GETI: case OP_GETFIELD:
      tm = TM_INDEX;
      break;
    case OP_SETTABUP: case OP_SETTABLE: case OP_SETI: case OP_SETFIELD:
      tm = TM_NEWINDEX;
      break;
    case OP_MMBIN: case OP_MMBINI: case OP_MMBINK: {
      tm = cast(TMS, GETARG_C(i));
      break;
    }
    case OP_UNM: tm = TM_UNM; break;
    case OP_BNOT: tm = TM_BNOT; break;
    case OP_LEN: tm = TM_LEN; break;
    case OP_CONCAT: tm = TM_CONCAT; break;
    case OP_EQ: tm = TM_EQ; break;
    
    case OP_LT: case OP_LTI: case OP_GTI: tm = TM_LT; break;
    case OP_LE: case OP_LEI: case OP_GEI: tm = TM_LE; break;
    case OP_CLOSE: case OP_RETURN: tm = TM_CLOSE; break;
    default:
      return NULL;  
  }
  *name = getshrstr(G(L)->tmname[tm]) + 2;
  return "metamethod";
}



static const char *funcnamefromcall (lua_State *L, CallInfo *ci,
                                                   const char **name) {
  if (ci->callstatus & CIST_HOOKED) {  
    *name = "?";
    return "hook";
  }
  else if (ci->callstatus & CIST_FIN) {  
    *name = "__gc";
    return "metamethod";  
  }
  else if (isLua(ci))
    return funcnamefromcode(L, ci_func(ci)->p, currentpc(ci), name);
  else
    return NULL;
}






static int instack (CallInfo *ci, const TValue *o) {
  int pos;
  StkId base = ci->func.p + 1;
  for (pos = 0; base + pos < ci->top.p; pos++) {
    if (o == s2v(base + pos))
      return pos;
  }
  return -1;  
}



static const char *getupvalname (CallInfo *ci, const TValue *o,
                                 const char **name) {
  LClosure *c = ci_func(ci);
  int i;
  for (i = 0; i < c->nupvalues; i++) {
    if (c->upvals[i]->v.p == o) {
      *name = upvalname(c->p, i);
      return "upvalue";
    }
  }
  return NULL;
}


static const char *formatvarinfo (lua_State *L, const char *kind,
                                                const char *name) {
  if (kind == NULL)
    return "";  
  else
    return luaO_pushfstring(L, " (%s '%s')", kind, name);
}


static const char *varinfo (lua_State *L, const TValue *o) {
  CallInfo *ci = L->ci;
  const char *name = NULL;  
  const char *kind = NULL;
  if (isLua(ci)) {
    kind = getupvalname(ci, o, &name);  
    if (!kind) {  
      int reg = instack(ci, o);  
      if (reg >= 0)  
        kind = getobjname(ci_func(ci)->p, currentpc(ci), reg, &name);
    }
  }
  return formatvarinfo(L, kind, name);
}



static l_noret typeerror (lua_State *L, const TValue *o, const char *op,
                          const char *extra) {
  const char *t = luaT_objtypename(L, o);
  luaG_runerror(L, "attempt to %s a %s value%s", op, t, extra);
}



l_noret luaG_typeerror (lua_State *L, const TValue *o, const char *op) {
  typeerror(L, o, op, varinfo(L, o));
}



l_noret luaG_callerror (lua_State *L, const TValue *o) {
  CallInfo *ci = L->ci;
  const char *name = NULL;  
  const char *kind = funcnamefromcall(L, ci, &name);
  const char *extra = kind ? formatvarinfo(L, kind, name) : varinfo(L, o);
  typeerror(L, o, "call", extra);
}


l_noret luaG_forerror (lua_State *L, const TValue *o, const char *what) {
  luaG_runerror(L, "bad 'for' %s (number expected, got %s)",
                   what, luaT_objtypename(L, o));
}


l_noret luaG_concaterror (lua_State *L, const TValue *p1, const TValue *p2) {
  if (ttisstring(p1) || cvt2str(p1)) p1 = p2;
  luaG_typeerror(L, p1, "concatenate");
}


l_noret luaG_opinterror (lua_State *L, const TValue *p1,
                         const TValue *p2, const char *msg) {
  if (!ttisnumber(p1))  
    p2 = p1;  
  luaG_typeerror(L, p2, msg);
}



l_noret luaG_tointerror (lua_State *L, const TValue *p1, const TValue *p2) {
  lua_Integer temp;
  if (!luaV_tointegerns(p1, &temp, LUA_FLOORN2I))
    p2 = p1;
  luaG_runerror(L, "number%s has no integer representation", varinfo(L, p2));
}


l_noret luaG_ordererror (lua_State *L, const TValue *p1, const TValue *p2) {
  const char *t1 = luaT_objtypename(L, p1);
  const char *t2 = luaT_objtypename(L, p2);
  if (strcmp(t1, t2) == 0)
    luaG_runerror(L, "attempt to compare two %s values", t1);
  else
    luaG_runerror(L, "attempt to compare %s with %s", t1, t2);
}



const char *luaG_addinfo (lua_State *L, const char *msg, TString *src,
                                        int line) {
  char buff[LUA_IDSIZE];
  if (src) {
    size_t idlen;
    const char *id = getlstr(src, idlen);
    luaO_chunkid(buff, id, idlen);
  }
  else {  
    buff[0] = '?'; buff[1] = '\0';
  }
  return luaO_pushfstring(L, "%s:%d: %s", buff, line, msg);
}


l_noret luaG_errormsg (lua_State *L) {
  if (L->errfunc != 0) {  
    StkId errfunc = restorestack(L, L->errfunc);
    lua_assert(ttisfunction(s2v(errfunc)));
    setobjs2s(L, L->top.p, L->top.p - 1);  
    setobjs2s(L, L->top.p - 1, errfunc);  
    L->top.p++;  
    luaD_callnoyield(L, L->top.p - 2, 1);  
  }
  luaD_throw(L, LUA_ERRRUN);
}


l_noret luaG_runerror (lua_State *L, const char *fmt, ...) {
  CallInfo *ci = L->ci;
  const char *msg;
  va_list argp;
  luaC_checkGC(L);  
  va_start(argp, fmt);
  msg = luaO_pushvfstring(L, fmt, argp);  
  va_end(argp);
  if (isLua(ci)) {  
    luaG_addinfo(L, msg, ci_func(ci)->p->source, getcurrentline(ci));
    setobjs2s(L, L->top.p - 2, L->top.p - 1);  
    L->top.p--;
  }
  luaG_errormsg(L);
}



static int changedline (const Proto *p, int oldpc, int newpc) {
  if (p->lineinfo == NULL)  
    return 0;
  if (newpc - oldpc < MAXIWTHABS / 2) {  
    int delta = 0;  
    int pc = oldpc;
    for (;;) {
      int lineinfo = p->lineinfo[++pc];
      if (lineinfo == ABSLINEINFO)
        break;  
      delta += lineinfo;
      if (pc == newpc)
        return (delta != 0);  
    }
  }
  
  return (luaG_getfuncline(p, oldpc) != luaG_getfuncline(p, newpc));
}



int luaG_tracecall (lua_State *L) {
  CallInfo *ci = L->ci;
  Proto *p = ci_func(ci)->p;
  ci->u.l.trap = 1;  
  if (ci->u.l.savedpc == p->code) {  
    if (p->flag & PF_ISVARARG)
      return 0;  
    else if (!(ci->callstatus & CIST_HOOKYIELD))  
      luaD_hookcall(L, ci);  
  }
  return 1;  
}



int luaG_traceexec (lua_State *L, const Instruction *pc) {
  CallInfo *ci = L->ci;
  lu_byte mask = L->hookmask;
  const Proto *p = ci_func(ci)->p;
  int counthook;
  if (!(mask & (LUA_MASKLINE | LUA_MASKCOUNT))) {  
    ci->u.l.trap = 0;  
    return 0;  
  }
  pc++;  
  ci->u.l.savedpc = pc;  
  counthook = (mask & LUA_MASKCOUNT) && (--L->hookcount == 0);
  if (counthook)
    resethookcount(L);  
  else if (!(mask & LUA_MASKLINE))
    return 1;  
  if (ci->callstatus & CIST_HOOKYIELD) {  
    ci->callstatus &= ~CIST_HOOKYIELD;  
    return 1;  
  }
  if (!isIT(*(ci->u.l.savedpc - 1)))  
    L->top.p = ci->top.p;  
  if (counthook)
    luaD_hook(L, LUA_HOOKCOUNT, -1, 0, 0);  
  if (mask & LUA_MASKLINE) {
    
    int oldpc = (L->oldpc < p->sizecode) ? L->oldpc : 0;
    int npci = pcRel(pc, p);
    if (npci <= oldpc ||  
        changedline(p, oldpc, npci)) {  
      int newline = luaG_getfuncline(p, npci);
      luaD_hook(L, LUA_HOOKLINE, newline, 0, 0);  
    }
    L->oldpc = npci;  
  }
  if (L->status == LUA_YIELD) {  
    if (counthook)
      L->hookcount = 1;  
    ci->callstatus |= CIST_HOOKYIELD;  
    luaD_throw(L, LUA_YIELD);
  }
  return 1;  
}



//path: src/imports/../dependencies/lua/lfunc.c


#define lfunc_c
#define LUA_CORE





#include <stddef.h>





















CClosure *luaF_newCclosure (lua_State *L, int nupvals) {
  GCObject *o = luaC_newobj(L, LUA_VCCL, sizeCclosure(nupvals));
  CClosure *c = gco2ccl(o);
  c->nupvalues = cast_byte(nupvals);
  return c;
}


LClosure *luaF_newLclosure (lua_State *L, int nupvals) {
  GCObject *o = luaC_newobj(L, LUA_VLCL, sizeLclosure(nupvals));
  LClosure *c = gco2lcl(o);
  c->p = NULL;
  c->nupvalues = cast_byte(nupvals);
  while (nupvals--) c->upvals[nupvals] = NULL;
  return c;
}



void luaF_initupvals (lua_State *L, LClosure *cl) {
  int i;
  for (i = 0; i < cl->nupvalues; i++) {
    GCObject *o = luaC_newobj(L, LUA_VUPVAL, sizeof(UpVal));
    UpVal *uv = gco2upv(o);
    uv->v.p = &uv->u.value;  
    setnilvalue(uv->v.p);
    cl->upvals[i] = uv;
    luaC_objbarrier(L, cl, uv);
  }
}



static UpVal *newupval (lua_State *L, StkId level, UpVal **prev) {
  GCObject *o = luaC_newobj(L, LUA_VUPVAL, sizeof(UpVal));
  UpVal *uv = gco2upv(o);
  UpVal *next = *prev;
  uv->v.p = s2v(level);  
  uv->u.open.next = next;  
  uv->u.open.previous = prev;
  if (next)
    next->u.open.previous = &uv->u.open.next;
  *prev = uv;
  if (!isintwups(L)) {  
    L->twups = G(L)->twups;  
    G(L)->twups = L;
  }
  return uv;
}



UpVal *luaF_findupval (lua_State *L, StkId level) {
  UpVal **pp = &L->openupval;
  UpVal *p;
  lua_assert(isintwups(L) || L->openupval == NULL);
  while ((p = *pp) != NULL && uplevel(p) >= level) {  
    lua_assert(!isdead(G(L), p));
    if (uplevel(p) == level)  
      return p;  
    pp = &p->u.open.next;
  }
  
  return newupval(L, level, pp);
}



static void callclosemethod (lua_State *L, TValue *obj, TValue *err, int yy) {
  StkId top = L->top.p;
  const TValue *tm = luaT_gettmbyobj(L, obj, TM_CLOSE);
  setobj2s(L, top, tm);  
  setobj2s(L, top + 1, obj);  
  setobj2s(L, top + 2, err);  
  L->top.p = top + 3;  
  if (yy)
    luaD_call(L, top, 0);
  else
    luaD_callnoyield(L, top, 0);
}



static void checkclosemth (lua_State *L, StkId level) {
  const TValue *tm = luaT_gettmbyobj(L, s2v(level), TM_CLOSE);
  if (ttisnil(tm)) {  
    int idx = cast_int(level - L->ci->func.p);  
    const char *vname = luaG_findlocal(L, L->ci, idx, NULL);
    if (vname == NULL) vname = "?";
    luaG_runerror(L, "variable '%s' got a non-closable value", vname);
  }
}



static void prepcallclosemth (lua_State *L, StkId level, int status, int yy) {
  TValue *uv = s2v(level);  
  TValue *errobj;
  if (status == CLOSEKTOP)
    errobj = &G(L)->nilvalue;  
  else {  
    errobj = s2v(level + 1);  
    luaD_seterrorobj(L, status, level + 1);  
  }
  callclosemethod(L, uv, errobj, yy);
}



#define MAXDELTA  \
	((256ul << ((sizeof(L->stack.p->tbclist.delta) - 1) * 8)) - 1)



void luaF_newtbcupval (lua_State *L, StkId level) {
  lua_assert(level > L->tbclist.p);
  if (l_isfalse(s2v(level)))
    return;  
  checkclosemth(L, level);  
  while (cast_uint(level - L->tbclist.p) > MAXDELTA) {
    L->tbclist.p += MAXDELTA;  
    L->tbclist.p->tbclist.delta = 0;
  }
  level->tbclist.delta = cast(unsigned short, level - L->tbclist.p);
  L->tbclist.p = level;
}


void luaF_unlinkupval (UpVal *uv) {
  lua_assert(upisopen(uv));
  *uv->u.open.previous = uv->u.open.next;
  if (uv->u.open.next)
    uv->u.open.next->u.open.previous = uv->u.open.previous;
}



void luaF_closeupval (lua_State *L, StkId level) {
  UpVal *uv;
  StkId upl;  
  while ((uv = L->openupval) != NULL && (upl = uplevel(uv)) >= level) {
    TValue *slot = &uv->u.value;  
    lua_assert(uplevel(uv) < L->top.p);
    luaF_unlinkupval(uv);  
    setobj(L, slot, uv->v.p);  
    uv->v.p = slot;  
    if (!iswhite(uv)) {  
      nw2black(uv);  
      luaC_barrier(L, uv, slot);
    }
  }
}



static void poptbclist (lua_State *L) {
  StkId tbc = L->tbclist.p;
  lua_assert(tbc->tbclist.delta > 0);  
  tbc -= tbc->tbclist.delta;
  while (tbc > L->stack.p && tbc->tbclist.delta == 0)
    tbc -= MAXDELTA;  
  L->tbclist.p = tbc;
}



StkId luaF_close (lua_State *L, StkId level, int status, int yy) {
  ptrdiff_t levelrel = savestack(L, level);
  luaF_closeupval(L, level);  
  while (L->tbclist.p >= level) {  
    StkId tbc = L->tbclist.p;  
    poptbclist(L);  
    prepcallclosemth(L, tbc, status, yy);  
    level = restorestack(L, levelrel);
  }
  return level;
}


Proto *luaF_newproto (lua_State *L) {
  GCObject *o = luaC_newobj(L, LUA_VPROTO, sizeof(Proto));
  Proto *f = gco2p(o);
  f->k = NULL;
  f->sizek = 0;
  f->p = NULL;
  f->sizep = 0;
  f->code = NULL;
  f->sizecode = 0;
  f->lineinfo = NULL;
  f->sizelineinfo = 0;
  f->abslineinfo = NULL;
  f->sizeabslineinfo = 0;
  f->upvalues = NULL;
  f->sizeupvalues = 0;
  f->numparams = 0;
  f->flag = 0;
  f->maxstacksize = 0;
  f->locvars = NULL;
  f->sizelocvars = 0;
  f->linedefined = 0;
  f->lastlinedefined = 0;
  f->source = NULL;
  return f;
}


void luaF_freeproto (lua_State *L, Proto *f) {
  if (!(f->flag & PF_FIXED)) {
    luaM_freearray(L, f->code, f->sizecode);
    luaM_freearray(L, f->lineinfo, f->sizelineinfo);
    luaM_freearray(L, f->abslineinfo, f->sizeabslineinfo);
  }
  luaM_freearray(L, f->p, f->sizep);
  luaM_freearray(L, f->k, f->sizek);
  luaM_freearray(L, f->locvars, f->sizelocvars);
  luaM_freearray(L, f->upvalues, f->sizeupvalues);
  luaM_free(L, f);
}



const char *luaF_getlocalname (const Proto *f, int local_number, int pc) {
  int i;
  for (i = 0; i<f->sizelocvars && f->locvars[i].startpc <= pc; i++) {
    if (pc < f->locvars[i].endpc) {  
      local_number--;
      if (local_number == 0)
        return getstr(f->locvars[i].varname);
    }
  }
  return NULL;  
}



//path: src/imports/../dependencies/lua/lobject.c


#define lobject_c
#define LUA_CORE





#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>























int luaO_ceillog2 (unsigned int x) {
  static const lu_byte log_2[256] = {  
    0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
  };
  int l = 0;
  x--;
  while (x >= 256) { l += 8; x >>= 8; }
  return l + log_2[x];
}


unsigned int luaO_codeparam (unsigned int p) {
  if (p >= (cast(lu_mem, 0x1F) << (0xF - 7 - 1)) * 100u)  
    return 0xFF;  
  else {
    p = (cast(l_uint32, p) * 128 + 99) / 100;  
    if (p < 0x10)  
      return p;  
    else {
      int log = luaO_ceillog2(p + 1) - 5;  
      return ((p >> log) - 0x10) | ((log + 1) << 4);
    }
  }
}



l_obj luaO_applyparam (unsigned int p, l_obj x) {
  unsigned int m = p & 0xF;  
  int e = (p >> 4);  
  if (e > 0) {  
    e--;  
    m += 0x10;  
  }
  e -= 7;  
  if (e >= 0) {
    if (x < (MAX_LOBJ / 0x1F) >> e)  
      return (x * m) << e;  
    else  
      return MAX_LOBJ;
  }
  else {  
    e = -e;
    if (x < MAX_LOBJ / 0x1F)  
      return (x * m) >> e;  
    else if ((x >> e) <  MAX_LOBJ / 0x1F)  
      return (x >> e) * m;
    else  
      return MAX_LOBJ;
  }
}


static lua_Integer intarith (lua_State *L, int op, lua_Integer v1,
                                                   lua_Integer v2) {
  switch (op) {
    case LUA_OPADD: return intop(+, v1, v2);
    case LUA_OPSUB:return intop(-, v1, v2);
    case LUA_OPMUL:return intop(*, v1, v2);
    case LUA_OPMOD: return luaV_mod(L, v1, v2);
    case LUA_OPIDIV: return luaV_idiv(L, v1, v2);
    case LUA_OPBAND: return intop(&, v1, v2);
    case LUA_OPBOR: return intop(|, v1, v2);
    case LUA_OPBXOR: return intop(^, v1, v2);
    case LUA_OPSHL: return luaV_shiftl(v1, v2);
    case LUA_OPSHR: return luaV_shiftr(v1, v2);
    case LUA_OPUNM: return intop(-, 0, v1);
    case LUA_OPBNOT: return intop(^, ~l_castS2U(0), v1);
    default: lua_assert(0); return 0;
  }
}


static lua_Number numarith (lua_State *L, int op, lua_Number v1,
                                                  lua_Number v2) {
  switch (op) {
    case LUA_OPADD: return luai_numadd(L, v1, v2);
    case LUA_OPSUB: return luai_numsub(L, v1, v2);
    case LUA_OPMUL: return luai_nummul(L, v1, v2);
    case LUA_OPDIV: return luai_numdiv(L, v1, v2);
    case LUA_OPPOW: return luai_numpow(L, v1, v2);
    case LUA_OPIDIV: return luai_numidiv(L, v1, v2);
    case LUA_OPUNM: return luai_numunm(L, v1);
    case LUA_OPMOD: return luaV_modf(L, v1, v2);
    default: lua_assert(0); return 0;
  }
}


int luaO_rawarith (lua_State *L, int op, const TValue *p1, const TValue *p2,
                   TValue *res) {
  switch (op) {
    case LUA_OPBAND: case LUA_OPBOR: case LUA_OPBXOR:
    case LUA_OPSHL: case LUA_OPSHR:
    case LUA_OPBNOT: {  
      lua_Integer i1; lua_Integer i2;
      if (tointegerns(p1, &i1) && tointegerns(p2, &i2)) {
        setivalue(res, intarith(L, op, i1, i2));
        return 1;
      }
      else return 0;  
    }
    case LUA_OPDIV: case LUA_OPPOW: {  
      lua_Number n1; lua_Number n2;
      if (tonumberns(p1, n1) && tonumberns(p2, n2)) {
        setfltvalue(res, numarith(L, op, n1, n2));
        return 1;
      }
      else return 0;  
    }
    default: {  
      lua_Number n1; lua_Number n2;
      if (ttisinteger(p1) && ttisinteger(p2)) {
        setivalue(res, intarith(L, op, ivalue(p1), ivalue(p2)));
        return 1;
      }
      else if (tonumberns(p1, n1) && tonumberns(p2, n2)) {
        setfltvalue(res, numarith(L, op, n1, n2));
        return 1;
      }
      else return 0;  
    }
  }
}


void luaO_arith (lua_State *L, int op, const TValue *p1, const TValue *p2,
                 StkId res) {
  if (!luaO_rawarith(L, op, p1, p2, s2v(res))) {
    
    luaT_trybinTM(L, p1, p2, res, cast(TMS, (op - LUA_OPADD) + TM_ADD));
  }
}


int luaO_hexavalue (int c) {
  if (lisdigit(c)) return c - '0';
  else return (ltolower(c) - 'a') + 10;
}


static int isneg (const char **s) {
  if (**s == '-') { (*s)++; return 1; }
  else if (**s == '+') (*s)++;
  return 0;
}





#if !defined(lua_strx2number)


#define MAXSIGDIG	30


static lua_Number lua_strx2number (const char *s, char **endptr) {
  int dot = lua_getlocaledecpoint();
  lua_Number r = l_mathop(0.0);  
  int sigdig = 0;  
  int nosigdig = 0;  
  int e = 0;  
  int neg;  
  int hasdot = 0;  
  *endptr = cast_charp(s);  
  while (lisspace(cast_uchar(*s))) s++;  
  neg = isneg(&s);  
  if (!(*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')))  
    return l_mathop(0.0);  
  for (s += 2; ; s++) {  
    if (*s == dot) {
      if (hasdot) break;  
      else hasdot = 1;
    }
    else if (lisxdigit(cast_uchar(*s))) {
      if (sigdig == 0 && *s == '0')  
        nosigdig++;
      else if (++sigdig <= MAXSIGDIG)  
          r = (r * l_mathop(16.0)) + luaO_hexavalue(*s);
      else e++; 
      if (hasdot) e--;  
    }
    else break;  
  }
  if (nosigdig + sigdig == 0)  
    return l_mathop(0.0);  
  *endptr = cast_charp(s);  
  e *= 4;  
  if (*s == 'p' || *s == 'P') {  
    int exp1 = 0;  
    int neg1;  
    s++;  
    neg1 = isneg(&s);  
    if (!lisdigit(cast_uchar(*s)))
      return l_mathop(0.0);  
    while (lisdigit(cast_uchar(*s)))  
      exp1 = exp1 * 10 + *(s++) - '0';
    if (neg1) exp1 = -exp1;
    e += exp1;
    *endptr = cast_charp(s);  
  }
  if (neg) r = -r;
  return l_mathop(ldexp)(r, e);
}

#endif




#if !defined (L_MAXLENNUM)
#define L_MAXLENNUM	200
#endif


static const char *l_str2dloc (const char *s, lua_Number *result, int mode) {
  char *endptr;
  *result = (mode == 'x') ? lua_strx2number(s, &endptr)  
                          : lua_str2number(s, &endptr);
  if (endptr == s) return NULL;  
  while (lisspace(cast_uchar(*endptr))) endptr++;  
  return (*endptr == '\0') ? endptr : NULL;  
}



static const char *l_str2d (const char *s, lua_Number *result) {
  const char *endptr;
  const char *pmode = strpbrk(s, ".xXnN");  
  int mode = pmode ? ltolower(cast_uchar(*pmode)) : 0;
  if (mode == 'n')  
    return NULL;
  endptr = l_str2dloc(s, result, mode);  
  if (endptr == NULL) {  
    char buff[L_MAXLENNUM + 1];
    const char *pdot = strchr(s, '.');
    if (pdot == NULL || strlen(s) > L_MAXLENNUM)
      return NULL;  
    strcpy(buff, s);  
    buff[pdot - s] = lua_getlocaledecpoint();  
    endptr = l_str2dloc(buff, result, mode);  
    if (endptr != NULL)
      endptr = s + (endptr - buff);  
  }
  return endptr;
}


#define MAXBY10		cast(lua_Unsigned, LUA_MAXINTEGER / 10)
#define MAXLASTD	cast_int(LUA_MAXINTEGER % 10)

static const char *l_str2int (const char *s, lua_Integer *result) {
  lua_Unsigned a = 0;
  int empty = 1;
  int neg;
  while (lisspace(cast_uchar(*s))) s++;  
  neg = isneg(&s);
  if (s[0] == '0' &&
      (s[1] == 'x' || s[1] == 'X')) {  
    s += 2;  
    for (; lisxdigit(cast_uchar(*s)); s++) {
      a = a * 16 + luaO_hexavalue(*s);
      empty = 0;
    }
  }
  else {  
    for (; lisdigit(cast_uchar(*s)); s++) {
      int d = *s - '0';
      if (a >= MAXBY10 && (a > MAXBY10 || d > MAXLASTD + neg))  
        return NULL;  
      a = a * 10 + d;
      empty = 0;
    }
  }
  while (lisspace(cast_uchar(*s))) s++;  
  if (empty || *s != '\0') return NULL;  
  else {
    *result = l_castU2S((neg) ? 0u - a : a);
    return s;
  }
}


size_t luaO_str2num (const char *s, TValue *o) {
  lua_Integer i; lua_Number n;
  const char *e;
  if ((e = l_str2int(s, &i)) != NULL) {  
    setivalue(o, i);
  }
  else if ((e = l_str2d(s, &n)) != NULL) {  
    setfltvalue(o, n);
  }
  else
    return 0;  
  return (e - s) + 1;  
}


int luaO_utf8esc (char *buff, unsigned long x) {
  int n = 1;  
  lua_assert(x <= 0x7FFFFFFFu);
  if (x < 0x80)  
    buff[UTF8BUFFSZ - 1] = cast_char(x);
  else {  
    unsigned int mfb = 0x3f;  
    do {  
      buff[UTF8BUFFSZ - (n++)] = cast_char(0x80 | (x & 0x3f));
      x >>= 6;  
      mfb >>= 1;  
    } while (x > mfb);  
    buff[UTF8BUFFSZ - n] = cast_char((~mfb << 1) | x);  
  }
  return n;
}



#define MAXNUMBER2STR	44



static int tostringbuff (TValue *obj, char *buff) {
  int len;
  lua_assert(ttisnumber(obj));
  if (ttisinteger(obj))
    len = lua_integer2str(buff, MAXNUMBER2STR, ivalue(obj));
  else {
    len = lua_number2str(buff, MAXNUMBER2STR, fltvalue(obj));
    if (buff[strspn(buff, "-0123456789")] == '\0') {  
      buff[len++] = lua_getlocaledecpoint();
      buff[len++] = '0';  
    }
  }
  return len;
}



void luaO_tostring (lua_State *L, TValue *obj) {
  char buff[MAXNUMBER2STR];
  int len = tostringbuff(obj, buff);
  setsvalue(L, obj, luaS_newlstr(L, buff, len));
}







#define BUFVFS		(LUA_IDSIZE + MAXNUMBER2STR + 95)


typedef struct BuffFS {
  lua_State *L;
  int pushed;  
  int blen;  
  char space[BUFVFS];  
} BuffFS;



static void pushstr (BuffFS *buff, const char *str, size_t lstr) {
  lua_State *L = buff->L;
  setsvalue2s(L, L->top.p, luaS_newlstr(L, str, lstr));
  L->top.p++;  
  if (!buff->pushed)  
    buff->pushed = 1;  
  else  
    luaV_concat(L, 2);
}



static void clearbuff (BuffFS *buff) {
  pushstr(buff, buff->space, buff->blen);  
  buff->blen = 0;  
}



static char *getbuff (BuffFS *buff, int sz) {
  lua_assert(buff->blen <= BUFVFS); lua_assert(sz <= BUFVFS);
  if (sz > BUFVFS - buff->blen)  
    clearbuff(buff);
  return buff->space + buff->blen;
}


#define addsize(b,sz)	((b)->blen += (sz))



static void addstr2buff (BuffFS *buff, const char *str, size_t slen) {
  if (slen <= BUFVFS) {  
    char *bf = getbuff(buff, cast_int(slen));
    memcpy(bf, str, slen);  
    addsize(buff, cast_int(slen));
  }
  else {  
    clearbuff(buff);  
    pushstr(buff, str, slen);  
  }
}



static void addnum2buff (BuffFS *buff, TValue *num) {
  char *numbuff = getbuff(buff, MAXNUMBER2STR);
  int len = tostringbuff(num, numbuff);  
  addsize(buff, len);
}



const char *luaO_pushvfstring (lua_State *L, const char *fmt, va_list argp) {
  BuffFS buff;  
  const char *e;  
  buff.pushed = buff.blen = 0;
  buff.L = L;
  while ((e = strchr(fmt, '%')) != NULL) {
    addstr2buff(&buff, fmt, e - fmt);  
    switch (*(e + 1)) {  
      case 's': {  
        const char *s = va_arg(argp, char *);
        if (s == NULL) s = "(null)";
        addstr2buff(&buff, s, strlen(s));
        break;
      }
      case 'c': {  
        char c = cast_uchar(va_arg(argp, int));
        addstr2buff(&buff, &c, sizeof(char));
        break;
      }
      case 'd': {  
        TValue num;
        setivalue(&num, va_arg(argp, int));
        addnum2buff(&buff, &num);
        break;
      }
      case 'I': {  
        TValue num;
        setivalue(&num, cast(lua_Integer, va_arg(argp, l_uacInt)));
        addnum2buff(&buff, &num);
        break;
      }
      case 'f': {  
        TValue num;
        setfltvalue(&num, cast_num(va_arg(argp, l_uacNumber)));
        addnum2buff(&buff, &num);
        break;
      }
      case 'p': {  
        const int sz = 3 * sizeof(void*) + 8; 
        char *bf = getbuff(&buff, sz);
        void *p = va_arg(argp, void *);
        int len = lua_pointer2str(bf, sz, p);
        addsize(&buff, len);
        break;
      }
      case 'U': {  
        char bf[UTF8BUFFSZ];
        int len = luaO_utf8esc(bf, va_arg(argp, long));
        addstr2buff(&buff, bf + UTF8BUFFSZ - len, len);
        break;
      }
      case '%': {
        addstr2buff(&buff, "%", 1);
        break;
      }
      default: {
        luaG_runerror(L, "invalid option '%%%c' to 'lua_pushfstring'",
                         *(e + 1));
      }
    }
    fmt = e + 2;  
  }
  addstr2buff(&buff, fmt, strlen(fmt));  
  clearbuff(&buff);  
  lua_assert(buff.pushed == 1);
  return getstr(tsvalue(s2v(L->top.p - 1)));
}


const char *luaO_pushfstring (lua_State *L, const char *fmt, ...) {
  const char *msg;
  va_list argp;
  va_start(argp, fmt);
  msg = luaO_pushvfstring(L, fmt, argp);
  va_end(argp);
  return msg;
}




#define RETS	"..."
#define PRE	"[string \""
#define POS	"\"]"

#define addstr(a,b,l)	( memcpy(a,b,(l) * sizeof(char)), a += (l) )

void luaO_chunkid (char *out, const char *source, size_t srclen) {
  size_t bufflen = LUA_IDSIZE;  
  if (*source == '=') {  
    if (srclen <= bufflen)  
      memcpy(out, source + 1, srclen * sizeof(char));
    else {  
      addstr(out, source + 1, bufflen - 1);
      *out = '\0';
    }
  }
  else if (*source == '@') {  
    if (srclen <= bufflen)  
      memcpy(out, source + 1, srclen * sizeof(char));
    else {  
      addstr(out, RETS, LL(RETS));
      bufflen -= LL(RETS);
      memcpy(out, source + 1 + srclen - bufflen, bufflen * sizeof(char));
    }
  }
  else {  
    const char *nl = strchr(source, '\n');  
    addstr(out, PRE, LL(PRE));  
    bufflen -= LL(PRE RETS POS) + 1;  
    if (srclen < bufflen && nl == NULL) {  
      addstr(out, source, srclen);  
    }
    else {
      if (nl != NULL) srclen = nl - source;  
      if (srclen > bufflen) srclen = bufflen;
      addstr(out, source, srclen);
      addstr(out, RETS, LL(RETS));
    }
    memcpy(out, POS, (LL(POS) + 1) * sizeof(char));
  }
}



//path: src/imports/../dependencies/lua/ltm.c


#define ltm_c
#define LUA_CORE





#include <string.h>
























static const char udatatypename[] = "userdata";

LUAI_DDEF const char *const luaT_typenames_[LUA_TOTALTYPES] = {
  "no value",
  "nil", "boolean", udatatypename, "number",
  "string", "table", "function", udatatypename, "thread",
  "upvalue", "proto" 
};


void luaT_init (lua_State *L) {
  static const char *const luaT_eventname[] = {  
    "__index", "__newindex",
    "__gc", "__mode", "__len", "__eq",
    "__add", "__sub", "__mul", "__mod", "__pow",
    "__div", "__idiv",
    "__band", "__bor", "__bxor", "__shl", "__shr",
    "__unm", "__bnot", "__lt", "__le",
    "__concat", "__call", "__close"
  };
  int i;
  for (i=0; i<TM_N; i++) {
    G(L)->tmname[i] = luaS_new(L, luaT_eventname[i]);
    luaC_fix(L, obj2gco(G(L)->tmname[i]));  
  }
}



const TValue *luaT_gettm (Table *events, TMS event, TString *ename) {
  const TValue *tm = luaH_Hgetshortstr(events, ename);
  lua_assert(event <= TM_EQ);
  if (notm(tm)) {  
    events->flags |= cast_byte(1u<<event);  
    return NULL;
  }
  else return tm;
}


const TValue *luaT_gettmbyobj (lua_State *L, const TValue *o, TMS event) {
  Table *mt;
  switch (ttype(o)) {
    case LUA_TTABLE:
      mt = hvalue(o)->metatable;
      break;
    case LUA_TUSERDATA:
      mt = uvalue(o)->metatable;
      break;
    default:
      mt = G(L)->mt[ttype(o)];
  }
  return (mt ? luaH_Hgetshortstr(mt, G(L)->tmname[event]) : &G(L)->nilvalue);
}



const char *luaT_objtypename (lua_State *L, const TValue *o) {
  Table *mt;
  if ((ttistable(o) && (mt = hvalue(o)->metatable) != NULL) ||
      (ttisfulluserdata(o) && (mt = uvalue(o)->metatable) != NULL)) {
    const TValue *name = luaH_Hgetshortstr(mt, luaS_new(L, "__name"));
    if (ttisstring(name))  
      return getstr(tsvalue(name));  
  }
  return ttypename(ttype(o));  
}


void luaT_callTM (lua_State *L, const TValue *f, const TValue *p1,
                  const TValue *p2, const TValue *p3) {
  StkId func = L->top.p;
  setobj2s(L, func, f);  
  setobj2s(L, func + 1, p1);  
  setobj2s(L, func + 2, p2);  
  setobj2s(L, func + 3, p3);  
  L->top.p = func + 4;
  
  if (isLuacode(L->ci))
    luaD_call(L, func, 0);
  else
    luaD_callnoyield(L, func, 0);
}


int luaT_callTMres (lua_State *L, const TValue *f, const TValue *p1,
                    const TValue *p2, StkId res) {
  ptrdiff_t result = savestack(L, res);
  StkId func = L->top.p;
  setobj2s(L, func, f);  
  setobj2s(L, func + 1, p1);  
  setobj2s(L, func + 2, p2);  
  L->top.p += 3;
  
  if (isLuacode(L->ci))
    luaD_call(L, func, 1);
  else
    luaD_callnoyield(L, func, 1);
  res = restorestack(L, result);
  setobjs2s(L, res, --L->top.p);  
  return ttypetag(s2v(res));  
}


static int callbinTM (lua_State *L, const TValue *p1, const TValue *p2,
                      StkId res, TMS event) {
  const TValue *tm = luaT_gettmbyobj(L, p1, event);  
  if (notm(tm))
    tm = luaT_gettmbyobj(L, p2, event);  
  if (notm(tm))
    return -1;  
  else  
    return luaT_callTMres(L, tm, p1, p2, res);
}


void luaT_trybinTM (lua_State *L, const TValue *p1, const TValue *p2,
                    StkId res, TMS event) {
  if (l_unlikely(callbinTM(L, p1, p2, res, event) < 0)) {
    switch (event) {
      case TM_BAND: case TM_BOR: case TM_BXOR:
      case TM_SHL: case TM_SHR: case TM_BNOT: {
        if (ttisnumber(p1) && ttisnumber(p2))
          luaG_tointerror(L, p1, p2);
        else
          luaG_opinterror(L, p1, p2, "perform bitwise operation on");
      }
      /* FALLTHROUGH */
      default:
        luaG_opinterror(L, p1, p2, "perform arithmetic on");
    }
  }
}



void luaT_tryconcatTM (lua_State *L) {
  StkId p1 = L->top.p - 2;  
  if (l_unlikely(callbinTM(L, s2v(p1), s2v(p1 + 1), p1, TM_CONCAT) < 0))
    luaG_concaterror(L, s2v(p1), s2v(p1 + 1));
}


void luaT_trybinassocTM (lua_State *L, const TValue *p1, const TValue *p2,
                                       int flip, StkId res, TMS event) {
  if (flip)
    luaT_trybinTM(L, p2, p1, res, event);
  else
    luaT_trybinTM(L, p1, p2, res, event);
}


void luaT_trybiniTM (lua_State *L, const TValue *p1, lua_Integer i2,
                                   int flip, StkId res, TMS event) {
  TValue aux;
  setivalue(&aux, i2);
  luaT_trybinassocTM(L, p1, &aux, flip, res, event);
}



int luaT_callorderTM (lua_State *L, const TValue *p1, const TValue *p2,
                      TMS event) {
  int tag = callbinTM(L, p1, p2, L->top.p, event);  
  if (tag >= 0)  
    return !tagisfalse(tag);
#if defined(LUA_COMPAT_LT_LE)
  else if (event == TM_LE) {
    
    L->ci->callstatus |= CIST_LEQ;  
    tag = callbinTM(L, p2, p1, L->top.p, TM_LT);
    L->ci->callstatus ^= CIST_LEQ;  
    if (tag >= 0)  
      return tagisfalse(tag);
  }
#endif
  luaG_ordererror(L, p1, p2);  
  return 0;  
}


int luaT_callorderiTM (lua_State *L, const TValue *p1, int v2,
                       int flip, int isfloat, TMS event) {
  TValue aux; const TValue *p2;
  if (isfloat) {
    setfltvalue(&aux, cast_num(v2));
  }
  else
    setivalue(&aux, v2);
  if (flip) {  
    p2 = p1; p1 = &aux;  
  }
  else
    p2 = &aux;
  return luaT_callorderTM(L, p1, p2, event);
}


void luaT_adjustvarargs (lua_State *L, int nfixparams, CallInfo *ci,
                         const Proto *p) {
  int i;
  int actual = cast_int(L->top.p - ci->func.p) - 1;  
  int nextra = actual - nfixparams;  
  ci->u.l.nextraargs = nextra;
  luaD_checkstack(L, p->maxstacksize + 1);
  
  setobjs2s(L, L->top.p++, ci->func.p);
  
  for (i = 1; i <= nfixparams; i++) {
    setobjs2s(L, L->top.p++, ci->func.p + i);
    setnilvalue(s2v(ci->func.p + i));  
  }
  ci->func.p += actual + 1;
  ci->top.p += actual + 1;
  lua_assert(L->top.p <= ci->top.p && ci->top.p <= L->stack_last.p);
}


void luaT_getvarargs (lua_State *L, CallInfo *ci, StkId where, int wanted) {
  int i;
  int nextra = ci->u.l.nextraargs;
  if (wanted < 0) {
    wanted = nextra;  
    checkstackp(L, nextra, where);  
    L->top.p = where + nextra;  
  }
  for (i = 0; i < wanted && i < nextra; i++)
    setobjs2s(L, where + i, ci->func.p - nextra + i);
  for (; i < wanted; i++)   
    setnilvalue(s2v(where + i));
}



//path: src/imports/../dependencies/lua/lstring.c


#define lstring_c
#define LUA_CORE





#include <string.h>



















#define MAXSTRTB	cast_int(luaM_limitN(MAX_INT, TString*))



int luaS_eqlngstr (TString *a, TString *b) {
  size_t len = a->u.lnglen;
  lua_assert(a->tt == LUA_VLNGSTR && b->tt == LUA_VLNGSTR);
  return (a == b) ||  
    ((len == b->u.lnglen) &&  
     (memcmp(getlngstr(a), getlngstr(b), len) == 0));  
}


unsigned luaS_hash (const char *str, size_t l, unsigned seed) {
  unsigned int h = seed ^ cast_uint(l);
  for (; l > 0; l--)
    h ^= ((h<<5) + (h>>2) + cast_byte(str[l - 1]));
  return h;
}


unsigned luaS_hashlongstr (TString *ts) {
  lua_assert(ts->tt == LUA_VLNGSTR);
  if (ts->extra == 0) {  
    size_t len = ts->u.lnglen;
    ts->hash = luaS_hash(getlngstr(ts), len, ts->hash);
    ts->extra = 1;  
  }
  return ts->hash;
}


static void tablerehash (TString **vect, int osize, int nsize) {
  int i;
  for (i = osize; i < nsize; i++)  
    vect[i] = NULL;
  for (i = 0; i < osize; i++) {  
    TString *p = vect[i];
    vect[i] = NULL;
    while (p) {  
      TString *hnext = p->u.hnext;  
      unsigned int h = lmod(p->hash, nsize);  
      p->u.hnext = vect[h];  
      vect[h] = p;
      p = hnext;
    }
  }
}



void luaS_resize (lua_State *L, int nsize) {
  stringtable *tb = &G(L)->strt;
  int osize = tb->size;
  TString **newvect;
  if (nsize < osize)  
    tablerehash(tb->hash, osize, nsize);  
  newvect = luaM_reallocvector(L, tb->hash, osize, nsize, TString*);
  if (l_unlikely(newvect == NULL)) {  
    if (nsize < osize)  
      tablerehash(tb->hash, nsize, osize);  
    
  }
  else {  
    tb->hash = newvect;
    tb->size = nsize;
    if (nsize > osize)
      tablerehash(newvect, osize, nsize);  
  }
}



void luaS_clearcache (global_State *g) {
  int i, j;
  for (i = 0; i < STRCACHE_N; i++)
    for (j = 0; j < STRCACHE_M; j++) {
      if (iswhite(g->strcache[i][j]))  
        g->strcache[i][j] = g->memerrmsg;  
    }
}



void luaS_init (lua_State *L) {
  global_State *g = G(L);
  int i, j;
  stringtable *tb = &G(L)->strt;
  tb->hash = luaM_newvector(L, MINSTRTABSIZE, TString*);
  tablerehash(tb->hash, 0, MINSTRTABSIZE);  
  tb->size = MINSTRTABSIZE;
  
  g->memerrmsg = luaS_newliteral(L, MEMERRMSG);
  luaC_fix(L, obj2gco(g->memerrmsg));  
  for (i = 0; i < STRCACHE_N; i++)  
    for (j = 0; j < STRCACHE_M; j++)
      g->strcache[i][j] = g->memerrmsg;
}


size_t luaS_sizelngstr (size_t len, int kind) {
  switch (kind) {
    case LSTRREG:  
      
      return offsetof(TString, falloc) + (len + 1) * sizeof(char);
    case LSTRFIX:  
      
      return offsetof(TString, falloc);
    default:  
      lua_assert(kind == LSTRMEM);
      return sizeof(TString);
  }
}



static TString *createstrobj (lua_State *L, size_t totalsize, int tag,
                              unsigned h) {
  TString *ts;
  GCObject *o;
  o = luaC_newobj(L, tag, totalsize);
  ts = gco2ts(o);
  ts->hash = h;
  ts->extra = 0;
  return ts;
}


TString *luaS_createlngstrobj (lua_State *L, size_t l) {
  size_t totalsize = luaS_sizelngstr(l, LSTRREG);
  TString *ts = createstrobj(L, totalsize, LUA_VLNGSTR, G(L)->seed);
  ts->u.lnglen = l;
  ts->shrlen = LSTRREG;  
  ts->contents = cast_charp(ts) + offsetof(TString, falloc);
  ts->contents[l] = '\0';  
  return ts;
}


void luaS_remove (lua_State *L, TString *ts) {
  stringtable *tb = &G(L)->strt;
  TString **p = &tb->hash[lmod(ts->hash, tb->size)];
  while (*p != ts)  
    p = &(*p)->u.hnext;
  *p = (*p)->u.hnext;  
  tb->nuse--;
}


static void growstrtab (lua_State *L, stringtable *tb) {
  if (l_unlikely(tb->nuse == MAX_INT)) {  
    luaC_fullgc(L, 1);  
    if (tb->nuse == MAX_INT)  
      luaM_error(L);  
  }
  if (tb->size <= MAXSTRTB / 2)  
    luaS_resize(L, tb->size * 2);
}



static TString *internshrstr (lua_State *L, const char *str, size_t l) {
  TString *ts;
  global_State *g = G(L);
  stringtable *tb = &g->strt;
  unsigned int h = luaS_hash(str, l, g->seed);
  TString **list = &tb->hash[lmod(h, tb->size)];
  lua_assert(str != NULL);  
  for (ts = *list; ts != NULL; ts = ts->u.hnext) {
    if (l == cast_uint(ts->shrlen) &&
        (memcmp(str, getshrstr(ts), l * sizeof(char)) == 0)) {
      
      if (isdead(g, ts))  
        changewhite(ts);  
      return ts;
    }
  }
  
  if (tb->nuse >= tb->size) {  
    growstrtab(L, tb);
    list = &tb->hash[lmod(h, tb->size)];  
  }
  ts = createstrobj(L, sizestrshr(l), LUA_VSHRSTR, h);
  ts->shrlen = cast_byte(l);
  getshrstr(ts)[l] = '\0';  
  memcpy(getshrstr(ts), str, l * sizeof(char));
  ts->u.hnext = *list;
  *list = ts;
  tb->nuse++;
  return ts;
}



TString *luaS_newlstr (lua_State *L, const char *str, size_t l) {
  if (l <= LUAI_MAXSHORTLEN)  
    return internshrstr(L, str, l);
  else {
    TString *ts;
    if (l_unlikely(l * sizeof(char) >= (MAX_SIZE - sizeof(TString))))
      luaM_toobig(L);
    ts = luaS_createlngstrobj(L, l);
    memcpy(getlngstr(ts), str, l * sizeof(char));
    return ts;
  }
}



TString *luaS_new (lua_State *L, const char *str) {
  unsigned int i = point2uint(str) % STRCACHE_N;  
  int j;
  TString **p = G(L)->strcache[i];
  for (j = 0; j < STRCACHE_M; j++) {
    if (strcmp(str, getstr(p[j])) == 0)  
      return p[j];  
  }
  
  for (j = STRCACHE_M - 1; j > 0; j--)
    p[j] = p[j - 1];  
  
  p[0] = luaS_newlstr(L, str, strlen(str));
  return p[0];
}


Udata *luaS_newudata (lua_State *L, size_t s, int nuvalue) {
  Udata *u;
  int i;
  GCObject *o;
  if (l_unlikely(s > MAX_SIZE - udatamemoffset(nuvalue)))
    luaM_toobig(L);
  o = luaC_newobj(L, LUA_VUSERDATA, sizeudata(nuvalue, s));
  u = gco2u(o);
  u->len = s;
  u->nuvalue = nuvalue;
  u->metatable = NULL;
  for (i = 0; i < nuvalue; i++)
    setnilvalue(&u->uv[i].uv);
  return u;
}


struct NewExt {
  int kind;
  const char *s;
   size_t len;
  TString *ts;  
};


static void f_newext (lua_State *L, void *ud) {
  struct NewExt *ne = cast(struct NewExt *, ud);
  size_t size = luaS_sizelngstr(0, ne->kind);
  ne->ts = createstrobj(L, size, LUA_VLNGSTR, G(L)->seed);
}


static void f_pintern (lua_State *L, void *ud) {
  struct NewExt *ne = cast(struct NewExt *, ud);
  ne->ts = internshrstr(L, ne->s, ne->len);
}


TString *luaS_newextlstr (lua_State *L,
	          const char *s, size_t len, lua_Alloc falloc, void *ud) {
  struct NewExt ne;
  if (len <= LUAI_MAXSHORTLEN) {  
    ne.s = s; ne.len = len;
    if (!falloc)
      f_pintern(L, &ne);  
    else {
      int status = luaD_rawrunprotected(L, f_pintern, &ne);
      (*falloc)(ud, cast_voidp(s), len + 1, 0);  
      if (status != LUA_OK)  
        luaM_error(L);  
    }
    return ne.ts;
  }
  
  if (!falloc) {
    ne.kind = LSTRFIX;
    f_newext(L, &ne);  
  }
  else {
    ne.kind = LSTRMEM;
    if (luaD_rawrunprotected(L, f_newext, &ne) != LUA_OK) {  
      (*falloc)(ud, cast_voidp(s), len + 1, 0);  
      luaM_error(L);  
    }
    ne.ts->falloc = falloc;
    ne.ts->ud = ud;
  }
  ne.ts->shrlen = ne.kind;
  ne.ts->u.lnglen = len;
  ne.ts->contents = cast_charp(s);
  return ne.ts;
}




//path: src/imports/../dependencies/lua/ltable.c


#define ltable_c
#define LUA_CORE







#include <limits.h>

























#define LIMFORLAST    2  


typedef struct { Node *dummy; Node follows_pNode; } Limbox_aux;

typedef union {
  Node *lastfree;
  char padding[offsetof(Limbox_aux, follows_pNode)];
} Limbox;

#define haslastfree(t)     ((t)->lsizenode > LIMFORLAST)
#define getlastfree(t)     ((cast(Limbox *, (t)->node) - 1)->lastfree)



#define MAXABITS	cast_int(sizeof(int) * CHAR_BIT - 1)



#define MAXASIZEB	((MAX_SIZET/sizeof(ArrayCell)) * NM)



#define MAXASIZE  \
    (((1u << MAXABITS) < MAXASIZEB) ? (1u << MAXABITS) : cast_uint(MAXASIZEB))


#define MAXHBITS	(MAXABITS - 1)



#define MAXHSIZE	luaM_limitN(1u << MAXHBITS, Node)



#define hashpow2(t,n)		(gnode(t, lmod((n), sizenode(t))))


#define hashmod(t,n)	(gnode(t, ((n) % ((sizenode(t)-1)|1))))


#define hashstr(t,str)		hashpow2(t, (str)->hash)
#define hashboolean(t,p)	hashpow2(t, p)


#define hashpointer(t,p)	hashmod(t, point2uint(p))


#define dummynode		(&dummynode_)

static const Node dummynode_ = {
  {{NULL}, LUA_VEMPTY,  
   LUA_VNIL, 0, {NULL}}  
};


static const TValue absentkey = {ABSTKEYCONSTANT};



static Node *hashint (const Table *t, lua_Integer i) {
  lua_Unsigned ui = l_castS2U(i);
  if (ui <= cast_uint(INT_MAX))
    return hashmod(t, cast_int(ui));
  else
    return hashmod(t, ui);
}



#if !defined(l_hashfloat)
static int l_hashfloat (lua_Number n) {
  int i;
  lua_Integer ni;
  n = l_mathop(private_lua_cembed_frexp)(n, &i) * -cast_num(INT_MIN);
  if (!lua_numbertointeger(n, &ni)) {  
    lua_assert(luai_numisnan(n) || l_mathop(fabs)(n) == cast_num(HUGE_VAL));
    return 0;
  }
  else {  
    unsigned int u = cast_uint(i) + cast_uint(ni);
    return cast_int(u <= cast_uint(INT_MAX) ? u : ~u);
  }
}
#endif



static Node *mainpositionTV (const Table *t, const TValue *key) {
  switch (ttypetag(key)) {
    case LUA_VNUMINT: {
      lua_Integer i = ivalue(key);
      return hashint(t, i);
    }
    case LUA_VNUMFLT: {
      lua_Number n = fltvalue(key);
      return hashmod(t, l_hashfloat(n));
    }
    case LUA_VSHRSTR: {
      TString *ts = tsvalue(key);
      return hashstr(t, ts);
    }
    case LUA_VLNGSTR: {
      TString *ts = tsvalue(key);
      return hashpow2(t, luaS_hashlongstr(ts));
    }
    case LUA_VFALSE:
      return hashboolean(t, 0);
    case LUA_VTRUE:
      return hashboolean(t, 1);
    case LUA_VLIGHTUSERDATA: {
      void *p = pvalue(key);
      return hashpointer(t, p);
    }
    case LUA_VLCF: {
      lua_CFunction f = fvalue(key);
      return hashpointer(t, f);
    }
    default: {
      GCObject *o = gcvalue(key);
      return hashpointer(t, o);
    }
  }
}


l_sinline Node *mainpositionfromnode (const Table *t, Node *nd) {
  TValue key;
  getnodekey(cast(lua_State *, NULL), &key, nd);
  return mainpositionTV(t, &key);
}



static int equalkey (const TValue *k1, const Node *n2, int deadok) {
  if ((rawtt(k1) != keytt(n2)) &&  
       !(deadok && keyisdead(n2) && iscollectable(k1)))
   return 0;  
  switch (keytt(n2)) {
    case LUA_VNIL: case LUA_VFALSE: case LUA_VTRUE:
      return 1;
    case LUA_VNUMINT:
      return (ivalue(k1) == keyival(n2));
    case LUA_VNUMFLT:
      return luai_numeq(fltvalue(k1), fltvalueraw(keyval(n2)));
    case LUA_VLIGHTUSERDATA:
      return pvalue(k1) == pvalueraw(keyval(n2));
    case LUA_VLCF:
      return fvalue(k1) == fvalueraw(keyval(n2));
    case ctb(LUA_VLNGSTR):
      return luaS_eqlngstr(tsvalue(k1), keystrval(n2));
    default:
      return gcvalue(k1) == gcvalueraw(keyval(n2));
  }
}



#define limitequalsasize(t)	(isrealasize(t) || ispow2((t)->alimit))



LUAI_FUNC unsigned int luaH_realasize (const Table *t) {
  if (limitequalsasize(t))
    return t->alimit;  
  else {
    unsigned int size = t->alimit;
    
    size |= (size >> 1);
    size |= (size >> 2);
    size |= (size >> 4);
    size |= (size >> 8);
#if (UINT_MAX >> 14) > 3  
    size |= (size >> 16);
#if (UINT_MAX >> 30) > 3
    size |= (size >> 32);  
#endif
#endif
    size++;
    lua_assert(ispow2(size) && size/2 < t->alimit && t->alimit < size);
    return size;
  }
}



static int ispow2realasize (const Table *t) {
  return (!isrealasize(t) || ispow2(t->alimit));
}


static unsigned int setlimittosize (Table *t) {
  t->alimit = luaH_realasize(t);
  setrealasize(t);
  return t->alimit;
}


#define limitasasize(t)	check_exp(isrealasize(t), t->alimit)




static const TValue *getgeneric (Table *t, const TValue *key, int deadok) {
  Node *n = mainpositionTV(t, key);
  for (;;) {  
    if (equalkey(key, n, deadok))
      return gval(n);  
    else {
      int nx = gnext(n);
      if (nx == 0)
        return &absentkey;  
      n += nx;
    }
  }
}



static unsigned int arrayindex (lua_Integer k) {
  if (l_castS2U(k) - 1u < MAXASIZE)  
    return cast_uint(k);  
  else
    return 0;
}



static unsigned findindex (lua_State *L, Table *t, TValue *key,
                               unsigned asize) {
  unsigned int i;
  if (ttisnil(key)) return 0;  
  i = ttisinteger(key) ? arrayindex(ivalue(key)) : 0;
  if (i - 1u < asize)  
    return i;  
  else {
    const TValue *n = getgeneric(t, key, 1);
    if (l_unlikely(isabstkey(n)))
      luaG_runerror(L, "invalid key to 'next'");  
    i = cast_int(nodefromval(n) - gnode(t, 0));  
    
    return (i + 1) + asize;
  }
}


int luaH_next (lua_State *L, Table *t, StkId key) {
  unsigned int asize = luaH_realasize(t);
  unsigned int i = findindex(L, t, s2v(key), asize);  
  for (; i < asize; i++) {  
    int tag = *getArrTag(t, i);
    if (!tagisempty(tag)) {  
      setivalue(s2v(key), i + 1);
      farr2val(t, i + 1, tag, s2v(key + 1));
      return 1;
    }
  }
  for (i -= asize; cast_int(i) < sizenode(t); i++) {  
    if (!isempty(gval(gnode(t, i)))) {  
      Node *n = gnode(t, i);
      getnodekey(L, s2v(key), n);
      setobj2s(L, key + 1, gval(n));
      return 1;
    }
  }
  return 0;  
}


static void freehash (lua_State *L, Table *t) {
  if (!isdummy(t)) {
    size_t bsize = sizenode(t) * sizeof(Node);  
    char *arr = cast_charp(t->node);
    if (haslastfree(t)) {
      bsize += sizeof(Limbox);
      arr -= sizeof(Limbox);
    }
    luaM_freearray(L, arr, bsize);
  }
}



static int keyinarray (Table *t, lua_Integer key) {
  lua_Unsigned alimit = t->alimit;
  if (l_castS2U(key) - 1u < alimit)  
    return 1;
  else if (!isrealasize(t) &&  
           (((l_castS2U(key) - 1u) & ~(alimit - 1u)) < alimit)) {
    t->alimit = cast_uint(key);  
    return 1;
  }
  else
    return 0;
}





static unsigned computesizes (unsigned nums[], unsigned *pna) {
  int i;
  unsigned int twotoi;  
  unsigned int a = 0;  
  unsigned int na = 0;  
  unsigned int optimal = 0;  
  
  for (i = 0, twotoi = 1;
       twotoi > 0 && *pna > twotoi / 2;
       i++, twotoi *= 2) {
    a += nums[i];
    if (a > twotoi/2) {  
      optimal = twotoi;  
      na = a;  
    }
  }
  lua_assert((optimal == 0 || optimal / 2 < na) && na <= optimal);
  *pna = na;
  return optimal;
}


static int countint (lua_Integer key, unsigned int *nums) {
  unsigned int k = arrayindex(key);
  if (k != 0) {  
    nums[luaO_ceillog2(k)]++;  
    return 1;
  }
  else
    return 0;
}


l_sinline int arraykeyisempty (const Table *t, lua_Integer key) {
  int tag = *getArrTag(t, key - 1);
  return tagisempty(tag);
}



static unsigned numusearray (const Table *t, unsigned *nums) {
  int lg;
  unsigned int ttlg;  
  unsigned int ause = 0;  
  unsigned int i = 1;  
  unsigned int asize = limitasasize(t);  
  
  for (lg = 0, ttlg = 1; lg <= MAXABITS; lg++, ttlg *= 2) {
    unsigned int lc = 0;  
    unsigned int lim = ttlg;
    if (lim > asize) {
      lim = asize;  
      if (i > lim)
        break;  
    }
    
    for (; i <= lim; i++) {
      if (!arraykeyisempty(t, i))
        lc++;
    }
    nums[lg] += lc;
    ause += lc;
  }
  return ause;
}


static int numusehash (const Table *t, unsigned *nums, unsigned *pna) {
  int totaluse = 0;  
  int ause = 0;  
  int i = sizenode(t);
  while (i--) {
    Node *n = &t->node[i];
    if (!isempty(gval(n))) {
      if (keyisinteger(n))
        ause += countint(keyival(n), nums);
      totaluse++;
    }
  }
  *pna += ause;
  return totaluse;
}



static size_t concretesize (unsigned int size) {
  unsigned int numcells = (size + NM - 1) / NM;   
  unsigned int extra = NM - 1 - ((size + NM - 1) % NM);
  return numcells * sizeof(ArrayCell) - extra * sizeof(Value);
}


static ArrayCell *resizearray (lua_State *L , Table *t,
                               unsigned oldasize,
                               unsigned newasize) {
  size_t oldasizeb = concretesize(oldasize);
  size_t newasizeb = concretesize(newasize);
  void *a = luaM_reallocvector(L, t->array, oldasizeb, newasizeb, lu_byte);
  return cast(ArrayCell*, a);
}



static void setnodevector (lua_State *L, Table *t, unsigned size) {
  if (size == 0) {  
    t->node = cast(Node *, dummynode);  
    t->lsizenode = 0;
    setdummy(t);  
  }
  else {
    int i;
    int lsize = luaO_ceillog2(size);
    if (lsize > MAXHBITS || (1u << lsize) > MAXHSIZE)
      luaG_runerror(L, "table overflow");
    size = twoto(lsize);
    if (lsize <= LIMFORLAST)  
      t->node = luaM_newvector(L, size, Node);
    else {
      size_t bsize = size * sizeof(Node) + sizeof(Limbox);
      char *node = luaM_newblock(L, bsize);
      t->node = cast(Node *, node + sizeof(Limbox));
      getlastfree(t) = gnode(t, size);  
    }
    t->lsizenode = cast_byte(lsize);
    setnodummy(t);
    for (i = 0; i < cast_int(size); i++) {
      Node *n = gnode(t, i);
      gnext(n) = 0;
      setnilkey(n);
      setempty(gval(n));
    }
  }
}



static void reinsert (lua_State *L, Table *ot, Table *t) {
  int j;
  int size = sizenode(ot);
  for (j = 0; j < size; j++) {
    Node *old = gnode(ot, j);
    if (!isempty(gval(old))) {
      
      TValue k;
      getnodekey(L, &k, old);
      luaH_set(L, t, &k, gval(old));
    }
  }
}



static void exchangehashpart (Table *t1, Table *t2) {
  lu_byte lsizenode = t1->lsizenode;
  Node *node = t1->node;
  int bitdummy1 = t1->flags & BITDUMMY;
  t1->lsizenode = t2->lsizenode;
  t1->node = t2->node;
  t1->flags = (t1->flags & NOTBITDUMMY) | (t2->flags & BITDUMMY);
  t2->lsizenode = lsizenode;
  t2->node = node;
  t2->flags = (t2->flags & NOTBITDUMMY) | bitdummy1;
}



static void reinsertOldSlice (lua_State *L, Table *t, unsigned oldasize,
                                            unsigned newasize) {
  unsigned i;
  t->alimit = newasize;  
  for (i = newasize; i < oldasize; i++) {  
    int tag = *getArrTag(t, i);
    if (!tagisempty(tag)) {  
      TValue aux;
      farr2val(t, i + 1, tag, &aux);  
      luaH_setint(L, t, i + 1, &aux);  
    }
  }
  t->alimit = oldasize;  
}



static void clearNewSlice (Table *t, unsigned oldasize, unsigned newasize) {
  for (; oldasize < newasize; oldasize++)
    *getArrTag(t, oldasize) = LUA_VEMPTY;
}



void luaH_resize (lua_State *L, Table *t, unsigned newasize,
                                          unsigned nhsize) {
  Table newt;  
  unsigned int oldasize = setlimittosize(t);
  ArrayCell *newarray;
  if (newasize > MAXASIZE)
    luaG_runerror(L, "table overflow");
  
  newt.flags = 0;
  setnodevector(L, &newt, nhsize);
  if (newasize < oldasize) {  
    
    exchangehashpart(t, &newt);  
    reinsertOldSlice(L, t, oldasize, newasize);
    exchangehashpart(t, &newt);  
  }
  
  newarray = resizearray(L, t, oldasize, newasize);
  if (l_unlikely(newarray == NULL && newasize > 0)) {  
    freehash(L, &newt);  
    luaM_error(L);  
  }
  
  exchangehashpart(t, &newt);  
  t->array = newarray;  
  t->alimit = newasize;
  clearNewSlice(t, oldasize, newasize);
  
  reinsert(L, &newt, t);  
  freehash(L, &newt);  
}


void luaH_resizearray (lua_State *L, Table *t, unsigned int nasize) {
  int nsize = allocsizenode(t);
  luaH_resize(L, t, nasize, nsize);
}


static void rehash (lua_State *L, Table *t, const TValue *ek) {
  unsigned int asize;  
  unsigned int na;  
  unsigned int nums[MAXABITS + 1];
  int i;
  int totaluse;
  for (i = 0; i <= MAXABITS; i++) nums[i] = 0;  
  setlimittosize(t);
  na = numusearray(t, nums);  
  totaluse = na;  
  totaluse += numusehash(t, nums, &na);  
  
  if (ttisinteger(ek))
    na += countint(ivalue(ek), nums);
  totaluse++;
  
  asize = computesizes(nums, &na);
  
  luaH_resize(L, t, asize, totaluse - na);
}






Table *luaH_new (lua_State *L) {
  GCObject *o = luaC_newobj(L, LUA_VTABLE, sizeof(Table));
  Table *t = gco2t(o);
  t->metatable = NULL;
  t->flags = cast_byte(maskflags);  
  t->array = NULL;
  t->alimit = 0;
  setnodevector(L, t, 0);
  return t;
}



void luaH_free (lua_State *L, Table *t) {
  unsigned int realsize = luaH_realasize(t);
  size_t sizeb = concretesize(realsize);
  lua_assert((sizeb == 0 && realsize == 0) ||
             cast_charp(t->array) + sizeb - sizeof(Value) ==
             cast_charp(getArrVal(t, realsize - 1)));
  freehash(L, t);
  luaM_freemem(L, t->array, sizeb);
  luaM_free(L, t);
}


static Node *getfreepos (Table *t) {
  if (haslastfree(t)) {  
    
    while (getlastfree(t) > t->node) {
      Node *free = --getlastfree(t);
      if (keyisnil(free))
        return free;
    }
  }
  else {  
    if (!isdummy(t)) {
      int i = sizenode(t);
      while (i--) {  
        Node *free = gnode(t, i);
        if (keyisnil(free))
          return free;
      }
    }
  }
  return NULL;  
}




static void luaH_newkey (lua_State *L, Table *t, const TValue *key,
                                                 TValue *value) {
  Node *mp;
  TValue aux;
  if (l_unlikely(ttisnil(key)))
    luaG_runerror(L, "table index is nil");
  else if (ttisfloat(key)) {
    lua_Number f = fltvalue(key);
    lua_Integer k;
    if (luaV_flttointeger(f, &k, F2Ieq)) {  
      setivalue(&aux, k);
      key = &aux;  
    }
    else if (l_unlikely(luai_numisnan(f)))
      luaG_runerror(L, "table index is NaN");
  }
  if (ttisnil(value))
    return;  
  mp = mainpositionTV(t, key);
  if (!isempty(gval(mp)) || isdummy(t)) {  
    Node *othern;
    Node *f = getfreepos(t);  
    if (f == NULL) {  
      rehash(L, t, key);  
      
      luaH_set(L, t, key, value);  
      return;
    }
    lua_assert(!isdummy(t));
    othern = mainpositionfromnode(t, mp);
    if (othern != mp) {  
      
      while (othern + gnext(othern) != mp)  
        othern += gnext(othern);
      gnext(othern) = cast_int(f - othern);  
      *f = *mp;  
      if (gnext(mp) != 0) {
        gnext(f) += cast_int(mp - f);  
        gnext(mp) = 0;  
      }
      setempty(gval(mp));
    }
    else {  
      
      if (gnext(mp) != 0)
        gnext(f) = cast_int((mp + gnext(mp)) - f);  
      else lua_assert(gnext(f) == 0);
      gnext(mp) = cast_int(f - mp);
      mp = f;
    }
  }
  setnodekey(L, mp, key);
  luaC_barrierback(L, obj2gco(t), key);
  lua_assert(isempty(gval(mp)));
  setobj2t(L, gval(mp), value);
}


static const TValue *getintfromhash (Table *t, lua_Integer key) {
  Node *n = hashint(t, key);
  lua_assert(l_castS2U(key) - 1u >= luaH_realasize(t));
  for (;;) {  
    if (keyisinteger(n) && keyival(n) == key)
      return gval(n);  
    else {
      int nx = gnext(n);
      if (nx == 0) break;
      n += nx;
    }
  }
  return &absentkey;
}


static int hashkeyisempty (Table *t, lua_Integer key) {
  const TValue *val = getintfromhash(t, key);
  return isempty(val);
}


static int finishnodeget (const TValue *val, TValue *res) {
  if (!ttisnil(val)) {
    setobj(((lua_State*)NULL), res, val);
  }
  return ttypetag(val);
}


int luaH_getint (Table *t, lua_Integer key, TValue *res) {
  if (keyinarray(t, key)) {
    int tag = *getArrTag(t, key - 1);
    if (!tagisempty(tag))
      farr2val(t, key, tag, res);
    return tag;
  }
  else
    return finishnodeget(getintfromhash(t, key), res);
}



const TValue *luaH_Hgetshortstr (Table *t, TString *key) {
  Node *n = hashstr(t, key);
  lua_assert(key->tt == LUA_VSHRSTR);
  for (;;) {  
    if (keyisshrstr(n) && eqshrstr(keystrval(n), key))
      return gval(n);  
    else {
      int nx = gnext(n);
      if (nx == 0)
        return &absentkey;  
      n += nx;
    }
  }
}


int luaH_getshortstr (Table *t, TString *key, TValue *res) {
  return finishnodeget(luaH_Hgetshortstr(t, key), res);
}


static const TValue *Hgetstr (Table *t, TString *key) {
  if (key->tt == LUA_VSHRSTR)
    return luaH_Hgetshortstr(t, key);
  else {  
    TValue ko;
    setsvalue(cast(lua_State *, NULL), &ko, key);
    return getgeneric(t, &ko, 0);
  }
}


int luaH_getstr (Table *t, TString *key, TValue *res) {
  return finishnodeget(Hgetstr(t, key), res);
}


TString *luaH_getstrkey (Table *t, TString *key) {
  const TValue *o = Hgetstr(t, key);
  if (!isabstkey(o))  
    return keystrval(nodefromval(o));  
  else
    return NULL;
}



int luaH_get (Table *t, const TValue *key, TValue *res) {
  const TValue *slot;
  switch (ttypetag(key)) {
    case LUA_VSHRSTR:
      slot = luaH_Hgetshortstr(t, tsvalue(key));
      break;
    case LUA_VNUMINT:
      return luaH_getint(t, ivalue(key), res);
    case LUA_VNIL:
      slot = &absentkey;
      break;
    case LUA_VNUMFLT: {
      lua_Integer k;
      if (luaV_flttointeger(fltvalue(key), &k, F2Ieq)) 
        return luaH_getint(t, k, res);  
      
    }  
    default:
      slot = getgeneric(t, key, 0);
      break;
  }
  return finishnodeget(slot, res);
}


static int finishnodeset (Table *t, const TValue *slot, TValue *val) {
  if (!ttisnil(slot)) {
    setobj(((lua_State*)NULL), cast(TValue*, slot), val);
    return HOK;  
  }
  else if (isabstkey(slot))
    return HNOTFOUND;  
  else  
    return cast_int((cast(Node*, slot) - t->node)) + HFIRSTNODE;
}


static int rawfinishnodeset (const TValue *slot, TValue *val) {
  if (isabstkey(slot))
    return 0;  
  else {
    setobj(((lua_State*)NULL), cast(TValue*, slot), val);
    return 1;  
  }
}


int luaH_psetint (Table *t, lua_Integer key, TValue *val) {
  if (keyinarray(t, key)) {
    lu_byte *tag = getArrTag(t, key - 1);
    if (!tagisempty(*tag) || checknoTM(t->metatable, TM_NEWINDEX)) {
      fval2arr(t, key, tag, val);
      return HOK;  
    }
    else
      return ~cast_int(key);  
  }
  else
    return finishnodeset(t, getintfromhash(t, key), val);
}


int luaH_psetshortstr (Table *t, TString *key, TValue *val) {
  return finishnodeset(t, luaH_Hgetshortstr(t, key), val);
}


int luaH_psetstr (Table *t, TString *key, TValue *val) {
  return finishnodeset(t, Hgetstr(t, key), val);
}


int luaH_pset (Table *t, const TValue *key, TValue *val) {
  switch (ttypetag(key)) {
    case LUA_VSHRSTR: return luaH_psetshortstr(t, tsvalue(key), val);
    case LUA_VNUMINT: return luaH_psetint(t, ivalue(key), val);
    case LUA_VNIL: return HNOTFOUND;
    case LUA_VNUMFLT: {
      lua_Integer k;
      if (luaV_flttointeger(fltvalue(key), &k, F2Ieq)) 
        return luaH_psetint(t, k, val);  
      
    }  
    default:
      return finishnodeset(t, getgeneric(t, key, 0), val);
  }
}




void luaH_finishset (lua_State *L, Table *t, const TValue *key,
                                    TValue *value, int hres) {
  lua_assert(hres != HOK);
  if (hres == HNOTFOUND) {
    luaH_newkey(L, t, key, value);
  }
  else if (hres > 0) {  
    setobj2t(L, gval(gnode(t, hres - HFIRSTNODE)), value);
  }
  else {  
    hres = ~hres;  
    obj2arr(t, hres, value);
  }
}



void luaH_set (lua_State *L, Table *t, const TValue *key, TValue *value) {
  int hres = luaH_pset(t, key, value);
  if (hres != HOK)
    luaH_finishset(L, t, key, value, hres);
}



void luaH_setint (lua_State *L, Table *t, lua_Integer key, TValue *value) {
  if (keyinarray(t, key))
    obj2arr(t, key, value);
  else {
    int ok = rawfinishnodeset(getintfromhash(t, key), value);
    if (!ok) {
      TValue k;
      setivalue(&k, key);
      luaH_newkey(L, t, &k, value);
    }
  }
}



static lua_Unsigned hash_search (Table *t, lua_Unsigned j) {
  lua_Unsigned i;
  if (j == 0) j++;  
  do {
    i = j;  
    if (j <= l_castS2U(LUA_MAXINTEGER) / 2)
      j *= 2;
    else {
      j = LUA_MAXINTEGER;
      if (hashkeyisempty(t, j))  
        break;  
      else  
        return j;  
    }
  } while (!hashkeyisempty(t, j));  
  
  while (j - i > 1u) {  
    lua_Unsigned m = (i + j) / 2;
    if (hashkeyisempty(t, m)) j = m;
    else i = m;
  }
  return i;
}


static unsigned int binsearch (Table *array, unsigned int i, unsigned int j) {
  while (j - i > 1u) {  
    unsigned int m = (i + j) / 2;
    if (arraykeyisempty(array, m)) j = m;
    else i = m;
  }
  return i;
}



lua_Unsigned luaH_getn (Table *t) {
  unsigned int limit = t->alimit;
  if (limit > 0 && arraykeyisempty(t, limit)) {  
    
    if (limit >= 2 && !arraykeyisempty(t, limit - 1)) {
      
      if (ispow2realasize(t) && !ispow2(limit - 1)) {
        t->alimit = limit - 1;
        setnorealasize(t);  
      }
      return limit - 1;
    }
    else {  
      unsigned int boundary = binsearch(t, 0, limit);
      
      if (ispow2realasize(t) && boundary > luaH_realasize(t) / 2) {
        t->alimit = boundary;  
        setnorealasize(t);
      }
      return boundary;
    }
  }
  
  if (!limitequalsasize(t)) {  
    
    if (arraykeyisempty(t, limit + 1))  
      return limit;  
    
    limit = luaH_realasize(t);
    if (arraykeyisempty(t, limit)) {  
      
      unsigned int boundary = binsearch(t, t->alimit, limit);
      t->alimit = boundary;
      return boundary;
    }
    
  }
  
  lua_assert(limit == luaH_realasize(t) &&
             (limit == 0 || !arraykeyisempty(t, limit)));
  if (isdummy(t) || hashkeyisempty(t, cast(lua_Integer, limit + 1)))
    return limit;  
  else  
    return hash_search(t, limit);
}



#if defined(LUA_DEBUG)



Node *luaH_mainposition (const Table *t, const TValue *key) {
  return mainpositionTV(t, key);
}

#endif


//path: src/imports/../dependencies/lua/ldo.c


#define ldo_c
#define LUA_CORE





#include <setjmp.h>
#include <stdlib.h>
#include <string.h>







































#define errorstatus(s)	((s) > LUA_YIELD)





#if !defined(LUAI_THROW)				

#if defined(__cplusplus) && !defined(LUA_USE_LONGJMP)	


#define LUAI_THROW(L,c)		throw(c)
#define LUAI_TRY(L,c,a) \
	try { a } catch(...) { if ((c)->status == 0) (c)->status = -1; }
#define luai_jmpbuf		int  

#elif defined(LUA_USE_POSIX)				


#define LUAI_THROW(L,c)		_longjmp((c)->b, 1)
#define LUAI_TRY(L,c,a)		if (_setjmp((c)->b) == 0) { a }
#define luai_jmpbuf		jmp_buf

#else							


#define LUAI_THROW(L,c)		longjmp((c)->b, 1)
#define LUAI_TRY(L,c,a)		if (setjmp((c)->b) == 0) { a }
#define luai_jmpbuf		jmp_buf

#endif							

#endif							




struct lua_longjmp {
  struct lua_longjmp *previous;
  luai_jmpbuf b;
  volatile int status;  
};


void luaD_seterrorobj (lua_State *L, int errcode, StkId oldtop) {
  switch (errcode) {
    case LUA_ERRMEM: {  
      setsvalue2s(L, oldtop, G(L)->memerrmsg); 
      break;
    }
    case LUA_ERRERR: {
      setsvalue2s(L, oldtop, luaS_newliteral(L, "error in private_lua_error handling"));
      break;
    }
    case LUA_OK: {  
      setnilvalue(s2v(oldtop));  
      break;
    }
    default: {
      lua_assert(errorstatus(errcode));  
      setobjs2s(L, oldtop, L->top.p - 1);  
      break;
    }
  }
  L->top.p = oldtop + 1;
}


l_noret luaD_throw (lua_State *L, int errcode) {
  if (L->errorJmp) {  
    L->errorJmp->status = errcode;  
    LUAI_THROW(L, L->errorJmp);  
  }
  else {  
    global_State *g = G(L);
    errcode = luaE_resetthread(L, errcode);  
    if (g->mainthread->errorJmp) {  
      setobjs2s(L, g->mainthread->top.p++, L->top.p - 1);  
      luaD_throw(g->mainthread, errcode);  
    }
    else {  
      if (g->panic) {  
        lua_unlock(L);
        g->panic(L);  
      }
      abort();
    }
  }
}


int luaD_rawrunprotected (lua_State *L, Pfunc f, void *ud) {
  l_uint32 oldnCcalls = L->nCcalls;
  struct lua_longjmp lj;
  lj.status = LUA_OK;
  lj.previous = L->errorJmp;  
  L->errorJmp = &lj;
  LUAI_TRY(L, &lj,
    (*f)(L, ud);
  );
  L->errorJmp = lj.previous;  
  L->nCcalls = oldnCcalls;
  return lj.status;
}








static void relstack (lua_State *L) {
  CallInfo *ci;
  UpVal *up;
  L->top.offset = savestack(L, L->top.p);
  L->tbclist.offset = savestack(L, L->tbclist.p);
  for (up = L->openupval; up != NULL; up = up->u.open.next)
    up->v.offset = savestack(L, uplevel(up));
  for (ci = L->ci; ci != NULL; ci = ci->previous) {
    ci->top.offset = savestack(L, ci->top.p);
    ci->func.offset = savestack(L, ci->func.p);
  }
}



static void correctstack (lua_State *L) {
  CallInfo *ci;
  UpVal *up;
  L->top.p = restorestack(L, L->top.offset);
  L->tbclist.p = restorestack(L, L->tbclist.offset);
  for (up = L->openupval; up != NULL; up = up->u.open.next)
    up->v.p = s2v(restorestack(L, up->v.offset));
  for (ci = L->ci; ci != NULL; ci = ci->previous) {
    ci->top.p = restorestack(L, ci->top.offset);
    ci->func.p = restorestack(L, ci->func.offset);
    if (isLua(ci))
      ci->u.l.trap = 1;  
  }
}



#define ERRORSTACKSIZE	(LUAI_MAXSTACK + 200)


int luaD_reallocstack (lua_State *L, int newsize, int raiseerror) {
  int oldsize = stacksize(L);
  int i;
  StkId newstack;
  int oldgcstop = G(L)->gcstopem;
  lua_assert(newsize <= LUAI_MAXSTACK || newsize == ERRORSTACKSIZE);
  relstack(L);  
  G(L)->gcstopem = 1;  
  newstack = luaM_reallocvector(L, L->stack.p, oldsize + EXTRA_STACK,
                                   newsize + EXTRA_STACK, StackValue);
  G(L)->gcstopem = oldgcstop;  
  if (l_unlikely(newstack == NULL)) {  
    correctstack(L);  
    if (raiseerror)
      luaM_error(L);
    else return 0;  
  }
  L->stack.p = newstack;
  correctstack(L);  
  L->stack_last.p = L->stack.p + newsize;
  for (i = oldsize + EXTRA_STACK; i < newsize + EXTRA_STACK; i++)
    setnilvalue(s2v(newstack + i)); 
  return 1;
}



int luaD_growstack (lua_State *L, int n, int raiseerror) {
  int size = stacksize(L);
  if (l_unlikely(size > LUAI_MAXSTACK)) {
    
    lua_assert(stacksize(L) == ERRORSTACKSIZE);
    if (raiseerror)
      luaD_throw(L, LUA_ERRERR);  
    return 0;  
  }
  else if (n < LUAI_MAXSTACK) {  
    int newsize = 2 * size;  
    int needed = cast_int(L->top.p - L->stack.p) + n;
    if (newsize > LUAI_MAXSTACK)  
      newsize = LUAI_MAXSTACK;
    if (newsize < needed)  
      newsize = needed;
    if (l_likely(newsize <= LUAI_MAXSTACK))
      return luaD_reallocstack(L, newsize, raiseerror);
  }
  
  
  luaD_reallocstack(L, ERRORSTACKSIZE, raiseerror);
  if (raiseerror)
    luaG_runerror(L, "stack overflow");
  return 0;
}



static int stackinuse (lua_State *L) {
  CallInfo *ci;
  int res;
  StkId lim = L->top.p;
  for (ci = L->ci; ci != NULL; ci = ci->previous) {
    if (lim < ci->top.p) lim = ci->top.p;
  }
  lua_assert(lim <= L->stack_last.p + EXTRA_STACK);
  res = cast_int(lim - L->stack.p) + 1;  
  if (res < LUA_MINSTACK)
    res = LUA_MINSTACK;  
  return res;
}



void luaD_shrinkstack (lua_State *L) {
  int inuse = stackinuse(L);
  int max = (inuse > LUAI_MAXSTACK / 3) ? LUAI_MAXSTACK : inuse * 3;
  
  if (inuse <= LUAI_MAXSTACK && stacksize(L) > max) {
    int nsize = (inuse > LUAI_MAXSTACK / 2) ? LUAI_MAXSTACK : inuse * 2;
    luaD_reallocstack(L, nsize, 0);  
  }
  else  
    condmovestack(L,{},{});  
  luaE_shrinkCI(L);  
}


void luaD_inctop (lua_State *L) {
  luaD_checkstack(L, 1);
  L->top.p++;
}





void luaD_hook (lua_State *L, int event, int line,
                              int ftransfer, int ntransfer) {
  lua_Hook hook = L->hook;
  if (hook && L->allowhook) {  
    int mask = CIST_HOOKED;
    CallInfo *ci = L->ci;
    ptrdiff_t top = savestack(L, L->top.p);  
    ptrdiff_t ci_top = savestack(L, ci->top.p);  
    lua_Debug ar;
    ar.event = event;
    ar.currentline = line;
    ar.i_ci = ci;
    if (ntransfer != 0) {
      mask |= CIST_TRAN;  
      ci->u2.transferinfo.ftransfer = ftransfer;
      ci->u2.transferinfo.ntransfer = ntransfer;
    }
    if (isLua(ci) && L->top.p < ci->top.p)
      L->top.p = ci->top.p;  
    luaD_checkstack(L, LUA_MINSTACK);  
    if (ci->top.p < L->top.p + LUA_MINSTACK)
      ci->top.p = L->top.p + LUA_MINSTACK;
    L->allowhook = 0;  
    ci->callstatus |= mask;
    lua_unlock(L);
    (*hook)(L, &ar);
    lua_lock(L);
    lua_assert(!L->allowhook);
    L->allowhook = 1;
    ci->top.p = restorestack(L, ci_top);
    L->top.p = restorestack(L, top);
    ci->callstatus &= ~mask;
  }
}



void luaD_hookcall (lua_State *L, CallInfo *ci) {
  L->oldpc = 0;  
  if (L->hookmask & LUA_MASKCALL) {  
    int event = (ci->callstatus & CIST_TAIL) ? LUA_HOOKTAILCALL
                                             : LUA_HOOKCALL;
    Proto *p = ci_func(ci)->p;
    ci->u.l.savedpc++;  
    luaD_hook(L, event, -1, 1, p->numparams);
    ci->u.l.savedpc--;  
  }
}



static void rethook (lua_State *L, CallInfo *ci, int nres) {
  if (L->hookmask & LUA_MASKRET) {  
    StkId firstres = L->top.p - nres;  
    int delta = 0;  
    int ftransfer;
    if (isLua(ci)) {
      Proto *p = ci_func(ci)->p;
      if (p->flag & PF_ISVARARG)
        delta = ci->u.l.nextraargs + p->numparams + 1;
    }
    ci->func.p += delta;  
    ftransfer = cast(unsigned short, firstres - ci->func.p);
    luaD_hook(L, LUA_HOOKRET, -1, ftransfer, nres);  
    ci->func.p -= delta;
  }
  if (isLua(ci = ci->previous))
    L->oldpc = pcRel(ci->u.l.savedpc, ci_func(ci)->p);  
}



static StkId tryfuncTM (lua_State *L, StkId func) {
  const TValue *tm;
  StkId p;
  checkstackp(L, 1, func);  
  tm = luaT_gettmbyobj(L, s2v(func), TM_CALL);  
  if (l_unlikely(ttisnil(tm)))
    luaG_callerror(L, s2v(func));  
  for (p = L->top.p; p > func; p--)  
    setobjs2s(L, p, p-1);
  L->top.p++;  
  setobj2s(L, func, tm);  
  return func;
}



l_sinline void moveresults (lua_State *L, StkId res, int nres, int wanted) {
  StkId firstresult;
  int i;
  switch (wanted) {  
    case 0:  
      L->top.p = res;
      return;
    case 1:  
      if (nres == 0)   
        setnilvalue(s2v(res));  
      else  
        setobjs2s(L, res, L->top.p - nres);  
      L->top.p = res + 1;
      return;
    case LUA_MULTRET:
      wanted = nres;  
      break;
    default:  
      if (hastocloseCfunc(wanted)) {  
        L->ci->callstatus |= CIST_CLSRET;  
        L->ci->u2.nres = nres;
        res = luaF_close(L, res, CLOSEKTOP, 1);
        L->ci->callstatus &= ~CIST_CLSRET;
        if (L->hookmask) {  
          ptrdiff_t savedres = savestack(L, res);
          rethook(L, L->ci, nres);
          res = restorestack(L, savedres);  
        }
        wanted = decodeNresults(wanted);
        if (wanted == LUA_MULTRET)
          wanted = nres;  
      }
      break;
  }
  
  firstresult = L->top.p - nres;  
  if (nres > wanted)  
    nres = wanted;  
  for (i = 0; i < nres; i++)  
    setobjs2s(L, res + i, firstresult + i);
  for (; i < wanted; i++)  
    setnilvalue(s2v(res + i));
  L->top.p = res + wanted;  
}



void luaD_poscall (lua_State *L, CallInfo *ci, int nres) {
  int wanted = ci->nresults;
  if (l_unlikely(L->hookmask && !hastocloseCfunc(wanted)))
    rethook(L, ci, nres);
  
  moveresults(L, ci->func.p, nres, wanted);
  
  lua_assert(!(ci->callstatus &
        (CIST_HOOKED | CIST_YPCALL | CIST_FIN | CIST_TRAN | CIST_CLSRET)));
  L->ci = ci->previous;  
}



#define next_ci(L)  (L->ci->next ? L->ci->next : luaE_extendCI(L))


l_sinline CallInfo *prepCallInfo (lua_State *L, StkId func, int nret,
                                                int mask, StkId top) {
  CallInfo *ci = L->ci = next_ci(L);  
  ci->func.p = func;
  ci->nresults = nret;
  ci->callstatus = mask;
  ci->top.p = top;
  return ci;
}



l_sinline int precallC (lua_State *L, StkId func, int nresults,
                                            lua_CFunction f) {
  int n;  
  CallInfo *ci;
  checkstackp(L, LUA_MINSTACK, func);  
  L->ci = ci = prepCallInfo(L, func, nresults, CIST_C,
                               L->top.p + LUA_MINSTACK);
  lua_assert(ci->top.p <= L->stack_last.p);
  if (l_unlikely(L->hookmask & LUA_MASKCALL)) {
    int narg = cast_int(L->top.p - func) - 1;
    luaD_hook(L, LUA_HOOKCALL, -1, 1, narg);
  }
  lua_unlock(L);
  n = (*f)(L);  
  lua_lock(L);
  api_checknelems(L, n);
  luaD_poscall(L, ci, n);
  return n;
}



int luaD_pretailcall (lua_State *L, CallInfo *ci, StkId func,
                                    int narg1, int delta) {
 retry:
  switch (ttypetag(s2v(func))) {
    case LUA_VCCL:  
      return precallC(L, func, LUA_MULTRET, clCvalue(s2v(func))->f);
    case LUA_VLCF:  
      return precallC(L, func, LUA_MULTRET, fvalue(s2v(func)));
    case LUA_VLCL: {  
      Proto *p = clLvalue(s2v(func))->p;
      int fsize = p->maxstacksize;  
      int nfixparams = p->numparams;
      int i;
      checkstackp(L, fsize - delta, func);
      ci->func.p -= delta;  
      for (i = 0; i < narg1; i++)  
        setobjs2s(L, ci->func.p + i, func + i);
      func = ci->func.p;  
      for (; narg1 <= nfixparams; narg1++)
        setnilvalue(s2v(func + narg1));  
      ci->top.p = func + 1 + fsize;  
      lua_assert(ci->top.p <= L->stack_last.p);
      ci->u.l.savedpc = p->code;  
      ci->callstatus |= CIST_TAIL;
      L->top.p = func + narg1;  
      return -1;
    }
    default: {  
      func = tryfuncTM(L, func);  
      
      narg1++;
      goto retry;  
    }
  }
}



CallInfo *luaD_precall (lua_State *L, StkId func, int nresults) {
 retry:
  switch (ttypetag(s2v(func))) {
    case LUA_VCCL:  
      precallC(L, func, nresults, clCvalue(s2v(func))->f);
      return NULL;
    case LUA_VLCF:  
      precallC(L, func, nresults, fvalue(s2v(func)));
      return NULL;
    case LUA_VLCL: {  
      CallInfo *ci;
      Proto *p = clLvalue(s2v(func))->p;
      int narg = cast_int(L->top.p - func) - 1;  
      int nfixparams = p->numparams;
      int fsize = p->maxstacksize;  
      checkstackp(L, fsize, func);
      L->ci = ci = prepCallInfo(L, func, nresults, 0, func + 1 + fsize);
      ci->u.l.savedpc = p->code;  
      for (; narg < nfixparams; narg++)
        setnilvalue(s2v(L->top.p++));  
      lua_assert(ci->top.p <= L->stack_last.p);
      return ci;
    }
    default: {  
      func = tryfuncTM(L, func);  
      
      goto retry;  
    }
  }
}



l_sinline void ccall (lua_State *L, StkId func, int nResults, l_uint32 inc) {
  CallInfo *ci;
  L->nCcalls += inc;
  if (l_unlikely(getCcalls(L) >= LUAI_MAXCCALLS)) {
    checkstackp(L, 0, func);  
    luaE_checkcstack(L);
  }
  if ((ci = luaD_precall(L, func, nResults)) != NULL) {  
    ci->callstatus = CIST_FRESH;  
    luaV_execute(L, ci);  
  }
  L->nCcalls -= inc;
}



void luaD_call (lua_State *L, StkId func, int nResults) {
  ccall(L, func, nResults, 1);
}



void luaD_callnoyield (lua_State *L, StkId func, int nResults) {
  ccall(L, func, nResults, nyci);
}



static int finishpcallk (lua_State *L,  CallInfo *ci) {
  int status = getcistrecst(ci);  
  if (l_likely(status == LUA_OK))  
    status = LUA_YIELD;  
  else {  
    StkId func = restorestack(L, ci->u2.funcidx);
    L->allowhook = getoah(ci->callstatus);  
    func = luaF_close(L, func, status, 1);  
    luaD_seterrorobj(L, status, func);
    luaD_shrinkstack(L);   
    setcistrecst(ci, LUA_OK);  
  }
  ci->callstatus &= ~CIST_YPCALL;
  L->errfunc = ci->u.c.old_errfunc;
  
  return status;
}



static void finishCcall (lua_State *L, CallInfo *ci) {
  int n;  
  if (ci->callstatus & CIST_CLSRET) {  
    lua_assert(hastocloseCfunc(ci->nresults));
    n = ci->u2.nres;  
    
  }
  else {
    int status = LUA_YIELD;  
    
    lua_assert(ci->u.c.k != NULL && yieldable(L));
    if (ci->callstatus & CIST_YPCALL)   
      status = finishpcallk(L, ci);  
    adjustresults(L, LUA_MULTRET);  
    lua_unlock(L);
    n = (*ci->u.c.k)(L, status, ci->u.c.ctx);  
    lua_lock(L);
    api_checknelems(L, n);
  }
  luaD_poscall(L, ci, n);  
}



static void unroll (lua_State *L, void *ud) {
  CallInfo *ci;
  UNUSED(ud);
  while ((ci = L->ci) != &L->base_ci) {  
    if (!isLua(ci))  
      finishCcall(L, ci);  
    else {  
      luaV_finishOp(L);  
      luaV_execute(L, ci);  
    }
  }
}



static CallInfo *findpcall (lua_State *L) {
  CallInfo *ci;
  for (ci = L->ci; ci != NULL; ci = ci->previous) {  
    if (ci->callstatus & CIST_YPCALL)
      return ci;
  }
  return NULL;  
}



static int resume_error (lua_State *L, const char *msg, int narg) {
  api_checkpop(L, narg);
  L->top.p -= narg;  
  setsvalue2s(L, L->top.p, luaS_new(L, msg));  
  api_incr_top(L);
  lua_unlock(L);
  return LUA_ERRRUN;
}



static void resume (lua_State *L, void *ud) {
  int n = *(cast(int*, ud));  
  StkId firstArg = L->top.p - n;  
  CallInfo *ci = L->ci;
  if (L->status == LUA_OK)  
    ccall(L, firstArg - 1, LUA_MULTRET, 0);  
  else {  
    lua_assert(L->status == LUA_YIELD);
    L->status = LUA_OK;  
    if (isLua(ci)) {  
      
      lua_assert(ci->callstatus & CIST_HOOKYIELD);
      ci->u.l.savedpc--;
      L->top.p = firstArg;  
      luaV_execute(L, ci);  
    }
    else {  
      if (ci->u.c.k != NULL) {  
        lua_unlock(L);
        n = (*ci->u.c.k)(L, LUA_YIELD, ci->u.c.ctx); 
        lua_lock(L);
        api_checknelems(L, n);
      }
      luaD_poscall(L, ci, n);  
    }
    unroll(L, NULL);  
  }
}



static int precover (lua_State *L, int status) {
  CallInfo *ci;
  while (errorstatus(status) && (ci = findpcall(L)) != NULL) {
    L->ci = ci;  
    setcistrecst(ci, status);  
    status = luaD_rawrunprotected(L, unroll, NULL);
  }
  return status;
}


LUA_API int lua_resume (lua_State *L, lua_State *from, int nargs,
                                      int *nresults) {
  int status;
  lua_lock(L);
  if (L->status == LUA_OK) {  
    if (L->ci != &L->base_ci)  
      return resume_error(L, "cannot resume non-suspended coroutine", nargs);
    else if (L->top.p - (L->ci->func.p + 1) == nargs)  
      return resume_error(L, "cannot resume dead coroutine", nargs);
  }
  else if (L->status != LUA_YIELD)  
    return resume_error(L, "cannot resume dead coroutine", nargs);
  L->nCcalls = (from) ? getCcalls(from) : 0;
  if (getCcalls(L) >= LUAI_MAXCCALLS)
    return resume_error(L, "C stack overflow", nargs);
  L->nCcalls++;
  luai_userstateresume(L, nargs);
  api_checkpop(L, (L->status == LUA_OK) ? nargs + 1 : nargs);
  status = luaD_rawrunprotected(L, resume, &nargs);
   
  status = precover(L, status);
  if (l_likely(!errorstatus(status)))
    lua_assert(status == L->status);  
  else {  
    L->status = cast_byte(status);  
    luaD_seterrorobj(L, status, L->top.p);  
    L->ci->top.p = L->top.p;
  }
  *nresults = (status == LUA_YIELD) ? L->ci->u2.nyield
                                    : cast_int(L->top.p - (L->ci->func.p + 1));
  lua_unlock(L);
  return status;
}


LUA_API int lua_isyieldable (lua_State *L) {
  return yieldable(L);
}


LUA_API int lua_yieldk (lua_State *L, int nresults, lua_KContext ctx,
                        lua_KFunction k) {
  CallInfo *ci;
  luai_userstateyield(L, nresults);
  lua_lock(L);
  ci = L->ci;
  api_checkpop(L, nresults);
  if (l_unlikely(!yieldable(L))) {
    if (L != G(L)->mainthread)
      luaG_runerror(L, "attempt to yield across a C-call boundary");
    else
      luaG_runerror(L, "attempt to yield from outside a coroutine");
  }
  L->status = LUA_YIELD;
  ci->u2.nyield = nresults;  
  if (isLua(ci)) {  
    lua_assert(!isLuacode(ci));
    api_check(L, nresults == 0, "hooks cannot yield values");
    api_check(L, k == NULL, "hooks cannot continue after yielding");
  }
  else {
    if ((ci->u.c.k = k) != NULL)  
      ci->u.c.ctx = ctx;  
    luaD_throw(L, LUA_YIELD);
  }
  lua_assert(ci->callstatus & CIST_HOOKED);  
  lua_unlock(L);
  return 0;  
}



struct CloseP {
  StkId level;
  int status;
};



static void closepaux (lua_State *L, void *ud) {
  struct CloseP *pcl = cast(struct CloseP *, ud);
  luaF_close(L, pcl->level, pcl->status, 0);
}



int luaD_closeprotected (lua_State *L, ptrdiff_t level, int status) {
  CallInfo *old_ci = L->ci;
  lu_byte old_allowhooks = L->allowhook;
  for (;;) {  
    struct CloseP pcl;
    pcl.level = restorestack(L, level); pcl.status = status;
    status = luaD_rawrunprotected(L, &closepaux, &pcl);
    if (l_likely(status == LUA_OK))  
      return pcl.status;
    else {  
      L->ci = old_ci;
      L->allowhook = old_allowhooks;
    }
  }
}



int luaD_pcall (lua_State *L, Pfunc func, void *u,
                ptrdiff_t old_top, ptrdiff_t ef) {
  int status;
  CallInfo *old_ci = L->ci;
  lu_byte old_allowhooks = L->allowhook;
  ptrdiff_t old_errfunc = L->errfunc;
  L->errfunc = ef;
  status = luaD_rawrunprotected(L, func, u);
  if (l_unlikely(status != LUA_OK)) {  
    L->ci = old_ci;
    L->allowhook = old_allowhooks;
    status = luaD_closeprotected(L, old_top, status);
    luaD_seterrorobj(L, status, restorestack(L, old_top));
    luaD_shrinkstack(L);   
  }
  L->errfunc = old_errfunc;
  return status;
}




struct SParser {  
  ZIO *z;
  Mbuffer buff;  
  Dyndata dyd;  
  const char *mode;
  const char *name;
};


static void checkmode (lua_State *L, const char *mode, const char *x) {
  if (strchr(mode, x[0]) == NULL) {
    luaO_pushfstring(L,
       "attempt to load a %s chunk (mode is '%s')", x, mode);
    luaD_throw(L, LUA_ERRSYNTAX);
  }
}


static void f_parser (lua_State *L, void *ud) {
  LClosure *cl;
  struct SParser *p = cast(struct SParser *, ud);
  const char *mode = p->mode ? p->mode : "bt";
  int c = zgetc(p->z);  
  if (c == LUA_SIGNATURE[0]) {
    int fixed = 0;
    if (strchr(mode, 'B') != NULL)
      fixed = 1;
    else
      checkmode(L, mode, "binary");
    cl = luaU_undump(L, p->z, p->name, fixed);
  }
  else {
    checkmode(L, mode, "text");
    cl = luaY_parser(L, p->z, &p->buff, &p->dyd, p->name, c);
  }
  lua_assert(cl->nupvalues == cl->p->sizeupvalues);
  luaF_initupvals(L, cl);
}


int luaD_protectedparser (lua_State *L, ZIO *z, const char *name,
                                        const char *mode) {
  struct SParser p;
  int status;
  incnny(L);  
  p.z = z; p.name = name; p.mode = mode;
  p.dyd.actvar.arr = NULL; p.dyd.actvar.size = 0;
  p.dyd.gt.arr = NULL; p.dyd.gt.size = 0;
  p.dyd.label.arr = NULL; p.dyd.label.size = 0;
  luaZ_initbuffer(L, &p.buff);
  status = luaD_pcall(L, f_parser, &p, savestack(L, L->top.p), L->errfunc);
  luaZ_freebuffer(L, &p.buff);
  luaM_freearray(L, p.dyd.actvar.arr, p.dyd.actvar.size);
  luaM_freearray(L, p.dyd.gt.arr, p.dyd.gt.size);
  luaM_freearray(L, p.dyd.label.arr, p.dyd.label.size);
  decnny(L);
  return status;
}




//path: src/imports/../dependencies/lua/lvm.c


#define lvm_c
#define LUA_CORE




#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





























#if !defined(LUA_USE_JUMPTABLE)
#if defined(__GNUC__)
#define LUA_USE_JUMPTABLE	1
#else
#define LUA_USE_JUMPTABLE	0
#endif
#endif




#define MAXTAGLOOP	2000





#define NBM		(l_floatatt(MANT_DIG))


#if ((((LUA_MAXINTEGER >> (NBM / 4)) >> (NBM / 4)) >> (NBM / 4)) \
	>> (NBM - (3 * (NBM / 4))))  >  0


#define MAXINTFITSF	((lua_Unsigned)1 << NBM)


#define l_intfitsf(i)	((MAXINTFITSF + l_castS2U(i)) <= (2 * MAXINTFITSF))

#else  

#define l_intfitsf(i)	1

#endif



static int l_strton (const TValue *obj, TValue *result) {
  lua_assert(obj != result);
  if (!cvt2num(obj))  
    return 0;
  else {
  TString *st = tsvalue(obj);
  size_t stlen;
  const char *s = getlstr(st, stlen);
  return (luaO_str2num(s, result) == stlen + 1);
  }
}



int luaV_tonumber_ (const TValue *obj, lua_Number *n) {
  TValue v;
  if (ttisinteger(obj)) {
    *n = cast_num(ivalue(obj));
    return 1;
  }
  else if (l_strton(obj, &v)) {  
    *n = nvalue(&v);  
    return 1;
  }
  else
    return 0;  
}



int luaV_flttointeger (lua_Number n, lua_Integer *p, F2Imod mode) {
  lua_Number f = l_floor(n);
  if (n != f) {  
    if (mode == F2Ieq) return 0;  
    else if (mode == F2Iceil)  
      f += 1;  
  }
  return lua_numbertointeger(f, p);
}



int luaV_tointegerns (const TValue *obj, lua_Integer *p, F2Imod mode) {
  if (ttisfloat(obj))
    return luaV_flttointeger(fltvalue(obj), p, mode);
  else if (ttisinteger(obj)) {
    *p = ivalue(obj);
    return 1;
  }
  else
    return 0;
}



int luaV_tointeger (const TValue *obj, lua_Integer *p, F2Imod mode) {
  TValue v;
  if (l_strton(obj, &v))  
    obj = &v;  
  return luaV_tointegerns(obj, p, mode);
}



static int forlimit (lua_State *L, lua_Integer init, const TValue *lim,
                                   lua_Integer *p, lua_Integer step) {
  if (!luaV_tointeger(lim, p, (step < 0 ? F2Iceil : F2Ifloor))) {
    
    lua_Number flim;  
    if (!tonumber(lim, &flim)) 
      luaG_forerror(L, lim, "limit");
    
    if (luai_numlt(0, flim)) {  
      if (step < 0) return 1;  
      *p = LUA_MAXINTEGER;  
    }
    else {  
      if (step > 0) return 1;  
      *p = LUA_MININTEGER;  
    }
  }
  return (step > 0 ? init > *p : init < *p);  
}



static int forprep (lua_State *L, StkId ra) {
  TValue *pinit = s2v(ra);
  TValue *plimit = s2v(ra + 1);
  TValue *pstep = s2v(ra + 2);
  if (ttisinteger(pinit) && ttisinteger(pstep)) { 
    lua_Integer init = ivalue(pinit);
    lua_Integer step = ivalue(pstep);
    lua_Integer limit;
    if (step == 0)
      luaG_runerror(L, "'for' step is zero");
    if (forlimit(L, init, plimit, &limit, step))
      return 1;  
    else {  
      lua_Unsigned count;
      if (step > 0) {  
        count = l_castS2U(limit) - l_castS2U(init);
        if (step != 1)  
          count /= l_castS2U(step);
      }
      else {  
        count = l_castS2U(init) - l_castS2U(limit);
        
        count /= l_castS2U(-(step + 1)) + 1u;
      }
      
      chgivalue(s2v(ra), l_castU2S(count));  
      setivalue(s2v(ra + 1), step);  
      chgivalue(s2v(ra + 2), init);  
    }
  }
  else {  
    lua_Number init; lua_Number limit; lua_Number step;
    if (l_unlikely(!tonumber(plimit, &limit)))
      luaG_forerror(L, plimit, "limit");
    if (l_unlikely(!tonumber(pstep, &step)))
      luaG_forerror(L, pstep, "step");
    if (l_unlikely(!tonumber(pinit, &init)))
      luaG_forerror(L, pinit, "initial value");
    if (step == 0)
      luaG_runerror(L, "'for' step is zero");
    if (luai_numlt(0, step) ? luai_numlt(limit, init)
                            : luai_numlt(init, limit))
      return 1;  
    else {
      
      setfltvalue(s2v(ra), limit);
      setfltvalue(s2v(ra + 1), step);
      setfltvalue(s2v(ra + 2), init);  
    }
  }
  return 0;
}



static int floatforloop (StkId ra) {
  lua_Number step = fltvalue(s2v(ra + 1));
  lua_Number limit = fltvalue(s2v(ra));
  lua_Number idx = fltvalue(s2v(ra + 2));  
  idx = luai_numadd(L, idx, step);  
  if (luai_numlt(0, step) ? luai_numle(idx, limit)
                          : luai_numle(limit, idx)) {
    chgfltvalue(s2v(ra + 2), idx);  
    return 1;  
  }
  else
    return 0;  
}



int luaV_finishget (lua_State *L, const TValue *t, TValue *key, StkId val,
                      int tag) {
  int loop;  
  const TValue *tm;  
  for (loop = 0; loop < MAXTAGLOOP; loop++) {
    if (tag == LUA_VNOTABLE) {  
      lua_assert(!ttistable(t));
      tm = luaT_gettmbyobj(L, t, TM_INDEX);
      if (l_unlikely(notm(tm)))
        luaG_typeerror(L, t, "index");  
      
    }
    else {  
      tm = fasttm(L, hvalue(t)->metatable, TM_INDEX);  
      if (tm == NULL) {  
        setnilvalue(s2v(val));  
        return LUA_VNIL;
      }
      
    }
    if (ttisfunction(tm)) {  
      tag = luaT_callTMres(L, tm, t, key, val);  
      return tag;  
    }
    t = tm;  
    luaV_fastget(t, key, s2v(val), luaH_get, tag);
    if (!tagisempty(tag))
      return tag;  
    
  }
  luaG_runerror(L, "'__index' chain too long; possible loop");
  return 0;  
}



void luaV_finishset (lua_State *L, const TValue *t, TValue *key,
                      TValue *val, int hres) {
  int loop;  
  for (loop = 0; loop < MAXTAGLOOP; loop++) {
    const TValue *tm;  
    if (hres != HNOTATABLE) {  
      Table *h = hvalue(t);  
      tm = fasttm(L, h->metatable, TM_NEWINDEX);  
      if (tm == NULL) {  
        luaH_finishset(L, h, key, val, hres);  
        invalidateTMcache(h);
        luaC_barrierback(L, obj2gco(h), val);
        return;
      }
      
    }
    else {  
      tm = luaT_gettmbyobj(L, t, TM_NEWINDEX);
      if (l_unlikely(notm(tm)))
        luaG_typeerror(L, t, "index");
    }
    
    if (ttisfunction(tm)) {
      luaT_callTM(L, tm, t, key, val);
      return;
    }
    t = tm;  
    luaV_fastset(t, key, val, hres, luaH_pset);
    if (hres == HOK)
      return;  
    
  }
  luaG_runerror(L, "'__newindex' chain too long; possible loop");
}



static int l_strcmp (const TString *ts1, const TString *ts2) {
  size_t rl1;  
  const char *s1 = getlstr(ts1, rl1);
  size_t rl2;
  const char *s2 = getlstr(ts2, rl2);
  for (;;) {  
    int temp = strcoll(s1, s2);
    if (temp != 0)  
      return temp;  
    else {  
      size_t zl1 = strlen(s1);  
      size_t zl2 = strlen(s2);  
      if (zl2 == rl2)  
        return (zl1 == rl1) ? 0 : 1;  
      else if (zl1 == rl1)  
        return -1;  
      
      zl1++; zl2++;
      s1 += zl1; rl1 -= zl1; s2 += zl2; rl2 -= zl2;
    }
  }
}



l_sinline int LTintfloat (lua_Integer i, lua_Number f) {
  if (l_intfitsf(i))
    return luai_numlt(cast_num(i), f);  
  else {  
    lua_Integer fi;
    if (luaV_flttointeger(f, &fi, F2Iceil))  
      return i < fi;   
    else  
      return f > 0;  
  }
}



l_sinline int LEintfloat (lua_Integer i, lua_Number f) {
  if (l_intfitsf(i))
    return luai_numle(cast_num(i), f);  
  else {  
    lua_Integer fi;
    if (luaV_flttointeger(f, &fi, F2Ifloor))  
      return i <= fi;   
    else  
      return f > 0;  
  }
}



l_sinline int LTfloatint (lua_Number f, lua_Integer i) {
  if (l_intfitsf(i))
    return luai_numlt(f, cast_num(i));  
  else {  
    lua_Integer fi;
    if (luaV_flttointeger(f, &fi, F2Ifloor))  
      return fi < i;   
    else  
      return f < 0;  
  }
}



l_sinline int LEfloatint (lua_Number f, lua_Integer i) {
  if (l_intfitsf(i))
    return luai_numle(f, cast_num(i));  
  else {  
    lua_Integer fi;
    if (luaV_flttointeger(f, &fi, F2Iceil))  
      return fi <= i;   
    else  
      return f < 0;  
  }
}



l_sinline int LTnum (const TValue *l, const TValue *r) {
  lua_assert(ttisnumber(l) && ttisnumber(r));
  if (ttisinteger(l)) {
    lua_Integer li = ivalue(l);
    if (ttisinteger(r))
      return li < ivalue(r);  
    else  
      return LTintfloat(li, fltvalue(r));  
  }
  else {
    lua_Number lf = fltvalue(l);  
    if (ttisfloat(r))
      return luai_numlt(lf, fltvalue(r));  
    else  
      return LTfloatint(lf, ivalue(r));
  }
}



l_sinline int LEnum (const TValue *l, const TValue *r) {
  lua_assert(ttisnumber(l) && ttisnumber(r));
  if (ttisinteger(l)) {
    lua_Integer li = ivalue(l);
    if (ttisinteger(r))
      return li <= ivalue(r);  
    else  
      return LEintfloat(li, fltvalue(r));  
  }
  else {
    lua_Number lf = fltvalue(l);  
    if (ttisfloat(r))
      return luai_numle(lf, fltvalue(r));  
    else  
      return LEfloatint(lf, ivalue(r));
  }
}



static int lessthanothers (lua_State *L, const TValue *l, const TValue *r) {
  lua_assert(!ttisnumber(l) || !ttisnumber(r));
  if (ttisstring(l) && ttisstring(r))  
    return l_strcmp(tsvalue(l), tsvalue(r)) < 0;
  else
    return luaT_callorderTM(L, l, r, TM_LT);
}



int luaV_lessthan (lua_State *L, const TValue *l, const TValue *r) {
  if (ttisnumber(l) && ttisnumber(r))  
    return LTnum(l, r);
  else return lessthanothers(L, l, r);
}



static int lessequalothers (lua_State *L, const TValue *l, const TValue *r) {
  lua_assert(!ttisnumber(l) || !ttisnumber(r));
  if (ttisstring(l) && ttisstring(r))  
    return l_strcmp(tsvalue(l), tsvalue(r)) <= 0;
  else
    return luaT_callorderTM(L, l, r, TM_LE);
}



int luaV_lessequal (lua_State *L, const TValue *l, const TValue *r) {
  if (ttisnumber(l) && ttisnumber(r))  
    return LEnum(l, r);
  else return lessequalothers(L, l, r);
}



int luaV_equalobj (lua_State *L, const TValue *t1, const TValue *t2) {
  const TValue *tm;
  if (ttypetag(t1) != ttypetag(t2)) {  
    if (ttype(t1) != ttype(t2) || ttype(t1) != LUA_TNUMBER)
      return 0;  
    else {  
      
      lua_Integer i1, i2;
      return (luaV_tointegerns(t1, &i1, F2Ieq) &&
              luaV_tointegerns(t2, &i2, F2Ieq) &&
              i1 == i2);
    }
  }
  
  switch (ttypetag(t1)) {
    case LUA_VNIL: case LUA_VFALSE: case LUA_VTRUE: return 1;
    case LUA_VNUMINT: return (ivalue(t1) == ivalue(t2));
    case LUA_VNUMFLT: return luai_numeq(fltvalue(t1), fltvalue(t2));
    case LUA_VLIGHTUSERDATA: return pvalue(t1) == pvalue(t2);
    case LUA_VLCF: return fvalue(t1) == fvalue(t2);
    case LUA_VSHRSTR: return eqshrstr(tsvalue(t1), tsvalue(t2));
    case LUA_VLNGSTR: return luaS_eqlngstr(tsvalue(t1), tsvalue(t2));
    case LUA_VUSERDATA: {
      if (uvalue(t1) == uvalue(t2)) return 1;
      else if (L == NULL) return 0;
      tm = fasttm(L, uvalue(t1)->metatable, TM_EQ);
      if (tm == NULL)
        tm = fasttm(L, uvalue(t2)->metatable, TM_EQ);
      break;  
    }
    case LUA_VTABLE: {
      if (hvalue(t1) == hvalue(t2)) return 1;
      else if (L == NULL) return 0;
      tm = fasttm(L, hvalue(t1)->metatable, TM_EQ);
      if (tm == NULL)
        tm = fasttm(L, hvalue(t2)->metatable, TM_EQ);
      break;  
    }
    default:
      return gcvalue(t1) == gcvalue(t2);
  }
  if (tm == NULL)  
    return 0;  
  else {
    int tag = luaT_callTMres(L, tm, t1, t2, L->top.p);  
    return !tagisfalse(tag);
  }
}



#define tostring(L,o)  \
	(ttisstring(o) || (cvt2str(o) && (luaO_tostring(L, o), 1)))

#define isemptystr(o)	(ttisshrstring(o) && tsvalue(o)->shrlen == 0)


static void copy2buff (StkId top, int n, char *buff) {
  size_t tl = 0;  
  do {
    TString *st = tsvalue(s2v(top - n));
    size_t l;  
    const char *s = getlstr(st, l);
    memcpy(buff + tl, s, l * sizeof(char));
    tl += l;
  } while (--n > 0);
}



void luaV_concat (lua_State *L, int total) {
  if (total == 1)
    return;  
  do {
    StkId top = L->top.p;
    int n = 2;  
    if (!(ttisstring(s2v(top - 2)) || cvt2str(s2v(top - 2))) ||
        !tostring(L, s2v(top - 1)))
      luaT_tryconcatTM(L);  
    else if (isemptystr(s2v(top - 1)))  
      cast_void(tostring(L, s2v(top - 2)));  
    else if (isemptystr(s2v(top - 2))) {  
      setobjs2s(L, top - 2, top - 1);  
    }
    else {
      
      size_t tl = tsslen(tsvalue(s2v(top - 1)));
      TString *ts;
      
      for (n = 1; n < total && tostring(L, s2v(top - n - 1)); n++) {
        size_t l = tsslen(tsvalue(s2v(top - n - 1)));
        if (l_unlikely(l >= MAX_SIZE - sizeof(TString) - tl)) {
          L->top.p = top - total;  
          luaG_runerror(L, "string length overflow");
        }
        tl += l;
      }
      if (tl <= LUAI_MAXSHORTLEN) {  
        char buff[LUAI_MAXSHORTLEN];
        copy2buff(top, n, buff);  
        ts = luaS_newlstr(L, buff, tl);
      }
      else {  
        ts = luaS_createlngstrobj(L, tl);
        copy2buff(top, n, getlngstr(ts));
      }
      setsvalue2s(L, top - n, ts);  
    }
    total -= n - 1;  
    L->top.p -= n - 1;  
  } while (total > 1);  
}



void luaV_objlen (lua_State *L, StkId ra, const TValue *rb) {
  const TValue *tm;
  switch (ttypetag(rb)) {
    case LUA_VTABLE: {
      Table *h = hvalue(rb);
      tm = fasttm(L, h->metatable, TM_LEN);
      if (tm) break;  
      setivalue(s2v(ra), luaH_getn(h));  
      return;
    }
    case LUA_VSHRSTR: {
      setivalue(s2v(ra), tsvalue(rb)->shrlen);
      return;
    }
    case LUA_VLNGSTR: {
      setivalue(s2v(ra), tsvalue(rb)->u.lnglen);
      return;
    }
    default: {  
      tm = luaT_gettmbyobj(L, rb, TM_LEN);
      if (l_unlikely(notm(tm)))  
        luaG_typeerror(L, rb, "get length of");
      break;
    }
  }
  luaT_callTMres(L, tm, rb, rb, ra);
}



lua_Integer luaV_idiv (lua_State *L, lua_Integer m, lua_Integer n) {
  if (l_unlikely(l_castS2U(n) + 1u <= 1u)) {  
    if (n == 0)
      luaG_runerror(L, "attempt to divide by zero");
    return intop(-, 0, m);   
  }
  else {
    lua_Integer q = m / n;  
    if ((m ^ n) < 0 && m % n != 0)  
      q -= 1;  
    return q;
  }
}



lua_Integer luaV_mod (lua_State *L, lua_Integer m, lua_Integer n) {
  if (l_unlikely(l_castS2U(n) + 1u <= 1u)) {  
    if (n == 0)
      luaG_runerror(L, "attempt to perform 'n%%0'");
    return 0;   
  }
  else {
    lua_Integer r = m % n;
    if (r != 0 && (r ^ n) < 0)  
      r += n;  
    return r;
  }
}



lua_Number luaV_modf (lua_State *L, lua_Number m, lua_Number n) {
  lua_Number r;
  luai_nummod(L, m, n, r);
  return r;
}



#define NBITS	cast_int(sizeof(lua_Integer) * CHAR_BIT)



lua_Integer luaV_shiftl (lua_Integer x, lua_Integer y) {
  if (y < 0) {  
    if (y <= -NBITS) return 0;
    else return intop(>>, x, -y);
  }
  else {  
    if (y >= NBITS) return 0;
    else return intop(<<, x, y);
  }
}



static void pushclosure (lua_State *L, Proto *p, UpVal **encup, StkId base,
                         StkId ra) {
  int nup = p->sizeupvalues;
  Upvaldesc *uv = p->upvalues;
  int i;
  LClosure *ncl = luaF_newLclosure(L, nup);
  ncl->p = p;
  setclLvalue2s(L, ra, ncl);  
  for (i = 0; i < nup; i++) {  
    if (uv[i].instack)  
      ncl->upvals[i] = luaF_findupval(L, base + uv[i].idx);
    else  
      ncl->upvals[i] = encup[uv[i].idx];
    luaC_objbarrier(L, ncl, ncl->upvals[i]);
  }
}



void luaV_finishOp (lua_State *L) {
  CallInfo *ci = L->ci;
  StkId base = ci->func.p + 1;
  Instruction inst = *(ci->u.l.savedpc - 1);  
  OpCode op = GET_OPCODE(inst);
  switch (op) {  
    case OP_MMBIN: case OP_MMBINI: case OP_MMBINK: {
      setobjs2s(L, base + GETARG_A(*(ci->u.l.savedpc - 2)), --L->top.p);
      break;
    }
    case OP_UNM: case OP_BNOT: case OP_LEN:
    case OP_GETTABUP: case OP_GETTABLE: case OP_GETI:
    case OP_GETFIELD: case OP_SELF: {
      setobjs2s(L, base + GETARG_A(inst), --L->top.p);
      break;
    }
    case OP_LT: case OP_LE:
    case OP_LTI: case OP_LEI:
    case OP_GTI: case OP_GEI:
    case OP_EQ: {  
      int res = !l_isfalse(s2v(L->top.p - 1));
      L->top.p--;
#if defined(LUA_COMPAT_LT_LE)
      if (ci->callstatus & CIST_LEQ) {  
        ci->callstatus ^= CIST_LEQ;  
        res = !res;  
      }
#endif
      lua_assert(GET_OPCODE(*ci->u.l.savedpc) == OP_JMP);
      if (res != GETARG_k(inst))  
        ci->u.l.savedpc++;  
      break;
    }
    case OP_CONCAT: {
      StkId top = L->top.p - 1;  
      int a = GETARG_A(inst);      
      int total = cast_int(top - 1 - (base + a));  
      setobjs2s(L, top - 2, top);  
      L->top.p = top - 1;  
      luaV_concat(L, total);  
      break;
    }
    case OP_CLOSE: {  
      ci->u.l.savedpc--;  
      break;
    }
    case OP_RETURN: {  
      StkId ra = base + GETARG_A(inst);
      
      L->top.p = ra + ci->u2.nres;
      
      ci->u.l.savedpc--;
      break;
    }
    default: {
      
      lua_assert(op == OP_TFORCALL || op == OP_CALL ||
           op == OP_TAILCALL || op == OP_SETTABUP || op == OP_SETTABLE ||
           op == OP_SETI || op == OP_SETFIELD);
      break;
    }
  }
}






#define l_addi(L,a,b)	intop(+, a, b)
#define l_subi(L,a,b)	intop(-, a, b)
#define l_muli(L,a,b)	intop(*, a, b)
#define l_band(a,b)	intop(&, a, b)
#define l_bor(a,b)	intop(|, a, b)
#define l_bxor(a,b)	intop(^, a, b)

#define l_lti(a,b)	(a < b)
#define l_lei(a,b)	(a <= b)
#define l_gti(a,b)	(a > b)
#define l_gei(a,b)	(a >= b)



#define op_arithI(L,iop,fop) {  \
  StkId ra = RA(i); \
  TValue *v1 = vRB(i);  \
  int imm = GETARG_sC(i);  \
  if (ttisinteger(v1)) {  \
    lua_Integer iv1 = ivalue(v1);  \
    pc++; setivalue(s2v(ra), iop(L, iv1, imm));  \
  }  \
  else if (ttisfloat(v1)) {  \
    lua_Number nb = fltvalue(v1);  \
    lua_Number fimm = cast_num(imm);  \
    pc++; setfltvalue(s2v(ra), fop(L, nb, fimm)); \
  }}



#define op_arithf_aux(L,v1,v2,fop) {  \
  lua_Number n1; lua_Number n2;  \
  if (tonumberns(v1, n1) && tonumberns(v2, n2)) {  \
    pc++; setfltvalue(s2v(ra), fop(L, n1, n2));  \
  }}



#define op_arithf(L,fop) {  \
  StkId ra = RA(i); \
  TValue *v1 = vRB(i);  \
  TValue *v2 = vRC(i);  \
  op_arithf_aux(L, v1, v2, fop); }



#define op_arithfK(L,fop) {  \
  StkId ra = RA(i); \
  TValue *v1 = vRB(i);  \
  TValue *v2 = KC(i); lua_assert(ttisnumber(v2));  \
  op_arithf_aux(L, v1, v2, fop); }



#define op_arith_aux(L,v1,v2,iop,fop) {  \
  StkId ra = RA(i); \
  if (ttisinteger(v1) && ttisinteger(v2)) {  \
    lua_Integer i1 = ivalue(v1); lua_Integer i2 = ivalue(v2);  \
    pc++; setivalue(s2v(ra), iop(L, i1, i2));  \
  }  \
  else op_arithf_aux(L, v1, v2, fop); }



#define op_arith(L,iop,fop) {  \
  TValue *v1 = vRB(i);  \
  TValue *v2 = vRC(i);  \
  op_arith_aux(L, v1, v2, iop, fop); }



#define op_arithK(L,iop,fop) {  \
  TValue *v1 = vRB(i);  \
  TValue *v2 = KC(i); lua_assert(ttisnumber(v2));  \
  op_arith_aux(L, v1, v2, iop, fop); }



#define op_bitwiseK(L,op) {  \
  StkId ra = RA(i); \
  TValue *v1 = vRB(i);  \
  TValue *v2 = KC(i);  \
  lua_Integer i1;  \
  lua_Integer i2 = ivalue(v2);  \
  if (tointegerns(v1, &i1)) {  \
    pc++; setivalue(s2v(ra), op(i1, i2));  \
  }}



#define op_bitwise(L,op) {  \
  StkId ra = RA(i); \
  TValue *v1 = vRB(i);  \
  TValue *v2 = vRC(i);  \
  lua_Integer i1; lua_Integer i2;  \
  if (tointegerns(v1, &i1) && tointegerns(v2, &i2)) {  \
    pc++; setivalue(s2v(ra), op(i1, i2));  \
  }}



#define op_order(L,opi,opn,other) {  \
  StkId ra = RA(i); \
  int cond;  \
  TValue *rb = vRB(i);  \
  if (ttisinteger(s2v(ra)) && ttisinteger(rb)) {  \
    lua_Integer ia = ivalue(s2v(ra));  \
    lua_Integer ib = ivalue(rb);  \
    cond = opi(ia, ib);  \
  }  \
  else if (ttisnumber(s2v(ra)) && ttisnumber(rb))  \
    cond = opn(s2v(ra), rb);  \
  else  \
    Protect(cond = other(L, s2v(ra), rb));  \
  docondjump(); }



#define op_orderI(L,opi,opf,inv,tm) {  \
  StkId ra = RA(i); \
  int cond;  \
  int im = GETARG_sB(i);  \
  if (ttisinteger(s2v(ra)))  \
    cond = opi(ivalue(s2v(ra)), im);  \
  else if (ttisfloat(s2v(ra))) {  \
    lua_Number fa = fltvalue(s2v(ra));  \
    lua_Number fim = cast_num(im);  \
    cond = opf(fa, fim);  \
  }  \
  else {  \
    int isf = GETARG_C(i);  \
    Protect(cond = luaT_callorderiTM(L, s2v(ra), im, inv, isf, tm));  \
  }  \
  docondjump(); }









#define RA(i)	(base+GETARG_A(i))
#define RB(i)	(base+GETARG_B(i))
#define vRB(i)	s2v(RB(i))
#define KB(i)	(k+GETARG_B(i))
#define RC(i)	(base+GETARG_C(i))
#define vRC(i)	s2v(RC(i))
#define KC(i)	(k+GETARG_C(i))
#define RKC(i)	((TESTARG_k(i)) ? k + GETARG_C(i) : s2v(base + GETARG_C(i)))



#define updatetrap(ci)  (trap = ci->u.l.trap)

#define updatebase(ci)	(base = ci->func.p + 1)


#define updatestack(ci)  \
	{ if (l_unlikely(trap)) { updatebase(ci); ra = RA(i); } }



#define dojump(ci,i,e)	{ pc += GETARG_sJ(i) + e; updatetrap(ci); }



#define donextjump(ci)	{ Instruction ni = *pc; dojump(ci, ni, 1); }


#define docondjump()	if (cond != GETARG_k(i)) pc++; else donextjump(ci);



#define savepc(L)	(ci->u.l.savedpc = pc)



#define savestate(L,ci)		(savepc(L), L->top.p = ci->top.p)



#define Protect(exp)  (savestate(L,ci), (exp), updatetrap(ci))


#define ProtectNT(exp)  (savepc(L), (exp), updatetrap(ci))


#define halfProtect(exp)  (savestate(L,ci), (exp))


#define checkGC(L,c)  \
	{ luaC_condGC(L, (savepc(L), L->top.p = (c)), \
                         updatetrap(ci)); \
           luai_threadyield(L); }



#define vmfetch()	{ \
  if (l_unlikely(trap)) {   \
    trap = luaG_traceexec(L, pc);   \
    updatebase(ci);   \
  } \
  i = *(pc++); \
}

#define vmdispatch(o)	switch(o)
#define vmcase(l)	case l:
#define vmbreak		break


void luaV_execute (lua_State *L, CallInfo *ci) {
  LClosure *cl;
  TValue *k;
  StkId base;
  const Instruction *pc;
  int trap;
#if LUA_USE_JUMPTABLE
//path: src/imports/../dependencies/lua/ljumptab.h



#undef vmdispatch
#undef vmcase
#undef vmbreak

#define vmdispatch(x)     goto *disptab[x];

#define vmcase(l)     L_##l:

#define vmbreak		vmfetch(); vmdispatch(GET_OPCODE(i));


static const void *const disptab[NUM_OPCODES] = {

#if 0
** you can update the following list with this command:
**
**  sed -n '/^OP_/\!d; s/OP_/\&\&L_OP_/ ; s/,.*/,/ ; s/\/.*// ; p'  lopcodes.h
**
#endif

&&L_OP_MOVE,
&&L_OP_LOADI,
&&L_OP_LOADF,
&&L_OP_LOADK,
&&L_OP_LOADKX,
&&L_OP_LOADFALSE,
&&L_OP_LFALSESKIP,
&&L_OP_LOADTRUE,
&&L_OP_LOADNIL,
&&L_OP_GETUPVAL,
&&L_OP_SETUPVAL,
&&L_OP_GETTABUP,
&&L_OP_GETTABLE,
&&L_OP_GETI,
&&L_OP_GETFIELD,
&&L_OP_SETTABUP,
&&L_OP_SETTABLE,
&&L_OP_SETI,
&&L_OP_SETFIELD,
&&L_OP_NEWTABLE,
&&L_OP_SELF,
&&L_OP_ADDI,
&&L_OP_ADDK,
&&L_OP_SUBK,
&&L_OP_MULK,
&&L_OP_MODK,
&&L_OP_POWK,
&&L_OP_DIVK,
&&L_OP_IDIVK,
&&L_OP_BANDK,
&&L_OP_BORK,
&&L_OP_BXORK,
&&L_OP_SHRI,
&&L_OP_SHLI,
&&L_OP_ADD,
&&L_OP_SUB,
&&L_OP_MUL,
&&L_OP_MOD,
&&L_OP_POW,
&&L_OP_DIV,
&&L_OP_IDIV,
&&L_OP_BAND,
&&L_OP_BOR,
&&L_OP_BXOR,
&&L_OP_SHL,
&&L_OP_SHR,
&&L_OP_MMBIN,
&&L_OP_MMBINI,
&&L_OP_MMBINK,
&&L_OP_UNM,
&&L_OP_BNOT,
&&L_OP_NOT,
&&L_OP_LEN,
&&L_OP_CONCAT,
&&L_OP_CLOSE,
&&L_OP_TBC,
&&L_OP_JMP,
&&L_OP_EQ,
&&L_OP_LT,
&&L_OP_LE,
&&L_OP_EQK,
&&L_OP_EQI,
&&L_OP_LTI,
&&L_OP_LEI,
&&L_OP_GTI,
&&L_OP_GEI,
&&L_OP_TEST,
&&L_OP_TESTSET,
&&L_OP_CALL,
&&L_OP_TAILCALL,
&&L_OP_RETURN,
&&L_OP_RETURN0,
&&L_OP_RETURN1,
&&L_OP_FORLOOP,
&&L_OP_FORPREP,
&&L_OP_TFORPREP,
&&L_OP_TFORCALL,
&&L_OP_TFORLOOP,
&&L_OP_SETLIST,
&&L_OP_CLOSURE,
&&L_OP_VARARG,
&&L_OP_VARARGPREP,
&&L_OP_EXTRAARG

};


#endif
 startfunc:
  trap = L->hookmask;
 returning:  
  cl = ci_func(ci);
  k = cl->p->k;
  pc = ci->u.l.savedpc;
  if (l_unlikely(trap))
    trap = luaG_tracecall(L);
  base = ci->func.p + 1;
  
  for (;;) {
    Instruction i;  
    vmfetch();
    #if 0
      
      printf("line: %d\n", luaG_getfuncline(cl->p, pcRel(pc, cl->p)));
    #endif
    lua_assert(base == ci->func.p + 1);
    lua_assert(base <= L->top.p && L->top.p <= L->stack_last.p);
    
    lua_assert(isIT(i) || (cast_void(L->top.p = base), 1));
    vmdispatch (GET_OPCODE(i)) {
      vmcase(OP_MOVE) {
        StkId ra = RA(i);
        setobjs2s(L, ra, RB(i));
        vmbreak;
      }
      vmcase(OP_LOADI) {
        StkId ra = RA(i);
        lua_Integer b = GETARG_sBx(i);
        setivalue(s2v(ra), b);
        vmbreak;
      }
      vmcase(OP_LOADF) {
        StkId ra = RA(i);
        int b = GETARG_sBx(i);
        setfltvalue(s2v(ra), cast_num(b));
        vmbreak;
      }
      vmcase(OP_LOADK) {
        StkId ra = RA(i);
        TValue *rb = k + GETARG_Bx(i);
        setobj2s(L, ra, rb);
        vmbreak;
      }
      vmcase(OP_LOADKX) {
        StkId ra = RA(i);
        TValue *rb;
        rb = k + GETARG_Ax(*pc); pc++;
        setobj2s(L, ra, rb);
        vmbreak;
      }
      vmcase(OP_LOADFALSE) {
        StkId ra = RA(i);
        setbfvalue(s2v(ra));
        vmbreak;
      }
      vmcase(OP_LFALSESKIP) {
        StkId ra = RA(i);
        setbfvalue(s2v(ra));
        pc++;  
        vmbreak;
      }
      vmcase(OP_LOADTRUE) {
        StkId ra = RA(i);
        setbtvalue(s2v(ra));
        vmbreak;
      }
      vmcase(OP_LOADNIL) {
        StkId ra = RA(i);
        int b = GETARG_B(i);
        do {
          setnilvalue(s2v(ra++));
        } while (b--);
        vmbreak;
      }
      vmcase(OP_GETUPVAL) {
        StkId ra = RA(i);
        int b = GETARG_B(i);
        setobj2s(L, ra, cl->upvals[b]->v.p);
        vmbreak;
      }
      vmcase(OP_SETUPVAL) {
        StkId ra = RA(i);
        UpVal *uv = cl->upvals[GETARG_B(i)];
        setobj(L, uv->v.p, s2v(ra));
        luaC_barrier(L, uv, s2v(ra));
        vmbreak;
      }
      vmcase(OP_GETTABUP) {
        StkId ra = RA(i);
        TValue *upval = cl->upvals[GETARG_B(i)]->v.p;
        TValue *rc = KC(i);
        TString *key = tsvalue(rc);  
        int tag;
        luaV_fastget(upval, key, s2v(ra), luaH_getshortstr, tag);
        if (tagisempty(tag))
          Protect(luaV_finishget(L, upval, rc, ra, tag));
        vmbreak;
      }
      vmcase(OP_GETTABLE) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        TValue *rc = vRC(i);
        int tag;
        if (ttisinteger(rc)) {  
          luaV_fastgeti(rb, ivalue(rc), s2v(ra), tag);
        }
        else
          luaV_fastget(rb, rc, s2v(ra), luaH_get, tag);
        if (tagisempty(tag))
          Protect(luaV_finishget(L, rb, rc, ra, tag));
        vmbreak;
      }
      vmcase(OP_GETI) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        int c = GETARG_C(i);
        int tag;
        luaV_fastgeti(rb, c, s2v(ra), tag);
        if (tagisempty(tag)) {
          TValue key;
          setivalue(&key, c);
          Protect(luaV_finishget(L, rb, &key, ra, tag));
        }
        vmbreak;
      }
      vmcase(OP_GETFIELD) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        TValue *rc = KC(i);
        TString *key = tsvalue(rc);  
        int tag;
        luaV_fastget(rb, key, s2v(ra), luaH_getshortstr, tag);
        if (tagisempty(tag))
          Protect(luaV_finishget(L, rb, rc, ra, tag));
        vmbreak;
      }
      vmcase(OP_SETTABUP) {
        int hres;
        TValue *upval = cl->upvals[GETARG_A(i)]->v.p;
        TValue *rb = KB(i);
        TValue *rc = RKC(i);
        TString *key = tsvalue(rb);  
        luaV_fastset(upval, key, rc, hres, luaH_psetshortstr);
        if (hres == HOK)
          luaV_finishfastset(L, upval, rc);
        else
          Protect(luaV_finishset(L, upval, rb, rc, hres));
        vmbreak;
      }
      vmcase(OP_SETTABLE) {
        StkId ra = RA(i);
        int hres;
        TValue *rb = vRB(i);  
        TValue *rc = RKC(i);  
        if (ttisinteger(rb)) {  
          luaV_fastseti(s2v(ra), ivalue(rb), rc, hres);
        }
        else {
          luaV_fastset(s2v(ra), rb, rc, hres, luaH_pset);
        }
        if (hres == HOK)
          luaV_finishfastset(L, s2v(ra), rc);
        else
          Protect(luaV_finishset(L, s2v(ra), rb, rc, hres));
        vmbreak;
      }
      vmcase(OP_SETI) {
        StkId ra = RA(i);
        int hres;
        int b = GETARG_B(i);
        TValue *rc = RKC(i);
        luaV_fastseti(s2v(ra), b, rc, hres);
        if (hres == HOK)
          luaV_finishfastset(L, s2v(ra), rc);
        else {
          TValue key;
          setivalue(&key, b);
          Protect(luaV_finishset(L, s2v(ra), &key, rc, hres));
        }
        vmbreak;
      }
      vmcase(OP_SETFIELD) {
        StkId ra = RA(i);
        int hres;
        TValue *rb = KB(i);
        TValue *rc = RKC(i);
        TString *key = tsvalue(rb);  
        luaV_fastset(s2v(ra), key, rc, hres, luaH_psetshortstr);
        if (hres == HOK)
          luaV_finishfastset(L, s2v(ra), rc);
        else
          Protect(luaV_finishset(L, s2v(ra), rb, rc, hres));
        vmbreak;
      }
      vmcase(OP_NEWTABLE) {
        StkId ra = RA(i);
        int b = GETARG_B(i);  
        int c = GETARG_C(i);  
        Table *t;
        if (b > 0)
          b = 1 << (b - 1);  
        lua_assert((!TESTARG_k(i)) == (GETARG_Ax(*pc) == 0));
        if (TESTARG_k(i))  
          c += GETARG_Ax(*pc) * (MAXARG_C + 1);  
        pc++;  
        L->top.p = ra + 1;  
        t = luaH_new(L);  
        sethvalue2s(L, ra, t);
        if (b != 0 || c != 0)
          luaH_resize(L, t, c, b);  
        checkGC(L, ra + 1);
        vmbreak;
      }
      vmcase(OP_SELF) {
        StkId ra = RA(i);
        int tag;
        TValue *rb = vRB(i);
        TValue *rc = RKC(i);
        TString *key = tsvalue(rc);  
        setobj2s(L, ra + 1, rb);
        luaV_fastget(rb, key, s2v(ra), luaH_getstr, tag);
        if (tagisempty(tag))
          Protect(luaV_finishget(L, rb, rc, ra, tag));
        vmbreak;
      }
      vmcase(OP_ADDI) {
        op_arithI(L, l_addi, luai_numadd);
        vmbreak;
      }
      vmcase(OP_ADDK) {
        op_arithK(L, l_addi, luai_numadd);
        vmbreak;
      }
      vmcase(OP_SUBK) {
        op_arithK(L, l_subi, luai_numsub);
        vmbreak;
      }
      vmcase(OP_MULK) {
        op_arithK(L, l_muli, luai_nummul);
        vmbreak;
      }
      vmcase(OP_MODK) {
        savestate(L, ci);  
        op_arithK(L, luaV_mod, luaV_modf);
        vmbreak;
      }
      vmcase(OP_POWK) {
        op_arithfK(L, luai_numpow);
        vmbreak;
      }
      vmcase(OP_DIVK) {
        op_arithfK(L, luai_numdiv);
        vmbreak;
      }
      vmcase(OP_IDIVK) {
        savestate(L, ci);  
        op_arithK(L, luaV_idiv, luai_numidiv);
        vmbreak;
      }
      vmcase(OP_BANDK) {
        op_bitwiseK(L, l_band);
        vmbreak;
      }
      vmcase(OP_BORK) {
        op_bitwiseK(L, l_bor);
        vmbreak;
      }
      vmcase(OP_BXORK) {
        op_bitwiseK(L, l_bxor);
        vmbreak;
      }
      vmcase(OP_SHRI) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        int ic = GETARG_sC(i);
        lua_Integer ib;
        if (tointegerns(rb, &ib)) {
          pc++; setivalue(s2v(ra), luaV_shiftl(ib, -ic));
        }
        vmbreak;
      }
      vmcase(OP_SHLI) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        int ic = GETARG_sC(i);
        lua_Integer ib;
        if (tointegerns(rb, &ib)) {
          pc++; setivalue(s2v(ra), luaV_shiftl(ic, ib));
        }
        vmbreak;
      }
      vmcase(OP_ADD) {
        op_arith(L, l_addi, luai_numadd);
        vmbreak;
      }
      vmcase(OP_SUB) {
        op_arith(L, l_subi, luai_numsub);
        vmbreak;
      }
      vmcase(OP_MUL) {
        op_arith(L, l_muli, luai_nummul);
        vmbreak;
      }
      vmcase(OP_MOD) {
        savestate(L, ci);  
        op_arith(L, luaV_mod, luaV_modf);
        vmbreak;
      }
      vmcase(OP_POW) {
        op_arithf(L, luai_numpow);
        vmbreak;
      }
      vmcase(OP_DIV) {  
        op_arithf(L, luai_numdiv);
        vmbreak;
      }
      vmcase(OP_IDIV) {  
        savestate(L, ci);  
        op_arith(L, luaV_idiv, luai_numidiv);
        vmbreak;
      }
      vmcase(OP_BAND) {
        op_bitwise(L, l_band);
        vmbreak;
      }
      vmcase(OP_BOR) {
        op_bitwise(L, l_bor);
        vmbreak;
      }
      vmcase(OP_BXOR) {
        op_bitwise(L, l_bxor);
        vmbreak;
      }
      vmcase(OP_SHR) {
        op_bitwise(L, luaV_shiftr);
        vmbreak;
      }
      vmcase(OP_SHL) {
        op_bitwise(L, luaV_shiftl);
        vmbreak;
      }
      vmcase(OP_MMBIN) {
        StkId ra = RA(i);
        Instruction pi = *(pc - 2);  
        TValue *rb = vRB(i);
        TMS tm = (TMS)GETARG_C(i);
        StkId result = RA(pi);
        lua_assert(OP_ADD <= GET_OPCODE(pi) && GET_OPCODE(pi) <= OP_SHR);
        Protect(luaT_trybinTM(L, s2v(ra), rb, result, tm));
        vmbreak;
      }
      vmcase(OP_MMBINI) {
        StkId ra = RA(i);
        Instruction pi = *(pc - 2);  
        int imm = GETARG_sB(i);
        TMS tm = (TMS)GETARG_C(i);
        int flip = GETARG_k(i);
        StkId result = RA(pi);
        Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
        vmbreak;
      }
      vmcase(OP_MMBINK) {
        StkId ra = RA(i);
        Instruction pi = *(pc - 2);  
        TValue *imm = KB(i);
        TMS tm = (TMS)GETARG_C(i);
        int flip = GETARG_k(i);
        StkId result = RA(pi);
        Protect(luaT_trybinassocTM(L, s2v(ra), imm, flip, result, tm));
        vmbreak;
      }
      vmcase(OP_UNM) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        lua_Number nb;
        if (ttisinteger(rb)) {
          lua_Integer ib = ivalue(rb);
          setivalue(s2v(ra), intop(-, 0, ib));
        }
        else if (tonumberns(rb, nb)) {
          setfltvalue(s2v(ra), luai_numunm(L, nb));
        }
        else
          Protect(luaT_trybinTM(L, rb, rb, ra, TM_UNM));
        vmbreak;
      }
      vmcase(OP_BNOT) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        lua_Integer ib;
        if (tointegerns(rb, &ib)) {
          setivalue(s2v(ra), intop(^, ~l_castS2U(0), ib));
        }
        else
          Protect(luaT_trybinTM(L, rb, rb, ra, TM_BNOT));
        vmbreak;
      }
      vmcase(OP_NOT) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        if (l_isfalse(rb))
          setbtvalue(s2v(ra));
        else
          setbfvalue(s2v(ra));
        vmbreak;
      }
      vmcase(OP_LEN) {
        StkId ra = RA(i);
        Protect(luaV_objlen(L, ra, vRB(i)));
        vmbreak;
      }
      vmcase(OP_CONCAT) {
        StkId ra = RA(i);
        int n = GETARG_B(i);  
        L->top.p = ra + n;  
        ProtectNT(luaV_concat(L, n));
        checkGC(L, L->top.p); 
        vmbreak;
      }
      vmcase(OP_CLOSE) {
        StkId ra = RA(i);
        Protect(luaF_close(L, ra, LUA_OK, 1));
        vmbreak;
      }
      vmcase(OP_TBC) {
        StkId ra = RA(i);
        
        halfProtect(luaF_newtbcupval(L, ra));
        vmbreak;
      }
      vmcase(OP_JMP) {
        dojump(ci, i, 0);
        vmbreak;
      }
      vmcase(OP_EQ) {
        StkId ra = RA(i);
        int cond;
        TValue *rb = vRB(i);
        Protect(cond = luaV_equalobj(L, s2v(ra), rb));
        docondjump();
        vmbreak;
      }
      vmcase(OP_LT) {
        op_order(L, l_lti, LTnum, lessthanothers);
        vmbreak;
      }
      vmcase(OP_LE) {
        op_order(L, l_lei, LEnum, lessequalothers);
        vmbreak;
      }
      vmcase(OP_EQK) {
        StkId ra = RA(i);
        TValue *rb = KB(i);
        
        int cond = luaV_rawequalobj(s2v(ra), rb);
        docondjump();
        vmbreak;
      }
      vmcase(OP_EQI) {
        StkId ra = RA(i);
        int cond;
        int im = GETARG_sB(i);
        if (ttisinteger(s2v(ra)))
          cond = (ivalue(s2v(ra)) == im);
        else if (ttisfloat(s2v(ra)))
          cond = luai_numeq(fltvalue(s2v(ra)), cast_num(im));
        else
          cond = 0;  
        docondjump();
        vmbreak;
      }
      vmcase(OP_LTI) {
        op_orderI(L, l_lti, luai_numlt, 0, TM_LT);
        vmbreak;
      }
      vmcase(OP_LEI) {
        op_orderI(L, l_lei, luai_numle, 0, TM_LE);
        vmbreak;
      }
      vmcase(OP_GTI) {
        op_orderI(L, l_gti, luai_numgt, 1, TM_LT);
        vmbreak;
      }
      vmcase(OP_GEI) {
        op_orderI(L, l_gei, luai_numge, 1, TM_LE);
        vmbreak;
      }
      vmcase(OP_TEST) {
        StkId ra = RA(i);
        int cond = !l_isfalse(s2v(ra));
        docondjump();
        vmbreak;
      }
      vmcase(OP_TESTSET) {
        StkId ra = RA(i);
        TValue *rb = vRB(i);
        if (l_isfalse(rb) == GETARG_k(i))
          pc++;
        else {
          setobj2s(L, ra, rb);
          donextjump(ci);
        }
        vmbreak;
      }
      vmcase(OP_CALL) {
        StkId ra = RA(i);
        CallInfo *newci;
        int b = GETARG_B(i);
        int nresults = GETARG_C(i) - 1;
        if (b != 0)  
          L->top.p = ra + b;  
        
        savepc(L);  
        if ((newci = luaD_precall(L, ra, nresults)) == NULL)
          updatetrap(ci);  
        else {  
          ci = newci;
          goto startfunc;
        }
        vmbreak;
      }
      vmcase(OP_TAILCALL) {
        StkId ra = RA(i);
        int b = GETARG_B(i);  
        int n;  
        int nparams1 = GETARG_C(i);
        
        int delta = (nparams1) ? ci->u.l.nextraargs + nparams1 : 0;
        if (b != 0)
          L->top.p = ra + b;
        else  
          b = cast_int(L->top.p - ra);
        savepc(ci);  
        if (TESTARG_k(i)) {
          luaF_closeupval(L, base);  
          lua_assert(L->tbclist.p < base);  
          lua_assert(base == ci->func.p + 1);
        }
        if ((n = luaD_pretailcall(L, ci, ra, b, delta)) < 0)  
          goto startfunc;  
        else {  
          ci->func.p -= delta;  
          luaD_poscall(L, ci, n);  
          updatetrap(ci);  
          goto ret;  
        }
      }
      vmcase(OP_RETURN) {
        StkId ra = RA(i);
        int n = GETARG_B(i) - 1;  
        int nparams1 = GETARG_C(i);
        if (n < 0)  
          n = cast_int(L->top.p - ra);  
        savepc(ci);
        if (TESTARG_k(i)) {  
          ci->u2.nres = n;  
          if (L->top.p < ci->top.p)
            L->top.p = ci->top.p;
          luaF_close(L, base, CLOSEKTOP, 1);
          updatetrap(ci);
          updatestack(ci);
        }
        if (nparams1)  
          ci->func.p -= ci->u.l.nextraargs + nparams1;
        L->top.p = ra + n;  
        luaD_poscall(L, ci, n);
        updatetrap(ci);  
        goto ret;
      }
      vmcase(OP_RETURN0) {
        if (l_unlikely(L->hookmask)) {
          StkId ra = RA(i);
          L->top.p = ra;
          savepc(ci);
          luaD_poscall(L, ci, 0);  
          trap = 1;
        }
        else {  
          int nres;
          L->ci = ci->previous;  
          L->top.p = base - 1;
          for (nres = ci->nresults; l_unlikely(nres > 0); nres--)
            setnilvalue(s2v(L->top.p++));  
        }
        goto ret;
      }
      vmcase(OP_RETURN1) {
        if (l_unlikely(L->hookmask)) {
          StkId ra = RA(i);
          L->top.p = ra + 1;
          savepc(ci);
          luaD_poscall(L, ci, 1);  
          trap = 1;
        }
        else {  
          int nres = ci->nresults;
          L->ci = ci->previous;  
          if (nres == 0)
            L->top.p = base - 1;  
          else {
            StkId ra = RA(i);
            setobjs2s(L, base - 1, ra);  
            L->top.p = base;
            for (; l_unlikely(nres > 1); nres--)
              setnilvalue(s2v(L->top.p++));  
          }
        }
       ret:  
        if (ci->callstatus & CIST_FRESH)
          return;  
        else {
          ci = ci->previous;
          goto returning;  
        }
      }
      vmcase(OP_FORLOOP) {
        StkId ra = RA(i);
        if (ttisinteger(s2v(ra + 1))) {  
          lua_Unsigned count = l_castS2U(ivalue(s2v(ra)));
          if (count > 0) {  
            lua_Integer step = ivalue(s2v(ra + 1));
            lua_Integer idx = ivalue(s2v(ra + 2));  
            chgivalue(s2v(ra), count - 1);  
            idx = intop(+, idx, step);  
            chgivalue(s2v(ra + 2), idx);  
            pc -= GETARG_Bx(i);  
          }
        }
        else if (floatforloop(ra))  
          pc -= GETARG_Bx(i);  
        updatetrap(ci);  
        vmbreak;
      }
      vmcase(OP_FORPREP) {
        StkId ra = RA(i);
        savestate(L, ci);  
        if (forprep(L, ra))
          pc += GETARG_Bx(i) + 1;  
        vmbreak;
      }
      vmcase(OP_TFORPREP) {
       
       StkId ra = RA(i);
       TValue temp;  
       setobj(L, &temp, s2v(ra + 3));
       setobjs2s(L, ra + 3, ra + 2);
       setobj2s(L, ra + 2, &temp);
        
        halfProtect(luaF_newtbcupval(L, ra + 2));
        pc += GETARG_Bx(i);  
        i = *(pc++);  
        lua_assert(GET_OPCODE(i) == OP_TFORCALL && ra == RA(i));
        goto l_tforcall;
      }
      vmcase(OP_TFORCALL) {
       l_tforcall: {
        
        StkId ra = RA(i);
        setobjs2s(L, ra + 5, ra + 3);  
        setobjs2s(L, ra + 4, ra + 1);  
        setobjs2s(L, ra + 3, ra);  
        L->top.p = ra + 3 + 3;
        ProtectNT(luaD_call(L, ra + 3, GETARG_C(i)));  
        updatestack(ci);  
        i = *(pc++);  
        lua_assert(GET_OPCODE(i) == OP_TFORLOOP && ra == RA(i));
        goto l_tforloop;
      }}
      vmcase(OP_TFORLOOP) {
       l_tforloop: {
        StkId ra = RA(i);
        if (!ttisnil(s2v(ra + 3)))  
          pc -= GETARG_Bx(i);  
        vmbreak;
      }}
      vmcase(OP_SETLIST) {
        StkId ra = RA(i);
        int n = GETARG_B(i);
        unsigned int last = GETARG_C(i);
        Table *h = hvalue(s2v(ra));
        if (n == 0)
          n = cast_int(L->top.p - ra) - 1;  
        else
          L->top.p = ci->top.p;  
        last += n;
        if (TESTARG_k(i)) {
          last += GETARG_Ax(*pc) * (MAXARG_C + 1);
          pc++;
        }
        if (last > luaH_realasize(h))  
          luaH_resizearray(L, h, last);  
        for (; n > 0; n--) {
          TValue *val = s2v(ra + n);
          obj2arr(h, last, val);
          last--;
          luaC_barrierback(L, obj2gco(h), val);
        }
        vmbreak;
      }
      vmcase(OP_CLOSURE) {
        StkId ra = RA(i);
        Proto *p = cl->p->p[GETARG_Bx(i)];
        halfProtect(pushclosure(L, p, cl->upvals, base, ra));
        checkGC(L, ra + 1);
        vmbreak;
      }
      vmcase(OP_VARARG) {
        StkId ra = RA(i);
        int n = GETARG_C(i) - 1;  
        Protect(luaT_getvarargs(L, ci, ra, n));
        vmbreak;
      }
      vmcase(OP_VARARGPREP) {
        ProtectNT(luaT_adjustvarargs(L, GETARG_A(i), ci, cl->p));
        if (l_unlikely(trap)) {  
          luaD_hookcall(L, ci);
          L->oldpc = 1;  
        }
        updatebase(ci);  
        vmbreak;
      }
      vmcase(OP_EXTRAARG) {
        lua_assert(0);
        vmbreak;
      }
    }
  }
}



//path: src/imports/../dependencies/lua/lapi.c


#define lapi_c
#define LUA_CORE





#include <limits.h>
#include <stdarg.h>
#include <string.h>

































const char lua_ident[] =
  "$LuaVersion: " LUA_COPYRIGHT " $"
  "$LuaAuthors: " LUA_AUTHORS " $";




#define isvalid(L, o)	(!ttisnil(o) || o != &G(L)->nilvalue)



#define ispseudo(i)		((i) <= LUA_REGISTRYINDEX)


#define isupvalue(i)		((i) < LUA_REGISTRYINDEX)



static void advancegc (lua_State *L, size_t delta) {
  delta >>= 5;  
  if (delta > 0) {
    global_State *g = G(L);
    luaE_setdebt(g, g->GCdebt - delta);
  }
}



static TValue *index2value (lua_State *L, int idx) {
  CallInfo *ci = L->ci;
  if (idx > 0) {
    StkId o = ci->func.p + idx;
    api_check(L, idx <= ci->top.p - (ci->func.p + 1), "unacceptable index");
    if (o >= L->top.p) return &G(L)->nilvalue;
    else return s2v(o);
  }
  else if (!ispseudo(idx)) {  
    api_check(L, idx != 0 && -idx <= L->top.p - (ci->func.p + 1),
                 "invalid index");
    return s2v(L->top.p + idx);
  }
  else if (idx == LUA_REGISTRYINDEX)
    return &G(L)->l_registry;
  else {  
    idx = LUA_REGISTRYINDEX - idx;
    api_check(L, idx <= MAXUPVAL + 1, "upvalue index too large");
    if (ttisCclosure(s2v(ci->func.p))) {  
      CClosure *func = clCvalue(s2v(ci->func.p));
      return (idx <= func->nupvalues) ? &func->upvalue[idx-1]
                                      : &G(L)->nilvalue;
    }
    else {  
      api_check(L, ttislcf(s2v(ci->func.p)), "caller not a C function");
      return &G(L)->nilvalue;  
    }
  }
}




static StkId index2stack (lua_State *L, int idx) {
  CallInfo *ci = L->ci;
  if (idx > 0) {
    StkId o = ci->func.p + idx;
    api_check(L, o < L->top.p, "invalid index");
    return o;
  }
  else {    
    api_check(L, idx != 0 && -idx <= L->top.p - (ci->func.p + 1),
                 "invalid index");
    api_check(L, !ispseudo(idx), "invalid index");
    return L->top.p + idx;
  }
}


LUA_API int lua_checkstack (lua_State *L, int n) {
  int res;
  CallInfo *ci;
  lua_lock(L);
  ci = L->ci;
  api_check(L, n >= 0, "negative 'n'");
  if (L->stack_last.p - L->top.p > n)  
    res = 1;  
  else  
    res = luaD_growstack(L, n, 0);
  if (res && ci->top.p < L->top.p + n)
    ci->top.p = L->top.p + n;  
  lua_unlock(L);
  return res;
}


LUA_API void lua_xmove (lua_State *from, lua_State *to, int n) {
  int i;
  if (from == to) return;
  lua_lock(to);
  api_checkpop(from, n);
  api_check(from, G(from) == G(to), "moving among independent states");
  api_check(from, to->ci->top.p - to->top.p >= n, "stack overflow");
  from->top.p -= n;
  for (i = 0; i < n; i++) {
    setobjs2s(to, to->top.p, from->top.p + i);
    to->top.p++;  
  }
  lua_unlock(to);
}


LUA_API lua_CFunction lua_atpanic (lua_State *L, lua_CFunction panicf) {
  lua_CFunction old;
  lua_lock(L);
  old = G(L)->panic;
  G(L)->panic = panicf;
  lua_unlock(L);
  return old;
}


LUA_API lua_Number lua_version (lua_State *L) {
  UNUSED(L);
  return LUA_VERSION_NUM;
}







LUA_API int lua_absindex (lua_State *L, int idx) {
  return (idx > 0 || ispseudo(idx))
         ? idx
         : cast_int(L->top.p - L->ci->func.p) + idx;
}


LUA_API int lua_gettop (lua_State *L) {
  return cast_int(L->top.p - (L->ci->func.p + 1));
}


LUA_API void lua_settop (lua_State *L, int idx) {
  CallInfo *ci;
  StkId func, newtop;
  ptrdiff_t diff;  
  lua_lock(L);
  ci = L->ci;
  func = ci->func.p;
  if (idx >= 0) {
    api_check(L, idx <= ci->top.p - (func + 1), "new top too large");
    diff = ((func + 1) + idx) - L->top.p;
    for (; diff > 0; diff--)
      setnilvalue(s2v(L->top.p++));  
  }
  else {
    api_check(L, -(idx+1) <= (L->top.p - (func + 1)), "invalid new top");
    diff = idx + 1;  
  }
  newtop = L->top.p + diff;
  if (diff < 0 && L->tbclist.p >= newtop) {
    lua_assert(hastocloseCfunc(ci->nresults));
    newtop = luaF_close(L, newtop, CLOSEKTOP, 0);
  }
  L->top.p = newtop;  
  lua_unlock(L);
}


LUA_API void lua_closeslot (lua_State *L, int idx) {
  StkId level;
  lua_lock(L);
  level = index2stack(L, idx);
  api_check(L, hastocloseCfunc(L->ci->nresults) && L->tbclist.p == level,
     "no variable to close at given level");
  level = luaF_close(L, level, CLOSEKTOP, 0);
  setnilvalue(s2v(level));
  lua_unlock(L);
}



static void reverse (lua_State *L, StkId from, StkId to) {
  for (; from < to; from++, to--) {
    TValue temp;
    setobj(L, &temp, s2v(from));
    setobjs2s(L, from, to);
    setobj2s(L, to, &temp);
  }
}



LUA_API void lua_rotate (lua_State *L, int idx, int n) {
  StkId p, t, m;
  lua_lock(L);
  t = L->top.p - 1;  
  p = index2stack(L, idx);  
  api_check(L, L->tbclist.p < p, "moving a to-be-closed slot");
  api_check(L, (n >= 0 ? n : -n) <= (t - p + 1), "invalid 'n'");
  m = (n >= 0 ? t - n : p - n - 1);  
  reverse(L, p, m);  
  reverse(L, m + 1, t);  
  reverse(L, p, t);  
  lua_unlock(L);
}


LUA_API void lua_copy (lua_State *L, int fromidx, int toidx) {
  TValue *fr, *to;
  lua_lock(L);
  fr = index2value(L, fromidx);
  to = index2value(L, toidx);
  api_check(L, isvalid(L, to), "invalid index");
  setobj(L, to, fr);
  if (isupvalue(toidx))  
    luaC_barrier(L, clCvalue(s2v(L->ci->func.p)), fr);
  
  lua_unlock(L);
}


LUA_API void lua_pushvalue (lua_State *L, int idx) {
  lua_lock(L);
  setobj2s(L, L->top.p, index2value(L, idx));
  api_incr_top(L);
  lua_unlock(L);
}






LUA_API int lua_type (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (isvalid(L, o) ? ttype(o) : LUA_TNONE);
}


LUA_API const char *lua_typename (lua_State *L, int t) {
  UNUSED(L);
  api_check(L, LUA_TNONE <= t && t < LUA_NUMTYPES, "invalid type");
  return ttypename(t);
}


LUA_API int lua_iscfunction (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (ttislcf(o) || (ttisCclosure(o)));
}


LUA_API int lua_isinteger (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return ttisinteger(o);
}


LUA_API int lua_isnumber (lua_State *L, int idx) {
  lua_Number n;
  const TValue *o = index2value(L, idx);
  return tonumber(o, &n);
}


LUA_API int lua_isstring (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (ttisstring(o) || cvt2str(o));
}


LUA_API int lua_isuserdata (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (ttisfulluserdata(o) || ttislightuserdata(o));
}


LUA_API int lua_rawequal (lua_State *L, int index1, int index2) {
  const TValue *o1 = index2value(L, index1);
  const TValue *o2 = index2value(L, index2);
  return (isvalid(L, o1) && isvalid(L, o2)) ? luaV_rawequalobj(o1, o2) : 0;
}


LUA_API void lua_arith (lua_State *L, int op) {
  lua_lock(L);
  if (op != LUA_OPUNM && op != LUA_OPBNOT)
    api_checkpop(L, 2);  
  else {  
    api_checkpop(L, 1);
    setobjs2s(L, L->top.p, L->top.p - 1);
    api_incr_top(L);
  }
  
  luaO_arith(L, op, s2v(L->top.p - 2), s2v(L->top.p - 1), L->top.p - 2);
  L->top.p--;  
  lua_unlock(L);
}


LUA_API int lua_compare (lua_State *L, int index1, int index2, int op) {
  const TValue *o1;
  const TValue *o2;
  int i = 0;
  lua_lock(L);  
  o1 = index2value(L, index1);
  o2 = index2value(L, index2);
  if (isvalid(L, o1) && isvalid(L, o2)) {
    switch (op) {
      case LUA_OPEQ: i = luaV_equalobj(L, o1, o2); break;
      case LUA_OPLT: i = luaV_lessthan(L, o1, o2); break;
      case LUA_OPLE: i = luaV_lessequal(L, o1, o2); break;
      default: api_check(L, 0, "invalid option");
    }
  }
  lua_unlock(L);
  return i;
}


LUA_API size_t lua_stringtonumber (lua_State *L, const char *s) {
  size_t sz = luaO_str2num(s, s2v(L->top.p));
  if (sz != 0)
    api_incr_top(L);
  return sz;
}


LUA_API lua_Number lua_tonumberx (lua_State *L, int idx, int *pisnum) {
  lua_Number n = 0;
  const TValue *o = index2value(L, idx);
  int isnum = tonumber(o, &n);
  if (pisnum)
    *pisnum = isnum;
  return n;
}


LUA_API lua_Integer lua_tointegerx (lua_State *L, int idx, int *pisnum) {
  lua_Integer res = 0;
  const TValue *o = index2value(L, idx);
  int isnum = tointeger(o, &res);
  if (pisnum)
    *pisnum = isnum;
  return res;
}


LUA_API int lua_toboolean (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return !l_isfalse(o);
}


LUA_API const char *lua_tolstring (lua_State *L, int idx, size_t *len) {
  TValue *o;
  lua_lock(L);
  o = index2value(L, idx);
  if (!ttisstring(o)) {
    if (!cvt2str(o)) {  
      if (len != NULL) *len = 0;
      lua_unlock(L);
      return NULL;
    }
    luaO_tostring(L, o);
    luaC_checkGC(L);
    o = index2value(L, idx);  
  }
  lua_unlock(L);
  if (len != NULL)
    return getlstr(tsvalue(o), *len);
  else
    return getstr(tsvalue(o));
}


LUA_API lua_Unsigned lua_rawlen (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  switch (ttypetag(o)) {
    case LUA_VSHRSTR: return tsvalue(o)->shrlen;
    case LUA_VLNGSTR: return tsvalue(o)->u.lnglen;
    case LUA_VUSERDATA: return uvalue(o)->len;
    case LUA_VTABLE: return luaH_getn(hvalue(o));
    default: return 0;
  }
}


LUA_API lua_CFunction lua_tocfunction (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  if (ttislcf(o)) return fvalue(o);
  else if (ttisCclosure(o))
    return clCvalue(o)->f;
  else return NULL;  
}


l_sinline void *touserdata (const TValue *o) {
  switch (ttype(o)) {
    case LUA_TUSERDATA: return getudatamem(uvalue(o));
    case LUA_TLIGHTUSERDATA: return pvalue(o);
    default: return NULL;
  }
}


LUA_API void *lua_touserdata (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return touserdata(o);
}


LUA_API lua_State *lua_tothread (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (!ttisthread(o)) ? NULL : thvalue(o);
}



LUA_API const void *lua_topointer (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  switch (ttypetag(o)) {
    case LUA_VLCF: return cast_voidp(cast_sizet(fvalue(o)));
    case LUA_VUSERDATA: case LUA_VLIGHTUSERDATA:
      return touserdata(o);
    default: {
      if (iscollectable(o))
        return gcvalue(o);
      else
        return NULL;
    }
  }
}






LUA_API void lua_pushnil (lua_State *L) {
  lua_lock(L);
  setnilvalue(s2v(L->top.p));
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushnumber (lua_State *L, lua_Number n) {
  lua_lock(L);
  setfltvalue(s2v(L->top.p), n);
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushinteger (lua_State *L, lua_Integer n) {
  lua_lock(L);
  setivalue(s2v(L->top.p), n);
  api_incr_top(L);
  lua_unlock(L);
}



LUA_API const char *lua_pushlstring (lua_State *L, const char *s, size_t len) {
  TString *ts;
  lua_lock(L);
  ts = (len == 0) ? luaS_new(L, "") : luaS_newlstr(L, s, len);
  setsvalue2s(L, L->top.p, ts);
  api_incr_top(L);
  advancegc(L, len);
  luaC_checkGC(L);
  lua_unlock(L);
  return getstr(ts);
}


LUA_API const char *lua_pushextlstring (lua_State *L,
	        const char *s, size_t len, lua_Alloc falloc, void *ud) {
  TString *ts;
  lua_lock(L);
  api_check(L, s[len] == '\0', "string not ending with zero");
  ts = luaS_newextlstr (L, s, len, falloc, ud);
  setsvalue2s(L, L->top.p, ts);
  api_incr_top(L);
  if (falloc != NULL)  
    advancegc(L, len);  
  luaC_checkGC(L);
  lua_unlock(L);
  return getstr(ts);
}


LUA_API const char *lua_pushstring (lua_State *L, const char *s) {
  lua_lock(L);
  if (s == NULL)
    setnilvalue(s2v(L->top.p));
  else {
    TString *ts;
    ts = luaS_new(L, s);
    setsvalue2s(L, L->top.p, ts);
    s = getstr(ts);  
  }
  api_incr_top(L);
  luaC_checkGC(L);
  lua_unlock(L);
  return s;
}


LUA_API const char *lua_pushvfstring (lua_State *L, const char *fmt,
                                      va_list argp) {
  const char *ret;
  lua_lock(L);
  ret = luaO_pushvfstring(L, fmt, argp);
  luaC_checkGC(L);
  lua_unlock(L);
  return ret;
}


LUA_API const char *lua_pushfstring (lua_State *L, const char *fmt, ...) {
  const char *ret;
  va_list argp;
  lua_lock(L);
  va_start(argp, fmt);
  ret = luaO_pushvfstring(L, fmt, argp);
  va_end(argp);
  luaC_checkGC(L);
  lua_unlock(L);
  return ret;
}


LUA_API void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n) {
  lua_lock(L);
  if (n == 0) {
    setfvalue(s2v(L->top.p), fn);
    api_incr_top(L);
  }
  else {
    int i;
    CClosure *cl;
    api_checkpop(L, n);
    api_check(L, n <= MAXUPVAL, "upvalue index too large");
    cl = luaF_newCclosure(L, n);
    cl->f = fn;
    for (i = 0; i < n; i++) {
      setobj2n(L, &cl->upvalue[i], s2v(L->top.p - n + i));
      
      lua_assert(iswhite(cl));
    }
    L->top.p -= n;
    setclCvalue(L, s2v(L->top.p), cl);
    api_incr_top(L);
    luaC_checkGC(L);
  }
  lua_unlock(L);
}


LUA_API void lua_pushboolean (lua_State *L, int b) {
  lua_lock(L);
  if (b)
    setbtvalue(s2v(L->top.p));
  else
    setbfvalue(s2v(L->top.p));
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushlightuserdata (lua_State *L, void *p) {
  lua_lock(L);
  setpvalue(s2v(L->top.p), p);
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API int lua_pushthread (lua_State *L) {
  lua_lock(L);
  setthvalue(L, s2v(L->top.p), L);
  api_incr_top(L);
  lua_unlock(L);
  return (G(L)->mainthread == L);
}






static int auxgetstr (lua_State *L, const TValue *t, const char *k) {
  int tag;
  TString *str = luaS_new(L, k);
  luaV_fastget(t, str, s2v(L->top.p), luaH_getstr, tag);
  if (!tagisempty(tag)) {
    api_incr_top(L);
  }
  else {
    setsvalue2s(L, L->top.p, str);
    api_incr_top(L);
    tag = luaV_finishget(L, t, s2v(L->top.p - 1), L->top.p - 1, tag);
  }
  lua_unlock(L);
  return novariant(tag);
}


static void getGlobalTable (lua_State *L, TValue *gt) {
  Table *registry = hvalue(&G(L)->l_registry);
  int tag = luaH_getint(registry, LUA_RIDX_GLOBALS, gt);
  (void)tag;  
  api_check(L, novariant(tag) == LUA_TTABLE, "global table must exist");
}


LUA_API int lua_getglobal (lua_State *L, const char *name) {
  TValue gt;
  lua_lock(L);
  getGlobalTable(L, &gt);
  return auxgetstr(L, &gt, name);
}


LUA_API int lua_gettable (lua_State *L, int idx) {
  int tag;
  TValue *t;
  lua_lock(L);
  api_checkpop(L, 1);
  t = index2value(L, idx);
  luaV_fastget(t, s2v(L->top.p - 1), s2v(L->top.p - 1), luaH_get, tag);
  if (tagisempty(tag))
    tag = luaV_finishget(L, t, s2v(L->top.p - 1), L->top.p - 1, tag);
  lua_unlock(L);
  return novariant(tag);
}


LUA_API int lua_getfield (lua_State *L, int idx, const char *k) {
  lua_lock(L);
  return auxgetstr(L, index2value(L, idx), k);
}


LUA_API int lua_geti (lua_State *L, int idx, lua_Integer n) {
  TValue *t;
  int tag;
  lua_lock(L);
  t = index2value(L, idx);
  luaV_fastgeti(t, n, s2v(L->top.p), tag);
  if (tagisempty(tag)) {
    TValue key;
    setivalue(&key, n);
    tag = luaV_finishget(L, t, &key, L->top.p, tag);
  }
  api_incr_top(L);
  lua_unlock(L);
  return novariant(tag);
}


static int finishrawget (lua_State *L, int tag) {
  if (tagisempty(tag))  
    setnilvalue(s2v(L->top.p));
  api_incr_top(L);
  lua_unlock(L);
  return novariant(tag);
}


l_sinline Table *gettable (lua_State *L, int idx) {
  TValue *t = index2value(L, idx);
  api_check(L, ttistable(t), "table expected");
  return hvalue(t);
}


LUA_API int lua_rawget (lua_State *L, int idx) {
  Table *t;
  int tag;
  lua_lock(L);
  api_checkpop(L, 1);
  t = gettable(L, idx);
  tag = luaH_get(t, s2v(L->top.p - 1), s2v(L->top.p - 1));
  L->top.p--;  
  return finishrawget(L, tag);
}


LUA_API int lua_rawgeti (lua_State *L, int idx, lua_Integer n) {
  Table *t;
  int tag;
  lua_lock(L);
  t = gettable(L, idx);
  luaH_fastgeti(t, n, s2v(L->top.p), tag);
  return finishrawget(L, tag);
}


LUA_API int lua_rawgetp (lua_State *L, int idx, const void *p) {
  Table *t;
  TValue k;
  lua_lock(L);
  t = gettable(L, idx);
  setpvalue(&k, cast_voidp(p));
  return finishrawget(L, luaH_get(t, &k, s2v(L->top.p)));
}


LUA_API void lua_createtable (lua_State *L, unsigned narray, unsigned nrec) {
  Table *t;
  lua_lock(L);
  t = luaH_new(L);
  sethvalue2s(L, L->top.p, t);
  api_incr_top(L);
  if (narray > 0 || nrec > 0)
    luaH_resize(L, t, narray, nrec);
  luaC_checkGC(L);
  lua_unlock(L);
}


LUA_API int lua_getmetatable (lua_State *L, int objindex) {
  const TValue *obj;
  Table *mt;
  int res = 0;
  lua_lock(L);
  obj = index2value(L, objindex);
  switch (ttype(obj)) {
    case LUA_TTABLE:
      mt = hvalue(obj)->metatable;
      break;
    case LUA_TUSERDATA:
      mt = uvalue(obj)->metatable;
      break;
    default:
      mt = G(L)->mt[ttype(obj)];
      break;
  }
  if (mt != NULL) {
    sethvalue2s(L, L->top.p, mt);
    api_incr_top(L);
    res = 1;
  }
  lua_unlock(L);
  return res;
}


LUA_API int lua_getiuservalue (lua_State *L, int idx, int n) {
  TValue *o;
  int t;
  lua_lock(L);
  o = index2value(L, idx);
  api_check(L, ttisfulluserdata(o), "full userdata expected");
  if (n <= 0 || n > uvalue(o)->nuvalue) {
    setnilvalue(s2v(L->top.p));
    t = LUA_TNONE;
  }
  else {
    setobj2s(L, L->top.p, &uvalue(o)->uv[n - 1].uv);
    t = ttype(s2v(L->top.p));
  }
  api_incr_top(L);
  lua_unlock(L);
  return t;
}





static void auxsetstr (lua_State *L, const TValue *t, const char *k) {
  int hres;
  TString *str = luaS_new(L, k);
  api_checkpop(L, 1);
  luaV_fastset(t, str, s2v(L->top.p - 1), hres, luaH_psetstr);
  if (hres == HOK) {
    luaV_finishfastset(L, t, s2v(L->top.p - 1));
    L->top.p--;  
  }
  else {
    setsvalue2s(L, L->top.p, str);  
    api_incr_top(L);
    luaV_finishset(L, t, s2v(L->top.p - 1), s2v(L->top.p - 2), hres);
    L->top.p -= 2;  
  }
  lua_unlock(L);  
}


LUA_API void lua_setglobal (lua_State *L, const char *name) {
  TValue gt;
  lua_lock(L);  
  getGlobalTable(L, &gt);
  auxsetstr(L, &gt, name);
}


LUA_API void lua_settable (lua_State *L, int idx) {
  TValue *t;
  int hres;
  lua_lock(L);
  api_checkpop(L, 2);
  t = index2value(L, idx);
  luaV_fastset(t, s2v(L->top.p - 2), s2v(L->top.p - 1), hres, luaH_pset);
  if (hres == HOK) {
    luaV_finishfastset(L, t, s2v(L->top.p - 1));
  }
  else
    luaV_finishset(L, t, s2v(L->top.p - 2), s2v(L->top.p - 1), hres);
  L->top.p -= 2;  
  lua_unlock(L);
}


LUA_API void lua_setfield (lua_State *L, int idx, const char *k) {
  lua_lock(L);  
  auxsetstr(L, index2value(L, idx), k);
}


LUA_API void lua_seti (lua_State *L, int idx, lua_Integer n) {
  TValue *t;
  int hres;
  lua_lock(L);
  api_checkpop(L, 1);
  t = index2value(L, idx);
  luaV_fastseti(t, n, s2v(L->top.p - 1), hres);
  if (hres == HOK)
    luaV_finishfastset(L, t, s2v(L->top.p - 1));
  else {
    TValue temp;
    setivalue(&temp, n);
    luaV_finishset(L, t, &temp, s2v(L->top.p - 1), hres);
  }
  L->top.p--;  
  lua_unlock(L);
}


static void aux_rawset (lua_State *L, int idx, TValue *key, int n) {
  Table *t;
  lua_lock(L);
  api_checkpop(L, n);
  t = gettable(L, idx);
  luaH_set(L, t, key, s2v(L->top.p - 1));
  invalidateTMcache(t);
  luaC_barrierback(L, obj2gco(t), s2v(L->top.p - 1));
  L->top.p -= n;
  lua_unlock(L);
}


LUA_API void lua_rawset (lua_State *L, int idx) {
  aux_rawset(L, idx, s2v(L->top.p - 2), 2);
}


LUA_API void lua_rawsetp (lua_State *L, int idx, const void *p) {
  TValue k;
  setpvalue(&k, cast_voidp(p));
  aux_rawset(L, idx, &k, 1);
}


LUA_API void lua_rawseti (lua_State *L, int idx, lua_Integer n) {
  Table *t;
  lua_lock(L);
  api_checkpop(L, 1);
  t = gettable(L, idx);
  luaH_setint(L, t, n, s2v(L->top.p - 1));
  luaC_barrierback(L, obj2gco(t), s2v(L->top.p - 1));
  L->top.p--;
  lua_unlock(L);
}


LUA_API int lua_setmetatable (lua_State *L, int objindex) {
  TValue *obj;
  Table *mt;
  lua_lock(L);
  api_checkpop(L, 1);
  obj = index2value(L, objindex);
  if (ttisnil(s2v(L->top.p - 1)))
    mt = NULL;
  else {
    api_check(L, ttistable(s2v(L->top.p - 1)), "table expected");
    mt = hvalue(s2v(L->top.p - 1));
  }
  switch (ttype(obj)) {
    case LUA_TTABLE: {
      hvalue(obj)->metatable = mt;
      if (mt) {
        luaC_objbarrier(L, gcvalue(obj), mt);
        luaC_checkfinalizer(L, gcvalue(obj), mt);
      }
      break;
    }
    case LUA_TUSERDATA: {
      uvalue(obj)->metatable = mt;
      if (mt) {
        luaC_objbarrier(L, uvalue(obj), mt);
        luaC_checkfinalizer(L, gcvalue(obj), mt);
      }
      break;
    }
    default: {
      G(L)->mt[ttype(obj)] = mt;
      break;
    }
  }
  L->top.p--;
  lua_unlock(L);
  return 1;
}


LUA_API int lua_setiuservalue (lua_State *L, int idx, int n) {
  TValue *o;
  int res;
  lua_lock(L);
  api_checkpop(L, 1);
  o = index2value(L, idx);
  api_check(L, ttisfulluserdata(o), "full userdata expected");
  if (!(cast_uint(n) - 1u < cast_uint(uvalue(o)->nuvalue)))
    res = 0;  
  else {
    setobj(L, &uvalue(o)->uv[n - 1].uv, s2v(L->top.p - 1));
    luaC_barrierback(L, gcvalue(o), s2v(L->top.p - 1));
    res = 1;
  }
  L->top.p--;
  lua_unlock(L);
  return res;
}





#define checkresults(L,na,nr) \
     api_check(L, (nr) == LUA_MULTRET \
               || (L->ci->top.p - L->top.p >= (nr) - (na)), \
	"results from function overflow current stack size")


LUA_API void lua_callk (lua_State *L, int nargs, int nresults,
                        lua_KContext ctx, lua_KFunction k) {
  StkId func;
  lua_lock(L);
  api_check(L, k == NULL || !isLua(L->ci),
    "cannot use continuations inside hooks");
  api_checkpop(L, nargs + 1);
  api_check(L, L->status == LUA_OK, "cannot do calls on non-normal thread");
  checkresults(L, nargs, nresults);
  func = L->top.p - (nargs+1);
  if (k != NULL && yieldable(L)) {  
    L->ci->u.c.k = k;  
    L->ci->u.c.ctx = ctx;  
    luaD_call(L, func, nresults);  
  }
  else  
    luaD_callnoyield(L, func, nresults);  
  adjustresults(L, nresults);
  lua_unlock(L);
}




struct CallS {  
  StkId func;
  int nresults;
};


static void f_call (lua_State *L, void *ud) {
  struct CallS *c = cast(struct CallS *, ud);
  luaD_callnoyield(L, c->func, c->nresults);
}



LUA_API int lua_pcallk (lua_State *L, int nargs, int nresults, int errfunc,
                        lua_KContext ctx, lua_KFunction k) {
  struct CallS c;
  int status;
  ptrdiff_t func;
  lua_lock(L);
  api_check(L, k == NULL || !isLua(L->ci),
    "cannot use continuations inside hooks");
  api_checkpop(L, nargs + 1);
  api_check(L, L->status == LUA_OK, "cannot do calls on non-normal thread");
  checkresults(L, nargs, nresults);
  if (errfunc == 0)
    func = 0;
  else {
    StkId o = index2stack(L, errfunc);
    api_check(L, ttisfunction(s2v(o)), "private_lua_error handler must be a function");
    func = savestack(L, o);
  }
  c.func = L->top.p - (nargs+1);  
  if (k == NULL || !yieldable(L)) {  
    c.nresults = nresults;  
    status = luaD_pcall(L, f_call, &c, savestack(L, c.func), func);
  }
  else {  
    CallInfo *ci = L->ci;
    ci->u.c.k = k;  
    ci->u.c.ctx = ctx;  
    
    ci->u2.funcidx = cast_int(savestack(L, c.func));
    ci->u.c.old_errfunc = L->errfunc;
    L->errfunc = func;
    setoah(ci->callstatus, L->allowhook);  
    ci->callstatus |= CIST_YPCALL;  
    luaD_call(L, c.func, nresults);  
    ci->callstatus &= ~CIST_YPCALL;
    L->errfunc = ci->u.c.old_errfunc;
    status = LUA_OK;  
  }
  adjustresults(L, nresults);
  lua_unlock(L);
  return status;
}


LUA_API int lua_load (lua_State *L, lua_Reader reader, void *data,
                      const char *chunkname, const char *mode) {
  ZIO z;
  int status;
  lua_lock(L);
  if (!chunkname) chunkname = "?";
  luaZ_init(L, &z, reader, data);
  status = luaD_protectedparser(L, &z, chunkname, mode);
  if (status == LUA_OK) {  
    LClosure *f = clLvalue(s2v(L->top.p - 1));  
    if (f->nupvalues >= 1) {  
      
      TValue gt;
      getGlobalTable(L, &gt);
      
      setobj(L, f->upvals[0]->v.p, &gt);
      luaC_barrier(L, f->upvals[0], &gt);
    }
  }
  lua_unlock(L);
  return status;
}



LUA_API int lua_dump (lua_State *L, lua_Writer writer, void *data, int strip) {
  int status;
  ptrdiff_t otop = savestack(L, L->top.p);  
  TValue *f = s2v(L->top.p - 1);  
  lua_lock(L);
  api_checkpop(L, 1);
  api_check(L, isLfunction(f), "Lua function expected");
  status = luaU_dump(L, clLvalue(f)->p, writer, data, strip);
  L->top.p = restorestack(L, otop);  
  lua_unlock(L);
  return status;
}


LUA_API int lua_status (lua_State *L) {
  return L->status;
}



LUA_API int lua_gc (lua_State *L, int what, ...) {
  va_list argp;
  int res = 0;
  global_State *g = G(L);
  if (g->gcstp & (GCSTPGC | GCSTPCLS))  
    return -1;  
  lua_lock(L);
  va_start(argp, what);
  switch (what) {
    case LUA_GCSTOP: {
      g->gcstp = GCSTPUSR;  
      break;
    }
    case LUA_GCRESTART: {
      luaE_setdebt(g, 0);
      g->gcstp = 0;  
      break;
    }
    case LUA_GCCOLLECT: {
      luaC_fullgc(L, 0);
      break;
    }
    case LUA_GCCOUNT: {
      
      res = cast_int(g->totalbytes >> 10);
      break;
    }
    case LUA_GCCOUNTB: {
      res = cast_int(g->totalbytes & 0x3ff);
      break;
    }
    case LUA_GCSTEP: {
      lu_byte oldstp = g->gcstp;
      l_obj n = va_arg(argp, int);
      int work = 0;  
      g->gcstp = 0;  
      if (n <= 0)
        n = g->GCdebt;  
      luaE_setdebt(g, g->GCdebt - n);
      luaC_condGC(L, (void)0, work = 1);
      if (work && g->gcstate == GCSpause)  
        res = 1;  
      g->gcstp = oldstp;  
      break;
    }
    case LUA_GCISRUNNING: {
      res = gcrunning(g);
      break;
    }
    case LUA_GCGEN: {
      res = (g->gckind == KGC_INC) ? LUA_GCINC : LUA_GCGEN;
      luaC_changemode(L, KGC_GENMINOR);
      break;
    }
    case LUA_GCINC: {
      res = (g->gckind == KGC_INC) ? LUA_GCINC : LUA_GCGEN;
      luaC_changemode(L, KGC_INC);
      break;
    }
    case LUA_GCPARAM: {
      int param = va_arg(argp, int);
      int value = va_arg(argp, int);
      api_check(L, 0 <= param && param < LUA_GCPN, "invalid parameter");
      res = cast_int(luaO_applyparam(g->gcparams[param], 100));
      if (value >= 0)
        g->gcparams[param] = luaO_codeparam(value);
      break;
    }
    default: res = -1;  
  }
  va_end(argp);
  lua_unlock(L);
  return res;
}






LUA_API int lua_error (lua_State *L) {
  TValue *errobj;
  lua_lock(L);
  errobj = s2v(L->top.p - 1);
  api_checkpop(L, 1);
  
  if (ttisshrstring(errobj) && eqshrstr(tsvalue(errobj), G(L)->memerrmsg))
    luaM_error(L);  
  else
    luaG_errormsg(L);  
  
  return 0;  
}


LUA_API int lua_next (lua_State *L, int idx) {
  Table *t;
  int more;
  lua_lock(L);
  api_checkpop(L, 1);
  t = gettable(L, idx);
  more = luaH_next(L, t, L->top.p - 1);
  if (more)
    api_incr_top(L);
  else  
    L->top.p--;  
  lua_unlock(L);
  return more;
}


LUA_API void lua_toclose (lua_State *L, int idx) {
  int nresults;
  StkId o;
  lua_lock(L);
  o = index2stack(L, idx);
  nresults = L->ci->nresults;
  api_check(L, L->tbclist.p < o, "given index below or equal a marked one");
  luaF_newtbcupval(L, o);  
  if (!hastocloseCfunc(nresults))  
    L->ci->nresults = codeNresults(nresults);  
  lua_assert(hastocloseCfunc(L->ci->nresults));
  lua_unlock(L);
}


LUA_API void lua_concat (lua_State *L, int n) {
  lua_lock(L);
  api_checknelems(L, n);
  if (n > 0) {
    luaV_concat(L, n);
    luaC_checkGC(L);
  }
  else {  
    setsvalue2s(L, L->top.p, luaS_newlstr(L, "", 0));  
    api_incr_top(L);
  }
  lua_unlock(L);
}


LUA_API void lua_len (lua_State *L, int idx) {
  TValue *t;
  lua_lock(L);
  t = index2value(L, idx);
  luaV_objlen(L, L->top.p, t);
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API lua_Alloc lua_getallocf (lua_State *L, void **ud) {
  lua_Alloc f;
  lua_lock(L);
  if (ud) *ud = G(L)->ud;
  f = G(L)->frealloc;
  lua_unlock(L);
  return f;
}


LUA_API void lua_setallocf (lua_State *L, lua_Alloc f, void *ud) {
  lua_lock(L);
  G(L)->ud = ud;
  G(L)->frealloc = f;
  lua_unlock(L);
}


void lua_setwarnf (lua_State *L, lua_WarnFunction f, void *ud) {
  lua_lock(L);
  G(L)->ud_warn = ud;
  G(L)->warnf = f;
  lua_unlock(L);
}


void lua_warning (lua_State *L, const char *msg, int tocont) {
  lua_lock(L);
  luaE_warning(L, msg, tocont);
  lua_unlock(L);
}



LUA_API void *lua_newuserdatauv (lua_State *L, size_t size, int nuvalue) {
  Udata *u;
  lua_lock(L);
  api_check(L, 0 <= nuvalue && nuvalue < USHRT_MAX, "invalid value");
  u = luaS_newudata(L, size, nuvalue);
  setuvalue(L, s2v(L->top.p), u);
  api_incr_top(L);
  advancegc(L, size);
  luaC_checkGC(L);
  lua_unlock(L);
  return getudatamem(u);
}



static const char *aux_upvalue (TValue *fi, int n, TValue **val,
                                GCObject **owner) {
  switch (ttypetag(fi)) {
    case LUA_VCCL: {  
      CClosure *f = clCvalue(fi);
      if (!(cast_uint(n) - 1u < cast_uint(f->nupvalues)))
        return NULL;  
      *val = &f->upvalue[n-1];
      if (owner) *owner = obj2gco(f);
      return "";
    }
    case LUA_VLCL: {  
      LClosure *f = clLvalue(fi);
      TString *name;
      Proto *p = f->p;
      if (!(cast_uint(n) - 1u  < cast_uint(p->sizeupvalues)))
        return NULL;  
      *val = f->upvals[n-1]->v.p;
      if (owner) *owner = obj2gco(f->upvals[n - 1]);
      name = p->upvalues[n-1].name;
      return (name == NULL) ? "(no name)" : getstr(name);
    }
    default: return NULL;  
  }
}


LUA_API const char *lua_getupvalue (lua_State *L, int funcindex, int n) {
  const char *name;
  TValue *val = NULL;  
  lua_lock(L);
  name = aux_upvalue(index2value(L, funcindex), n, &val, NULL);
  if (name) {
    setobj2s(L, L->top.p, val);
    api_incr_top(L);
  }
  lua_unlock(L);
  return name;
}


LUA_API const char *lua_setupvalue (lua_State *L, int funcindex, int n) {
  const char *name;
  TValue *val = NULL;  
  GCObject *owner = NULL;  
  TValue *fi;
  lua_lock(L);
  fi = index2value(L, funcindex);
  api_checknelems(L, 1);
  name = aux_upvalue(fi, n, &val, &owner);
  if (name) {
    L->top.p--;
    setobj(L, val, s2v(L->top.p));
    luaC_barrier(L, owner, val);
  }
  lua_unlock(L);
  return name;
}


static UpVal **getupvalref (lua_State *L, int fidx, int n, LClosure **pf) {
  static const UpVal *const nullup = NULL;
  LClosure *f;
  TValue *fi = index2value(L, fidx);
  api_check(L, ttisLclosure(fi), "Lua function expected");
  f = clLvalue(fi);
  if (pf) *pf = f;
  if (1 <= n && n <= f->p->sizeupvalues)
    return &f->upvals[n - 1];  
  else
    return (UpVal**)&nullup;
}


LUA_API void *lua_upvalueid (lua_State *L, int fidx, int n) {
  TValue *fi = index2value(L, fidx);
  switch (ttypetag(fi)) {
    case LUA_VLCL: {  
      return *getupvalref(L, fidx, n, NULL);
    }
    case LUA_VCCL: {  
      CClosure *f = clCvalue(fi);
      if (1 <= n && n <= f->nupvalues)
        return &f->upvalue[n - 1];
      
    }  
    case LUA_VLCF:
      return NULL;  
    default: {
      api_check(L, 0, "function expected");
      return NULL;
    }
  }
}


LUA_API void lua_upvaluejoin (lua_State *L, int fidx1, int n1,
                                            int fidx2, int n2) {
  LClosure *f1;
  UpVal **up1 = getupvalref(L, fidx1, n1, &f1);
  UpVal **up2 = getupvalref(L, fidx2, n2, NULL);
  api_check(L, *up1 != NULL && *up2 != NULL, "invalid upvalue index");
  *up1 = *up2;
  luaC_objbarrier(L, f1, *up1);
}






//path: src/imports/../dependencies/lua/lauxlib.c


#define lauxlib_c
#define LUA_LIB





#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>







//path: src/imports/../dependencies/lua/lauxlib.h



#ifndef lauxlib_h
#define lauxlib_h


#include <stddef.h>
#include <stdio.h>








#define LUA_GNAME	"_G"


typedef struct luaL_Buffer luaL_Buffer;



#define LUA_ERRFILE     (LUA_ERRERR+1)



#define LUA_LOADED_TABLE	"_LOADED"



#define LUA_PRELOAD_TABLE	"_PRELOAD"


typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;


#define LUAL_NUMSIZES	(sizeof(lua_Integer)*16 + sizeof(lua_Number))

LUALIB_API void (luaL_checkversion_) (lua_State *L, lua_Number ver, size_t sz);
#define luaL_checkversion(L)  \
	  luaL_checkversion_(L, LUA_VERSION_NUM, LUAL_NUMSIZES)

LUALIB_API int (luaL_getmetafield) (lua_State *L, int obj, const char *e);
LUALIB_API int (luaL_callmeta) (lua_State *L, int obj, const char *e);
LUALIB_API const char *(luaL_tolstring) (lua_State *L, int idx, size_t *len);
LUALIB_API int (luaL_argerror) (lua_State *L, int arg, const char *extramsg);
LUALIB_API int (luaL_typeerror) (lua_State *L, int arg, const char *tname);
LUALIB_API const char *(luaL_checklstring) (lua_State *L, int arg,
                                                          size_t *l);
LUALIB_API const char *(luaL_optlstring) (lua_State *L, int arg,
                                          const char *def, size_t *l);
LUALIB_API lua_Number (luaL_checknumber) (lua_State *L, int arg);
LUALIB_API lua_Number (luaL_optnumber) (lua_State *L, int arg, lua_Number def);

LUALIB_API lua_Integer (luaL_checkinteger) (lua_State *L, int arg);
LUALIB_API lua_Integer (luaL_optinteger) (lua_State *L, int arg,
                                          lua_Integer def);

LUALIB_API void (luaL_checkstack) (lua_State *L, int sz, const char *msg);
LUALIB_API void (luaL_checktype) (lua_State *L, int arg, int t);
LUALIB_API void (luaL_checkany) (lua_State *L, int arg);

LUALIB_API int   (luaL_newmetatable) (lua_State *L, const char *tname);
LUALIB_API void  (luaL_setmetatable) (lua_State *L, const char *tname);
LUALIB_API void *(luaL_testudata) (lua_State *L, int ud, const char *tname);
LUALIB_API void *(luaL_checkudata) (lua_State *L, int ud, const char *tname);

LUALIB_API void (luaL_where) (lua_State *L, int lvl);
LUALIB_API int (luaL_error) (lua_State *L, const char *fmt, ...);

LUALIB_API int (luaL_checkoption) (lua_State *L, int arg, const char *def,
                                   const char *const lst[]);

LUALIB_API int (luaL_fileresult) (lua_State *L, int stat, const char *fname);
LUALIB_API int (luaL_execresult) (lua_State *L, int stat);



#define LUA_NOREF       (-2)
#define LUA_REFNIL      (-1)

LUALIB_API int (luaL_ref) (lua_State *L, int t);
LUALIB_API void (luaL_unref) (lua_State *L, int t, int ref);

LUALIB_API int (luaL_loadfilex) (lua_State *L, const char *filename,
                                               const char *mode);

#define luaL_loadfile(L,f)	luaL_loadfilex(L,f,NULL)

LUALIB_API int (luaL_loadbufferx) (lua_State *L, const char *buff, size_t sz,
                                   const char *name, const char *mode);
LUALIB_API int (luaL_loadstring) (lua_State *L, const char *s);

LUALIB_API lua_State *(luaL_newstate) (void);

LUALIB_API unsigned luaL_makeseed (lua_State *L);

LUALIB_API lua_Integer (luaL_len) (lua_State *L, int idx);

LUALIB_API void (luaL_addgsub) (luaL_Buffer *b, const char *s,
                                     const char *p, const char *r);
LUALIB_API const char *(luaL_gsub) (lua_State *L, const char *s,
                                    const char *p, const char *r);

LUALIB_API void (luaL_setfuncs) (lua_State *L, const luaL_Reg *l, int nup);

LUALIB_API int (luaL_getsubtable) (lua_State *L, int idx, const char *fname);

LUALIB_API void (luaL_traceback) (lua_State *L, lua_State *L1,
                                  const char *msg, int level);

LUALIB_API void (luaL_requiref) (lua_State *L, const char *modname,
                                 lua_CFunction openf, int glb);




#define luaL_newlibtable(L,l)	\
  lua_createtable(L, 0, sizeof(l)/sizeof((l)[0]) - 1)

#define luaL_newlib(L,l)  \
  (luaL_checkversion(L), luaL_newlibtable(L,l), luaL_setfuncs(L,l,0))

#define luaL_argcheck(L, cond,arg,extramsg)	\
	((void)(luai_likely(cond) || luaL_argerror(L, (arg), (extramsg))))

#define luaL_argexpected(L,cond,arg,tname)	\
	((void)(luai_likely(cond) || luaL_typeerror(L, (arg), (tname))))

#define luaL_checkstring(L,n)	(luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d)	(luaL_optlstring(L, (n), (d), NULL))

#define luaL_typename(L,i)	lua_typename(L, lua_type(L,(i)))

#define luaL_dofile(L, fn) \
	(luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0))

#define luaL_dostring(L, s) \
	(luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))

#define luaL_getmetatable(L,n)	(lua_getfield(L, LUA_REGISTRYINDEX, (n)))

#define luaL_opt(L,f,n,d)	(lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))

#define luaL_loadbuffer(L,s,sz,n)	luaL_loadbufferx(L,s,sz,n,NULL)



#define luaL_intop(op,v1,v2)  \
	((lua_Integer)((lua_Unsigned)(v1) op (lua_Unsigned)(v2)))



#define luaL_pushfail(L)	lua_pushnil(L)



#if !defined(lua_assert)

#if defined LUAI_ASSERT
  #include <assert.h>
  #define lua_assert(c)		assert(c)
#else
  #define lua_assert(c)		((void)0)
#endif

#endif





struct luaL_Buffer {
  char *b;  
  size_t size;  
  size_t n;  
  lua_State *L;
  union {
    LUAI_MAXALIGN;  
    char b[LUAL_BUFFERSIZE];  
  } init;
};


#define luaL_bufflen(bf)	((bf)->n)
#define luaL_buffaddr(bf)	((bf)->b)


#define luaL_addchar(B,c) \
  ((void)((B)->n < (B)->size || luaL_prepbuffsize((B), 1)), \
   ((B)->b[(B)->n++] = (c)))

#define luaL_addsize(B,s)	((B)->n += (s))

#define luaL_buffsub(B,s)	((B)->n -= (s))

LUALIB_API void (luaL_buffinit) (lua_State *L, luaL_Buffer *B);
LUALIB_API char *(luaL_prepbuffsize) (luaL_Buffer *B, size_t sz);
LUALIB_API void (luaL_addlstring) (luaL_Buffer *B, const char *s, size_t l);
LUALIB_API void (luaL_addstring) (luaL_Buffer *B, const char *s);
LUALIB_API void (luaL_addvalue) (luaL_Buffer *B);
LUALIB_API void (luaL_pushresult) (luaL_Buffer *B);
LUALIB_API void (luaL_pushresultsize) (luaL_Buffer *B, size_t sz);
LUALIB_API char *(luaL_buffinitsize) (lua_State *L, luaL_Buffer *B, size_t sz);

#define luaL_prepbuffer(B)	luaL_prepbuffsize(B, LUAL_BUFFERSIZE)









#define LUA_FILEHANDLE          "FILE*"


typedef struct luaL_Stream {
  FILE *f;  
  lua_CFunction closef;  
} luaL_Stream;






#if !defined(lua_writestring)
#define lua_writestring(s,l)   fwrite((s), sizeof(char), (l), stdout)
#endif


#if !defined(lua_writeline)
#define lua_writeline()        (lua_writestring("\n", 1), fflush(stdout))
#endif


#if !defined(lua_writestringerror)
#define lua_writestringerror(s,p) \
        (fprintf(stderr, (s), (p)), fflush(stderr))
#endif





#if defined(LUA_COMPAT_APIINTCASTS)

#define luaL_checkunsigned(L,a)	((lua_Unsigned)luaL_checkinteger(L,a))
#define luaL_optunsigned(L,a,d)	\
	((lua_Unsigned)luaL_optinteger(L,a,(lua_Integer)(d)))

#define luaL_checkint(L,n)	((int)luaL_checkinteger(L, (n)))
#define luaL_optint(L,n,d)	((int)luaL_optinteger(L, (n), (d)))

#define luaL_checklong(L,n)	((long)luaL_checkinteger(L, (n)))
#define luaL_optlong(L,n,d)	((long)luaL_optinteger(L, (n), (d)))

#endif




#endif






#if !defined(MAX_SIZET)

#define MAX_SIZET	((size_t)(~(size_t)0))
#endif





#define LEVELS1	10	
#define LEVELS2	11	




static int findfield (lua_State *L, int objidx, int level) {
  if (level == 0 || !lua_istable(L, -1))
    return 0;  
  lua_pushnil(L);  
  while (lua_next(L, -2)) {  
    if (lua_type(L, -2) == LUA_TSTRING) {  
      if (lua_rawequal(L, objidx, -1)) {  
        lua_pop(L, 1);  
        return 1;
      }
      else if (findfield(L, objidx, level - 1)) {  
        
        lua_pushliteral(L, ".");  
        lua_replace(L, -3);  
        lua_concat(L, 3);  
        return 1;
      }
    }
    lua_pop(L, 1);  
  }
  return 0;  
}



static int pushglobalfuncname (lua_State *L, lua_Debug *ar) {
  int top = lua_gettop(L);
  lua_getinfo(L, "f", ar);  
  lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
  if (findfield(L, top + 1, 2)) {
    const char *name = lua_tostring(L, -1);
    if (strncmp(name, LUA_GNAME ".", 3) == 0) {  
      lua_pushstring(L, name + 3);  
      lua_remove(L, -2);  
    }
    lua_copy(L, -1, top + 1);  
    lua_settop(L, top + 1);  
    return 1;
  }
  else {
    lua_settop(L, top);  
    return 0;
  }
}


static void pushfuncname (lua_State *L, lua_Debug *ar) {
  if (pushglobalfuncname(L, ar)) {  
    lua_pushfstring(L, "function '%s'", lua_tostring(L, -1));
    lua_remove(L, -2);  
  }
  else if (*ar->namewhat != '\0')  
    lua_pushfstring(L, "%s '%s'", ar->namewhat, ar->name);  
  else if (*ar->what == 'm')  
      lua_pushliteral(L, "main chunk");
  else if (*ar->what != 'C')  
    lua_pushfstring(L, "function <%s:%d>", ar->short_src, ar->linedefined);
  else  
    lua_pushliteral(L, "?");
}


static int lastlevel (lua_State *L) {
  lua_Debug ar;
  int li = 1, le = 1;
  
  while (lua_getstack(L, le, &ar)) { li = le; le *= 2; }
  
  while (li < le) {
    int m = (li + le)/2;
    if (lua_getstack(L, m, &ar)) li = m + 1;
    else le = m;
  }
  return le - 1;
}


LUALIB_API void luaL_traceback (lua_State *L, lua_State *L1,
                                const char *msg, int level) {
  luaL_Buffer b;
  lua_Debug ar;
  int last = lastlevel(L1);
  int limit2show = (last - level > LEVELS1 + LEVELS2) ? LEVELS1 : -1;
  luaL_buffinit(L, &b);
  if (msg) {
    luaL_addstring(&b, msg);
    luaL_addchar(&b, '\n');
  }
  luaL_addstring(&b, "stack traceback:");
  while (lua_getstack(L1, level++, &ar)) {
    if (limit2show-- == 0) {  
      int n = last - level - LEVELS2 + 1;  
      lua_pushfstring(L, "\n\t...\t(skipping %d levels)", n);
      luaL_addvalue(&b);  
      level += n;  
    }
    else {
      lua_getinfo(L1, "Slnt", &ar);
      if (ar.currentline <= 0)
        lua_pushfstring(L, "\n\t%s: in ", ar.short_src);
      else
        lua_pushfstring(L, "\n\t%s:%d: in ", ar.short_src, ar.currentline);
      luaL_addvalue(&b);
      pushfuncname(L, &ar);
      luaL_addvalue(&b);
      if (ar.istailcall)
        luaL_addstring(&b, "\n\t(...tail calls...)");
    }
  }
  luaL_pushresult(&b);
}






LUALIB_API int luaL_argerror (lua_State *L, int arg, const char *extramsg) {
  lua_Debug ar;
  if (!lua_getstack(L, 0, &ar))  
    return luaL_error(L, "bad argument #%d (%s)", arg, extramsg);
  lua_getinfo(L, "n", &ar);
  if (strcmp(ar.namewhat, "method") == 0) {
    arg--;  
    if (arg == 0)  
      return luaL_error(L, "calling '%s' on bad self (%s)",
                           ar.name, extramsg);
  }
  if (ar.name == NULL)
    ar.name = (pushglobalfuncname(L, &ar)) ? lua_tostring(L, -1) : "?";
  return luaL_error(L, "bad argument #%d to '%s' (%s)",
                        arg, ar.name, extramsg);
}


LUALIB_API int luaL_typeerror (lua_State *L, int arg, const char *tname) {
  const char *msg;
  const char *typearg;  
  if (luaL_getmetafield(L, arg, "__name") == LUA_TSTRING)
    typearg = lua_tostring(L, -1);  
  else if (lua_type(L, arg) == LUA_TLIGHTUSERDATA)
    typearg = "light userdata";  
  else
    typearg = luaL_typename(L, arg);  
  msg = lua_pushfstring(L, "%s expected, got %s", tname, typearg);
  return luaL_argerror(L, arg, msg);
}


static void tag_error (lua_State *L, int arg, int tag) {
  luaL_typeerror(L, arg, lua_typename(L, tag));
}



LUALIB_API void luaL_where (lua_State *L, int level) {
  lua_Debug ar;
  if (lua_getstack(L, level, &ar)) {  
    lua_getinfo(L, "Sl", &ar);  
    if (ar.currentline > 0) {  
      lua_pushfstring(L, "%s:%d: ", ar.short_src, ar.currentline);
      return;
    }
  }
  lua_pushfstring(L, "");  
}



LUALIB_API int luaL_error (lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  luaL_where(L, 1);
  lua_pushvfstring(L, fmt, argp);
  va_end(argp);
  lua_concat(L, 2);
  return lua_error(L);
}


LUALIB_API int luaL_fileresult (lua_State *L, int stat, const char *fname) {
  int en = errno;  
  if (stat) {
    lua_pushboolean(L, 1);
    return 1;
  }
  else {
    luaL_pushfail(L);
    if (fname)
      lua_pushfstring(L, "%s: %s", fname, strerror(en));
    else
      lua_pushstring(L, strerror(en));
    lua_pushinteger(L, en);
    return 3;
  }
}


#if !defined(l_inspectstat)	

#if defined(LUA_USE_POSIX)

#include <sys/wait.h>


#define l_inspectstat(stat,what)  \
   if (WIFEXITED(stat)) { stat = WEXITSTATUS(stat); } \
   else if (WIFSIGNALED(stat)) { stat = WTERMSIG(stat); what = "signal"; }

#else

#define l_inspectstat(stat,what)  

#endif

#endif				


LUALIB_API int luaL_execresult (lua_State *L, int stat) {
  if (stat != 0 && errno != 0)  
    return luaL_fileresult(L, 0, NULL);
  else {
    const char *what = "exit";  
    l_inspectstat(stat, what);  
    if (*what == 'e' && stat == 0)  
      lua_pushboolean(L, 1);
    else
      luaL_pushfail(L);
    lua_pushstring(L, what);
    lua_pushinteger(L, stat);
    return 3;  
  }
}







LUALIB_API int luaL_newmetatable (lua_State *L, const char *tname) {
  if (luaL_getmetatable(L, tname) != LUA_TNIL)  
    return 0;  
  lua_pop(L, 1);
  lua_createtable(L, 0, 2);  
  lua_pushstring(L, tname);
  lua_setfield(L, -2, "__name");  
  lua_pushvalue(L, -1);
  lua_setfield(L, LUA_REGISTRYINDEX, tname);  
  return 1;
}


LUALIB_API void luaL_setmetatable (lua_State *L, const char *tname) {
  luaL_getmetatable(L, tname);
  lua_setmetatable(L, -2);
}


LUALIB_API void *luaL_testudata (lua_State *L, int ud, const char *tname) {
  void *p = lua_touserdata(L, ud);
  if (p != NULL) {  
    if (lua_getmetatable(L, ud)) {  
      luaL_getmetatable(L, tname);  
      if (!lua_rawequal(L, -1, -2))  
        p = NULL;  
      lua_pop(L, 2);  
      return p;
    }
  }
  return NULL;  
}


LUALIB_API void *luaL_checkudata (lua_State *L, int ud, const char *tname) {
  void *p = luaL_testudata(L, ud, tname);
  luaL_argexpected(L, p != NULL, ud, tname);
  return p;
}






LUALIB_API int luaL_checkoption (lua_State *L, int arg, const char *def,
                                 const char *const lst[]) {
  const char *name = (def) ? luaL_optstring(L, arg, def) :
                             luaL_checkstring(L, arg);
  int i;
  for (i=0; lst[i]; i++)
    if (strcmp(lst[i], name) == 0)
      return i;
  return luaL_argerror(L, arg,
                       lua_pushfstring(L, "invalid option '%s'", name));
}



LUALIB_API void luaL_checkstack (lua_State *L, int space, const char *msg) {
  if (l_unlikely(!lua_checkstack(L, space))) {
    if (msg)
      luaL_error(L, "stack overflow (%s)", msg);
    else
      luaL_error(L, "stack overflow");
  }
}


LUALIB_API void luaL_checktype (lua_State *L, int arg, int t) {
  if (l_unlikely(lua_type(L, arg) != t))
    tag_error(L, arg, t);
}


LUALIB_API void luaL_checkany (lua_State *L, int arg) {
  if (l_unlikely(lua_type(L, arg) == LUA_TNONE))
    luaL_argerror(L, arg, "value expected");
}


LUALIB_API const char *luaL_checklstring (lua_State *L, int arg, size_t *len) {
  const char *s = lua_tolstring(L, arg, len);
  if (l_unlikely(!s)) tag_error(L, arg, LUA_TSTRING);
  return s;
}


LUALIB_API const char *luaL_optlstring (lua_State *L, int arg,
                                        const char *def, size_t *len) {
  if (lua_isnoneornil(L, arg)) {
    if (len)
      *len = (def ? strlen(def) : 0);
    return def;
  }
  else return luaL_checklstring(L, arg, len);
}


LUALIB_API lua_Number luaL_checknumber (lua_State *L, int arg) {
  int isnum;
  lua_Number d = lua_tonumberx(L, arg, &isnum);
  if (l_unlikely(!isnum))
    tag_error(L, arg, LUA_TNUMBER);
  return d;
}


LUALIB_API lua_Number luaL_optnumber (lua_State *L, int arg, lua_Number def) {
  return luaL_opt(L, luaL_checknumber, arg, def);
}


static void interror (lua_State *L, int arg) {
  if (lua_isnumber(L, arg))
    luaL_argerror(L, arg, "number has no integer representation");
  else
    tag_error(L, arg, LUA_TNUMBER);
}


LUALIB_API lua_Integer luaL_checkinteger (lua_State *L, int arg) {
  int isnum;
  lua_Integer d = lua_tointegerx(L, arg, &isnum);
  if (l_unlikely(!isnum)) {
    interror(L, arg);
  }
  return d;
}


LUALIB_API lua_Integer luaL_optinteger (lua_State *L, int arg,
                                                      lua_Integer def) {
  return luaL_opt(L, luaL_checkinteger, arg, def);
}







typedef struct UBox {
  void *box;
  size_t bsize;
} UBox;



static void *resizebox (lua_State *L, int idx, size_t newsize) {
  UBox *box = (UBox *)lua_touserdata(L, idx);
  if (box->bsize == newsize)  
    return box->box;  
  else {
    void *ud;
    lua_Alloc allocf = lua_getallocf(L, &ud);
    void *temp = allocf(ud, box->box, box->bsize, newsize);
    if (l_unlikely(temp == NULL && newsize > 0)) {  
      lua_pushliteral(L, "not enough memory");
      lua_error(L);  
    }
    box->box = temp;
    box->bsize = newsize;
    return temp;
  }
}


static int boxgc (lua_State *L) {
  resizebox(L, 1, 0);
  return 0;
}


static const luaL_Reg boxmt[] = {  
  {"__gc", boxgc},
  {"__close", boxgc},
  {NULL, NULL}
};


static void newbox (lua_State *L) {
  UBox *box = (UBox *)lua_newuserdatauv(L, sizeof(UBox), 0);
  box->box = NULL;
  box->bsize = 0;
  if (luaL_newmetatable(L, "_UBOX*"))  
    luaL_setfuncs(L, boxmt, 0);  
  lua_setmetatable(L, -2);
}



#define buffonstack(B)	((B)->b != (B)->init.b)



#define checkbufferlevel(B,idx)  \
  lua_assert(buffonstack(B) ? lua_touserdata(B->L, idx) != NULL  \
                            : lua_touserdata(B->L, idx) == (void*)B)



static size_t newbuffsize (luaL_Buffer *B, size_t sz) {
  size_t newsize = (B->size / 2) * 3;  
  if (l_unlikely(MAX_SIZET - sz - 1 < B->n))  
    return luaL_error(B->L, "buffer too large");
  if (newsize < B->n + sz + 1)  
    newsize = B->n + sz + 1;
  return newsize;
}



static char *prepbuffsize (luaL_Buffer *B, size_t sz, int boxidx) {
  checkbufferlevel(B, boxidx);
  if (B->size - B->n >= sz)  
    return B->b + B->n;
  else {
    lua_State *L = B->L;
    char *newbuff;
    size_t newsize = newbuffsize(B, sz);
    
    if (buffonstack(B))  
      newbuff = (char *)resizebox(L, boxidx, newsize);  
    else {  
      lua_remove(L, boxidx);  
      newbox(L);  
      lua_insert(L, boxidx);  
      lua_toclose(L, boxidx);
      newbuff = (char *)resizebox(L, boxidx, newsize);
      memcpy(newbuff, B->b, B->n * sizeof(char));  
    }
    B->b = newbuff;
    B->size = newsize;
    return newbuff + B->n;
  }
}


LUALIB_API char *luaL_prepbuffsize (luaL_Buffer *B, size_t sz) {
  return prepbuffsize(B, sz, -1);
}


LUALIB_API void luaL_addlstring (luaL_Buffer *B, const char *s, size_t l) {
  if (l > 0) {  
    char *b = prepbuffsize(B, l, -1);
    memcpy(b, s, l * sizeof(char));
    luaL_addsize(B, l);
  }
}


LUALIB_API void luaL_addstring (luaL_Buffer *B, const char *s) {
  luaL_addlstring(B, s, strlen(s));
}


LUALIB_API void luaL_pushresult (luaL_Buffer *B) {
  lua_State *L = B->L;
  checkbufferlevel(B, -1);
  if (!buffonstack(B))  
    lua_pushlstring(L, B->b, B->n);  
  else {  
    UBox *box = (UBox *)lua_touserdata(L, -1);
    void *ud;
    lua_Alloc allocf = lua_getallocf(L, &ud);  
    size_t len = B->n;  
    char *s;
    resizebox(L, -1, len + 1);  
    s = (char*)box->box;  
    s[len] = '\0';  
    
    box->bsize = 0;  box->box = NULL;
    lua_pushextlstring(L, s, len, allocf, ud);
    lua_closeslot(L, -2);  
  }
  lua_remove(L, -2);  
}


LUALIB_API void luaL_pushresultsize (luaL_Buffer *B, size_t sz) {
  luaL_addsize(B, sz);
  luaL_pushresult(B);
}



LUALIB_API void luaL_addvalue (luaL_Buffer *B) {
  lua_State *L = B->L;
  size_t len;
  const char *s = lua_tolstring(L, -1, &len);
  char *b = prepbuffsize(B, len, -2);
  memcpy(b, s, len * sizeof(char));
  luaL_addsize(B, len);
  lua_pop(L, 1);  
}


LUALIB_API void luaL_buffinit (lua_State *L, luaL_Buffer *B) {
  B->L = L;
  B->b = B->init.b;
  B->n = 0;
  B->size = LUAL_BUFFERSIZE;
  lua_pushlightuserdata(L, (void*)B);  
}


LUALIB_API char *luaL_buffinitsize (lua_State *L, luaL_Buffer *B, size_t sz) {
  luaL_buffinit(L, B);
  return prepbuffsize(B, sz, -1);
}







LUALIB_API int luaL_ref (lua_State *L, int t) {
  int ref;
  if (lua_isnil(L, -1)) {
    lua_pop(L, 1);  
    return LUA_REFNIL;  
  }
  t = lua_absindex(L, t);
  if (lua_rawgeti(L, t, 1) == LUA_TNUMBER)  
    ref = (int)lua_tointeger(L, -1);  
  else {  
    lua_assert(!lua_toboolean(L, -1));  
    ref = 0;  
    lua_pushinteger(L, 0);  
    lua_rawseti(L, t, 1);  
  }
  lua_pop(L, 1);  
  if (ref != 0) {  
    lua_rawgeti(L, t, ref);  
    lua_rawseti(L, t, 1);  
  }
  else  
    ref = (int)lua_rawlen(L, t) + 1;  
  lua_rawseti(L, t, ref);
  return ref;
}


LUALIB_API void luaL_unref (lua_State *L, int t, int ref) {
  if (ref >= 0) {
    t = lua_absindex(L, t);
    lua_rawgeti(L, t, 1);
    lua_assert(lua_isinteger(L, -1));
    lua_rawseti(L, t, ref);  
    lua_pushinteger(L, ref);
    lua_rawseti(L, t, 1);  
  }
}






typedef struct LoadF {
  int n;  
  FILE *f;  
  char buff[BUFSIZ];  
} LoadF;


static const char *getF (lua_State *L, void *ud, size_t *size) {
  LoadF *lf = (LoadF *)ud;
  (void)L;  
  if (lf->n > 0) {  
    *size = lf->n;  
    lf->n = 0;  
  }
  else {  
    
    if (feof(lf->f)) return NULL;
    *size = fread(lf->buff, 1, sizeof(lf->buff), lf->f);  
  }
  return lf->buff;
}


static int errfile (lua_State *L, const char *what, int fnameindex) {
  const char *serr = strerror(errno);
  const char *filename = lua_tostring(L, fnameindex) + 1;
  lua_pushfstring(L, "cannot %s %s: %s", what, filename, serr);
  lua_remove(L, fnameindex);
  return LUA_ERRFILE;
}



static int skipBOM (FILE *f) {
  int c = getc(f);  
  if (c == 0xEF && getc(f) == 0xBB && getc(f) == 0xBF)  
    return getc(f);  
  else  
    return c;  
}



static int skipcomment (FILE *f, int *cp) {
  int c = *cp = skipBOM(f);
  if (c == '#') {  
    do {  
      c = getc(f);
    } while (c != EOF && c != '\n');
    *cp = getc(f);  
    return 1;  
  }
  else return 0;  
}


LUALIB_API int luaL_loadfilex (lua_State *L, const char *filename,
                                             const char *mode) {
  LoadF lf;
  int status, readstatus;
  int c;
  int fnameindex = lua_gettop(L) + 1;  
  if (filename == NULL) {
    lua_pushliteral(L, "=stdin");
    lf.f = stdin;
  }
  else {
    lua_pushfstring(L, "@%s", filename);
    lf.f = fopen(filename, "r");
    if (lf.f == NULL) return errfile(L, "open", fnameindex);
  }
  lf.n = 0;
  if (skipcomment(lf.f, &c))  
    lf.buff[lf.n++] = '\n';  
  if (c == LUA_SIGNATURE[0]) {  
    lf.n = 0;  
    if (filename) {  
      lf.f = freopen(filename, "rb", lf.f);  
      if (lf.f == NULL) return errfile(L, "reopen", fnameindex);
      skipcomment(lf.f, &c);  
    }
  }
  if (c != EOF)
    lf.buff[lf.n++] = c;  
  status = lua_load(L, getF, &lf, lua_tostring(L, -1), mode);
  readstatus = ferror(lf.f);
  if (filename) fclose(lf.f);  
  if (readstatus) {
    lua_settop(L, fnameindex);  
    return errfile(L, "read", fnameindex);
  }
  lua_remove(L, fnameindex);
  return status;
}


typedef struct LoadS {
  const char *s;
  size_t size;
} LoadS;


static const char *getS (lua_State *L, void *ud, size_t *size) {
  LoadS *ls = (LoadS *)ud;
  (void)L;  
  if (ls->size == 0) return NULL;
  *size = ls->size;
  ls->size = 0;
  return ls->s;
}


LUALIB_API int luaL_loadbufferx (lua_State *L, const char *buff, size_t size,
                                 const char *name, const char *mode) {
  LoadS ls;
  ls.s = buff;
  ls.size = size;
  return lua_load(L, getS, &ls, name, mode);
}


LUALIB_API int luaL_loadstring (lua_State *L, const char *s) {
  return luaL_loadbuffer(L, s, strlen(s), s);
}





LUALIB_API int luaL_getmetafield (lua_State *L, int obj, const char *event) {
  if (!lua_getmetatable(L, obj))  
    return LUA_TNIL;
  else {
    int tt;
    lua_pushstring(L, event);
    tt = lua_rawget(L, -2);
    if (tt == LUA_TNIL)  
      lua_pop(L, 2);  
    else
      lua_remove(L, -2);  
    return tt;  
  }
}


LUALIB_API int luaL_callmeta (lua_State *L, int obj, const char *event) {
  obj = lua_absindex(L, obj);
  if (luaL_getmetafield(L, obj, event) == LUA_TNIL)  
    return 0;
  lua_pushvalue(L, obj);
  lua_call(L, 1, 1);
  return 1;
}


LUALIB_API lua_Integer luaL_len (lua_State *L, int idx) {
  lua_Integer l;
  int isnum;
  lua_len(L, idx);
  l = lua_tointegerx(L, -1, &isnum);
  if (l_unlikely(!isnum))
    luaL_error(L, "object length is not an integer");
  lua_pop(L, 1);  
  return l;
}


LUALIB_API const char *luaL_tolstring (lua_State *L, int idx, size_t *len) {
  idx = lua_absindex(L,idx);
  if (luaL_callmeta(L, idx, "__tostring")) {  
    if (!lua_isstring(L, -1))
      luaL_error(L, "'__tostring' must return a string");
  }
  else {
    switch (lua_type(L, idx)) {
      case LUA_TNUMBER: {
        if (lua_isinteger(L, idx))
          lua_pushfstring(L, "%I", (LUAI_UACINT)lua_tointeger(L, idx));
        else
          lua_pushfstring(L, "%f", (LUAI_UACNUMBER)lua_tonumber(L, idx));
        break;
      }
      case LUA_TSTRING:
        lua_pushvalue(L, idx);
        break;
      case LUA_TBOOLEAN:
        lua_pushstring(L, (lua_toboolean(L, idx) ? "true" : "false"));
        break;
      case LUA_TNIL:
        lua_pushliteral(L, "nil");
        break;
      default: {
        int tt = luaL_getmetafield(L, idx, "__name");  
        const char *kind = (tt == LUA_TSTRING) ? lua_tostring(L, -1) :
                                                 luaL_typename(L, idx);
        lua_pushfstring(L, "%s: %p", kind, lua_topointer(L, idx));
        if (tt != LUA_TNIL)
          lua_remove(L, -2);  
        break;
      }
    }
  }
  return lua_tolstring(L, -1, len);
}



LUALIB_API void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
  luaL_checkstack(L, nup, "too many upvalues");
  for (; l->name != NULL; l++) {  
    if (l->func == NULL)  
      lua_pushboolean(L, 0);
    else {
      int i;
      for (i = 0; i < nup; i++)  
        lua_pushvalue(L, -nup);
      lua_pushcclosure(L, l->func, nup);  
    }
    lua_setfield(L, -(nup + 2), l->name);
  }
  lua_pop(L, nup);  
}



LUALIB_API int luaL_getsubtable (lua_State *L, int idx, const char *fname) {
  if (lua_getfield(L, idx, fname) == LUA_TTABLE)
    return 1;  
  else {
    lua_pop(L, 1);  
    idx = lua_absindex(L, idx);
    lua_newtable(L);
    lua_pushvalue(L, -1);  
    lua_setfield(L, idx, fname);  
    return 0;  
  }
}



LUALIB_API void luaL_requiref (lua_State *L, const char *modname,
                               lua_CFunction openf, int glb) {
  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
  lua_getfield(L, -1, modname);  
  if (!lua_toboolean(L, -1)) {  
    lua_pop(L, 1);  
    lua_pushcfunction(L, openf);
    lua_pushstring(L, modname);  
    lua_call(L, 1, 1);  
    lua_pushvalue(L, -1);  
    lua_setfield(L, -3, modname);  
  }
  lua_remove(L, -2);  
  if (glb) {
    lua_pushvalue(L, -1);  
    lua_setglobal(L, modname);  
  }
}


LUALIB_API void luaL_addgsub (luaL_Buffer *b, const char *s,
                                     const char *p, const char *r) {
  const char *wild;
  size_t l = strlen(p);
  while ((wild = strstr(s, p)) != NULL) {
    luaL_addlstring(b, s, wild - s);  
    luaL_addstring(b, r);  
    s = wild + l;  
  }
  luaL_addstring(b, s);  
}


LUALIB_API const char *luaL_gsub (lua_State *L, const char *s,
                                  const char *p, const char *r) {
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  luaL_addgsub(&b, s, p, r);
  luaL_pushresult(&b);
  return lua_tostring(L, -1);
}


static void *l_alloc (void *ud, void *ptr, size_t osize, size_t nsize) {
  (void)ud; (void)osize;  
  if (nsize == 0) {
    free(ptr);
    return NULL;
  }
  else
    return realloc(ptr, nsize);
}



static int panic (lua_State *L) {
  const char *msg = (lua_type(L, -1) == LUA_TSTRING)
                  ? lua_tostring(L, -1)
                  : "private_lua_error object is not a string";
  lua_writestringerror("PANIC: unprotected private_lua_error in call to Lua API (%s)\n",
                        msg);
  return 0;  
}



static void warnfoff (void *ud, const char *message, int tocont);
static void warnfon (void *ud, const char *message, int tocont);
static void warnfcont (void *ud, const char *message, int tocont);



static int checkcontrol (lua_State *L, const char *message, int tocont) {
  if (tocont || *(message++) != '@')  
    return 0;
  else {
    if (strcmp(message, "off") == 0)
      lua_setwarnf(L, warnfoff, L);  
    else if (strcmp(message, "on") == 0)
      lua_setwarnf(L, warnfon, L);   
    return 1;  
  }
}


static void warnfoff (void *ud, const char *message, int tocont) {
  checkcontrol((lua_State *)ud, message, tocont);
}



static void warnfcont (void *ud, const char *message, int tocont) {
  lua_State *L = (lua_State *)ud;
  lua_writestringerror("%s", message);  
  if (tocont)  
    lua_setwarnf(L, warnfcont, L);  
  else {  
    lua_writestringerror("%s", "\n");  
    lua_setwarnf(L, warnfon, L);  
  }
}


static void warnfon (void *ud, const char *message, int tocont) {
  if (checkcontrol((lua_State *)ud, message, tocont))  
    return;  
  lua_writestringerror("%s", "Lua warning: ");  
  warnfcont(ud, message, tocont);  
}




#if !defined(luai_makeseed)

#include <time.h>



#define BUFSEEDB	(sizeof(void*) + sizeof(time_t))


#define BUFSEED		((BUFSEEDB + sizeof(int) - 1) / sizeof(int))


#define addbuff(b,v)	(memcpy(&b[0], &(v), sizeof(v)), b += sizeof(v))


static unsigned int luai_makeseed (void) {
  unsigned int buff[BUFSEED];
  unsigned int res;
  unsigned int i;
  time_t t = time(NULL);
  char *b = (char*)buff;
  addbuff(b, b);  
  addbuff(b, t);  
  
  memset(b, 0, sizeof(buff) - BUFSEEDB);
  res = buff[0];
  for (i = 1; i < BUFSEED; i++)
    res ^= (res >> 3) + (res << 7) + buff[i];
  return res;
}

#endif


LUALIB_API unsigned int luaL_makeseed (lua_State *L) {
  (void)L;  
  return luai_makeseed();
}


LUALIB_API lua_State *luaL_newstate (void) {
  lua_State *L = lua_newstate(l_alloc, NULL, luai_makeseed());
  if (l_likely(L)) {
    lua_atpanic(L, &panic);
    lua_setwarnf(L, warnfoff, L);  
  }
  return L;
}


LUALIB_API void luaL_checkversion_ (lua_State *L, lua_Number ver, size_t sz) {
  lua_Number v = lua_version(L);
  if (sz != LUAL_NUMSIZES)  
    luaL_error(L, "core and library have incompatible numeric types");
  else if (v != ver)
    luaL_error(L, "version mismatch: app. needs %f, Lua core provides %f",
                  (LUAI_UACNUMBER)ver, (LUAI_UACNUMBER)v);
}







#ifndef MAKE_LUAC
//path: src/imports/../dependencies/lua/lbaselib.c


#define lbaselib_c
#define LUA_LIB





#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>






//path: src/imports/../dependencies/lua/lualib.h



#ifndef lualib_h
#define lualib_h






#define LUA_VERSUFFIX          "_" LUA_VERSION_MAJOR "_" LUA_VERSION_MINOR

#define LUA_GLIBK		1
LUAMOD_API int (luaopen_base) (lua_State *L);

#define LUA_LOADLIBNAME	"package"
#define LUA_LOADLIBK	(LUA_GLIBK << 1)
LUAMOD_API int (luaopen_package) (lua_State *L);


#define LUA_COLIBNAME	"coroutine"
#define LUA_COLIBK	(LUA_LOADLIBK << 1)
LUAMOD_API int (luaopen_coroutine) (lua_State *L);

#define LUA_DBLIBNAME	"debug"
#define LUA_DBLIBK	(LUA_COLIBK << 1)
LUAMOD_API int (luaopen_debug) (lua_State *L);

#define LUA_IOLIBNAME	"io"
#define LUA_IOLIBK	(LUA_DBLIBK << 1)
LUAMOD_API int (luaopen_io) (lua_State *L);

#define LUA_MATHLIBNAME	"math"
#define LUA_MATHLIBK	(LUA_IOLIBK << 1)
LUAMOD_API int (luaopen_math) (lua_State *L);

#define LUA_OSLIBNAME	"os"
#define LUA_OSLIBK	(LUA_MATHLIBK << 1)
LUAMOD_API int (luaopen_os) (lua_State *L);

#define LUA_STRLIBNAME	"string"
#define LUA_STRLIBK	(LUA_OSLIBK << 1)
LUAMOD_API int (luaopen_string) (lua_State *L);

#define LUA_TABLIBNAME	"table"
#define LUA_TABLIBK	(LUA_STRLIBK << 1)
LUAMOD_API int (luaopen_table) (lua_State *L);

#define LUA_UTF8LIBNAME	"utf8"
#define LUA_UTF8LIBK	(LUA_TABLIBK << 1)
LUAMOD_API int (luaopen_utf8) (lua_State *L);



LUALIB_API void (luaL_openselectedlibs) (lua_State *L, int load, int preload);


#define luaL_openlibs(L)	luaL_openselectedlibs(L, ~0, 0)


#endif




static int luaB_print (lua_State *L) {
  int n = lua_gettop(L);  
  int i;
  for (i = 1; i <= n; i++) {  
    size_t l;
    const char *s = luaL_tolstring(L, i, &l);  
    if (i > 1)  
      lua_writestring("\t", 1);  
    lua_writestring(s, l);  
    lua_pop(L, 1);  
  }
  lua_writeline();
  return 0;
}



static int luaB_warn (lua_State *L) {
  int n = lua_gettop(L);  
  int i;
  luaL_checkstring(L, 1);  
  for (i = 2; i <= n; i++)
    luaL_checkstring(L, i);  
  for (i = 1; i < n; i++)  
    lua_warning(L, lua_tostring(L, i), 1);
  lua_warning(L, lua_tostring(L, n), 0);  
  return 0;
}


#define SPACECHARS	" \f\n\r\t\v"

static const char *b_str2int (const char *s, int base, lua_Integer *pn) {
  lua_Unsigned n = 0;
  int neg = 0;
  s += strspn(s, SPACECHARS);  
  if (*s == '-') { s++; neg = 1; }  
  else if (*s == '+') s++;
  if (!isalnum((unsigned char)*s))  
    return NULL;
  do {
    int digit = (isdigit((unsigned char)*s)) ? *s - '0'
                   : (toupper((unsigned char)*s) - 'A') + 10;
    if (digit >= base) return NULL;  
    n = n * base + digit;
    s++;
  } while (isalnum((unsigned char)*s));
  s += strspn(s, SPACECHARS);  
  *pn = (lua_Integer)((neg) ? (0u - n) : n);
  return s;
}


static int luaB_tonumber (lua_State *L) {
  if (lua_isnoneornil(L, 2)) {  
    if (lua_type(L, 1) == LUA_TNUMBER) {  
      lua_settop(L, 1);  
      return 1;
    }
    else {
      size_t l;
      const char *s = lua_tolstring(L, 1, &l);
      if (s != NULL && lua_stringtonumber(L, s) == l + 1)
        return 1;  
      
      luaL_checkany(L, 1);  
    }
  }
  else {
    size_t l;
    const char *s;
    lua_Integer n = 0;  
    lua_Integer base = luaL_checkinteger(L, 2);
    luaL_checktype(L, 1, LUA_TSTRING);  
    s = lua_tolstring(L, 1, &l);
    luaL_argcheck(L, 2 <= base && base <= 36, 2, "base out of range");
    if (b_str2int(s, (int)base, &n) == s + l) {
      lua_pushinteger(L, n);
      return 1;
    }  
  }  
  luaL_pushfail(L);  
  return 1;
}


static int luaB_error (lua_State *L) {
  int level = (int)luaL_optinteger(L, 2, 1);
  lua_settop(L, 1);
  if (lua_type(L, 1) == LUA_TSTRING && level > 0) {
    luaL_where(L, level);   
    lua_pushvalue(L, 1);
    lua_concat(L, 2);
  }
  return lua_error(L);
}


static int luaB_getmetatable (lua_State *L) {
  luaL_checkany(L, 1);
  if (!lua_getmetatable(L, 1)) {
    lua_pushnil(L);
    return 1;  
  }
  luaL_getmetafield(L, 1, "__metatable");
  return 1;  
}


static int luaB_setmetatable (lua_State *L) {
  int t = lua_type(L, 2);
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_argexpected(L, t == LUA_TNIL || t == LUA_TTABLE, 2, "nil or table");
  if (l_unlikely(luaL_getmetafield(L, 1, "__metatable") != LUA_TNIL))
    return luaL_error(L, "cannot change a protected metatable");
  lua_settop(L, 2);
  lua_setmetatable(L, 1);
  return 1;
}


static int luaB_rawequal (lua_State *L) {
  luaL_checkany(L, 1);
  luaL_checkany(L, 2);
  lua_pushboolean(L, lua_rawequal(L, 1, 2));
  return 1;
}


static int luaB_rawlen (lua_State *L) {
  int t = lua_type(L, 1);
  luaL_argexpected(L, t == LUA_TTABLE || t == LUA_TSTRING, 1,
                      "table or string");
  lua_pushinteger(L, lua_rawlen(L, 1));
  return 1;
}


static int luaB_rawget (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checkany(L, 2);
  lua_settop(L, 2);
  lua_rawget(L, 1);
  return 1;
}

static int luaB_rawset (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checkany(L, 2);
  luaL_checkany(L, 3);
  lua_settop(L, 3);
  lua_rawset(L, 1);
  return 1;
}


static int pushmode (lua_State *L, int oldmode) {
  if (oldmode == -1)
    luaL_pushfail(L);  
  else
    lua_pushstring(L, (oldmode == LUA_GCINC) ? "incremental"
                                             : "generational");
  return 1;
}



#define checkvalres(res) { if (res == -1) break; }

static int luaB_collectgarbage (lua_State *L) {
  static const char *const opts[] = {"stop", "restart", "collect",
    "count", "step", "isrunning", "generational", "incremental",
    "param", NULL};
  static const char optsnum[] = {LUA_GCSTOP, LUA_GCRESTART, LUA_GCCOLLECT,
    LUA_GCCOUNT, LUA_GCSTEP, LUA_GCISRUNNING, LUA_GCGEN, LUA_GCINC,
    LUA_GCPARAM};
  int o = optsnum[luaL_checkoption(L, 1, "collect", opts)];
  switch (o) {
    case LUA_GCCOUNT: {
      int k = lua_gc(L, o);
      int b = lua_gc(L, LUA_GCCOUNTB);
      checkvalres(k);
      lua_pushnumber(L, (lua_Number)k + ((lua_Number)b/1024));
      return 1;
    }
    case LUA_GCSTEP: {
      lua_Integer n = luaL_optinteger(L, 2, 0);
      int res = lua_gc(L, o, (int)n);
      checkvalres(res);
      lua_pushboolean(L, res);
      return 1;
    }
    case LUA_GCISRUNNING: {
      int res = lua_gc(L, o);
      checkvalres(res);
      lua_pushboolean(L, res);
      return 1;
    }
    case LUA_GCGEN: {
      return pushmode(L, lua_gc(L, o));
    }
    case LUA_GCINC: {
      return pushmode(L, lua_gc(L, o));
    }
    case LUA_GCPARAM: {
      static const char *const params[] = {
        "minormul", "majorminor", "minormajor",
        "pause", "stepmul", "stepsize", NULL};
      static const char pnum[] = {
        LUA_GCPMINORMUL, LUA_GCPMAJORMINOR, LUA_GCPMINORMAJOR,
        LUA_GCPPAUSE, LUA_GCPSTEPMUL, LUA_GCPSTEPSIZE};
      int p = pnum[luaL_checkoption(L, 2, NULL, params)];
      lua_Integer value = luaL_optinteger(L, 3, -1);
      lua_pushinteger(L, lua_gc(L, o, p, (int)value));
      return 1;
    }
    default: {
      int res = lua_gc(L, o);
      checkvalres(res);
      lua_pushinteger(L, res);
      return 1;
    }
  }
  luaL_pushfail(L);  
  return 1;
}


static int luaB_type (lua_State *L) {
  int t = lua_type(L, 1);
  luaL_argcheck(L, t != LUA_TNONE, 1, "value expected");
  lua_pushstring(L, lua_typename(L, t));
  return 1;
}


static int luaB_next (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 2);  
  if (lua_next(L, 1))
    return 2;
  else {
    lua_pushnil(L);
    return 1;
  }
}


static int pairscont (lua_State *L, int status, lua_KContext k) {
  (void)L; (void)status; (void)k;  
  return 3;
}

static int luaB_pairs (lua_State *L) {
  luaL_checkany(L, 1);
  if (luaL_getmetafield(L, 1, "__pairs") == LUA_TNIL) {  
    lua_pushcfunction(L, luaB_next);  
    lua_pushvalue(L, 1);  
    lua_pushnil(L);  
  }
  else {
    lua_pushvalue(L, 1);  
    lua_callk(L, 1, 3, 0, pairscont);  
  }
  return 3;
}



static int ipairsaux (lua_State *L) {
  lua_Integer i = luaL_checkinteger(L, 2);
  i = luaL_intop(+, i, 1);
  lua_pushinteger(L, i);
  return (lua_geti(L, 1, i) == LUA_TNIL) ? 1 : 2;
}



static int luaB_ipairs (lua_State *L) {
  luaL_checkany(L, 1);
  lua_pushcfunction(L, ipairsaux);  
  lua_pushvalue(L, 1);  
  lua_pushinteger(L, 0);  
  return 3;
}


static int load_aux (lua_State *L, int status, int envidx) {
  if (l_likely(status == LUA_OK)) {
    if (envidx != 0) {  
      lua_pushvalue(L, envidx);  
      if (!lua_setupvalue(L, -2, 1))  
        lua_pop(L, 1);  
    }
    return 1;
  }
  else {  
    luaL_pushfail(L);
    lua_insert(L, -2);  
    return 2;  
  }
}


static const char *getmode (lua_State *L, int idx) {
  const char *mode = luaL_optstring(L, idx, "bt");
  if (strchr(mode, 'B') != NULL)  
    luaL_argerror(L, idx, "invalid mode");
  return mode;
}


static int luaB_loadfile (lua_State *L) {
  const char *fname = luaL_optstring(L, 1, NULL);
  const char *mode = getmode(L, 2);
  int env = (!lua_isnone(L, 3) ? 3 : 0);  
  int status = luaL_loadfilex(L, fname, mode);
  return load_aux(L, status, env);
}






#define RESERVEDSLOT	5



static const char *generic_reader (lua_State *L, void *ud, size_t *size) {
  (void)(ud);  
  luaL_checkstack(L, 2, "too many nested functions");
  lua_pushvalue(L, 1);  
  lua_call(L, 0, 1);  
  if (lua_isnil(L, -1)) {
    lua_pop(L, 1);  
    *size = 0;
    return NULL;
  }
  else if (l_unlikely(!lua_isstring(L, -1)))
    luaL_error(L, "reader function must return a string");
  lua_replace(L, RESERVEDSLOT);  
  return lua_tolstring(L, RESERVEDSLOT, size);
}


static int luaB_load (lua_State *L) {
  int status;
  size_t l;
  const char *s = lua_tolstring(L, 1, &l);
  const char *mode = getmode(L, 3);
  int env = (!lua_isnone(L, 4) ? 4 : 0);  
  if (s != NULL) {  
    const char *chunkname = luaL_optstring(L, 2, s);
    status = luaL_loadbufferx(L, s, l, chunkname, mode);
  }
  else {  
    const char *chunkname = luaL_optstring(L, 2, "=(load)");
    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_settop(L, RESERVEDSLOT);  
    status = lua_load(L, generic_reader, NULL, chunkname, mode);
  }
  return load_aux(L, status, env);
}




static int dofilecont (lua_State *L, int d1, lua_KContext d2) {
  (void)d1;  (void)d2;  
  return lua_gettop(L) - 1;
}


static int luaB_dofile (lua_State *L) {
  const char *fname = luaL_optstring(L, 1, NULL);
  lua_settop(L, 1);
  if (l_unlikely(luaL_loadfile(L, fname) != LUA_OK))
    return lua_error(L);
  lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);
  return dofilecont(L, 0, 0);
}


static int luaB_assert (lua_State *L) {
  if (l_likely(lua_toboolean(L, 1)))  
    return lua_gettop(L);  
  else {  
    luaL_checkany(L, 1);  
    lua_remove(L, 1);  
    lua_pushliteral(L, "assertion failed!");  
    lua_settop(L, 1);  
    return luaB_error(L);  
  }
}


static int luaB_select (lua_State *L) {
  int n = lua_gettop(L);
  if (lua_type(L, 1) == LUA_TSTRING && *lua_tostring(L, 1) == '#') {
    lua_pushinteger(L, n-1);
    return 1;
  }
  else {
    lua_Integer i = luaL_checkinteger(L, 1);
    if (i < 0) i = n + i;
    else if (i > n) i = n;
    luaL_argcheck(L, 1 <= i, 1, "index out of range");
    return n - (int)i;
  }
}



static int finishpcall (lua_State *L, int status, lua_KContext extra) {
  if (l_unlikely(status != LUA_OK && status != LUA_YIELD)) {  
    lua_pushboolean(L, 0);  
    lua_pushvalue(L, -2);  
    return 2;  
  }
  else
    return lua_gettop(L) - (int)extra;  
}


static int luaB_pcall (lua_State *L) {
  int status;
  luaL_checkany(L, 1);
  lua_pushboolean(L, 1);  
  lua_insert(L, 1);  
  status = lua_pcallk(L, lua_gettop(L) - 2, LUA_MULTRET, 0, 0, finishpcall);
  return finishpcall(L, status, 0);
}



static int luaB_xpcall (lua_State *L) {
  int status;
  int n = lua_gettop(L);
  luaL_checktype(L, 2, LUA_TFUNCTION);  
  lua_pushboolean(L, 1);  
  lua_pushvalue(L, 1);  
  lua_rotate(L, 3, 2);  
  status = lua_pcallk(L, n - 2, LUA_MULTRET, 2, 2, finishpcall);
  return finishpcall(L, status, 2);
}


static int luaB_tostring (lua_State *L) {
  luaL_checkany(L, 1);
  luaL_tolstring(L, 1, NULL);
  return 1;
}


static const luaL_Reg base_funcs[] = {
  {"assert", luaB_assert},
  {"collectgarbage", luaB_collectgarbage},
  {"dofile", luaB_dofile},
  {"error", luaB_error},
  {"getmetatable", luaB_getmetatable},
  {"ipairs", luaB_ipairs},
  {"loadfile", luaB_loadfile},
  {"load", luaB_load},
  {"next", luaB_next},
  {"pairs", luaB_pairs},
  {"pcall", luaB_pcall},
  {"print", luaB_print},
  {"warn", luaB_warn},
  {"rawequal", luaB_rawequal},
  {"rawlen", luaB_rawlen},
  {"rawget", luaB_rawget},
  {"rawset", luaB_rawset},
  {"select", luaB_select},
  {"setmetatable", luaB_setmetatable},
  {"tonumber", luaB_tonumber},
  {"tostring", luaB_tostring},
  {"type", luaB_type},
  {"xpcall", luaB_xpcall},
  
  {LUA_GNAME, NULL},
  {"_VERSION", NULL},
  {NULL, NULL}
};


LUAMOD_API int luaopen_base (lua_State *L) {
  
  lua_pushglobaltable(L);
  luaL_setfuncs(L, base_funcs, 0);
  
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, LUA_GNAME);
  
  lua_pushliteral(L, LUA_VERSION);
  lua_setfield(L, -2, "_VERSION");
  return 1;
}



//path: src/imports/../dependencies/lua/lcorolib.c


#define lcorolib_c
#define LUA_LIB





#include <stdlib.h>










static lua_State *getco (lua_State *L) {
  lua_State *co = lua_tothread(L, 1);
  luaL_argexpected(L, co, 1, "thread");
  return co;
}



static int auxresume (lua_State *L, lua_State *co, int narg) {
  int status, nres;
  if (l_unlikely(!lua_checkstack(co, narg))) {
    lua_pushliteral(L, "too many arguments to resume");
    return -1;  
  }
  lua_xmove(L, co, narg);
  status = lua_resume(co, L, narg, &nres);
  if (l_likely(status == LUA_OK || status == LUA_YIELD)) {
    if (l_unlikely(!lua_checkstack(L, nres + 1))) {
      lua_pop(co, nres);  
      lua_pushliteral(L, "too many results to resume");
      return -1;  
    }
    lua_xmove(co, L, nres);  
    return nres;
  }
  else {
    lua_xmove(co, L, 1);  
    return -1;  
  }
}


static int luaB_coresume (lua_State *L) {
  lua_State *co = getco(L);
  int r;
  r = auxresume(L, co, lua_gettop(L) - 1);
  if (l_unlikely(r < 0)) {
    lua_pushboolean(L, 0);
    lua_insert(L, -2);
    return 2;  
  }
  else {
    lua_pushboolean(L, 1);
    lua_insert(L, -(r + 1));
    return r + 1;  
  }
}


static int luaB_auxwrap (lua_State *L) {
  lua_State *co = lua_tothread(L, lua_upvalueindex(1));
  int r = auxresume(L, co, lua_gettop(L));
  if (l_unlikely(r < 0)) {  
    int stat = lua_status(co);
    if (stat != LUA_OK && stat != LUA_YIELD) {  
      stat = lua_closethread(co, L);  
      lua_assert(stat != LUA_OK);
      lua_xmove(co, L, 1);  
    }
    if (stat != LUA_ERRMEM &&  
        lua_type(L, -1) == LUA_TSTRING) {  
      luaL_where(L, 1);  
      lua_insert(L, -2);
      lua_concat(L, 2);
    }
    return lua_error(L);  
  }
  return r;
}


static int luaB_cocreate (lua_State *L) {
  lua_State *NL;
  luaL_checktype(L, 1, LUA_TFUNCTION);
  NL = lua_newthread(L);
  lua_pushvalue(L, 1);  
  lua_xmove(L, NL, 1);  
  return 1;
}


static int luaB_cowrap (lua_State *L) {
  luaB_cocreate(L);
  lua_pushcclosure(L, luaB_auxwrap, 1);
  return 1;
}


static int luaB_yield (lua_State *L) {
  return lua_yield(L, lua_gettop(L));
}


#define COS_RUN		0
#define COS_DEAD	1
#define COS_YIELD	2
#define COS_NORM	3


static const char *const statname[] =
  {"running", "dead", "suspended", "normal"};


static int auxstatus (lua_State *L, lua_State *co) {
  if (L == co) return COS_RUN;
  else {
    switch (lua_status(co)) {
      case LUA_YIELD:
        return COS_YIELD;
      case LUA_OK: {
        lua_Debug ar;
        if (lua_getstack(co, 0, &ar))  
          return COS_NORM;  
        else if (lua_gettop(co) == 0)
            return COS_DEAD;
        else
          return COS_YIELD;  
      }
      default:  
        return COS_DEAD;
    }
  }
}


static int luaB_costatus (lua_State *L) {
  lua_State *co = getco(L);
  lua_pushstring(L, statname[auxstatus(L, co)]);
  return 1;
}


static int luaB_yieldable (lua_State *L) {
  lua_State *co = lua_isnone(L, 1) ? L : getco(L);
  lua_pushboolean(L, lua_isyieldable(co));
  return 1;
}


static int luaB_corunning (lua_State *L) {
  int ismain = lua_pushthread(L);
  lua_pushboolean(L, ismain);
  return 2;
}


static int luaB_close (lua_State *L) {
  lua_State *co = getco(L);
  int status = auxstatus(L, co);
  switch (status) {
    case COS_DEAD: case COS_YIELD: {
      status = lua_closethread(co, L);
      if (status == LUA_OK) {
        lua_pushboolean(L, 1);
        return 1;
      }
      else {
        lua_pushboolean(L, 0);
        lua_xmove(co, L, 1);  
        return 2;
      }
    }
    default:  
      return luaL_error(L, "cannot close a %s coroutine", statname[status]);
  }
}


static const luaL_Reg co_funcs[] = {
  {"create", luaB_cocreate},
  {"resume", luaB_coresume},
  {"running", luaB_corunning},
  {"status", luaB_costatus},
  {"wrap", luaB_cowrap},
  {"yield", luaB_yield},
  {"isyieldable", luaB_yieldable},
  {"close", luaB_close},
  {NULL, NULL}
};



LUAMOD_API int luaopen_coroutine (lua_State *L) {
  luaL_newlib(L, co_funcs);
  return 1;
}



//path: src/imports/../dependencies/lua/ldblib.c


#define ldblib_c
#define LUA_LIB





#include <stdio.h>
#include <stdlib.h>
#include <string.h>











static const char *const HOOKKEY = "_HOOKKEY";



static void checkstack (lua_State *L, lua_State *L1, int n) {
  if (l_unlikely(L != L1 && !lua_checkstack(L1, n)))
    luaL_error(L, "stack overflow");
}


static int db_getregistry (lua_State *L) {
  lua_pushvalue(L, LUA_REGISTRYINDEX);
  return 1;
}


static int db_getmetatable (lua_State *L) {
  luaL_checkany(L, 1);
  if (!lua_getmetatable(L, 1)) {
    lua_pushnil(L);  
  }
  return 1;
}


static int db_setmetatable (lua_State *L) {
  int t = lua_type(L, 2);
  luaL_argexpected(L, t == LUA_TNIL || t == LUA_TTABLE, 2, "nil or table");
  lua_settop(L, 2);
  lua_setmetatable(L, 1);
  return 1;  
}


static int db_getuservalue (lua_State *L) {
  int n = (int)luaL_optinteger(L, 2, 1);
  if (lua_type(L, 1) != LUA_TUSERDATA)
    luaL_pushfail(L);
  else if (lua_getiuservalue(L, 1, n) != LUA_TNONE) {
    lua_pushboolean(L, 1);
    return 2;
  }
  return 1;
}


static int db_setuservalue (lua_State *L) {
  int n = (int)luaL_optinteger(L, 3, 1);
  luaL_checktype(L, 1, LUA_TUSERDATA);
  luaL_checkany(L, 2);
  lua_settop(L, 2);
  if (!lua_setiuservalue(L, 1, n))
    luaL_pushfail(L);
  return 1;
}



static lua_State *getthread (lua_State *L, int *arg) {
  if (lua_isthread(L, 1)) {
    *arg = 1;
    return lua_tothread(L, 1);
  }
  else {
    *arg = 0;
    return L;  
  }
}



static void settabss (lua_State *L, const char *k, const char *v) {
  lua_pushstring(L, v);
  lua_setfield(L, -2, k);
}

static void settabsi (lua_State *L, const char *k, int v) {
  lua_pushinteger(L, v);
  lua_setfield(L, -2, k);
}

static void settabsb (lua_State *L, const char *k, int v) {
  lua_pushboolean(L, v);
  lua_setfield(L, -2, k);
}



static void treatstackoption (lua_State *L, lua_State *L1, const char *fname) {
  if (L == L1)
    lua_rotate(L, -2, 1);  
  else
    lua_xmove(L1, L, 1);  
  lua_setfield(L, -2, fname);  
}



static int db_getinfo (lua_State *L) {
  lua_Debug ar;
  int arg;
  lua_State *L1 = getthread(L, &arg);
  const char *options = luaL_optstring(L, arg+2, "flnSrtu");
  checkstack(L, L1, 3);
  luaL_argcheck(L, options[0] != '>', arg + 2, "invalid option '>'");
  if (lua_isfunction(L, arg + 1)) {  
    options = lua_pushfstring(L, ">%s", options);  
    lua_pushvalue(L, arg + 1);  
    lua_xmove(L, L1, 1);
  }
  else {  
    if (!lua_getstack(L1, (int)luaL_checkinteger(L, arg + 1), &ar)) {
      luaL_pushfail(L);  
      return 1;
    }
  }
  if (!lua_getinfo(L1, options, &ar))
    return luaL_argerror(L, arg+2, "invalid option");
  lua_newtable(L);  
  if (strchr(options, 'S')) {
    lua_pushlstring(L, ar.source, ar.srclen);
    lua_setfield(L, -2, "source");
    settabss(L, "short_src", ar.short_src);
    settabsi(L, "linedefined", ar.linedefined);
    settabsi(L, "lastlinedefined", ar.lastlinedefined);
    settabss(L, "what", ar.what);
  }
  if (strchr(options, 'l'))
    settabsi(L, "currentline", ar.currentline);
  if (strchr(options, 'u')) {
    settabsi(L, "nups", ar.nups);
    settabsi(L, "nparams", ar.nparams);
    settabsb(L, "isvararg", ar.isvararg);
  }
  if (strchr(options, 'n')) {
    settabss(L, "name", ar.name);
    settabss(L, "namewhat", ar.namewhat);
  }
  if (strchr(options, 'r')) {
    settabsi(L, "ftransfer", ar.ftransfer);
    settabsi(L, "ntransfer", ar.ntransfer);
  }
  if (strchr(options, 't'))
    settabsb(L, "istailcall", ar.istailcall);
  if (strchr(options, 'L'))
    treatstackoption(L, L1, "activelines");
  if (strchr(options, 'f'))
    treatstackoption(L, L1, "func");
  return 1;  
}


static int db_getlocal (lua_State *L) {
  int arg;
  lua_State *L1 = getthread(L, &arg);
  int nvar = (int)luaL_checkinteger(L, arg + 2);  
  if (lua_isfunction(L, arg + 1)) {  
    lua_pushvalue(L, arg + 1);  
    lua_pushstring(L, lua_getlocal(L, NULL, nvar));  
    return 1;  
  }
  else {  
    lua_Debug ar;
    const char *name;
    int level = (int)luaL_checkinteger(L, arg + 1);
    if (l_unlikely(!lua_getstack(L1, level, &ar)))  
      return luaL_argerror(L, arg+1, "level out of range");
    checkstack(L, L1, 1);
    name = lua_getlocal(L1, &ar, nvar);
    if (name) {
      lua_xmove(L1, L, 1);  
      lua_pushstring(L, name);  
      lua_rotate(L, -2, 1);  
      return 2;
    }
    else {
      luaL_pushfail(L);  
      return 1;
    }
  }
}


static int db_setlocal (lua_State *L) {
  int arg;
  const char *name;
  lua_State *L1 = getthread(L, &arg);
  lua_Debug ar;
  int level = (int)luaL_checkinteger(L, arg + 1);
  int nvar = (int)luaL_checkinteger(L, arg + 2);
  if (l_unlikely(!lua_getstack(L1, level, &ar)))  
    return luaL_argerror(L, arg+1, "level out of range");
  luaL_checkany(L, arg+3);
  lua_settop(L, arg+3);
  checkstack(L, L1, 1);
  lua_xmove(L, L1, 1);
  name = lua_setlocal(L1, &ar, nvar);
  if (name == NULL)
    lua_pop(L1, 1);  
  lua_pushstring(L, name);
  return 1;
}



static int auxupvalue (lua_State *L, int get) {
  const char *name;
  int n = (int)luaL_checkinteger(L, 2);  
  luaL_checktype(L, 1, LUA_TFUNCTION);  
  name = get ? lua_getupvalue(L, 1, n) : lua_setupvalue(L, 1, n);
  if (name == NULL) return 0;
  lua_pushstring(L, name);
  lua_insert(L, -(get+1));  
  return get + 1;
}


static int db_getupvalue (lua_State *L) {
  return auxupvalue(L, 1);
}


static int db_setupvalue (lua_State *L) {
  luaL_checkany(L, 3);
  return auxupvalue(L, 0);
}



static void *checkupval (lua_State *L, int argf, int argnup, int *pnup) {
  void *id;
  int nup = (int)luaL_checkinteger(L, argnup);  
  luaL_checktype(L, argf, LUA_TFUNCTION);  
  id = lua_upvalueid(L, argf, nup);
  if (pnup) {
    luaL_argcheck(L, id != NULL, argnup, "invalid upvalue index");
    *pnup = nup;
  }
  return id;
}


static int db_upvalueid (lua_State *L) {
  void *id = checkupval(L, 1, 2, NULL);
  if (id != NULL)
    lua_pushlightuserdata(L, id);
  else
    luaL_pushfail(L);
  return 1;
}


static int db_upvaluejoin (lua_State *L) {
  int n1, n2;
  checkupval(L, 1, 2, &n1);
  checkupval(L, 3, 4, &n2);
  luaL_argcheck(L, !lua_iscfunction(L, 1), 1, "Lua function expected");
  luaL_argcheck(L, !lua_iscfunction(L, 3), 3, "Lua function expected");
  lua_upvaluejoin(L, 1, n1, 3, n2);
  return 0;
}



static void hookf (lua_State *L, lua_Debug *ar) {
  static const char *const hooknames[] =
    {"call", "return", "line", "count", "tail call"};
  lua_getfield(L, LUA_REGISTRYINDEX, HOOKKEY);
  lua_pushthread(L);
  if (lua_rawget(L, -2) == LUA_TFUNCTION) {  
    lua_pushstring(L, hooknames[(int)ar->event]);  
    if (ar->currentline >= 0)
      lua_pushinteger(L, ar->currentline);  
    else lua_pushnil(L);
    lua_assert(lua_getinfo(L, "lS", ar));
    lua_call(L, 2, 0);  
  }
}



static int makemask (const char *smask, int count) {
  int mask = 0;
  if (strchr(smask, 'c')) mask |= LUA_MASKCALL;
  if (strchr(smask, 'r')) mask |= LUA_MASKRET;
  if (strchr(smask, 'l')) mask |= LUA_MASKLINE;
  if (count > 0) mask |= LUA_MASKCOUNT;
  return mask;
}



static char *unmakemask (int mask, char *smask) {
  int i = 0;
  if (mask & LUA_MASKCALL) smask[i++] = 'c';
  if (mask & LUA_MASKRET) smask[i++] = 'r';
  if (mask & LUA_MASKLINE) smask[i++] = 'l';
  smask[i] = '\0';
  return smask;
}


static int db_sethook (lua_State *L) {
  int arg, mask, count;
  lua_Hook func;
  lua_State *L1 = getthread(L, &arg);
  if (lua_isnoneornil(L, arg+1)) {  
    lua_settop(L, arg+1);
    func = NULL; mask = 0; count = 0;  
  }
  else {
    const char *smask = luaL_checkstring(L, arg+2);
    luaL_checktype(L, arg+1, LUA_TFUNCTION);
    count = (int)luaL_optinteger(L, arg + 3, 0);
    func = hookf; mask = makemask(smask, count);
  }
  if (!luaL_getsubtable(L, LUA_REGISTRYINDEX, HOOKKEY)) {
    
    lua_pushliteral(L, "k");
    lua_setfield(L, -2, "__mode");  
    lua_pushvalue(L, -1);
    lua_setmetatable(L, -2);  
  }
  checkstack(L, L1, 1);
  lua_pushthread(L1); lua_xmove(L1, L, 1);  
  lua_pushvalue(L, arg + 1);  
  lua_rawset(L, -3);  
  lua_sethook(L1, func, mask, count);
  return 0;
}


static int db_gethook (lua_State *L) {
  int arg;
  lua_State *L1 = getthread(L, &arg);
  char buff[5];
  int mask = lua_gethookmask(L1);
  lua_Hook hook = lua_gethook(L1);
  if (hook == NULL) {  
    luaL_pushfail(L);
    return 1;
  }
  else if (hook != hookf)  
    lua_pushliteral(L, "external hook");
  else {  
    lua_getfield(L, LUA_REGISTRYINDEX, HOOKKEY);
    checkstack(L, L1, 1);
    lua_pushthread(L1); lua_xmove(L1, L, 1);
    lua_rawget(L, -2);   
    lua_remove(L, -2);  
  }
  lua_pushstring(L, unmakemask(mask, buff));  
  lua_pushinteger(L, lua_gethookcount(L1));  
  return 3;
}


static int db_debug (lua_State *L) {
  for (;;) {
    char buffer[250];
    lua_writestringerror("%s", "lua_debug> ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL ||
        strcmp(buffer, "cont\n") == 0)
      return 0;
    if (luaL_loadbuffer(L, buffer, strlen(buffer), "=(debug command)") ||
        lua_pcall(L, 0, 0, 0))
      lua_writestringerror("%s\n", luaL_tolstring(L, -1, NULL));
    lua_settop(L, 0);  
  }
}


static int db_traceback (lua_State *L) {
  int arg;
  lua_State *L1 = getthread(L, &arg);
  const char *msg = lua_tostring(L, arg + 1);
  if (msg == NULL && !lua_isnoneornil(L, arg + 1))  
    lua_pushvalue(L, arg + 1);  
  else {
    int level = (int)luaL_optinteger(L, arg + 2, (L == L1) ? 1 : 0);
    luaL_traceback(L, L1, msg, level);
  }
  return 1;
}


static const luaL_Reg dblib[] = {
  {"debug", db_debug},
  {"getuservalue", db_getuservalue},
  {"gethook", db_gethook},
  {"getinfo", db_getinfo},
  {"getlocal", db_getlocal},
  {"getregistry", db_getregistry},
  {"getmetatable", db_getmetatable},
  {"getupvalue", db_getupvalue},
  {"upvaluejoin", db_upvaluejoin},
  {"upvalueid", db_upvalueid},
  {"setuservalue", db_setuservalue},
  {"sethook", db_sethook},
  {"setlocal", db_setlocal},
  {"setmetatable", db_setmetatable},
  {"setupvalue", db_setupvalue},
  {"traceback", db_traceback},
  {NULL, NULL}
};


LUAMOD_API int luaopen_debug (lua_State *L) {
  luaL_newlib(L, dblib);
  return 1;
}



//path: src/imports/../dependencies/lua/liolib.c


#define liolib_c
#define LUA_LIB





#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>













#if !defined(l_checkmode)


#if !defined(L_MODEEXT)
#define L_MODEEXT	"b"
#endif


static int l_checkmode (const char *mode) {
  return (*mode != '\0' && strchr("rwa", *(mode++)) != NULL &&
         (*mode != '+' || ((void)(++mode), 1)) &&  
         (strspn(mode, L_MODEEXT) == strlen(mode)));  
}

#endif



#if !defined(l_popen)		

#if defined(LUA_USE_POSIX)	

#define l_popen(L,c,m)		(fflush(NULL), popen(c,m))
#define l_pclose(L,file)	(pclose(file))

#elif defined(LUA_USE_WINDOWS)	

#define l_popen(L,c,m)		(_popen(c,m))
#define l_pclose(L,file)	(_pclose(file))

#if !defined(l_checkmodep)

#define l_checkmodep(m)	((m[0] == 'r' || m[0] == 'w') && \
  (m[1] == '\0' || ((m[1] == 'b' || m[1] == 't') && m[2] == '\0')))
#endif

#else				


#define l_popen(L,c,m)  \
	  ((void)c, (void)m, \
	  luaL_error(L, "'popen' not supported"), \
	  (FILE*)0)
#define l_pclose(L,file)		((void)L, (void)file, -1)

#endif				

#endif				


#if !defined(l_checkmodep)

#define l_checkmodep(m)        ((m[0] == 'r' || m[0] == 'w') && m[1] == '\0')
#endif




#if !defined(l_getc)		

#if defined(LUA_USE_POSIX)
#define l_getc(f)		getc_unlocked(f)
#define l_lockfile(f)		flockfile(f)
#define l_unlockfile(f)		funlockfile(f)
#else
#define l_getc(f)		getc(f)
#define l_lockfile(f)		((void)0)
#define l_unlockfile(f)		((void)0)
#endif

#endif				




#if !defined(l_fseek)		

#if defined(LUA_USE_POSIX)	

#include <sys/types.h>

#define l_fseek(f,o,w)		fseeko(f,o,w)
#define l_ftell(f)		ftello(f)
#define l_seeknum		off_t

#elif defined(LUA_USE_WINDOWS) && !defined(_CRTIMP_TYPEINFO) \
   && defined(_MSC_VER) && (_MSC_VER >= 1400)	


#define l_fseek(f,o,w)		_fseeki64(f,o,w)
#define l_ftell(f)		_ftelli64(f)
#define l_seeknum		__int64

#else				


#define l_fseek(f,o,w)		fseek(f,o,w)
#define l_ftell(f)		ftell(f)
#define l_seeknum		long

#endif				

#endif				





#define IO_PREFIX	"_IO_"
#define IOPREF_LEN	(sizeof(IO_PREFIX)/sizeof(char) - 1)
#define IO_INPUT	(IO_PREFIX "input")
#define IO_OUTPUT	(IO_PREFIX "output")


typedef luaL_Stream LStream;


#define tolstream(L)	((LStream *)luaL_checkudata(L, 1, LUA_FILEHANDLE))

#define isclosed(p)	((p)->closef == NULL)


static int io_type (lua_State *L) {
  LStream *p;
  luaL_checkany(L, 1);
  p = (LStream *)luaL_testudata(L, 1, LUA_FILEHANDLE);
  if (p == NULL)
    luaL_pushfail(L);  
  else if (isclosed(p))
    lua_pushliteral(L, "closed file");
  else
    lua_pushliteral(L, "file");
  return 1;
}


static int f_tostring (lua_State *L) {
  LStream *p = tolstream(L);
  if (isclosed(p))
    lua_pushliteral(L, "file (closed)");
  else
    lua_pushfstring(L, "file (%p)", p->f);
  return 1;
}


static FILE *tofile (lua_State *L) {
  LStream *p = tolstream(L);
  if (l_unlikely(isclosed(p)))
    luaL_error(L, "attempt to use a closed file");
  lua_assert(p->f);
  return p->f;
}



static LStream *newprefile (lua_State *L) {
  LStream *p = (LStream *)lua_newuserdatauv(L, sizeof(LStream), 0);
  p->closef = NULL;  
  luaL_setmetatable(L, LUA_FILEHANDLE);
  return p;
}



static int aux_close (lua_State *L) {
  LStream *p = tolstream(L);
  volatile lua_CFunction cf = p->closef;
  p->closef = NULL;  
  return (*cf)(L);  
}


static int f_close (lua_State *L) {
  tofile(L);  
  return aux_close(L);
}


static int io_close (lua_State *L) {
  if (lua_isnone(L, 1))  
    lua_getfield(L, LUA_REGISTRYINDEX, IO_OUTPUT);  
  return f_close(L);
}


static int f_gc (lua_State *L) {
  LStream *p = tolstream(L);
  if (!isclosed(p) && p->f != NULL)
    aux_close(L);  
  return 0;
}



static int io_fclose (lua_State *L) {
  LStream *p = tolstream(L);
  int res = fclose(p->f);
  return luaL_fileresult(L, (res == 0), NULL);
}


static LStream *newfile (lua_State *L) {
  LStream *p = newprefile(L);
  p->f = NULL;
  p->closef = &io_fclose;
  return p;
}


static void opencheck (lua_State *L, const char *fname, const char *mode) {
  LStream *p = newfile(L);
  p->f = fopen(fname, mode);
  if (l_unlikely(p->f == NULL))
    luaL_error(L, "cannot open file '%s' (%s)", fname, strerror(errno));
}


static int io_open (lua_State *L) {
  const char *filename = luaL_checkstring(L, 1);
  const char *mode = luaL_optstring(L, 2, "r");
  LStream *p = newfile(L);
  const char *md = mode;  
  luaL_argcheck(L, l_checkmode(md), 2, "invalid mode");
  p->f = fopen(filename, mode);
  return (p->f == NULL) ? luaL_fileresult(L, 0, filename) : 1;
}



static int io_pclose (lua_State *L) {
  LStream *p = tolstream(L);
  errno = 0;
  return luaL_execresult(L, l_pclose(L, p->f));
}


static int io_popen (lua_State *L) {
  const char *filename = luaL_checkstring(L, 1);
  const char *mode = luaL_optstring(L, 2, "r");
  LStream *p = newprefile(L);
  luaL_argcheck(L, l_checkmodep(mode), 2, "invalid mode");
  p->f = l_popen(L, filename, mode);
  p->closef = &io_pclose;
  return (p->f == NULL) ? luaL_fileresult(L, 0, filename) : 1;
}


static int io_tmpfile (lua_State *L) {
  LStream *p = newfile(L);
  p->f = tmpfile();
  return (p->f == NULL) ? luaL_fileresult(L, 0, NULL) : 1;
}


static FILE *getiofile (lua_State *L, const char *findex) {
  LStream *p;
  lua_getfield(L, LUA_REGISTRYINDEX, findex);
  p = (LStream *)lua_touserdata(L, -1);
  if (l_unlikely(isclosed(p)))
    luaL_error(L, "default %s file is closed", findex + IOPREF_LEN);
  return p->f;
}


static int g_iofile (lua_State *L, const char *f, const char *mode) {
  if (!lua_isnoneornil(L, 1)) {
    const char *filename = lua_tostring(L, 1);
    if (filename)
      opencheck(L, filename, mode);
    else {
      tofile(L);  
      lua_pushvalue(L, 1);
    }
    lua_setfield(L, LUA_REGISTRYINDEX, f);
  }
  
  lua_getfield(L, LUA_REGISTRYINDEX, f);
  return 1;
}


static int io_input (lua_State *L) {
  return g_iofile(L, IO_INPUT, "r");
}


static int io_output (lua_State *L) {
  return g_iofile(L, IO_OUTPUT, "w");
}


static int io_readline (lua_State *L);



#define MAXARGLINE	250


static void aux_lines (lua_State *L, int toclose) {
  int n = lua_gettop(L) - 1;  
  luaL_argcheck(L, n <= MAXARGLINE, MAXARGLINE + 2, "too many arguments");
  lua_pushvalue(L, 1);  
  lua_pushinteger(L, n);  
  lua_pushboolean(L, toclose);  
  lua_rotate(L, 2, 3);  
  lua_pushcclosure(L, io_readline, 3 + n);
}


static int f_lines (lua_State *L) {
  tofile(L);  
  aux_lines(L, 0);
  return 1;
}



static int io_lines (lua_State *L) {
  int toclose;
  if (lua_isnone(L, 1)) lua_pushnil(L);  
  if (lua_isnil(L, 1)) {  
    lua_getfield(L, LUA_REGISTRYINDEX, IO_INPUT);  
    lua_replace(L, 1);  
    tofile(L);  
    toclose = 0;  
  }
  else {  
    const char *filename = luaL_checkstring(L, 1);
    opencheck(L, filename, "r");
    lua_replace(L, 1);  
    toclose = 1;  
  }
  aux_lines(L, toclose);  
  if (toclose) {
    lua_pushnil(L);  
    lua_pushnil(L);  
    lua_pushvalue(L, 1);  
    return 4;
  }
  else
    return 1;
}






#if !defined (L_MAXLENNUM)
#define L_MAXLENNUM     200
#endif



typedef struct {
  FILE *f;  
  int c;  
  int n;  
  char buff[L_MAXLENNUM + 1];  
} RN;



static int nextc (RN *rn) {
  if (l_unlikely(rn->n >= L_MAXLENNUM)) {  
    rn->buff[0] = '\0';  
    return 0;  
  }
  else {
    rn->buff[rn->n++] = rn->c;  
    rn->c = l_getc(rn->f);  
    return 1;
  }
}



static int test2 (RN *rn, const char *set) {
  if (rn->c == set[0] || rn->c == set[1])
    return nextc(rn);
  else return 0;
}



static int readdigits (RN *rn, int hex) {
  int count = 0;
  while ((hex ? isxdigit(rn->c) : isdigit(rn->c)) && nextc(rn))
    count++;
  return count;
}



static int read_number (lua_State *L, FILE *f) {
  RN rn;
  int count = 0;
  int hex = 0;
  char decp[2];
  rn.f = f; rn.n = 0;
  decp[0] = lua_getlocaledecpoint();  
  decp[1] = '.';  
  l_lockfile(rn.f);
  do { rn.c = l_getc(rn.f); } while (isspace(rn.c));  
  test2(&rn, "-+");  
  if (test2(&rn, "00")) {
    if (test2(&rn, "xX")) hex = 1;  
    else count = 1;  
  }
  count += readdigits(&rn, hex);  
  if (test2(&rn, decp))  
    count += readdigits(&rn, hex);  
  if (count > 0 && test2(&rn, (hex ? "pP" : "eE"))) {  
    test2(&rn, "-+");  
    readdigits(&rn, 0);  
  }
  ungetc(rn.c, rn.f);  
  l_unlockfile(rn.f);
  rn.buff[rn.n] = '\0';  
  if (l_likely(lua_stringtonumber(L, rn.buff)))
    return 1;  
  else {  
   lua_pushnil(L);  
   return 0;  
  }
}


static int test_eof (lua_State *L, FILE *f) {
  int c = getc(f);
  ungetc(c, f);  
  lua_pushliteral(L, "");
  return (c != EOF);
}


static int read_line (lua_State *L, FILE *f, int chop) {
  luaL_Buffer b;
  int c;
  luaL_buffinit(L, &b);
  do {  
    char *buff = luaL_prepbuffer(&b);  
    int i = 0;
    l_lockfile(f);  
    while (i < LUAL_BUFFERSIZE && (c = l_getc(f)) != EOF && c != '\n')
      buff[i++] = c;  
    l_unlockfile(f);
    luaL_addsize(&b, i);
  } while (c != EOF && c != '\n');  
  if (!chop && c == '\n')  
    luaL_addchar(&b, c);  
  luaL_pushresult(&b);  
  
  return (c == '\n' || lua_rawlen(L, -1) > 0);
}


static void read_all (lua_State *L, FILE *f) {
  size_t nr;
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  do {  
    char *p = luaL_prepbuffer(&b);
    nr = fread(p, sizeof(char), LUAL_BUFFERSIZE, f);
    luaL_addsize(&b, nr);
  } while (nr == LUAL_BUFFERSIZE);
  luaL_pushresult(&b);  
}


static int read_chars (lua_State *L, FILE *f, size_t n) {
  size_t nr;  
  char *p;
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  p = luaL_prepbuffsize(&b, n);  
  nr = fread(p, sizeof(char), n, f);  
  luaL_addsize(&b, nr);
  luaL_pushresult(&b);  
  return (nr > 0);  
}


static int g_read (lua_State *L, FILE *f, int first) {
  int nargs = lua_gettop(L) - 1;
  int n, success;
  clearerr(f);
  if (nargs == 0) {  
    success = read_line(L, f, 1);
    n = first + 1;  
  }
  else {
    
    luaL_checkstack(L, nargs+LUA_MINSTACK, "too many arguments");
    success = 1;
    for (n = first; nargs-- && success; n++) {
      if (lua_type(L, n) == LUA_TNUMBER) {
        size_t l = (size_t)luaL_checkinteger(L, n);
        success = (l == 0) ? test_eof(L, f) : read_chars(L, f, l);
      }
      else {
        const char *p = luaL_checkstring(L, n);
        if (*p == '*') p++;  
        switch (*p) {
          case 'n':  
            success = read_number(L, f);
            break;
          case 'l':  
            success = read_line(L, f, 1);
            break;
          case 'L':  
            success = read_line(L, f, 0);
            break;
          case 'a':  
            read_all(L, f);  
            success = 1; 
            break;
          default:
            return luaL_argerror(L, n, "invalid format");
        }
      }
    }
  }
  if (ferror(f))
    return luaL_fileresult(L, 0, NULL);
  if (!success) {
    lua_pop(L, 1);  
    luaL_pushfail(L);  
  }
  return n - first;
}


static int io_read (lua_State *L) {
  return g_read(L, getiofile(L, IO_INPUT), 1);
}


static int f_read (lua_State *L) {
  return g_read(L, tofile(L), 2);
}



static int io_readline (lua_State *L) {
  LStream *p = (LStream *)lua_touserdata(L, lua_upvalueindex(1));
  int i;
  int n = (int)lua_tointeger(L, lua_upvalueindex(2));
  if (isclosed(p))  
    return luaL_error(L, "file is already closed");
  lua_settop(L , 1);
  luaL_checkstack(L, n, "too many arguments");
  for (i = 1; i <= n; i++)  
    lua_pushvalue(L, lua_upvalueindex(3 + i));
  n = g_read(L, p->f, 2);  
  lua_assert(n > 0);  
  if (lua_toboolean(L, -n))  
    return n;  
  else {  
    if (n > 1) {  
      
      return luaL_error(L, "%s", lua_tostring(L, -n + 1));
    }
    if (lua_toboolean(L, lua_upvalueindex(3))) {  
      lua_settop(L, 0);  
      lua_pushvalue(L, lua_upvalueindex(1));  
      aux_close(L);  
    }
    return 0;
  }
}




static int g_write (lua_State *L, FILE *f, int arg) {
  int nargs = lua_gettop(L) - arg;
  int status = 1;
  for (; nargs--; arg++) {
    if (lua_type(L, arg) == LUA_TNUMBER) {
      
      int len = lua_isinteger(L, arg)
                ? fprintf(f, LUA_INTEGER_FMT,
                             (LUAI_UACINT)lua_tointeger(L, arg))
                : fprintf(f, LUA_NUMBER_FMT,
                             (LUAI_UACNUMBER)lua_tonumber(L, arg));
      status = status && (len > 0);
    }
    else {
      size_t l;
      const char *s = luaL_checklstring(L, arg, &l);
      status = status && (fwrite(s, sizeof(char), l, f) == l);
    }
  }
  if (l_likely(status))
    return 1;  
  else return luaL_fileresult(L, status, NULL);
}


static int io_write (lua_State *L) {
  return g_write(L, getiofile(L, IO_OUTPUT), 1);
}


static int f_write (lua_State *L) {
  FILE *f = tofile(L);
  lua_pushvalue(L, 1);  
  return g_write(L, f, 2);
}


static int f_seek (lua_State *L) {
  static const int mode[] = {SEEK_SET, SEEK_CUR, SEEK_END};
  static const char *const modenames[] = {"set", "cur", "end", NULL};
  FILE *f = tofile(L);
  int op = luaL_checkoption(L, 2, "cur", modenames);
  lua_Integer p3 = luaL_optinteger(L, 3, 0);
  l_seeknum offset = (l_seeknum)p3;
  luaL_argcheck(L, (lua_Integer)offset == p3, 3,
                  "not an integer in proper range");
  op = l_fseek(f, offset, mode[op]);
  if (l_unlikely(op))
    return luaL_fileresult(L, 0, NULL);  
  else {
    lua_pushinteger(L, (lua_Integer)l_ftell(f));
    return 1;
  }
}


static int f_setvbuf (lua_State *L) {
  static const int mode[] = {_IONBF, _IOFBF, _IOLBF};
  static const char *const modenames[] = {"no", "full", "line", NULL};
  FILE *f = tofile(L);
  int op = luaL_checkoption(L, 2, NULL, modenames);
  lua_Integer sz = luaL_optinteger(L, 3, LUAL_BUFFERSIZE);
  int res = setvbuf(f, NULL, mode[op], (size_t)sz);
  return luaL_fileresult(L, res == 0, NULL);
}



static int io_flush (lua_State *L) {
  return luaL_fileresult(L, fflush(getiofile(L, IO_OUTPUT)) == 0, NULL);
}


static int f_flush (lua_State *L) {
  return luaL_fileresult(L, fflush(tofile(L)) == 0, NULL);
}



static const luaL_Reg iolib[] = {
  {"close", io_close},
  {"flush", io_flush},
  {"input", io_input},
  {"lines", io_lines},
  {"open", io_open},
  {"output", io_output},
  {"popen", io_popen},
  {"read", io_read},
  {"tmpfile", io_tmpfile},
  {"type", io_type},
  {"write", io_write},
  {NULL, NULL}
};



static const luaL_Reg meth[] = {
  {"read", f_read},
  {"write", f_write},
  {"lines", f_lines},
  {"flush", f_flush},
  {"seek", f_seek},
  {"close", f_close},
  {"setvbuf", f_setvbuf},
  {NULL, NULL}
};



static const luaL_Reg metameth[] = {
  {"__index", NULL},  
  {"__gc", f_gc},
  {"__close", f_gc},
  {"__tostring", f_tostring},
  {NULL, NULL}
};


static void createmeta (lua_State *L) {
  luaL_newmetatable(L, LUA_FILEHANDLE);  
  luaL_setfuncs(L, metameth, 0);  
  luaL_newlibtable(L, meth);  
  luaL_setfuncs(L, meth, 0);  
  lua_setfield(L, -2, "__index");  
  lua_pop(L, 1);  
}



static int io_noclose (lua_State *L) {
  LStream *p = tolstream(L);
  p->closef = &io_noclose;  
  luaL_pushfail(L);
  lua_pushliteral(L, "cannot close standard file");
  return 2;
}


static void createstdfile (lua_State *L, FILE *f, const char *k,
                           const char *fname) {
  LStream *p = newprefile(L);
  p->f = f;
  p->closef = &io_noclose;
  if (k != NULL) {
    lua_pushvalue(L, -1);
    lua_setfield(L, LUA_REGISTRYINDEX, k);  
  }
  lua_setfield(L, -2, fname);  
}


LUAMOD_API int luaopen_io (lua_State *L) {
  luaL_newlib(L, iolib);  
  createmeta(L);
  
  createstdfile(L, stdin, IO_INPUT, "stdin");
  createstdfile(L, stdout, IO_OUTPUT, "stdout");
  createstdfile(L, stderr, NULL, "stderr");
  return 1;
}



//path: src/imports/../dependencies/lua/lmathlib.c

#define lmathlib_c
#define LUA_LIB




#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>










#undef PI
#define PI	(l_mathop(3.141592653589793238462643383279502884))


static int math_abs (lua_State *L) {
  if (lua_isinteger(L, 1)) {
    lua_Integer n = lua_tointeger(L, 1);
    if (n < 0) n = (lua_Integer)(0u - (lua_Unsigned)n);
    lua_pushinteger(L, n);
  }
  else
    lua_pushnumber(L, l_mathop(private_lua_cembed_fabs)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_sin (lua_State *L) {
  lua_pushnumber(L, l_mathop(private_lua_cembed_sin)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_cos (lua_State *L) {
  lua_pushnumber(L, l_mathop(private_lua_cembed_cos)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_tan (lua_State *L) {
  lua_pushnumber(L, l_mathop(private_lua_cembed_tan)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_asin (lua_State *L) {
  lua_pushnumber(L, l_mathop(private_lua_cembed_asin)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_acos (lua_State *L) {
  lua_pushnumber(L, l_mathop(private_lua_cembed_acos)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_atan (lua_State *L) {
  lua_Number y = luaL_checknumber(L, 1);
  lua_Number x = luaL_optnumber(L, 2, 1);
  lua_pushnumber(L, l_mathop(private_lua_cembed_atan2)(y, x));
  return 1;
}


static int math_toint (lua_State *L) {
  int valid;
  lua_Integer n = lua_tointegerx(L, 1, &valid);
  if (l_likely(valid))
    lua_pushinteger(L, n);
  else {
    luaL_checkany(L, 1);
    luaL_pushfail(L);  
  }
  return 1;
}


static void pushnumint (lua_State *L, lua_Number d) {
  lua_Integer n;
  if (lua_numbertointeger(d, &n))  
    lua_pushinteger(L, n);  
  else
    lua_pushnumber(L, d);  
}


static int math_floor (lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1);  
  else {
    lua_Number d = l_mathop(private_lua_embed_floor)(luaL_checknumber(L, 1));
    pushnumint(L, d);
  }
  return 1;
}


static int math_ceil (lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1);  
  else {
    lua_Number d = l_mathop(private_lua_cembed_ceil)(luaL_checknumber(L, 1));
    pushnumint(L, d);
  }
  return 1;
}


static int math_fmod (lua_State *L) {
  if (lua_isinteger(L, 1) && lua_isinteger(L, 2)) {
    lua_Integer d = lua_tointeger(L, 2);
    if ((lua_Unsigned)d + 1u <= 1u) {  
      luaL_argcheck(L, d != 0, 2, "zero");
      lua_pushinteger(L, 0);  
    }
    else
      lua_pushinteger(L, lua_tointeger(L, 1) % d);
  }
  else
    lua_pushnumber(L, l_mathop(private_lua_embed_fmod)(luaL_checknumber(L, 1),
                                     luaL_checknumber(L, 2)));
  return 1;
}



static int math_modf (lua_State *L) {
  if (lua_isinteger(L ,1)) {
    lua_settop(L, 1);  
    lua_pushnumber(L, 0);  
  }
  else {
    lua_Number n = luaL_checknumber(L, 1);
    
    lua_Number ip = (n < 0) ? l_mathop(private_lua_cembed_ceil)(n) : l_mathop(private_lua_embed_floor)(n);
    pushnumint(L, ip);
    
    lua_pushnumber(L, (n == ip) ? l_mathop(0.0) : (n - ip));
  }
  return 2;
}


static int math_sqrt (lua_State *L) {
  lua_pushnumber(L, l_mathop(private_lua_cembed_sqrt)(luaL_checknumber(L, 1)));
  return 1;
}


static int math_ult (lua_State *L) {
  lua_Integer a = luaL_checkinteger(L, 1);
  lua_Integer b = luaL_checkinteger(L, 2);
  lua_pushboolean(L, (lua_Unsigned)a < (lua_Unsigned)b);
  return 1;
}

static int math_log (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number res;
  if (lua_isnoneornil(L, 2))
    res = l_mathop(private_lua_cembed_log)(x);
  else {
    lua_Number base = luaL_checknumber(L, 2);
#if !defined(LUA_USE_C89)
    if (base == l_mathop(2.0))
      res = l_mathop(private_lua_cembed_log2)(x);
    else
#endif
    if (base == l_mathop(10.0))
      res = l_mathop(private_lua_embed_log10)(x);
    else
      res = l_mathop(private_lua_cembed_log)(x)/l_mathop(private_lua_cembed_log)(base);
  }
  lua_pushnumber(L, res);
  return 1;
}

static int math_exp (lua_State *L) {
  lua_pushnumber(L, l_mathop(private_lua_cembed_exp)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_deg (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (l_mathop(180.0) / PI));
  return 1;
}

static int math_rad (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (PI / l_mathop(180.0)));
  return 1;
}


static int math_min (lua_State *L) {
  int n = lua_gettop(L);  
  int imin = 1;  
  int i;
  luaL_argcheck(L, n >= 1, 1, "value expected");
  for (i = 2; i <= n; i++) {
    if (lua_compare(L, i, imin, LUA_OPLT))
      imin = i;
  }
  lua_pushvalue(L, imin);
  return 1;
}


static int math_max (lua_State *L) {
  int n = lua_gettop(L);  
  int imax = 1;  
  int i;
  luaL_argcheck(L, n >= 1, 1, "value expected");
  for (i = 2; i <= n; i++) {
    if (lua_compare(L, imax, i, LUA_OPLT))
      imax = i;
  }
  lua_pushvalue(L, imax);
  return 1;
}


static int math_type (lua_State *L) {
  if (lua_type(L, 1) == LUA_TNUMBER)
    lua_pushstring(L, (lua_isinteger(L, 1)) ? "integer" : "float");
  else {
    luaL_checkany(L, 1);
    luaL_pushfail(L);
  }
  return 1;
}









#define FIGS	l_floatatt(MANT_DIG)

#if FIGS > 64

#undef FIGS
#define FIGS	64
#endif



#if !defined(LUA_RAND32) && !defined(Rand64)



#if ((ULONG_MAX >> 31) >> 31) >= 3


#define Rand64		unsigned long
#define SRand64		long

#elif !defined(LUA_USE_C89) && defined(LLONG_MAX)


#define Rand64		unsigned long long
#define SRand64		long long

#elif ((LUA_MAXUNSIGNED >> 31) >> 31) >= 3


#define Rand64		lua_Unsigned
#define SRand64		lua_Integer

#endif

#endif


#if defined(Rand64)  




#define trim64(x)	((x) & 0xffffffffffffffffu)



static Rand64 rotl (Rand64 x, int n) {
  return (x << n) | (trim64(x) >> (64 - n));
}

static Rand64 nextrand (Rand64 *state) {
  Rand64 state0 = state[0];
  Rand64 state1 = state[1];
  Rand64 state2 = state[2] ^ state0;
  Rand64 state3 = state[3] ^ state1;
  Rand64 res = rotl(state1 * 5, 7) * 9;
  state[0] = state0 ^ state3;
  state[1] = state1 ^ state2;
  state[2] = state2 ^ (state1 << 17);
  state[3] = rotl(state3, 45);
  return res;
}





#define shift64_FIG	(64 - FIGS)


#define scaleFIG	(l_mathop(0.5) / ((Rand64)1 << (FIGS - 1)))

static lua_Number I2d (Rand64 x) {
  SRand64 sx = (SRand64)(trim64(x) >> shift64_FIG);
  lua_Number res = (lua_Number)(sx) * scaleFIG;
  if (sx < 0)
    res += l_mathop(1.0);  
  lua_assert(0 <= res && res < 1);
  return res;
}


#define I2UInt(x)	((lua_Unsigned)trim64(x))


#define Int2I(x)	((Rand64)(x))


#else	


#if LUAI_IS32INT
typedef unsigned int lu_int32;
#else
typedef unsigned long lu_int32;
#endif



typedef struct Rand64 {
  lu_int32 h;  
  lu_int32 l;  
} Rand64;





#define trim32(x)	((x) & 0xffffffffu)





static Rand64 packI (lu_int32 h, lu_int32 l) {
  Rand64 result;
  result.h = h;
  result.l = l;
  return result;
}


static Rand64 Ishl (Rand64 i, int n) {
  lua_assert(n > 0 && n < 32);
  return packI((i.h << n) | (trim32(i.l) >> (32 - n)), i.l << n);
}


static void Ixor (Rand64 *i1, Rand64 i2) {
  i1->h ^= i2.h;
  i1->l ^= i2.l;
}


static Rand64 Iadd (Rand64 i1, Rand64 i2) {
  Rand64 result = packI(i1.h + i2.h, i1.l + i2.l);
  if (trim32(result.l) < trim32(i1.l))  
    result.h++;
  return result;
}


static Rand64 times5 (Rand64 i) {
  return Iadd(Ishl(i, 2), i);  
}


static Rand64 times9 (Rand64 i) {
  return Iadd(Ishl(i, 3), i);  
}


static Rand64 rotl (Rand64 i, int n) {
  lua_assert(n > 0 && n < 32);
  return packI((i.h << n) | (trim32(i.l) >> (32 - n)),
               (trim32(i.h) >> (32 - n)) | (i.l << n));
}


static Rand64 rotl1 (Rand64 i, int n) {
  lua_assert(n > 32 && n < 64);
  n = 64 - n;
  return packI((trim32(i.h) >> n) | (i.l << (32 - n)),
               (i.h << (32 - n)) | (trim32(i.l) >> n));
}


static Rand64 nextrand (Rand64 *state) {
  Rand64 res = times9(rotl(times5(state[1]), 7));
  Rand64 t = Ishl(state[1], 17);
  Ixor(&state[2], state[0]);
  Ixor(&state[3], state[1]);
  Ixor(&state[1], state[2]);
  Ixor(&state[0], state[3]);
  Ixor(&state[2], t);
  state[3] = rotl1(state[3], 45);
  return res;
}





#define UONE		((lu_int32)1)


#if FIGS <= 32


#define scaleFIG       (l_mathop(0.5) / (UONE << (FIGS - 1)))


static lua_Number I2d (Rand64 x) {
  lua_Number h = (lua_Number)(trim32(x.h) >> (32 - FIGS));
  return h * scaleFIG;
}

#else	


#define scaleFIG  \
    (l_mathop(1.0) / (UONE << 30) / l_mathop(8.0) / (UONE << (FIGS - 33)))


#define shiftLOW	(64 - FIGS)


#define shiftHI		((lua_Number)(UONE << (FIGS - 33)) * l_mathop(2.0))


static lua_Number I2d (Rand64 x) {
  lua_Number h = (lua_Number)trim32(x.h) * shiftHI;
  lua_Number l = (lua_Number)(trim32(x.l) >> shiftLOW);
  return (h + l) * scaleFIG;
}

#endif



static lua_Unsigned I2UInt (Rand64 x) {
  return (((lua_Unsigned)trim32(x.h) << 31) << 1) | (lua_Unsigned)trim32(x.l);
}


static Rand64 Int2I (lua_Unsigned n) {
  return packI((lu_int32)((n >> 31) >> 1), (lu_int32)n);
}

#endif  



typedef struct {
  Rand64 s[4];
} RanState;



static lua_Unsigned project (lua_Unsigned ran, lua_Unsigned n,
                             RanState *state) {
  if ((n & (n + 1)) == 0)  
    return ran & n;  
  else {
    lua_Unsigned lim = n;
    
    lim |= (lim >> 1);
    lim |= (lim >> 2);
    lim |= (lim >> 4);
    lim |= (lim >> 8);
    lim |= (lim >> 16);
#if (LUA_MAXUNSIGNED >> 31) >= 3
    lim |= (lim >> 32);  
#endif
    lua_assert((lim & (lim + 1)) == 0  
      && lim >= n  
      && (lim >> 1) < n);  
    while ((ran &= lim) > n)  
      ran = I2UInt(nextrand(state->s));  
    return ran;
  }
}


static int math_random (lua_State *L) {
  lua_Integer low, up;
  lua_Unsigned p;
  RanState *state = (RanState *)lua_touserdata(L, lua_upvalueindex(1));
  Rand64 rv = nextrand(state->s);  
  switch (lua_gettop(L)) {  
    case 0: {  
      lua_pushnumber(L, I2d(rv));  
      return 1;
    }
    case 1: {  
      low = 1;
      up = luaL_checkinteger(L, 1);
      if (up == 0) {  
        lua_pushinteger(L, I2UInt(rv));  
        return 1;
      }
      break;
    }
    case 2: {  
      low = luaL_checkinteger(L, 1);
      up = luaL_checkinteger(L, 2);
      break;
    }
    default: return luaL_error(L, "wrong number of arguments");
  }
  
  luaL_argcheck(L, low <= up, 1, "interval is empty");
  
  p = project(I2UInt(rv), (lua_Unsigned)up - (lua_Unsigned)low, state);
  lua_pushinteger(L, p + (lua_Unsigned)low);
  return 1;
}


static void setseed (lua_State *L, Rand64 *state,
                     lua_Unsigned n1, lua_Unsigned n2) {
  int i;
  state[0] = Int2I(n1);
  state[1] = Int2I(0xff);  
  state[2] = Int2I(n2);
  state[3] = Int2I(0);
  for (i = 0; i < 16; i++)
    nextrand(state);  
  lua_pushinteger(L, n1);
  lua_pushinteger(L, n2);
}


static int math_randomseed (lua_State *L) {
  RanState *state = (RanState *)lua_touserdata(L, lua_upvalueindex(1));
  lua_Unsigned n1, n2;
  if (lua_isnone(L, 1)) {
    n1 = luaL_makeseed(L);  
    n2 = I2UInt(nextrand(state->s));  
  }
  else {
    n1 = luaL_checkinteger(L, 1);
    n2 = luaL_optinteger(L, 2, 0);
  }
  setseed(L, state->s, n1, n2);
  return 2;  
}


static const luaL_Reg randfuncs[] = {
  {"random", math_random},
  {"randomseed", math_randomseed},
  {NULL, NULL}
};



static void setrandfunc (lua_State *L) {
  RanState *state = (RanState *)lua_newuserdatauv(L, sizeof(RanState), 0);
  setseed(L, state->s, luaL_makeseed(L), 0);  
  lua_pop(L, 2);  
  luaL_setfuncs(L, randfuncs, 1);
}





#if defined(LUA_COMPAT_MATHLIB)

static int math_cosh (lua_State *L) {
  lua_pushnumber(L, l_mathop(cosh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_sinh (lua_State *L) {
  lua_pushnumber(L, l_mathop(sinh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_tanh (lua_State *L) {
  lua_pushnumber(L, l_mathop(tanh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_pow (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number y = luaL_checknumber(L, 2);
  lua_pushnumber(L, l_mathop(pow)(x, y));
  return 1;
}

static int math_frexp (lua_State *L) {
  int e;
  lua_pushnumber(L, l_mathop(frexp)(luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}

static int math_ldexp (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  int ep = (int)luaL_checkinteger(L, 2);
  lua_pushnumber(L, l_mathop(ldexp)(x, ep));
  return 1;
}

static int math_log10 (lua_State *L) {
  lua_pushnumber(L, l_mathop(log10)(luaL_checknumber(L, 1)));
  return 1;
}

#endif




static const luaL_Reg mathlib[] = {
  {"abs",   math_abs},
  {"acos",  math_acos},
  {"asin",  math_asin},
  {"atan",  math_atan},
  {"ceil",  math_ceil},
  {"cos",   math_cos},
  {"deg",   math_deg},
  {"exp",   math_exp},
  {"tointeger", math_toint},
  {"floor", math_floor},
  {"fmod",   math_fmod},
  {"ult",   math_ult},
  {"log",   math_log},
  {"max",   math_max},
  {"min",   math_min},
  {"modf",   math_modf},
  {"rad",   math_rad},
  {"sin",   math_sin},
  {"sqrt",  math_sqrt},
  {"tan",   math_tan},
  {"type", math_type},
#if defined(LUA_COMPAT_MATHLIB)
  {"atan2", math_atan},
  {"cosh",   math_cosh},
  {"sinh",   math_sinh},
  {"tanh",   math_tanh},
  {"pow",   math_pow},
  {"frexp", math_frexp},
  {"ldexp", math_ldexp},
  {"log10", math_log10},
#endif
  
  {"random", NULL},
  {"randomseed", NULL},
  {"pi", NULL},
  {"huge", NULL},
  {"maxinteger", NULL},
  {"mininteger", NULL},
  {NULL, NULL}
};



LUAMOD_API int luaopen_math (lua_State *L) {
  luaL_newlib(L, mathlib);
  lua_pushnumber(L, PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, (lua_Number)HUGE_VAL);
  lua_setfield(L, -2, "huge");
  lua_pushinteger(L, LUA_MAXINTEGER);
  lua_setfield(L, -2, "maxinteger");
  lua_pushinteger(L, LUA_MININTEGER);
  lua_setfield(L, -2, "mininteger");
  setrandfunc(L);
  return 1;
}



//path: src/imports/../dependencies/lua/loadlib.c


#define loadlib_c
#define LUA_LIB





#include <stdio.h>
#include <stdlib.h>
#include <string.h>











#if !defined(LUA_CSUBSEP)
#define LUA_CSUBSEP		LUA_DIRSEP
#endif

#if !defined(LUA_LSUBSEP)
#define LUA_LSUBSEP		LUA_DIRSEP
#endif



#define LUA_POF		"luaopen_"


#define LUA_OFSEP	"_"



static const char *const CLIBS = "_CLIBS";

#define LIB_FAIL	"open"


#define setprogdir(L)           ((void)0)



typedef void (*voidf)(void);





static void lsys_unloadlib (void *lib);


static void *lsys_load (lua_State *L, const char *path, int seeglb);


static lua_CFunction lsys_sym (lua_State *L, void *lib, const char *sym);




#if defined(LUA_USE_DLOPEN)	


#include <dlfcn.h>


#if defined(__GNUC__)
#define cast_func(p) (__extension__ (lua_CFunction)(p))
#else
#define cast_func(p) ((lua_CFunction)(p))
#endif


static void lsys_unloadlib (void *lib) {
  dlclose(lib);
}


static void *lsys_load (lua_State *L, const char *path, int seeglb) {
  void *lib = dlopen(path, RTLD_NOW | (seeglb ? RTLD_GLOBAL : RTLD_LOCAL));
  if (l_unlikely(lib == NULL))
    lua_pushstring(L, dlerror());
  return lib;
}


static lua_CFunction lsys_sym (lua_State *L, void *lib, const char *sym) {
  lua_CFunction f = cast_func(dlsym(lib, sym));
  if (l_unlikely(f == NULL))
    lua_pushstring(L, dlerror());
  return f;
}





#elif defined(LUA_DL_DLL)	


#include <windows.h>



#if !defined(LUA_LLE_FLAGS)
#define LUA_LLE_FLAGS	0
#endif


#undef setprogdir



static void setprogdir (lua_State *L) {
  char buff[MAX_PATH + 1];
  char *lb;
  DWORD nsize = sizeof(buff)/sizeof(char);
  DWORD n = GetModuleFileNameA(NULL, buff, nsize);  
  if (n == 0 || n == nsize || (lb = strrchr(buff, '\\')) == NULL)
    luaL_error(L, "unable to get ModuleFileName");
  else {
    *lb = '\0';  
    luaL_gsub(L, lua_tostring(L, -1), LUA_EXEC_DIR, buff);
    lua_remove(L, -2);  
  }
}




static void pusherror (lua_State *L) {
  int error = GetLastError();
  char buffer[128];
  if (FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL, error, 0, buffer, sizeof(buffer)/sizeof(char), NULL))
    lua_pushstring(L, buffer);
  else
    lua_pushfstring(L, "system error %d\n", error);
}

static void lsys_unloadlib (void *lib) {
  FreeLibrary((HMODULE)lib);
}


static void *lsys_load (lua_State *L, const char *path, int seeglb) {
  HMODULE lib = LoadLibraryExA(path, NULL, LUA_LLE_FLAGS);
  (void)(seeglb);  
  if (lib == NULL) pusherror(L);
  return lib;
}


static lua_CFunction lsys_sym (lua_State *L, void *lib, const char *sym) {
  lua_CFunction f = (lua_CFunction)(voidf)GetProcAddress((HMODULE)lib, sym);
  if (f == NULL) pusherror(L);
  return f;
}




#else				


#undef LIB_FAIL
#define LIB_FAIL	"absent"


#define DLMSG	"dynamic libraries not enabled; check your Lua installation"


static void lsys_unloadlib (void *lib) {
  (void)(lib);  
}


static void *lsys_load (lua_State *L, const char *path, int seeglb) {
  (void)(path); (void)(seeglb);  
  lua_pushliteral(L, DLMSG);
  return NULL;
}


static lua_CFunction lsys_sym (lua_State *L, void *lib, const char *sym) {
  (void)(lib); (void)(sym);  
  lua_pushliteral(L, DLMSG);
  return NULL;
}


#endif				





#if !defined(LUA_PATH_VAR)
#define LUA_PATH_VAR    "LUA_PATH"
#endif

#if !defined(LUA_CPATH_VAR)
#define LUA_CPATH_VAR   "LUA_CPATH"
#endif




static int noenv (lua_State *L) {
  int b;
  lua_getfield(L, LUA_REGISTRYINDEX, "LUA_NOENV");
  b = lua_toboolean(L, -1);
  lua_pop(L, 1);  
  return b;
}



static void setpath (lua_State *L, const char *fieldname,
                                   const char *envname,
                                   const char *dft) {
  const char *dftmark;
  const char *nver = lua_pushfstring(L, "%s%s", envname, LUA_VERSUFFIX);
  const char *path = getenv(nver);  
  if (path == NULL)  
    path = getenv(envname);  
  if (path == NULL || noenv(L))  
    lua_pushextlstring(L, dft, strlen(dft), NULL, NULL);  
  else if ((dftmark = strstr(path, LUA_PATH_SEP LUA_PATH_SEP)) == NULL)
    lua_pushstring(L, path);  
  else {  
    size_t len = strlen(path);
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    if (path < dftmark) {  
      luaL_addlstring(&b, path, dftmark - path);  
      luaL_addchar(&b, *LUA_PATH_SEP);
    }
    luaL_addstring(&b, dft);  
    if (dftmark < path + len - 2) {  
      luaL_addchar(&b, *LUA_PATH_SEP);
      luaL_addlstring(&b, dftmark + 2, (path + len - 2) - dftmark);
    }
    luaL_pushresult(&b);
  }
  setprogdir(L);
  lua_setfield(L, -3, fieldname);  
  lua_pop(L, 1);  
}





static void *checkclib (lua_State *L, const char *path) {
  void *plib;
  lua_getfield(L, LUA_REGISTRYINDEX, CLIBS);
  lua_getfield(L, -1, path);
  plib = lua_touserdata(L, -1);  
  lua_pop(L, 2);  
  return plib;
}



static void addtoclib (lua_State *L, const char *path, void *plib) {
  lua_getfield(L, LUA_REGISTRYINDEX, CLIBS);
  lua_pushlightuserdata(L, plib);
  lua_pushvalue(L, -1);
  lua_setfield(L, -3, path);  
  lua_rawseti(L, -2, luaL_len(L, -2) + 1);  
  lua_pop(L, 1);  
}



static int gctm (lua_State *L) {
  lua_Integer n = luaL_len(L, 1);
  for (; n >= 1; n--) {  
    lua_rawgeti(L, 1, n);  
    lsys_unloadlib(lua_touserdata(L, -1));
    lua_pop(L, 1);  
  }
  return 0;
}




#define ERRLIB		1
#define ERRFUNC		2


static int lookforfunc (lua_State *L, const char *path, const char *sym) {
  void *reg = checkclib(L, path);  
  if (reg == NULL) {  
    reg = lsys_load(L, path, *sym == '*');  
    if (reg == NULL) return ERRLIB;  
    addtoclib(L, path, reg);
  }
  if (*sym == '*') {  
    lua_pushboolean(L, 1);  
    return 0;  
  }
  else {
    lua_CFunction f = lsys_sym(L, reg, sym);
    if (f == NULL)
      return ERRFUNC;  
    lua_pushcfunction(L, f);  
    return 0;  
  }
}


static int ll_loadlib (lua_State *L) {
  const char *path = luaL_checkstring(L, 1);
  const char *init = luaL_checkstring(L, 2);
  int stat = lookforfunc(L, path, init);
  if (l_likely(stat == 0))  
    return 1;  
  else {  
    luaL_pushfail(L);
    lua_insert(L, -2);
    lua_pushstring(L, (stat == ERRLIB) ?  LIB_FAIL : "init");
    return 3;  
  }
}






static int readable (const char *filename) {
  FILE *f = fopen(filename, "r");  
  if (f == NULL) return 0;  
  fclose(f);
  return 1;
}



static const char *getnextfilename (char **path, char *end) {
  char *sep;
  char *name = *path;
  if (name == end)
    return NULL;  
  else if (*name == '\0') {  
    *name = *LUA_PATH_SEP;  
    name++;  
  }
  sep = strchr(name, *LUA_PATH_SEP);  
  if (sep == NULL)  
    sep = end;  
  *sep = '\0';  
  *path = sep;  
  return name;
}



static void pusherrornotfound (lua_State *L, const char *path) {
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  luaL_addstring(&b, "no file '");
  luaL_addgsub(&b, path, LUA_PATH_SEP, "'\n\tno file '");
  luaL_addstring(&b, "'");
  luaL_pushresult(&b);
}


static const char *searchpath (lua_State *L, const char *name,
                                             const char *path,
                                             const char *sep,
                                             const char *dirsep) {
  luaL_Buffer buff;
  char *pathname;  
  char *endpathname;  
  const char *filename;
  
  if (*sep != '\0' && strchr(name, *sep) != NULL)
    name = luaL_gsub(L, name, sep, dirsep);  
  luaL_buffinit(L, &buff);
  
  luaL_addgsub(&buff, path, LUA_PATH_MARK, name);
  luaL_addchar(&buff, '\0');
  pathname = luaL_buffaddr(&buff);  
  endpathname = pathname + luaL_bufflen(&buff) - 1;
  while ((filename = getnextfilename(&pathname, endpathname)) != NULL) {
    if (readable(filename))  
      return lua_pushstring(L, filename);  
  }
  luaL_pushresult(&buff);  
  pusherrornotfound(L, lua_tostring(L, -1));  
  return NULL;  
}


static int ll_searchpath (lua_State *L) {
  const char *f = searchpath(L, luaL_checkstring(L, 1),
                                luaL_checkstring(L, 2),
                                luaL_optstring(L, 3, "."),
                                luaL_optstring(L, 4, LUA_DIRSEP));
  if (f != NULL) return 1;
  else {  
    luaL_pushfail(L);
    lua_insert(L, -2);
    return 2;  
  }
}


static const char *findfile (lua_State *L, const char *name,
                                           const char *pname,
                                           const char *dirsep) {
  const char *path;
  lua_getfield(L, lua_upvalueindex(1), pname);
  path = lua_tostring(L, -1);
  if (l_unlikely(path == NULL))
    luaL_error(L, "'package.%s' must be a string", pname);
  return searchpath(L, name, path, ".", dirsep);
}


static int checkload (lua_State *L, int stat, const char *filename) {
  if (l_likely(stat)) {  
    lua_pushstring(L, filename);  
    return 2;  
  }
  else
    return luaL_error(L, "error loading module '%s' from file '%s':\n\t%s",
                          lua_tostring(L, 1), filename, lua_tostring(L, -1));
}


static int searcher_Lua (lua_State *L) {
  const char *filename;
  const char *name = luaL_checkstring(L, 1);
  filename = findfile(L, name, "path", LUA_LSUBSEP);
  if (filename == NULL) return 1;  
  return checkload(L, (luaL_loadfile(L, filename) == LUA_OK), filename);
}



static int loadfunc (lua_State *L, const char *filename, const char *modname) {
  const char *openfunc;
  const char *mark;
  modname = luaL_gsub(L, modname, ".", LUA_OFSEP);
  mark = strchr(modname, *LUA_IGMARK);
  if (mark) {
    int stat;
    openfunc = lua_pushlstring(L, modname, mark - modname);
    openfunc = lua_pushfstring(L, LUA_POF"%s", openfunc);
    stat = lookforfunc(L, filename, openfunc);
    if (stat != ERRFUNC) return stat;
    modname = mark + 1;  
  }
  openfunc = lua_pushfstring(L, LUA_POF"%s", modname);
  return lookforfunc(L, filename, openfunc);
}


static int searcher_C (lua_State *L) {
  const char *name = luaL_checkstring(L, 1);
  const char *filename = findfile(L, name, "cpath", LUA_CSUBSEP);
  if (filename == NULL) return 1;  
  return checkload(L, (loadfunc(L, filename, name) == 0), filename);
}


static int searcher_Croot (lua_State *L) {
  const char *filename;
  const char *name = luaL_checkstring(L, 1);
  const char *p = strchr(name, '.');
  int stat;
  if (p == NULL) return 0;  
  lua_pushlstring(L, name, p - name);
  filename = findfile(L, lua_tostring(L, -1), "cpath", LUA_CSUBSEP);
  if (filename == NULL) return 1;  
  if ((stat = loadfunc(L, filename, name)) != 0) {
    if (stat != ERRFUNC)
      return checkload(L, 0, filename);  
    else {  
      lua_pushfstring(L, "no module '%s' in file '%s'", name, filename);
      return 1;
    }
  }
  lua_pushstring(L, filename);  
  return 2;
}


static int searcher_preload (lua_State *L) {
  const char *name = luaL_checkstring(L, 1);
  lua_getfield(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
  if (lua_getfield(L, -1, name) == LUA_TNIL) {  
    lua_pushfstring(L, "no field package.preload['%s']", name);
    return 1;
  }
  else {
    lua_pushliteral(L, ":preload:");
    return 2;
  }
}


static void findloader (lua_State *L, const char *name) {
  int i;
  luaL_Buffer msg;  
  
  if (l_unlikely(lua_getfield(L, lua_upvalueindex(1), "searchers")
                 != LUA_TTABLE))
    luaL_error(L, "'package.searchers' must be a table");
  luaL_buffinit(L, &msg);
  
  for (i = 1; ; i++) {
    luaL_addstring(&msg, "\n\t");  
    if (l_unlikely(lua_rawgeti(L, 3, i) == LUA_TNIL)) {  
      lua_pop(L, 1);  
      luaL_buffsub(&msg, 2);  
      luaL_pushresult(&msg);  
      luaL_error(L, "module '%s' not found:%s", name, lua_tostring(L, -1));
    }
    lua_pushstring(L, name);
    lua_call(L, 1, 2);  
    if (lua_isfunction(L, -2))  
      return;  
    else if (lua_isstring(L, -2)) {  
      lua_pop(L, 1);  
      luaL_addvalue(&msg);  
    }
    else {  
      lua_pop(L, 2);  
      luaL_buffsub(&msg, 2);  
    }
  }
}


static int ll_require (lua_State *L) {
  const char *name = luaL_checkstring(L, 1);
  lua_settop(L, 1);  
  lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
  lua_getfield(L, 2, name);  
  if (lua_toboolean(L, -1))  
    return 1;  
  
  lua_pop(L, 1);  
  findloader(L, name);
  lua_rotate(L, -2, 1);  
  lua_pushvalue(L, 1);  
  lua_pushvalue(L, -3);  
  
  lua_call(L, 2, 1);  
  
  if (!lua_isnil(L, -1))  
    lua_setfield(L, 2, name);  
  else
    lua_pop(L, 1);  
  if (lua_getfield(L, 2, name) == LUA_TNIL) {   
    lua_pushboolean(L, 1);  
    lua_copy(L, -1, -2);  
    lua_setfield(L, 2, name);  
  }
  lua_rotate(L, -2, 1);  
  return 2;  
}






static const luaL_Reg pk_funcs[] = {
  {"loadlib", ll_loadlib},
  {"searchpath", ll_searchpath},
  
  {"preload", NULL},
  {"cpath", NULL},
  {"path", NULL},
  {"searchers", NULL},
  {"loaded", NULL},
  {NULL, NULL}
};


static const luaL_Reg ll_funcs[] = {
  {"require", ll_require},
  {NULL, NULL}
};


static void createsearcherstable (lua_State *L) {
  static const lua_CFunction searchers[] = {
    searcher_preload,
    searcher_Lua,
    searcher_C,
    searcher_Croot,
    NULL
  };
  int i;
  
  lua_createtable(L, sizeof(searchers)/sizeof(searchers[0]) - 1, 0);
  
  for (i=0; searchers[i] != NULL; i++) {
    lua_pushvalue(L, -2);  
    lua_pushcclosure(L, searchers[i], 1);
    lua_rawseti(L, -2, i+1);
  }
  lua_setfield(L, -2, "searchers");  
}



static void createclibstable (lua_State *L) {
  luaL_getsubtable(L, LUA_REGISTRYINDEX, CLIBS);  
  lua_createtable(L, 0, 1);  
  lua_pushcfunction(L, gctm);
  lua_setfield(L, -2, "__gc");  
  lua_setmetatable(L, -2);
}


LUAMOD_API int luaopen_package (lua_State *L) {
  createclibstable(L);
  luaL_newlib(L, pk_funcs);  
  createsearcherstable(L);
  
  setpath(L, "path", LUA_PATH_VAR, LUA_PATH_DEFAULT);
  setpath(L, "cpath", LUA_CPATH_VAR, LUA_CPATH_DEFAULT);
  
  lua_pushliteral(L, LUA_DIRSEP "\n" LUA_PATH_SEP "\n" LUA_PATH_MARK "\n"
                     LUA_EXEC_DIR "\n" LUA_IGMARK "\n");
  lua_setfield(L, -2, "config");
  
  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
  lua_setfield(L, -2, "loaded");
  
  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
  lua_setfield(L, -2, "preload");
  lua_pushglobaltable(L);
  lua_pushvalue(L, -2);  
  luaL_setfuncs(L, ll_funcs, 1);  
  lua_pop(L, 1);  
  return 1;  
}



//path: src/imports/../dependencies/lua/loslib.c


#define loslib_c
#define LUA_LIB





#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>











#if !defined(LUA_STRFTIMEOPTIONS)	

#if defined(LUA_USE_WINDOWS)
#define LUA_STRFTIMEOPTIONS  "aAbBcdHIjmMpSUwWxXyYzZ%" \
    "||" "#c#x#d#H#I#j#m#M#S#U#w#W#y#Y"  
#elif defined(LUA_USE_C89)  
#define LUA_STRFTIMEOPTIONS  "aAbBcdHIjmMpSUwWxXyYZ%"
#else  
#define LUA_STRFTIMEOPTIONS  "aAbBcCdDeFgGhHIjmMnprRStTuUVwWxXyYzZ%" \
    "||" "EcECExEXEyEY" "OdOeOHOIOmOMOSOuOUOVOwOWOy"  
#endif

#endif					






#if !defined(LUA_NUMTIME)	

#define l_timet			lua_Integer
#define l_pushtime(L,t)		lua_pushinteger(L,(lua_Integer)(t))
#define l_gettime(L,arg)	luaL_checkinteger(L, arg)

#else				

#define l_timet			lua_Number
#define l_pushtime(L,t)		lua_pushnumber(L,(lua_Number)(t))
#define l_gettime(L,arg)	luaL_checknumber(L, arg)

#endif				


#if !defined(l_gmtime)		


#if defined(LUA_USE_POSIX)	

#define l_gmtime(t,r)		gmtime_r(t,r)
#define l_localtime(t,r)	localtime_r(t,r)

#else				


#define l_gmtime(t,r)		((void)(r)->tm_sec, gmtime(t))
#define l_localtime(t,r)	((void)(r)->tm_sec, localtime(t))

#endif				

#endif				





#if !defined(lua_tmpnam)	

#if defined(LUA_USE_POSIX)	

#include <unistd.h>

#define LUA_TMPNAMBUFSIZE	32

#if !defined(LUA_TMPNAMTEMPLATE)
#define LUA_TMPNAMTEMPLATE	"/tmp/lua_XXXXXX"
#endif

#define lua_tmpnam(b,e) { \
        strcpy(b, LUA_TMPNAMTEMPLATE); \
        e = mkstemp(b); \
        if (e != -1) close(e); \
        e = (e == -1); }

#else				


#define LUA_TMPNAMBUFSIZE	L_tmpnam
#define lua_tmpnam(b,e)		{ e = (tmpnam(b) == NULL); }

#endif				

#endif				



#if !defined(l_system)
#if defined(LUA_USE_IOS)

#define l_system(cmd) ((cmd) == NULL ? 0 : -1)
#else
#define l_system(cmd)	system(cmd)  
#endif
#endif


static int os_execute (lua_State *L) {
  const char *cmd = luaL_optstring(L, 1, NULL);
  int stat;
  errno = 0;
  stat = l_system(cmd);
  if (cmd != NULL)
    return luaL_execresult(L, stat);
  else {
    lua_pushboolean(L, stat);  
    return 1;
  }
}


static int os_remove (lua_State *L) {
  const char *filename = luaL_checkstring(L, 1);
  return luaL_fileresult(L, remove(filename) == 0, filename);
}


static int os_rename (lua_State *L) {
  const char *fromname = luaL_checkstring(L, 1);
  const char *toname = luaL_checkstring(L, 2);
  return luaL_fileresult(L, rename(fromname, toname) == 0, NULL);
}
#ifdef __linux
static int os_tmpname(lua_State *L) {
  char buff[100];
 /// const char *tmpdir = "/tmp/lua_XXXXXX";
  int fd = mkstemp(buff);
  if (fd == -1) {
    return luaL_error(L, "unable to generate a unique filename");
  }

  close(fd);
  lua_pushstring(L, buff);
  return 1;
}
#endif
#ifdef _WIN32

static int os_tmpname(lua_State *L) {
  char buff[100] = {0};
  const char *template_file = "lua_XXXXXX";
  
  GetTempPathA(sizeof(buff), buff);
 
  UINT  fd = GetTempFileNameA(buff, template_file, 0, buff);
  if (fd == -1) {
    return luaL_error(L, "unable to generate a unique filename");
  }
  lua_pushstring(L, buff);
  return 1;
}
#endif
static int os_getenv (lua_State *L) {
  lua_pushstring(L, getenv(luaL_checkstring(L, 1)));  
  return 1;
}


static int os_clock (lua_State *L) {
  lua_pushnumber(L, ((lua_Number)clock())/(lua_Number)CLOCKS_PER_SEC);
  return 1;
}





static void setfield (lua_State *L, const char *key, int value, int delta) {
  #if (defined(LUA_NUMTIME) && LUA_MAXINTEGER <= INT_MAX)
    if (l_unlikely(value > LUA_MAXINTEGER - delta))
      luaL_error(L, "field '%s' is out-of-bound", key);
  #endif
  lua_pushinteger(L, (lua_Integer)value + delta);
  lua_setfield(L, -2, key);
}


static void setboolfield (lua_State *L, const char *key, int value) {
  if (value < 0)  
    return;  
  lua_pushboolean(L, value);
  lua_setfield(L, -2, key);
}



static void setallfields (lua_State *L, struct tm *stm) {
  setfield(L, "year", stm->tm_year, 1900);
  setfield(L, "month", stm->tm_mon, 1);
  setfield(L, "day", stm->tm_mday, 0);
  setfield(L, "hour", stm->tm_hour, 0);
  setfield(L, "min", stm->tm_min, 0);
  setfield(L, "sec", stm->tm_sec, 0);
  setfield(L, "yday", stm->tm_yday, 1);
  setfield(L, "wday", stm->tm_wday, 1);
  setboolfield(L, "isdst", stm->tm_isdst);
}


static int getboolfield (lua_State *L, const char *key) {
  int res;
  res = (lua_getfield(L, -1, key) == LUA_TNIL) ? -1 : lua_toboolean(L, -1);
  lua_pop(L, 1);
  return res;
}


static int getfield (lua_State *L, const char *key, int d, int delta) {
  int isnum;
  int t = lua_getfield(L, -1, key);  
  lua_Integer res = lua_tointegerx(L, -1, &isnum);
  if (!isnum) {  
    if (l_unlikely(t != LUA_TNIL))  
      return luaL_error(L, "field '%s' is not an integer", key);
    else if (l_unlikely(d < 0))  
      return luaL_error(L, "field '%s' missing in date table", key);
    res = d;
  }
  else {
    if (!(res >= 0 ? res - delta <= INT_MAX : INT_MIN + delta <= res))
      return luaL_error(L, "field '%s' is out-of-bound", key);
    res -= delta;
  }
  lua_pop(L, 1);
  return (int)res;
}


static const char *checkoption (lua_State *L, const char *conv,
                                ptrdiff_t convlen, char *buff) {
  const char *option = LUA_STRFTIMEOPTIONS;
  int oplen = 1;  
  for (; *option != '\0' && oplen <= convlen; option += oplen) {
    if (*option == '|')  
      oplen++;  
    else if (memcmp(conv, option, oplen) == 0) {  
      memcpy(buff, conv, oplen);  
      buff[oplen] = '\0';
      return conv + oplen;  
    }
  }
  luaL_argerror(L, 1,
    lua_pushfstring(L, "invalid conversion specifier '%%%s'", conv));
  return conv;  
}


static time_t l_checktime (lua_State *L, int arg) {
  l_timet t = l_gettime(L, arg);
  luaL_argcheck(L, (time_t)t == t, arg, "time out-of-bounds");
  return (time_t)t;
}



#define SIZETIMEFMT	250


static int os_date (lua_State *L) {
  size_t slen;
  const char *s = luaL_optlstring(L, 1, "%c", &slen);
  time_t t = luaL_opt(L, l_checktime, 2, time(NULL));
  const char *se = s + slen;  
  struct tm tmr, *stm;
  if (*s == '!') {  
    stm = l_gmtime(&t, &tmr);
    s++;  
  }
  else
    stm = l_localtime(&t, &tmr);
  if (stm == NULL)  
    return luaL_error(L,
                 "date result cannot be represented in this installation");
  if (strcmp(s, "*t") == 0) {
    lua_createtable(L, 0, 9);  
    setallfields(L, stm);
  }
  else {
    char cc[4];  
    luaL_Buffer b;
    cc[0] = '%';
    luaL_buffinit(L, &b);
    while (s < se) {
      if (*s != '%')  
        luaL_addchar(&b, *s++);
      else {
        size_t reslen;
        char *buff = luaL_prepbuffsize(&b, SIZETIMEFMT);
        s++;  
        s = checkoption(L, s, se - s, cc + 1);  
        reslen = strftime(buff, SIZETIMEFMT, cc, stm);
        luaL_addsize(&b, reslen);
      }
    }
    luaL_pushresult(&b);
  }
  return 1;
}


static int os_time (lua_State *L) {
  time_t t;
  if (lua_isnoneornil(L, 1))  
    t = time(NULL);  
  else {
    struct tm ts;
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_settop(L, 1);  
    ts.tm_year = getfield(L, "year", -1, 1900);
    ts.tm_mon = getfield(L, "month", -1, 1);
    ts.tm_mday = getfield(L, "day", -1, 0);
    ts.tm_hour = getfield(L, "hour", 12, 0);
    ts.tm_min = getfield(L, "min", 0, 0);
    ts.tm_sec = getfield(L, "sec", 0, 0);
    ts.tm_isdst = getboolfield(L, "isdst");
    t = mktime(&ts);
    setallfields(L, &ts);  
  }
  if (t != (time_t)(l_timet)t || t == (time_t)(-1))
    return luaL_error(L,
                  "time result cannot be represented in this installation");
  l_pushtime(L, t);
  return 1;
}


static int os_difftime (lua_State *L) {
  time_t t1 = l_checktime(L, 1);
  time_t t2 = l_checktime(L, 2);
  lua_pushnumber(L, (lua_Number)difftime(t1, t2));
  return 1;
}




static int os_setlocale (lua_State *L) {
  static const int cat[] = {LC_ALL, LC_COLLATE, LC_CTYPE, LC_MONETARY,
                      LC_NUMERIC, LC_TIME};
  static const char *const catnames[] = {"all", "collate", "ctype", "monetary",
     "numeric", "time", NULL};
  const char *l = luaL_optstring(L, 1, NULL);
  int op = luaL_checkoption(L, 2, "all", catnames);
  lua_pushstring(L, setlocale(cat[op], l));
  return 1;
}


static int os_exit (lua_State *L) {
  int status;
  if (lua_isboolean(L, 1))
    status = (lua_toboolean(L, 1) ? EXIT_SUCCESS : EXIT_FAILURE);
  else
    status = (int)luaL_optinteger(L, 1, EXIT_SUCCESS);
  if (lua_toboolean(L, 2))
    lua_close(L);
  if (L) exit(status);  
  return 0;
}


static const luaL_Reg syslib[] = {
  {"clock",     os_clock},
  {"date",      os_date},
  {"difftime",  os_difftime},
  {"execute",   os_execute},
  {"exit",      os_exit},
  {"getenv",    os_getenv},
  {"remove",    os_remove},
  {"rename",    os_rename},
  {"setlocale", os_setlocale},
  {"time",      os_time},
  {"tmpname",   os_tmpname},
  {NULL, NULL}
};





LUAMOD_API int luaopen_os (lua_State *L) {
  luaL_newlib(L, syslib);
  return 1;
}



//path: src/imports/../dependencies/lua/lstrlib.c


#define lstrlib_c
#define LUA_LIB





#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>











#if !defined(LUA_MAXCAPTURES)
#define LUA_MAXCAPTURES		32
#endif



#define uchar(c)	((unsigned char)(c))



#define MAX_SIZET	((size_t)(~(size_t)0))

#define MAXSIZE  \
	(sizeof(size_t) < sizeof(int) ? MAX_SIZET : (size_t)(INT_MAX))




static int str_len (lua_State *L) {
  size_t l;
  luaL_checklstring(L, 1, &l);
  lua_pushinteger(L, (lua_Integer)l);
  return 1;
}



static size_t posrelatI (lua_Integer pos, size_t len) {
  if (pos > 0)
    return (size_t)pos;
  else if (pos == 0)
    return 1;
  else if (pos < -(lua_Integer)len)  
    return 1;  
  else return len + (size_t)pos + 1;
}



static size_t getendpos (lua_State *L, int arg, lua_Integer def,
                         size_t len) {
  lua_Integer pos = luaL_optinteger(L, arg, def);
  if (pos > (lua_Integer)len)
    return len;
  else if (pos >= 0)
    return (size_t)pos;
  else if (pos < -(lua_Integer)len)
    return 0;
  else return len + (size_t)pos + 1;
}


static int str_sub (lua_State *L) {
  size_t l;
  const char *s = luaL_checklstring(L, 1, &l);
  size_t start = posrelatI(luaL_checkinteger(L, 2), l);
  size_t end = getendpos(L, 3, -1, l);
  if (start <= end)
    lua_pushlstring(L, s + start - 1, (end - start) + 1);
  else lua_pushliteral(L, "");
  return 1;
}


static int str_reverse (lua_State *L) {
  size_t l, i;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  char *p = luaL_buffinitsize(L, &b, l);
  for (i = 0; i < l; i++)
    p[i] = s[l - i - 1];
  luaL_pushresultsize(&b, l);
  return 1;
}


static int str_lower (lua_State *L) {
  size_t l;
  size_t i;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  char *p = luaL_buffinitsize(L, &b, l);
  for (i=0; i<l; i++)
    p[i] = tolower(uchar(s[i]));
  luaL_pushresultsize(&b, l);
  return 1;
}


static int str_upper (lua_State *L) {
  size_t l;
  size_t i;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  char *p = luaL_buffinitsize(L, &b, l);
  for (i=0; i<l; i++)
    p[i] = toupper(uchar(s[i]));
  luaL_pushresultsize(&b, l);
  return 1;
}


static int str_rep (lua_State *L) {
  size_t l, lsep;
  const char *s = luaL_checklstring(L, 1, &l);
  lua_Integer n = luaL_checkinteger(L, 2);
  const char *sep = luaL_optlstring(L, 3, "", &lsep);
  if (n <= 0)
    lua_pushliteral(L, "");
  else if (l_unlikely(l + lsep < l || l + lsep > MAXSIZE / n))
    return luaL_error(L, "resulting string too large");
  else {
    size_t totallen = (size_t)n * l + (size_t)(n - 1) * lsep;
    luaL_Buffer b;
    char *p = luaL_buffinitsize(L, &b, totallen);
    while (n-- > 1) {  
      memcpy(p, s, l * sizeof(char)); p += l;
      if (lsep > 0) {  
        memcpy(p, sep, lsep * sizeof(char));
        p += lsep;
      }
    }
    memcpy(p, s, l * sizeof(char));  
    luaL_pushresultsize(&b, totallen);
  }
  return 1;
}


static int str_byte (lua_State *L) {
  size_t l;
  const char *s = luaL_checklstring(L, 1, &l);
  lua_Integer pi = luaL_optinteger(L, 2, 1);
  size_t posi = posrelatI(pi, l);
  size_t pose = getendpos(L, 3, pi, l);
  int n, i;
  if (posi > pose) return 0;  
  if (l_unlikely(pose - posi >= (size_t)INT_MAX))  
    return luaL_error(L, "string slice too long");
  n = (int)(pose -  posi) + 1;
  luaL_checkstack(L, n, "string slice too long");
  for (i=0; i<n; i++)
    lua_pushinteger(L, uchar(s[posi+i-1]));
  return n;
}


static int str_char (lua_State *L) {
  int n = lua_gettop(L);  
  int i;
  luaL_Buffer b;
  char *p = luaL_buffinitsize(L, &b, n);
  for (i=1; i<=n; i++) {
    lua_Unsigned c = (lua_Unsigned)luaL_checkinteger(L, i);
    luaL_argcheck(L, c <= (lua_Unsigned)UCHAR_MAX, i, "value out of range");
    p[i - 1] = uchar(c);
  }
  luaL_pushresultsize(&b, n);
  return 1;
}



struct str_Writer {
  int init;  
  luaL_Buffer B;
};


static int writer (lua_State *L, const void *b, size_t size, void *ud) {
  struct str_Writer *state = (struct str_Writer *)ud;
  if (!state->init) {
    state->init = 1;
    luaL_buffinit(L, &state->B);
  }
  if (b == NULL) {  
    luaL_pushresult(&state->B);  
    lua_replace(L, 1);  
  }
  else
    luaL_addlstring(&state->B, (const char *)b, size);
  return 0;
}


static int str_dump (lua_State *L) {
  struct str_Writer state;
  int strip = lua_toboolean(L, 2);
  luaL_argcheck(L, lua_type(L, 1) == LUA_TFUNCTION && !lua_iscfunction(L, 1),
                   1, "Lua function expected");
  
  lua_pushvalue(L, 1);
  state.init = 0;
  lua_dump(L, writer, &state, strip);
  lua_settop(L, 1);  
  return 1;
}





#if defined(LUA_NOCVTS2N)	



static const luaL_Reg stringmetamethods[] = {
  {"__index", NULL},  
  {NULL, NULL}
};

#else		

static int tonum (lua_State *L, int arg) {
  if (lua_type(L, arg) == LUA_TNUMBER) {  
    lua_pushvalue(L, arg);
    return 1;
  }
  else {  
    size_t len;
    const char *s = lua_tolstring(L, arg, &len);
    return (s != NULL && lua_stringtonumber(L, s) == len + 1);
  }
}


static void trymt (lua_State *L, const char *mtname) {
  lua_settop(L, 2);  
  if (l_unlikely(lua_type(L, 2) == LUA_TSTRING ||
                 !luaL_getmetafield(L, 2, mtname)))
    luaL_error(L, "attempt to %s a '%s' with a '%s'", mtname + 2,
                  luaL_typename(L, -2), luaL_typename(L, -1));
  lua_insert(L, -3);  
  lua_call(L, 2, 1);  
}


static int arith (lua_State *L, int op, const char *mtname) {
  if (tonum(L, 1) && tonum(L, 2))
    lua_arith(L, op);  
  else
    trymt(L, mtname);
  return 1;
}


static int arith_add (lua_State *L) {
  return arith(L, LUA_OPADD, "__add");
}

static int arith_sub (lua_State *L) {
  return arith(L, LUA_OPSUB, "__sub");
}

static int arith_mul (lua_State *L) {
  return arith(L, LUA_OPMUL, "__mul");
}

static int arith_mod (lua_State *L) {
  return arith(L, LUA_OPMOD, "__mod");
}

static int arith_pow (lua_State *L) {
  return arith(L, LUA_OPPOW, "__pow");
}

static int arith_div (lua_State *L) {
  return arith(L, LUA_OPDIV, "__div");
}

static int arith_idiv (lua_State *L) {
  return arith(L, LUA_OPIDIV, "__idiv");
}

static int arith_unm (lua_State *L) {
  return arith(L, LUA_OPUNM, "__unm");
}


static const luaL_Reg stringmetamethods[] = {
  {"__add", arith_add},
  {"__sub", arith_sub},
  {"__mul", arith_mul},
  {"__mod", arith_mod},
  {"__pow", arith_pow},
  {"__div", arith_div},
  {"__idiv", arith_idiv},
  {"__unm", arith_unm},
  {"__index", NULL},  
  {NULL, NULL}
};

#endif		






#define CAP_UNFINISHED	(-1)
#define CAP_POSITION	(-2)


typedef struct MatchState {
  const char *src_init;  
  const char *src_end;  
  const char *p_end;  
  lua_State *L;
  int matchdepth;  
  unsigned char level;  
  struct {
    const char *init;
    ptrdiff_t len;
  } capture[LUA_MAXCAPTURES];
} MatchState;



static const char *match (MatchState *ms, const char *s, const char *p);



#if !defined(MAXCCALLS)
#define MAXCCALLS	200
#endif


#define L_ESC		'%'
#define SPECIALS	"^$*+?.([%-"


static int check_capture (MatchState *ms, int l) {
  l -= '1';
  if (l_unlikely(l < 0 || l >= ms->level ||
                 ms->capture[l].len == CAP_UNFINISHED))
    return luaL_error(ms->L, "invalid capture index %%%d", l + 1);
  return l;
}


static int capture_to_close (MatchState *ms) {
  int level = ms->level;
  for (level--; level>=0; level--)
    if (ms->capture[level].len == CAP_UNFINISHED) return level;
  return luaL_error(ms->L, "invalid pattern capture");
}


static const char *classend (MatchState *ms, const char *p) {
  switch (*p++) {
    case L_ESC: {
      if (l_unlikely(p == ms->p_end))
        luaL_error(ms->L, "malformed pattern (ends with '%%')");
      return p+1;
    }
    case '[': {
      if (*p == '^') p++;
      do {  
        if (l_unlikely(p == ms->p_end))
          luaL_error(ms->L, "malformed pattern (missing ']')");
        if (*(p++) == L_ESC && p < ms->p_end)
          p++;  
      } while (*p != ']');
      return p+1;
    }
    default: {
      return p;
    }
  }
}


static int match_class (int c, int cl) {
  int res;
  switch (tolower(cl)) {
    case 'a' : res = isalpha(c); break;
    case 'c' : res = iscntrl(c); break;
    case 'd' : res = isdigit(c); break;
    case 'g' : res = isgraph(c); break;
    case 'l' : res = islower(c); break;
    case 'p' : res = ispunct(c); break;
    case 's' : res = isspace(c); break;
    case 'u' : res = isupper(c); break;
    case 'w' : res = isalnum(c); break;
    case 'x' : res = isxdigit(c); break;
    case 'z' : res = (c == 0); break;  
    default: return (cl == c);
  }
  return (islower(cl) ? res : !res);
}


static int matchbracketclass (int c, const char *p, const char *ec) {
  int sig = 1;
  if (*(p+1) == '^') {
    sig = 0;
    p++;  
  }
  while (++p < ec) {
    if (*p == L_ESC) {
      p++;
      if (match_class(c, uchar(*p)))
        return sig;
    }
    else if ((*(p+1) == '-') && (p+2 < ec)) {
      p+=2;
      if (uchar(*(p-2)) <= c && c <= uchar(*p))
        return sig;
    }
    else if (uchar(*p) == c) return sig;
  }
  return !sig;
}


static int singlematch (MatchState *ms, const char *s, const char *p,
                        const char *ep) {
  if (s >= ms->src_end)
    return 0;
  else {
    int c = uchar(*s);
    switch (*p) {
      case '.': return 1;  
      case L_ESC: return match_class(c, uchar(*(p+1)));
      case '[': return matchbracketclass(c, p, ep-1);
      default:  return (uchar(*p) == c);
    }
  }
}


static const char *matchbalance (MatchState *ms, const char *s,
                                   const char *p) {
  if (l_unlikely(p >= ms->p_end - 1))
    luaL_error(ms->L, "malformed pattern (missing arguments to '%%b')");
  if (*s != *p) return NULL;
  else {
    int b = *p;
    int e = *(p+1);
    int cont = 1;
    while (++s < ms->src_end) {
      if (*s == e) {
        if (--cont == 0) return s+1;
      }
      else if (*s == b) cont++;
    }
  }
  return NULL;  
}


static const char *max_expand (MatchState *ms, const char *s,
                                 const char *p, const char *ep) {
  ptrdiff_t i = 0;  
  while (singlematch(ms, s + i, p, ep))
    i++;
  
  while (i>=0) {
    const char *res = match(ms, (s+i), ep+1);
    if (res) return res;
    i--;  
  }
  return NULL;
}


static const char *min_expand (MatchState *ms, const char *s,
                                 const char *p, const char *ep) {
  for (;;) {
    const char *res = match(ms, s, ep+1);
    if (res != NULL)
      return res;
    else if (singlematch(ms, s, p, ep))
      s++;  
    else return NULL;
  }
}


static const char *start_capture (MatchState *ms, const char *s,
                                    const char *p, int what) {
  const char *res;
  int level = ms->level;
  if (level >= LUA_MAXCAPTURES) luaL_error(ms->L, "too many captures");
  ms->capture[level].init = s;
  ms->capture[level].len = what;
  ms->level = level+1;
  if ((res=match(ms, s, p)) == NULL)  
    ms->level--;  
  return res;
}


static const char *end_capture (MatchState *ms, const char *s,
                                  const char *p) {
  int l = capture_to_close(ms);
  const char *res;
  ms->capture[l].len = s - ms->capture[l].init;  
  if ((res = match(ms, s, p)) == NULL)  
    ms->capture[l].len = CAP_UNFINISHED;  
  return res;
}


static const char *match_capture (MatchState *ms, const char *s, int l) {
  size_t len;
  l = check_capture(ms, l);
  len = ms->capture[l].len;
  if ((size_t)(ms->src_end-s) >= len &&
      memcmp(ms->capture[l].init, s, len) == 0)
    return s+len;
  else return NULL;
}


static const char *match (MatchState *ms, const char *s, const char *p) {
  if (l_unlikely(ms->matchdepth-- == 0))
    luaL_error(ms->L, "pattern too complex");
  init: 
  if (p != ms->p_end) {  
    switch (*p) {
      case '(': {  
        if (*(p + 1) == ')')  
          s = start_capture(ms, s, p + 2, CAP_POSITION);
        else
          s = start_capture(ms, s, p + 1, CAP_UNFINISHED);
        break;
      }
      case ')': {  
        s = end_capture(ms, s, p + 1);
        break;
      }
      case '$': {
        if ((p + 1) != ms->p_end)  
          goto dflt;  
        s = (s == ms->src_end) ? s : NULL;  
        break;
      }
      case L_ESC: {  
        switch (*(p + 1)) {
          case 'b': {  
            s = matchbalance(ms, s, p + 2);
            if (s != NULL) {
              p += 4; goto init;  
            }  
            break;
          }
          case 'f': {  
            const char *ep; char previous;
            p += 2;
            if (l_unlikely(*p != '['))
              luaL_error(ms->L, "missing '[' after '%%f' in pattern");
            ep = classend(ms, p);  
            previous = (s == ms->src_init) ? '\0' : *(s - 1);
            if (!matchbracketclass(uchar(previous), p, ep - 1) &&
               matchbracketclass(uchar(*s), p, ep - 1)) {
              p = ep; goto init;  
            }
            s = NULL;  
            break;
          }
          case '0': case '1': case '2': case '3':
          case '4': case '5': case '6': case '7':
          case '8': case '9': {  
            s = match_capture(ms, s, uchar(*(p + 1)));
            if (s != NULL) {
              p += 2; goto init;  
            }
            break;
          }
          default: goto dflt;
        }
        break;
      }
      default: dflt: {  
        const char *ep = classend(ms, p);  
        
        if (!singlematch(ms, s, p, ep)) {
          if (*ep == '*' || *ep == '?' || *ep == '-') {  
            p = ep + 1; goto init;  
          }
          else  
            s = NULL;  
        }
        else {  
          switch (*ep) {  
            case '?': {  
              const char *res;
              if ((res = match(ms, s + 1, ep + 1)) != NULL)
                s = res;
              else {
                p = ep + 1; goto init;  
              }
              break;
            }
            case '+':  
              s++;  
              
            case '*':  
              s = max_expand(ms, s, p, ep);
              break;
            case '-':  
              s = min_expand(ms, s, p, ep);
              break;
            default:  
              s++; p = ep; goto init;  
          }
        }
        break;
      }
    }
  }
  ms->matchdepth++;
  return s;
}



static const char *lmemfind (const char *s1, size_t l1,
                               const char *s2, size_t l2) {
  if (l2 == 0) return s1;  
  else if (l2 > l1) return NULL;  
  else {
    const char *init;  
    l2--;  
    l1 = l1-l2;  
    while (l1 > 0 && (init = (const char *)memchr(s1, *s2, l1)) != NULL) {
      init++;   
      if (memcmp(init, s2+1, l2) == 0)
        return init-1;
      else {  
        l1 -= init-s1;
        s1 = init;
      }
    }
    return NULL;  
  }
}



static size_t get_onecapture (MatchState *ms, int i, const char *s,
                              const char *e, const char **cap) {
  if (i >= ms->level) {
    if (l_unlikely(i != 0))
      luaL_error(ms->L, "invalid capture index %%%d", i + 1);
    *cap = s;
    return e - s;
  }
  else {
    ptrdiff_t capl = ms->capture[i].len;
    *cap = ms->capture[i].init;
    if (l_unlikely(capl == CAP_UNFINISHED))
      luaL_error(ms->L, "unfinished capture");
    else if (capl == CAP_POSITION)
      lua_pushinteger(ms->L, (ms->capture[i].init - ms->src_init) + 1);
    return capl;
  }
}



static void push_onecapture (MatchState *ms, int i, const char *s,
                                                    const char *e) {
  const char *cap;
  ptrdiff_t l = get_onecapture(ms, i, s, e, &cap);
  if (l != CAP_POSITION)
    lua_pushlstring(ms->L, cap, l);
  
}


static int push_captures (MatchState *ms, const char *s, const char *e) {
  int i;
  int nlevels = (ms->level == 0 && s) ? 1 : ms->level;
  luaL_checkstack(ms->L, nlevels, "too many captures");
  for (i = 0; i < nlevels; i++)
    push_onecapture(ms, i, s, e);
  return nlevels;  
}



static int nospecials (const char *p, size_t l) {
  size_t upto = 0;
  do {
    if (strpbrk(p + upto, SPECIALS))
      return 0;  
    upto += strlen(p + upto) + 1;  
  } while (upto <= l);
  return 1;  
}


static void prepstate (MatchState *ms, lua_State *L,
                       const char *s, size_t ls, const char *p, size_t lp) {
  ms->L = L;
  ms->matchdepth = MAXCCALLS;
  ms->src_init = s;
  ms->src_end = s + ls;
  ms->p_end = p + lp;
}


static void reprepstate (MatchState *ms) {
  ms->level = 0;
  lua_assert(ms->matchdepth == MAXCCALLS);
}


static int str_find_aux (lua_State *L, int find) {
  size_t ls, lp;
  const char *s = luaL_checklstring(L, 1, &ls);
  const char *p = luaL_checklstring(L, 2, &lp);
  size_t init = posrelatI(luaL_optinteger(L, 3, 1), ls) - 1;
  if (init > ls) {  
    luaL_pushfail(L);  
    return 1;
  }
  
  if (find && (lua_toboolean(L, 4) || nospecials(p, lp))) {
    
    const char *s2 = lmemfind(s + init, ls - init, p, lp);
    if (s2) {
      lua_pushinteger(L, (s2 - s) + 1);
      lua_pushinteger(L, (s2 - s) + lp);
      return 2;
    }
  }
  else {
    MatchState ms;
    const char *s1 = s + init;
    int anchor = (*p == '^');
    if (anchor) {
      p++; lp--;  
    }
    prepstate(&ms, L, s, ls, p, lp);
    do {
      const char *res;
      reprepstate(&ms);
      if ((res=match(&ms, s1, p)) != NULL) {
        if (find) {
          lua_pushinteger(L, (s1 - s) + 1);  
          lua_pushinteger(L, res - s);   
          return push_captures(&ms, NULL, 0) + 2;
        }
        else
          return push_captures(&ms, s1, res);
      }
    } while (s1++ < ms.src_end && !anchor);
  }
  luaL_pushfail(L);  
  return 1;
}


static int str_find (lua_State *L) {
  return str_find_aux(L, 1);
}


static int str_match (lua_State *L) {
  return str_find_aux(L, 0);
}



typedef struct GMatchState {
  const char *src;  
  const char *p;  
  const char *lastmatch;  
  MatchState ms;  
} GMatchState;


static int gmatch_aux (lua_State *L) {
  GMatchState *gm = (GMatchState *)lua_touserdata(L, lua_upvalueindex(3));
  const char *src;
  gm->ms.L = L;
  for (src = gm->src; src <= gm->ms.src_end; src++) {
    const char *e;
    reprepstate(&gm->ms);
    if ((e = match(&gm->ms, src, gm->p)) != NULL && e != gm->lastmatch) {
      gm->src = gm->lastmatch = e;
      return push_captures(&gm->ms, src, e);
    }
  }
  return 0;  
}


static int gmatch (lua_State *L) {
  size_t ls, lp;
  const char *s = luaL_checklstring(L, 1, &ls);
  const char *p = luaL_checklstring(L, 2, &lp);
  size_t init = posrelatI(luaL_optinteger(L, 3, 1), ls) - 1;
  GMatchState *gm;
  lua_settop(L, 2);  
  gm = (GMatchState *)lua_newuserdatauv(L, sizeof(GMatchState), 0);
  if (init > ls)  
    init = ls + 1;  
  prepstate(&gm->ms, L, s, ls, p, lp);
  gm->src = s + init; gm->p = p; gm->lastmatch = NULL;
  lua_pushcclosure(L, gmatch_aux, 3);
  return 1;
}


static void add_s (MatchState *ms, luaL_Buffer *b, const char *s,
                                                   const char *e) {
  size_t l;
  lua_State *L = ms->L;
  const char *news = lua_tolstring(L, 3, &l);
  const char *p;
  while ((p = (char *)memchr(news, L_ESC, l)) != NULL) {
    luaL_addlstring(b, news, p - news);
    p++;  
    if (*p == L_ESC)  
      luaL_addchar(b, *p);
    else if (*p == '0')  
        luaL_addlstring(b, s, e - s);
    else if (isdigit(uchar(*p))) {  
      const char *cap;
      ptrdiff_t resl = get_onecapture(ms, *p - '1', s, e, &cap);
      if (resl == CAP_POSITION)
        luaL_addvalue(b);  
      else
        luaL_addlstring(b, cap, resl);
    }
    else
      luaL_error(L, "invalid use of '%c' in replacement string", L_ESC);
    l -= p + 1 - news;
    news = p + 1;
  }
  luaL_addlstring(b, news, l);
}



static int add_value (MatchState *ms, luaL_Buffer *b, const char *s,
                                      const char *e, int tr) {
  lua_State *L = ms->L;
  switch (tr) {
    case LUA_TFUNCTION: {  
      int n;
      lua_pushvalue(L, 3);  
      n = push_captures(ms, s, e);  
      lua_call(L, n, 1);  
      break;
    }
    case LUA_TTABLE: {  
      push_onecapture(ms, 0, s, e);  
      lua_gettable(L, 3);
      break;
    }
    default: {  
      add_s(ms, b, s, e);  
      return 1;  
    }
  }
  if (!lua_toboolean(L, -1)) {  
    lua_pop(L, 1);  
    luaL_addlstring(b, s, e - s);  
    return 0;  
  }
  else if (l_unlikely(!lua_isstring(L, -1)))
    return luaL_error(L, "invalid replacement value (a %s)",
                         luaL_typename(L, -1));
  else {
    luaL_addvalue(b);  
    return 1;  
  }
}


static int str_gsub (lua_State *L) {
  size_t srcl, lp;
  const char *src = luaL_checklstring(L, 1, &srcl);  
  const char *p = luaL_checklstring(L, 2, &lp);  
  const char *lastmatch = NULL;  
  int tr = lua_type(L, 3);  
  lua_Integer max_s = luaL_optinteger(L, 4, srcl + 1);  
  int anchor = (*p == '^');
  lua_Integer n = 0;  
  int changed = 0;  
  MatchState ms;
  luaL_Buffer b;
  luaL_argexpected(L, tr == LUA_TNUMBER || tr == LUA_TSTRING ||
                   tr == LUA_TFUNCTION || tr == LUA_TTABLE, 3,
                      "string/function/table");
  luaL_buffinit(L, &b);
  if (anchor) {
    p++; lp--;  
  }
  prepstate(&ms, L, src, srcl, p, lp);
  while (n < max_s) {
    const char *e;
    reprepstate(&ms);  
    if ((e = match(&ms, src, p)) != NULL && e != lastmatch) {  
      n++;
      changed = add_value(&ms, &b, src, e, tr) | changed;
      src = lastmatch = e;
    }
    else if (src < ms.src_end)  
      luaL_addchar(&b, *src++);
    else break;  
    if (anchor) break;
  }
  if (!changed)  
    lua_pushvalue(L, 1);  
  else {  
    luaL_addlstring(&b, src, ms.src_end-src);
    luaL_pushresult(&b);  
  }
  lua_pushinteger(L, n);  
  return 2;
}







#if !defined(lua_number2strx)	



#define SIZELENMOD	(sizeof(LUA_NUMBER_FRMLEN)/sizeof(char))



#define L_NBFD		((l_floatatt(MANT_DIG) - 1)%4 + 1)



static lua_Number adddigit (char *buff, int n, lua_Number x) {
  lua_Number dd = l_mathop(floor)(x);  
  int d = (int)dd;
  buff[n] = (d < 10 ? d + '0' : d - 10 + 'a');  
  return x - dd;  
}


static int num2straux (char *buff, int sz, lua_Number x) {
  
  if (x != x || x == (lua_Number)HUGE_VAL || x == -(lua_Number)HUGE_VAL)
    return l_sprintf(buff, sz, LUA_NUMBER_FMT, (LUAI_UACNUMBER)x);
  else if (x == 0) {  
    
    return l_sprintf(buff, sz, LUA_NUMBER_FMT "x0p+0", (LUAI_UACNUMBER)x);
  }
  else {
    int e;
    lua_Number m = l_mathop(frexp)(x, &e);  
    int n = 0;  
    if (m < 0) {  
      buff[n++] = '-';  
      m = -m;  
    }
    buff[n++] = '0'; buff[n++] = 'x';  
    m = adddigit(buff, n++, m * (1 << L_NBFD));  
    e -= L_NBFD;  
    if (m > 0) {  
      buff[n++] = lua_getlocaledecpoint();  
      do {  
        m = adddigit(buff, n++, m * 16);
      } while (m > 0);
    }
    n += l_sprintf(buff + n, sz - n, "p%+d", e);  
    lua_assert(n < sz);
    return n;
  }
}


static int lua_number2strx (lua_State *L, char *buff, int sz,
                            const char *fmt, lua_Number x) {
  int n = num2straux(buff, sz, x);
  if (fmt[SIZELENMOD] == 'A') {
    int i;
    for (i = 0; i < n; i++)
      buff[i] = toupper(uchar(buff[i]));
  }
  else if (l_unlikely(fmt[SIZELENMOD] != 'a'))
    return luaL_error(L, "modifiers for format '%%a'/'%%A' not implemented");
  return n;
}

#endif				



#define MAX_ITEMF	(110 + l_floatatt(MAX_10_EXP))



#define MAX_ITEM	120



#if !defined(L_FMTFLAGSF)


#define L_FMTFLAGSF	"-+#0 "


#define L_FMTFLAGSX	"-#0"


#define L_FMTFLAGSI	"-+0 "


#define L_FMTFLAGSU	"-0"


#define L_FMTFLAGSC	"-"

#endif



#define MAX_FORMAT	32


static void addquoted (luaL_Buffer *b, const char *s, size_t len) {
  luaL_addchar(b, '"');
  while (len--) {
    if (*s == '"' || *s == '\\' || *s == '\n') {
      luaL_addchar(b, '\\');
      luaL_addchar(b, *s);
    }
    else if (iscntrl(uchar(*s))) {
      char buff[10];
      if (!isdigit(uchar(*(s+1))))
        l_sprintf(buff, sizeof(buff), "\\%d", (int)uchar(*s));
      else
        l_sprintf(buff, sizeof(buff), "\\%03d", (int)uchar(*s));
      luaL_addstring(b, buff);
    }
    else
      luaL_addchar(b, *s);
    s++;
  }
  luaL_addchar(b, '"');
}



static int quotefloat (lua_State *L, char *buff, lua_Number n) {
  const char *s;  
  if (n == (lua_Number)HUGE_VAL)  
    s = "1e9999";
  else if (n == -(lua_Number)HUGE_VAL)  
    s = "-1e9999";
  else if (n != n)  
    s = "(0/0)";
  else {  
    int  nb = lua_number2strx(L, buff, MAX_ITEM,
                                 "%" LUA_NUMBER_FRMLEN "a", n);
    
    if (memchr(buff, '.', nb) == NULL) {  
      char point = lua_getlocaledecpoint();  
      char *ppoint = (char *)memchr(buff, point, nb);
      if (ppoint) *ppoint = '.';  
    }
    return nb;
  }
  
  return l_sprintf(buff, MAX_ITEM, "%s", s);
}


static void addliteral (lua_State *L, luaL_Buffer *b, int arg) {
  switch (lua_type(L, arg)) {
    case LUA_TSTRING: {
      size_t len;
      const char *s = lua_tolstring(L, arg, &len);
      addquoted(b, s, len);
      break;
    }
    case LUA_TNUMBER: {
      char *buff = luaL_prepbuffsize(b, MAX_ITEM);
      int nb;
      if (!lua_isinteger(L, arg))  
        nb = quotefloat(L, buff, lua_tonumber(L, arg));
      else {  
        lua_Integer n = lua_tointeger(L, arg);
        const char *format = (n == LUA_MININTEGER)  
                           ? "0x%" LUA_INTEGER_FRMLEN "x"  
                           : LUA_INTEGER_FMT;  
        nb = l_sprintf(buff, MAX_ITEM, format, (LUAI_UACINT)n);
      }
      luaL_addsize(b, nb);
      break;
    }
    case LUA_TNIL: case LUA_TBOOLEAN: {
      luaL_tolstring(L, arg, NULL);
      luaL_addvalue(b);
      break;
    }
    default: {
      luaL_argerror(L, arg, "value has no literal form");
    }
  }
}


static const char *get2digits (const char *s) {
  if (isdigit(uchar(*s))) {
    s++;
    if (isdigit(uchar(*s))) s++;  
  }
  return s;
}



static void checkformat (lua_State *L, const char *form, const char *flags,
                                       int precision) {
  const char *spec = form + 1;  
  spec += strspn(spec, flags);  
  if (*spec != '0') {  
    spec = get2digits(spec);  
    if (*spec == '.' && precision) {
      spec++;
      spec = get2digits(spec);  
    }
  }
  if (!isalpha(uchar(*spec)))  
    luaL_error(L, "invalid conversion specification: '%s'", form);
}



static const char *getformat (lua_State *L, const char *strfrmt,
                                            char *form) {
  
  size_t len = strspn(strfrmt, L_FMTFLAGSF "123456789.");
  len++;  
  
  if (len >= MAX_FORMAT - 10)
    luaL_error(L, "invalid format (too long)");
  *(form++) = '%';
  memcpy(form, strfrmt, len * sizeof(char));
  *(form + len) = '\0';
  return strfrmt + len - 1;
}



static void addlenmod (char *form, const char *lenmod) {
  size_t l = strlen(form);
  size_t lm = strlen(lenmod);
  char spec = form[l - 1];
  strcpy(form + l - 1, lenmod);
  form[l + lm - 1] = spec;
  form[l + lm] = '\0';
}


static int str_format (lua_State *L) {
  int top = lua_gettop(L);
  int arg = 1;
  size_t sfl;
  const char *strfrmt = luaL_checklstring(L, arg, &sfl);
  const char *strfrmt_end = strfrmt+sfl;
  const char *flags;
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  while (strfrmt < strfrmt_end) {
    if (*strfrmt != L_ESC)
      luaL_addchar(&b, *strfrmt++);
    else if (*++strfrmt == L_ESC)
      luaL_addchar(&b, *strfrmt++);  
    else { 
      char form[MAX_FORMAT];  
      int maxitem = MAX_ITEM;  
      char *buff = luaL_prepbuffsize(&b, maxitem);  
      int nb = 0;  
      if (++arg > top)
        return luaL_argerror(L, arg, "no value");
      strfrmt = getformat(L, strfrmt, form);
      switch (*strfrmt++) {
        case 'c': {
          checkformat(L, form, L_FMTFLAGSC, 0);
          nb = l_sprintf(buff, maxitem, form, (int)luaL_checkinteger(L, arg));
          break;
        }
        case 'd': case 'i':
          flags = L_FMTFLAGSI;
          goto intcase;
        case 'u':
          flags = L_FMTFLAGSU;
          goto intcase;
        case 'o': case 'x': case 'X':
          flags = L_FMTFLAGSX;
         intcase: {
          lua_Integer n = luaL_checkinteger(L, arg);
          checkformat(L, form, flags, 1);
          addlenmod(form, LUA_INTEGER_FRMLEN);
          nb = l_sprintf(buff, maxitem, form, (LUAI_UACINT)n);
          break;
        }
        case 'a': case 'A':
          checkformat(L, form, L_FMTFLAGSF, 1);
          addlenmod(form, LUA_NUMBER_FRMLEN);
          nb = lua_number2strx(L, buff, maxitem, form,
                                  luaL_checknumber(L, arg));
          break;
        case 'f':
          maxitem = MAX_ITEMF;  
          buff = luaL_prepbuffsize(&b, maxitem);
          
        case 'e': case 'E': case 'g': case 'G': {
          lua_Number n = luaL_checknumber(L, arg);
          checkformat(L, form, L_FMTFLAGSF, 1);
          addlenmod(form, LUA_NUMBER_FRMLEN);
          nb = l_sprintf(buff, maxitem, form, (LUAI_UACNUMBER)n);
          break;
        }
        case 'p': {
          const void *p = lua_topointer(L, arg);
          checkformat(L, form, L_FMTFLAGSC, 0);
          if (p == NULL) {  
            p = "(null)";  
            form[strlen(form) - 1] = 's';  
          }
          nb = l_sprintf(buff, maxitem, form, p);
          break;
        }
        case 'q': {
          if (form[2] != '\0')  
            return luaL_error(L, "specifier '%%q' cannot have modifiers");
          addliteral(L, &b, arg);
          break;
        }
        case 's': {
          size_t l;
          const char *s = luaL_tolstring(L, arg, &l);
          if (form[2] == '\0')  
            luaL_addvalue(&b);  
          else {
            luaL_argcheck(L, l == strlen(s), arg, "string contains zeros");
            checkformat(L, form, L_FMTFLAGSC, 1);
            if (strchr(form, '.') == NULL && l >= 100) {
              
              luaL_addvalue(&b);  
            }
            else {  
              nb = l_sprintf(buff, maxitem, form, s);
              lua_pop(L, 1);  
            }
          }
          break;
        }
        default: {  
          return luaL_error(L, "invalid conversion '%s' to 'format'", form);
        }
      }
      lua_assert(nb < maxitem);
      luaL_addsize(&b, nb);
    }
  }
  luaL_pushresult(&b);
  return 1;
}








#if !defined(LUAL_PACKPADBYTE)
#define LUAL_PACKPADBYTE		0x00
#endif


#define MAXINTSIZE	16


#define NB	CHAR_BIT


#define MC	((1 << NB) - 1)


#define SZINT	((int)sizeof(lua_Integer))



static const union {
  int dummy;
  char little;  
} nativeendian = {1};



typedef struct Header {
  lua_State *L;
  int islittle;
  int maxalign;
} Header;



typedef enum KOption {
  Kint,		
  Kuint,	
  Kfloat,	
  Knumber,	
  Kdouble,	
  Kchar,	
  Kstring,	
  Kzstr,	
  Kpadding,	
  Kpaddalign,	
  Knop		
} KOption;



static int digit (int c) { return '0' <= c && c <= '9'; }

static int getnum (const char **fmt, int df) {
  if (!digit(**fmt))  
    return df;  
  else {
    int a = 0;
    do {
      a = a*10 + (*((*fmt)++) - '0');
    } while (digit(**fmt) && a <= ((int)MAXSIZE - 9)/10);
    return a;
  }
}



static int getnumlimit (Header *h, const char **fmt, int df) {
  int sz = getnum(fmt, df);
  if (l_unlikely(sz > MAXINTSIZE || sz <= 0))
    return luaL_error(h->L, "integral size (%d) out of limits [1,%d]",
                            sz, MAXINTSIZE);
  return sz;
}



static void initheader (lua_State *L, Header *h) {
  h->L = L;
  h->islittle = nativeendian.little;
  h->maxalign = 1;
}



static KOption getoption (Header *h, const char **fmt, int *size) {
  
  struct cD { char c; union { LUAI_MAXALIGN; } u; };
  int opt = *((*fmt)++);
  *size = 0;  
  switch (opt) {
    case 'b': *size = sizeof(char); return Kint;
    case 'B': *size = sizeof(char); return Kuint;
    case 'h': *size = sizeof(short); return Kint;
    case 'H': *size = sizeof(short); return Kuint;
    case 'l': *size = sizeof(long); return Kint;
    case 'L': *size = sizeof(long); return Kuint;
    case 'j': *size = sizeof(lua_Integer); return Kint;
    case 'J': *size = sizeof(lua_Integer); return Kuint;
    case 'T': *size = sizeof(size_t); return Kuint;
    case 'f': *size = sizeof(float); return Kfloat;
    case 'n': *size = sizeof(lua_Number); return Knumber;
    case 'd': *size = sizeof(double); return Kdouble;
    case 'i': *size = getnumlimit(h, fmt, sizeof(int)); return Kint;
    case 'I': *size = getnumlimit(h, fmt, sizeof(int)); return Kuint;
    case 's': *size = getnumlimit(h, fmt, sizeof(size_t)); return Kstring;
    case 'c':
      *size = getnum(fmt, -1);
      if (l_unlikely(*size == -1))
        luaL_error(h->L, "missing size for format option 'c'");
      return Kchar;
    case 'z': return Kzstr;
    case 'x': *size = 1; return Kpadding;
    case 'X': return Kpaddalign;
    case ' ': break;
    case '<': h->islittle = 1; break;
    case '>': h->islittle = 0; break;
    case '=': h->islittle = nativeendian.little; break;
    case '!': {
      const int maxalign = offsetof(struct cD, u);
      h->maxalign = getnumlimit(h, fmt, maxalign);
      break;
    }
    default: luaL_error(h->L, "invalid format option '%c'", opt);
  }
  return Knop;
}



static KOption getdetails (Header *h, size_t totalsize,
                           const char **fmt, int *psize, int *ntoalign) {
  KOption opt = getoption(h, fmt, psize);
  int align = *psize;  
  if (opt == Kpaddalign) {  
    if (**fmt == '\0' || getoption(h, fmt, &align) == Kchar || align == 0)
      luaL_argerror(h->L, 1, "invalid next option for option 'X'");
  }
  if (align <= 1 || opt == Kchar)  
    *ntoalign = 0;
  else {
    if (align > h->maxalign)  
      align = h->maxalign;
    if (l_unlikely((align & (align - 1)) != 0))  
      luaL_argerror(h->L, 1, "format asks for alignment not power of 2");
    *ntoalign = (align - (int)(totalsize & (align - 1))) & (align - 1);
  }
  return opt;
}



static void packint (luaL_Buffer *b, lua_Unsigned n,
                     int islittle, int size, int neg) {
  char *buff = luaL_prepbuffsize(b, size);
  int i;
  buff[islittle ? 0 : size - 1] = (char)(n & MC);  
  for (i = 1; i < size; i++) {
    n >>= NB;
    buff[islittle ? i : size - 1 - i] = (char)(n & MC);
  }
  if (neg && size > SZINT) {  
    for (i = SZINT; i < size; i++)  
      buff[islittle ? i : size - 1 - i] = (char)MC;
  }
  luaL_addsize(b, size);  
}



static void copywithendian (char *dest, const char *src,
                            int size, int islittle) {
  if (islittle == nativeendian.little)
    memcpy(dest, src, size);
  else {
    dest += size - 1;
    while (size-- != 0)
      *(dest--) = *(src++);
  }
}


static int str_pack (lua_State *L) {
  luaL_Buffer b;
  Header h;
  const char *fmt = luaL_checkstring(L, 1);  
  int arg = 1;  
  size_t totalsize = 0;  
  initheader(L, &h);
  lua_pushnil(L);  
  luaL_buffinit(L, &b);
  while (*fmt != '\0') {
    int size, ntoalign;
    KOption opt = getdetails(&h, totalsize, &fmt, &size, &ntoalign);
    totalsize += ntoalign + size;
    while (ntoalign-- > 0)
     luaL_addchar(&b, LUAL_PACKPADBYTE);  
    arg++;
    switch (opt) {
      case Kint: {  
        lua_Integer n = luaL_checkinteger(L, arg);
        if (size < SZINT) {  
          lua_Integer lim = (lua_Integer)1 << ((size * NB) - 1);
          luaL_argcheck(L, -lim <= n && n < lim, arg, "integer overflow");
        }
        packint(&b, (lua_Unsigned)n, h.islittle, size, (n < 0));
        break;
      }
      case Kuint: {  
        lua_Integer n = luaL_checkinteger(L, arg);
        if (size < SZINT)  
          luaL_argcheck(L, (lua_Unsigned)n < ((lua_Unsigned)1 << (size * NB)),
                           arg, "unsigned overflow");
        packint(&b, (lua_Unsigned)n, h.islittle, size, 0);
        break;
      }
      case Kfloat: {  
        float f = (float)luaL_checknumber(L, arg);  
        char *buff = luaL_prepbuffsize(&b, sizeof(f));
        
        copywithendian(buff, (char *)&f, sizeof(f), h.islittle);
        luaL_addsize(&b, size);
        break;
      }
      case Knumber: {  
        lua_Number f = luaL_checknumber(L, arg);  
        char *buff = luaL_prepbuffsize(&b, sizeof(f));
        
        copywithendian(buff, (char *)&f, sizeof(f), h.islittle);
        luaL_addsize(&b, size);
        break;
      }
      case Kdouble: {  
        double f = (double)luaL_checknumber(L, arg);  
        char *buff = luaL_prepbuffsize(&b, sizeof(f));
        
        copywithendian(buff, (char *)&f, sizeof(f), h.islittle);
        luaL_addsize(&b, size);
        break;
      }
      case Kchar: {  
        size_t len;
        const char *s = luaL_checklstring(L, arg, &len);
        luaL_argcheck(L, len <= (size_t)size, arg,
                         "string longer than given size");
        luaL_addlstring(&b, s, len);  
        while (len++ < (size_t)size)  
          luaL_addchar(&b, LUAL_PACKPADBYTE);
        break;
      }
      case Kstring: {  
        size_t len;
        const char *s = luaL_checklstring(L, arg, &len);
        luaL_argcheck(L, size >= (int)sizeof(size_t) ||
                         len < ((size_t)1 << (size * NB)),
                         arg, "string length does not fit in given size");
        packint(&b, (lua_Unsigned)len, h.islittle, size, 0);  
        luaL_addlstring(&b, s, len);
        totalsize += len;
        break;
      }
      case Kzstr: {  
        size_t len;
        const char *s = luaL_checklstring(L, arg, &len);
        luaL_argcheck(L, strlen(s) == len, arg, "string contains zeros");
        luaL_addlstring(&b, s, len);
        luaL_addchar(&b, '\0');  
        totalsize += len + 1;
        break;
      }
      case Kpadding: luaL_addchar(&b, LUAL_PACKPADBYTE);  
      case Kpaddalign: case Knop:
        arg--;  
        break;
    }
  }
  luaL_pushresult(&b);
  return 1;
}


static int str_packsize (lua_State *L) {
  Header h;
  const char *fmt = luaL_checkstring(L, 1);  
  size_t totalsize = 0;  
  initheader(L, &h);
  while (*fmt != '\0') {
    int size, ntoalign;
    KOption opt = getdetails(&h, totalsize, &fmt, &size, &ntoalign);
    luaL_argcheck(L, opt != Kstring && opt != Kzstr, 1,
                     "variable-length format");
    size += ntoalign;  
    luaL_argcheck(L, totalsize <= MAXSIZE - size, 1,
                     "format result too large");
    totalsize += size;
  }
  lua_pushinteger(L, (lua_Integer)totalsize);
  return 1;
}



static lua_Integer unpackint (lua_State *L, const char *str,
                              int islittle, int size, int issigned) {
  lua_Unsigned res = 0;
  int i;
  int limit = (size  <= SZINT) ? size : SZINT;
  for (i = limit - 1; i >= 0; i--) {
    res <<= NB;
    res |= (lua_Unsigned)(unsigned char)str[islittle ? i : size - 1 - i];
  }
  if (size < SZINT) {  
    if (issigned) {  
      lua_Unsigned mask = (lua_Unsigned)1 << (size*NB - 1);
      res = ((res ^ mask) - mask);  
    }
  }
  else if (size > SZINT) {  
    int mask = (!issigned || (lua_Integer)res >= 0) ? 0 : MC;
    for (i = limit; i < size; i++) {
      if (l_unlikely((unsigned char)str[islittle ? i : size - 1 - i] != mask))
        luaL_error(L, "%d-byte integer does not fit into Lua Integer", size);
    }
  }
  return (lua_Integer)res;
}


static int str_unpack (lua_State *L) {
  Header h;
  const char *fmt = luaL_checkstring(L, 1);
  size_t ld;
  const char *data = luaL_checklstring(L, 2, &ld);
  size_t pos = posrelatI(luaL_optinteger(L, 3, 1), ld) - 1;
  int n = 0;  
  luaL_argcheck(L, pos <= ld, 3, "initial position out of string");
  initheader(L, &h);
  while (*fmt != '\0') {
    int size, ntoalign;
    KOption opt = getdetails(&h, pos, &fmt, &size, &ntoalign);
    luaL_argcheck(L, (size_t)ntoalign + size <= ld - pos, 2,
                    "data string too short");
    pos += ntoalign;  
    
    luaL_checkstack(L, 2, "too many results");
    n++;
    switch (opt) {
      case Kint:
      case Kuint: {
        lua_Integer res = unpackint(L, data + pos, h.islittle, size,
                                       (opt == Kint));
        lua_pushinteger(L, res);
        break;
      }
      case Kfloat: {
        float f;
        copywithendian((char *)&f, data + pos, sizeof(f), h.islittle);
        lua_pushnumber(L, (lua_Number)f);
        break;
      }
      case Knumber: {
        lua_Number f;
        copywithendian((char *)&f, data + pos, sizeof(f), h.islittle);
        lua_pushnumber(L, f);
        break;
      }
      case Kdouble: {
        double f;
        copywithendian((char *)&f, data + pos, sizeof(f), h.islittle);
        lua_pushnumber(L, (lua_Number)f);
        break;
      }
      case Kchar: {
        lua_pushlstring(L, data + pos, size);
        break;
      }
      case Kstring: {
        size_t len = (size_t)unpackint(L, data + pos, h.islittle, size, 0);
        luaL_argcheck(L, len <= ld - pos - size, 2, "data string too short");
        lua_pushlstring(L, data + pos + size, len);
        pos += len;  
        break;
      }
      case Kzstr: {
        size_t len = strlen(data + pos);
        luaL_argcheck(L, pos + len < ld, 2,
                         "unfinished string for format 'z'");
        lua_pushlstring(L, data + pos, len);
        pos += len + 1;  
        break;
      }
      case Kpaddalign: case Kpadding: case Knop:
        n--;  
        break;
    }
    pos += size;
  }
  lua_pushinteger(L, pos + 1);  
  return n + 1;
}




static const luaL_Reg strlib[] = {
  {"byte", str_byte},
  {"char", str_char},
  {"dump", str_dump},
  {"find", str_find},
  {"format", str_format},
  {"gmatch", gmatch},
  {"gsub", str_gsub},
  {"len", str_len},
  {"lower", str_lower},
  {"match", str_match},
  {"rep", str_rep},
  {"reverse", str_reverse},
  {"sub", str_sub},
  {"upper", str_upper},
  {"pack", str_pack},
  {"packsize", str_packsize},
  {"unpack", str_unpack},
  {NULL, NULL}
};


static void createmetatable (lua_State *L) {
  
  luaL_newlibtable(L, stringmetamethods);
  luaL_setfuncs(L, stringmetamethods, 0);
  lua_pushliteral(L, "");  
  lua_pushvalue(L, -2);  
  lua_setmetatable(L, -2);  
  lua_pop(L, 1);  
  lua_pushvalue(L, -2);  
  lua_setfield(L, -2, "__index");  
  lua_pop(L, 1);  
}



LUAMOD_API int luaopen_string (lua_State *L) {
  luaL_newlib(L, strlib);
  createmetatable(L);
  return 1;
}



//path: src/imports/../dependencies/lua/ltablib.c


#define ltablib_c
#define LUA_LIB





#include <limits.h>
#include <stddef.h>
#include <string.h>











#define TAB_R	1			
#define TAB_W	2			
#define TAB_L	4			
#define TAB_RW	(TAB_R | TAB_W)		


#define aux_getn(L,n,w)	(checktab(L, n, (w) | TAB_L), luaL_len(L, n))


static int checkfield (lua_State *L, const char *key, int n) {
  lua_pushstring(L, key);
  return (lua_rawget(L, -n) != LUA_TNIL);
}



static void checktab (lua_State *L, int arg, int what) {
  if (lua_type(L, arg) != LUA_TTABLE) {  
    int n = 1;  
    if (lua_getmetatable(L, arg) &&  
        (!(what & TAB_R) || checkfield(L, "__index", ++n)) &&
        (!(what & TAB_W) || checkfield(L, "__newindex", ++n)) &&
        (!(what & TAB_L) || checkfield(L, "__len", ++n))) {
      lua_pop(L, n);  
    }
    else
      luaL_checktype(L, arg, LUA_TTABLE);  
  }
}


static int tcreate (lua_State *L) {
  lua_Unsigned sizeseq = (lua_Unsigned)luaL_checkinteger(L, 1);
  lua_Unsigned sizerest = (lua_Unsigned)luaL_optinteger(L, 2, 0);
  luaL_argcheck(L, sizeseq <= UINT_MAX, 1, "out of range");
  luaL_argcheck(L, sizerest <= UINT_MAX, 2, "out of range");
  lua_createtable(L, (unsigned)sizeseq, (unsigned)sizerest);
  return 1;
}


static int tinsert (lua_State *L) {
  lua_Integer pos;  
  lua_Integer e = aux_getn(L, 1, TAB_RW);
  e = luaL_intop(+, e, 1);  
  switch (lua_gettop(L)) {
    case 2: {  
      pos = e;  
      break;
    }
    case 3: {
      lua_Integer i;
      pos = luaL_checkinteger(L, 2);  
      
      luaL_argcheck(L, (lua_Unsigned)pos - 1u < (lua_Unsigned)e, 2,
                       "position out of bounds");
      for (i = e; i > pos; i--) {  
        lua_geti(L, 1, i - 1);
        lua_seti(L, 1, i);  
      }
      break;
    }
    default: {
      return luaL_error(L, "wrong number of arguments to 'insert'");
    }
  }
  lua_seti(L, 1, pos);  
  return 0;
}


static int tremove (lua_State *L) {
  lua_Integer size = aux_getn(L, 1, TAB_RW);
  lua_Integer pos = luaL_optinteger(L, 2, size);
  if (pos != size)  
    
    luaL_argcheck(L, (lua_Unsigned)pos - 1u <= (lua_Unsigned)size, 2,
                     "position out of bounds");
  lua_geti(L, 1, pos);  
  for ( ; pos < size; pos++) {
    lua_geti(L, 1, pos + 1);
    lua_seti(L, 1, pos);  
  }
  lua_pushnil(L);
  lua_seti(L, 1, pos);  
  return 1;
}



static int tmove (lua_State *L) {
  lua_Integer f = luaL_checkinteger(L, 2);
  lua_Integer e = luaL_checkinteger(L, 3);
  lua_Integer t = luaL_checkinteger(L, 4);
  int tt = !lua_isnoneornil(L, 5) ? 5 : 1;  
  checktab(L, 1, TAB_R);
  checktab(L, tt, TAB_W);
  if (e >= f) {  
    lua_Integer n, i;
    luaL_argcheck(L, f > 0 || e < LUA_MAXINTEGER + f, 3,
                  "too many elements to move");
    n = e - f + 1;  
    luaL_argcheck(L, t <= LUA_MAXINTEGER - n + 1, 4,
                  "destination wrap around");
    if (t > e || t <= f || (tt != 1 && !lua_compare(L, 1, tt, LUA_OPEQ))) {
      for (i = 0; i < n; i++) {
        lua_geti(L, 1, f + i);
        lua_seti(L, tt, t + i);
      }
    }
    else {
      for (i = n - 1; i >= 0; i--) {
        lua_geti(L, 1, f + i);
        lua_seti(L, tt, t + i);
      }
    }
  }
  lua_pushvalue(L, tt);  
  return 1;
}


static void addfield (lua_State *L, luaL_Buffer *b, lua_Integer i) {
  lua_geti(L, 1, i);
  if (l_unlikely(!lua_isstring(L, -1)))
    luaL_error(L, "invalid value (%s) at index %I in table for 'concat'",
                  luaL_typename(L, -1), (LUAI_UACINT)i);
  luaL_addvalue(b);
}


static int tconcat (lua_State *L) {
  luaL_Buffer b;
  lua_Integer last = aux_getn(L, 1, TAB_R);
  size_t lsep;
  const char *sep = luaL_optlstring(L, 2, "", &lsep);
  lua_Integer i = luaL_optinteger(L, 3, 1);
  last = luaL_optinteger(L, 4, last);
  luaL_buffinit(L, &b);
  for (; i < last; i++) {
    addfield(L, &b, i);
    luaL_addlstring(&b, sep, lsep);
  }
  if (i == last)  
    addfield(L, &b, i);
  luaL_pushresult(&b);
  return 1;
}




static int tpack (lua_State *L) {
  int i;
  int n = lua_gettop(L);  
  lua_createtable(L, n, 1);  
  lua_insert(L, 1);  
  for (i = n; i >= 1; i--)  
    lua_seti(L, 1, i);
  lua_pushinteger(L, n);
  lua_setfield(L, 1, "n");  
  return 1;  
}


static int tunpack (lua_State *L) {
  lua_Unsigned n;
  lua_Integer i = luaL_optinteger(L, 2, 1);
  lua_Integer e = luaL_opt(L, luaL_checkinteger, 3, luaL_len(L, 1));
  if (i > e) return 0;  
  n = (lua_Unsigned)e - i;  
  if (l_unlikely(n >= (unsigned int)INT_MAX  ||
                 !lua_checkstack(L, (int)(++n))))
    return luaL_error(L, "too many results to unpack");
  for (; i < e; i++) {  
    lua_geti(L, 1, i);
  }
  lua_geti(L, 1, e);  
  return (int)n;
}









typedef unsigned int IdxT;



#if !defined(l_randomizePivot)
#define l_randomizePivot(L)	luaL_makeseed(L)
#endif					



#define RANLIMIT	100u


static void set2 (lua_State *L, IdxT i, IdxT j) {
  lua_seti(L, 1, i);
  lua_seti(L, 1, j);
}



static int sort_comp (lua_State *L, int a, int b) {
  if (lua_isnil(L, 2))  
    return lua_compare(L, a, b, LUA_OPLT);  
  else {  
    int res;
    lua_pushvalue(L, 2);    
    lua_pushvalue(L, a-1);  
    lua_pushvalue(L, b-2);  
    lua_call(L, 2, 1);      
    res = lua_toboolean(L, -1);  
    lua_pop(L, 1);          
    return res;
  }
}



static IdxT partition (lua_State *L, IdxT lo, IdxT up) {
  IdxT i = lo;  
  IdxT j = up - 1;  
  
  for (;;) {
    
    while ((void)lua_geti(L, 1, ++i), sort_comp(L, -1, -2)) {
      if (l_unlikely(i == up - 1))  
        luaL_error(L, "invalid order function for sorting");
      lua_pop(L, 1);  
    }
    
    
    while ((void)lua_geti(L, 1, --j), sort_comp(L, -3, -1)) {
      if (l_unlikely(j < i))  
        luaL_error(L, "invalid order function for sorting");
      lua_pop(L, 1);  
    }
    
    if (j < i) {  
      
      lua_pop(L, 1);  
      
      set2(L, up - 1, i);
      return i;
    }
    
    set2(L, i, j);
  }
}



static IdxT choosePivot (IdxT lo, IdxT up, unsigned int rnd) {
  IdxT r4 = (up - lo) / 4;  
  IdxT p = (rnd ^ lo ^ up) % (r4 * 2) + (lo + r4);
  lua_assert(lo + r4 <= p && p <= up - r4);
  return p;
}



static void auxsort (lua_State *L, IdxT lo, IdxT up, unsigned rnd) {
  while (lo < up) {  
    IdxT p;  
    IdxT n;  
    
    lua_geti(L, 1, lo);
    lua_geti(L, 1, up);
    if (sort_comp(L, -1, -2))  
      set2(L, lo, up);  
    else
      lua_pop(L, 2);  
    if (up - lo == 1)  
      return;  
    if (up - lo < RANLIMIT || rnd == 0)  
      p = (lo + up)/2;  
    else  
      p = choosePivot(lo, up, rnd);
    lua_geti(L, 1, p);
    lua_geti(L, 1, lo);
    if (sort_comp(L, -2, -1))  
      set2(L, p, lo);  
    else {
      lua_pop(L, 1);  
      lua_geti(L, 1, up);
      if (sort_comp(L, -1, -2))  
        set2(L, p, up);  
      else
        lua_pop(L, 2);
    }
    if (up - lo == 2)  
      return;  
    lua_geti(L, 1, p);  
    lua_pushvalue(L, -1);  
    lua_geti(L, 1, up - 1);  
    set2(L, p, up - 1);  
    p = partition(L, lo, up);
    
    if (p - lo < up - p) {  
      auxsort(L, lo, p - 1, rnd);  
      n = p - lo;  
      lo = p + 1;  
    }
    else {
      auxsort(L, p + 1, up, rnd);  
      n = up - p;  
      up = p - 1;  
    }
    if ((up - lo) / 128 > n) 
      rnd = l_randomizePivot(L);  
  }  
}


static int sort (lua_State *L) {
  lua_Integer n = aux_getn(L, 1, TAB_RW);
  if (n > 1) {  
    luaL_argcheck(L, n < INT_MAX, 1, "array too big");
    if (!lua_isnoneornil(L, 2))  
      luaL_checktype(L, 2, LUA_TFUNCTION);  
    lua_settop(L, 2);  
    auxsort(L, 1, (IdxT)n, 0);
  }
  return 0;
}




static const luaL_Reg tab_funcs[] = {
  {"concat", tconcat},
  {"create", tcreate},
  {"insert", tinsert},
  {"pack", tpack},
  {"unpack", tunpack},
  {"remove", tremove},
  {"move", tmove},
  {"sort", sort},
  {NULL, NULL}
};


LUAMOD_API int luaopen_table (lua_State *L) {
  luaL_newlib(L, tab_funcs);
  return 1;
}



//path: src/imports/../dependencies/lua/lutf8lib.c


#define lutf8lib_c
#define LUA_LIB





#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>










#define MAXUNICODE	0x10FFFFu

#define MAXUTF		0x7FFFFFFFu


#define MSGInvalid	"invalid UTF-8 code"


#if (UINT_MAX >> 30) >= 1
typedef unsigned int utfint;
#else
typedef unsigned long utfint;
#endif


#define iscont(c)	(((c) & 0xC0) == 0x80)
#define iscontp(p)	iscont(*(p))




static lua_Integer u_posrelat (lua_Integer pos, size_t len) {
  if (pos >= 0) return pos;
  else if (0u - (size_t)pos > len) return 0;
  else return (lua_Integer)len + pos + 1;
}



static const char *utf8_decode (const char *s, utfint *val, int strict) {
  static const utfint limits[] =
        {~(utfint)0, 0x80, 0x800, 0x10000u, 0x200000u, 0x4000000u};
  unsigned int c = (unsigned char)s[0];
  utfint res = 0;  
  if (c < 0x80)  
    res = c;
  else {
    int count = 0;  
    for (; c & 0x40; c <<= 1) {  
      unsigned int cc = (unsigned char)s[++count];  
      if (!iscont(cc))  
        return NULL;  
      res = (res << 6) | (cc & 0x3F);  
    }
    res |= ((utfint)(c & 0x7F) << (count * 5));  
    if (count > 5 || res > MAXUTF || res < limits[count])
      return NULL;  
    s += count;  
  }
  if (strict) {
    
    if (res > MAXUNICODE || (0xD800u <= res && res <= 0xDFFFu))
      return NULL;
  }
  if (val) *val = res;
  return s + 1;  
}



static int utflen (lua_State *L) {
  lua_Integer n = 0;  
  size_t len;  
  const char *s = luaL_checklstring(L, 1, &len);
  lua_Integer posi = u_posrelat(luaL_optinteger(L, 2, 1), len);
  lua_Integer posj = u_posrelat(luaL_optinteger(L, 3, -1), len);
  int lax = lua_toboolean(L, 4);
  luaL_argcheck(L, 1 <= posi && --posi <= (lua_Integer)len, 2,
                   "initial position out of bounds");
  luaL_argcheck(L, --posj < (lua_Integer)len, 3,
                   "final position out of bounds");
  while (posi <= posj) {
    const char *s1 = utf8_decode(s + posi, NULL, !lax);
    if (s1 == NULL) {  
      luaL_pushfail(L);  
      lua_pushinteger(L, posi + 1);  
      return 2;
    }
    posi = s1 - s;
    n++;
  }
  lua_pushinteger(L, n);
  return 1;
}



static int codepoint (lua_State *L) {
  size_t len;
  const char *s = luaL_checklstring(L, 1, &len);
  lua_Integer posi = u_posrelat(luaL_optinteger(L, 2, 1), len);
  lua_Integer pose = u_posrelat(luaL_optinteger(L, 3, posi), len);
  int lax = lua_toboolean(L, 4);
  int n;
  const char *se;
  luaL_argcheck(L, posi >= 1, 2, "out of bounds");
  luaL_argcheck(L, pose <= (lua_Integer)len, 3, "out of bounds");
  if (posi > pose) return 0;  
  if (pose - posi >= INT_MAX)  
    return luaL_error(L, "string slice too long");
  n = (int)(pose -  posi) + 1;  
  luaL_checkstack(L, n, "string slice too long");
  n = 0;  
  se = s + pose;  
  for (s += posi - 1; s < se;) {
    utfint code;
    s = utf8_decode(s, &code, !lax);
    if (s == NULL)
      return luaL_error(L, MSGInvalid);
    lua_pushinteger(L, code);
    n++;
  }
  return n;
}


static void pushutfchar (lua_State *L, int arg) {
  lua_Unsigned code = (lua_Unsigned)luaL_checkinteger(L, arg);
  luaL_argcheck(L, code <= MAXUTF, arg, "value out of range");
  lua_pushfstring(L, "%U", (long)code);
}



static int utfchar (lua_State *L) {
  int n = lua_gettop(L);  
  if (n == 1)  
    pushutfchar(L, 1);
  else {
    int i;
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    for (i = 1; i <= n; i++) {
      pushutfchar(L, i);
      luaL_addvalue(&b);
    }
    luaL_pushresult(&b);
  }
  return 1;
}



static int byteoffset (lua_State *L) {
  size_t len;
  const char *s = luaL_checklstring(L, 1, &len);
  lua_Integer n  = luaL_checkinteger(L, 2);
  lua_Integer posi = (n >= 0) ? 1 : len + 1;
  posi = u_posrelat(luaL_optinteger(L, 3, posi), len);
  luaL_argcheck(L, 1 <= posi && --posi <= (lua_Integer)len, 3,
                   "position out of bounds");
  if (n == 0) {
    
    while (posi > 0 && iscontp(s + posi)) posi--;
  }
  else {
    if (iscontp(s + posi))
      return luaL_error(L, "initial position is a continuation byte");
    if (n < 0) {
       while (n < 0 && posi > 0) {  
         do {  
           posi--;
         } while (posi > 0 && iscontp(s + posi));
         n++;
       }
     }
     else {
       n--;  
       while (n > 0 && posi < (lua_Integer)len) {
         do {  
           posi++;
         } while (iscontp(s + posi));  
         n--;
       }
     }
  }
  if (n == 0)  
    lua_pushinteger(L, posi + 1);
  else  
    luaL_pushfail(L);
  return 1;
}


static int iter_aux (lua_State *L, int strict) {
  size_t len;
  const char *s = luaL_checklstring(L, 1, &len);
  lua_Unsigned n = (lua_Unsigned)lua_tointeger(L, 2);
  if (n < len) {
    while (iscontp(s + n)) n++;  
  }
  if (n >= len)  
    return 0;  
  else {
    utfint code;
    const char *next = utf8_decode(s + n, &code, strict);
    if (next == NULL || iscontp(next))
      return luaL_error(L, MSGInvalid);
    lua_pushinteger(L, n + 1);
    lua_pushinteger(L, code);
    return 2;
  }
}


static int iter_auxstrict (lua_State *L) {
  return iter_aux(L, 1);
}

static int iter_auxlax (lua_State *L) {
  return iter_aux(L, 0);
}


static int iter_codes (lua_State *L) {
  int lax = lua_toboolean(L, 2);
  const char *s = luaL_checkstring(L, 1);
  luaL_argcheck(L, !iscontp(s), 1, MSGInvalid);
  lua_pushcfunction(L, lax ? iter_auxlax : iter_auxstrict);
  lua_pushvalue(L, 1);
  lua_pushinteger(L, 0);
  return 3;
}



#define UTF8PATT	"[\0-\x7F\xC2-\xFD][\x80-\xBF]*"


static const luaL_Reg funcs[] = {
  {"offset", byteoffset},
  {"codepoint", codepoint},
  {"char", utfchar},
  {"len", utflen},
  {"codes", iter_codes},
  
  {"charpattern", NULL},
  {NULL, NULL}
};


LUAMOD_API int luaopen_utf8 (lua_State *L) {
  luaL_newlib(L, funcs);
  lua_pushlstring(L, UTF8PATT, sizeof(UTF8PATT)/sizeof(char) - 1);
  lua_setfield(L, -2, "charpattern");
  return 1;
}



//path: src/imports/../dependencies/lua/linit.c



#define linit_c
#define LUA_LIB






#include <stddef.h>











static const luaL_Reg stdlibs[] = {
  {LUA_GNAME, luaopen_base},
  {LUA_LOADLIBNAME, luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_DBLIBNAME, luaopen_debug},
  {LUA_IOLIBNAME, luaopen_io},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_TABLIBNAME, luaopen_table},
  {LUA_UTF8LIBNAME, luaopen_utf8},
  {NULL, NULL}
};



LUALIB_API void luaL_openselectedlibs (lua_State *L, int load, int preload) {
  int mask;
  const luaL_Reg *lib;
  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
  for (lib = stdlibs, mask = 1; lib->name != NULL; lib++, mask <<= 1) {
    if (load & mask) {  
      luaL_requiref(L, lib->name, lib->func, 1);  
      lua_pop(L, 1);  
    }
    else if (preload & mask) {  
      lua_pushcfunction(L, lib->func);
      lua_setfield(L, -2, lib->name);  
    }
  }
  lua_assert((mask >> 1) == LUA_UTF8LIBK);
  lua_pop(L, 1);  
}



#endif


#ifdef MAKE_LUAC
//path: src/imports/../dependencies/lua/luac.c



#define luac_c
#define LUA_CORE




#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>












//path: src/imports/../dependencies/lua/lopnames.h


#if !defined(lopnames_h)
#define lopnames_h

#include <stddef.h>




static const char *const opnames[] = {
  "MOVE",
  "LOADI",
  "LOADF",
  "LOADK",
  "LOADKX",
  "LOADFALSE",
  "LFALSESKIP",
  "LOADTRUE",
  "LOADNIL",
  "GETUPVAL",
  "SETUPVAL",
  "GETTABUP",
  "GETTABLE",
  "GETI",
  "GETFIELD",
  "SETTABUP",
  "SETTABLE",
  "SETI",
  "SETFIELD",
  "NEWTABLE",
  "SELF",
  "ADDI",
  "ADDK",
  "SUBK",
  "MULK",
  "MODK",
  "POWK",
  "DIVK",
  "IDIVK",
  "BANDK",
  "BORK",
  "BXORK",
  "SHRI",
  "SHLI",
  "ADD",
  "SUB",
  "MUL",
  "MOD",
  "POW",
  "DIV",
  "IDIV",
  "BAND",
  "BOR",
  "BXOR",
  "SHL",
  "SHR",
  "MMBIN",
  "MMBINI",
  "MMBINK",
  "UNM",
  "BNOT",
  "NOT",
  "LEN",
  "CONCAT",
  "CLOSE",
  "TBC",
  "JMP",
  "EQ",
  "LT",
  "LE",
  "EQK",
  "EQI",
  "LTI",
  "LEI",
  "GTI",
  "GEI",
  "TEST",
  "TESTSET",
  "CALL",
  "TAILCALL",
  "RETURN",
  "RETURN0",
  "RETURN1",
  "FORLOOP",
  "FORPREP",
  "TFORPREP",
  "TFORCALL",
  "TFORLOOP",
  "SETLIST",
  "CLOSURE",
  "VARARG",
  "VARARGPREP",
  "EXTRAARG",
  NULL
};

#endif








static void PrintFunction(const Proto* f, int full);
#define luaU_print      PrintFunction

#define PROGNAME        "luac"          
#define OUTPUT          PROGNAME ".out" 

static int listing=0;                   
static int dumping=1;                   
static int stripping=0;                 
static char Output[]={ OUTPUT };        
static const char* output=Output;       
static const char* progname=PROGNAME;   
static TString **tmname;

static void fatal(const char* message)
{
 fprintf(stderr,"%s: %s\n",progname,message);
 exit(EXIT_FAILURE);
}

static void cannot(const char* what)
{
 fprintf(stderr,"%s: cannot %s %s: %s\n",progname,what,output,strerror(errno));
 exit(EXIT_FAILURE);
}

static void usage(const char* message)
{
 if (*message=='-')
  fprintf(stderr,"%s: unrecognized option '%s'\n",progname,message);
 else
  fprintf(stderr,"%s: %s\n",progname,message);
 fprintf(stderr,
  "usage: %s [options] [filenames]\n"
  "Available options are:\n"
  "  -l       list (use -l -l for full listing)\n"
  "  -o name  output to file 'name' (default is \"%s\")\n"
  "  -p       parse only\n"
  "  -s       strip debug information\n"
  "  -v       show version information\n"
  "  --       stop handling options\n"
  "  -        stop handling options and process stdin\n"
  ,progname,Output);
 exit(EXIT_FAILURE);
}

#define IS(s)   (strcmp(argv[i],s)==0)

static int doargs(int argc, char* argv[])
{
 int i;
 int version=0;
 if (argv[0]!=NULL && *argv[0]!=0) progname=argv[0];
 for (i=1; i<argc; i++)
 {
  if (*argv[i]!='-')                    
   break;
  else if (IS("--"))                    
  {
   ++i;
   if (version) ++version;
   break;
  }
  else if (IS("-"))                     
   break;
  else if (IS("-l"))                    
   ++listing;
  else if (IS("-o"))                    
  {
   output=argv[++i];
   if (output==NULL || *output==0 || (*output=='-' && output[1]!=0))
    usage("'-o' needs argument");
   if (IS("-")) output=NULL;
  }
  else if (IS("-p"))                    
   dumping=0;
  else if (IS("-s"))                    
   stripping=1;
  else if (IS("-v"))                    
   ++version;
  else                                  
   usage(argv[i]);
 }
 if (i==argc && (listing || !dumping))
 {
  dumping=0;
  argv[--i]=Output;
 }
 if (version)
 {
  printf("%s\n",LUA_COPYRIGHT);
  if (version==argc-1) exit(EXIT_SUCCESS);
 }
 return i;
}

#define FUNCTION "(function()end)();\n"

static const char* reader(lua_State* L, void* ud, size_t* size)
{
 UNUSED(L);
 if ((*(int*)ud)--)
 {
  *size=sizeof(FUNCTION)-1;
  return FUNCTION;
 }
 else
 {
  *size=0;
  return NULL;
 }
}

#define toproto(L,i) getproto(s2v(L->top.p+(i)))

static const Proto* combine(lua_State* L, int n)
{
 if (n==1)
  return toproto(L,-1);
 else
 {
  Proto* f;
  int i=n;
  if (lua_load(L,reader,&i,"=(" PROGNAME ")",NULL)!=LUA_OK) fatal(lua_tostring(L,-1));
  f=toproto(L,-1);
  for (i=0; i<n; i++)
  {
   f->p[i]=toproto(L,i-n-1);
   if (f->p[i]->sizeupvalues>0) f->p[i]->upvalues[0].instack=0;
  }
  return f;
 }
}

static int writer(lua_State* L, const void* p, size_t size, void* u)
{
 UNUSED(L);
 return (fwrite(p,size,1,(FILE*)u)!=1) && (size!=0);
}

static int pmain(lua_State* L)
{
 int argc=(int)lua_tointeger(L,1);
 char** argv=(char**)lua_touserdata(L,2);
 const Proto* f;
 int i;
 tmname=G(L)->tmname;
 if (!lua_checkstack(L,argc)) fatal("too many input files");
 for (i=0; i<argc; i++)
 {
  const char* filename=IS("-") ? NULL : argv[i];
  if (luaL_loadfile(L,filename)!=LUA_OK) fatal(lua_tostring(L,-1));
 }
 f=combine(L,argc);
 if (listing) luaU_print(f,listing>1);
 if (dumping)
 {
  FILE* D= (output==NULL) ? stdout : fopen(output,"wb");
  if (D==NULL) cannot("open");
  lua_lock(L);
  luaU_dump(L,f,writer,D,stripping);
  lua_unlock(L);
  if (ferror(D)) cannot("write");
  if (fclose(D)) cannot("close");
 }
 return 0;
}

int main(int argc, char* argv[])
{
 lua_State* L;
 int i=doargs(argc,argv);
 argc-=i; argv+=i;
 if (argc<=0) usage("no input files given");
 L=luaL_newstate();
 if (L==NULL) fatal("cannot create state: not enough memory");
 lua_pushcfunction(L,&pmain);
 lua_pushinteger(L,argc);
 lua_pushlightuserdata(L,argv);
 if (lua_pcall(L,2,0,0)!=LUA_OK) fatal(lua_tostring(L,-1));
 lua_close(L);
 return EXIT_SUCCESS;
}



#define UPVALNAME(x) ((f->upvalues[x].name) ? getstr(f->upvalues[x].name) : "-")
#define VOID(p) ((const void*)(p))
#define eventname(i) (getstr(tmname[i]))

static void PrintString(const TString* ts)
{
 const char* s=getstr(ts);
 size_t i,n=tsslen(ts);
 printf("\"");
 for (i=0; i<n; i++)
 {
  int c=(int)(unsigned char)s[i];
  switch (c)
  {
   case '"':
        printf("\\\"");
        break;
   case '\\':
        printf("\\\\");
        break;
   case '\a':
        printf("\\a");
        break;
   case '\b':
        printf("\\b");
        break;
   case '\f':
        printf("\\f");
        break;
   case '\n':
        printf("\\n");
        break;
   case '\r':
        printf("\\r");
        break;
   case '\t':
        printf("\\t");
        break;
   case '\v':
        printf("\\v");
        break;
   default:
        if (isprint(c)) printf("%c",c); else printf("\\%03d",c);
        break;
  }
 }
 printf("\"");
}

static void PrintType(const Proto* f, int i)
{
 const TValue* o=&f->k[i];
 switch (ttypetag(o))
 {
  case LUA_VNIL:
        printf("N");
        break;
  case LUA_VFALSE:
  case LUA_VTRUE:
        printf("B");
        break;
  case LUA_VNUMFLT:
        printf("F");
        break;
  case LUA_VNUMINT:
        printf("I");
        break;
  case LUA_VSHRSTR:
  case LUA_VLNGSTR:
        printf("S");
        break;
  default:                              
        printf("?%d",ttypetag(o));
        break;
 }
 printf("\t");
}

static void PrintConstant(const Proto* f, int i)
{
 const TValue* o=&f->k[i];
 switch (ttypetag(o))
 {
  case LUA_VNIL:
        printf("nil");
        break;
  case LUA_VFALSE:
        printf("false");
        break;
  case LUA_VTRUE:
        printf("true");
        break;
  case LUA_VNUMFLT:
        {
        char buff[100];
        sprintf(buff,LUA_NUMBER_FMT,fltvalue(o));
        printf("%s",buff);
        if (buff[strspn(buff,"-0123456789")]=='\0') printf(".0");
        break;
        }
  case LUA_VNUMINT:
        printf(LUA_INTEGER_FMT,ivalue(o));
        break;
  case LUA_VSHRSTR:
  case LUA_VLNGSTR:
        PrintString(tsvalue(o));
        break;
  default:                              
        printf("?%d",ttypetag(o));
        break;
 }
}

#define COMMENT         "\t; "
#define EXTRAARG        GETARG_Ax(code[pc+1])
#define EXTRAARGC       (EXTRAARG*(MAXARG_C+1))
#define ISK             (isk ? "k" : "")

static void PrintCode(const Proto* f)
{
 const Instruction* code=f->code;
 int pc,n=f->sizecode;
 for (pc=0; pc<n; pc++)
 {
  Instruction i=code[pc];
  OpCode o=GET_OPCODE(i);
  int a=GETARG_A(i);
  int b=GETARG_B(i);
  int c=GETARG_C(i);
  int ax=GETARG_Ax(i);
  int bx=GETARG_Bx(i);
  int sb=GETARG_sB(i);
  int sc=GETARG_sC(i);
  int sbx=GETARG_sBx(i);
  int isk=GETARG_k(i);
  int line=luaG_getfuncline(f,pc);
  printf("\t%d\t",pc+1);
  if (line>0) printf("[%d]\t",line); else printf("[-]\t");
  printf("%-9s\t",opnames[o]);
  switch (o)
  {
   case OP_MOVE:
        printf("%d %d",a,b);
        break;
   case OP_LOADI:
        printf("%d %d",a,sbx);
        break;
   case OP_LOADF:
        printf("%d %d",a,sbx);
        break;
   case OP_LOADK:
        printf("%d %d",a,bx);
        printf(COMMENT); PrintConstant(f,bx);
        break;
   case OP_LOADKX:
        printf("%d",a);
        printf(COMMENT); PrintConstant(f,EXTRAARG);
        break;
   case OP_LOADFALSE:
        printf("%d",a);
        break;
   case OP_LFALSESKIP:
        printf("%d",a);
        break;
   case OP_LOADTRUE:
        printf("%d",a);
        break;
   case OP_LOADNIL:
        printf("%d %d",a,b);
        printf(COMMENT "%d out",b+1);
        break;
   case OP_GETUPVAL:
        printf("%d %d",a,b);
        printf(COMMENT "%s",UPVALNAME(b));
        break;
   case OP_SETUPVAL:
        printf("%d %d",a,b);
        printf(COMMENT "%s",UPVALNAME(b));
        break;
   case OP_GETTABUP:
        printf("%d %d %d",a,b,c);
        printf(COMMENT "%s",UPVALNAME(b));
        printf(" "); PrintConstant(f,c);
        break;
   case OP_GETTABLE:
        printf("%d %d %d",a,b,c);
        break;
   case OP_GETI:
        printf("%d %d %d",a,b,c);
        break;
   case OP_GETFIELD:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_SETTABUP:
        printf("%d %d %d%s",a,b,c,ISK);
        printf(COMMENT "%s",UPVALNAME(a));
        printf(" "); PrintConstant(f,b);
        if (isk) { printf(" "); PrintConstant(f,c); }
        break;
   case OP_SETTABLE:
        printf("%d %d %d%s",a,b,c,ISK);
        if (isk) { printf(COMMENT); PrintConstant(f,c); }
        break;
   case OP_SETI:
        printf("%d %d %d%s",a,b,c,ISK);
        if (isk) { printf(COMMENT); PrintConstant(f,c); }
        break;
   case OP_SETFIELD:
        printf("%d %d %d%s",a,b,c,ISK);
        printf(COMMENT); PrintConstant(f,b);
        if (isk) { printf(" "); PrintConstant(f,c); }
        break;
   case OP_NEWTABLE:
        printf("%d %d %d",a,b,c);
        printf(COMMENT "%d",c+EXTRAARGC);
        break;
   case OP_SELF:
        printf("%d %d %d%s",a,b,c,ISK);
        if (isk) { printf(COMMENT); PrintConstant(f,c); }
        break;
   case OP_ADDI:
        printf("%d %d %d",a,b,sc);
        break;
   case OP_ADDK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_SUBK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_MULK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_MODK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_POWK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_DIVK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_IDIVK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_BANDK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_BORK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_BXORK:
        printf("%d %d %d",a,b,c);
        printf(COMMENT); PrintConstant(f,c);
        break;
   case OP_SHRI:
        printf("%d %d %d",a,b,sc);
        break;
   case OP_SHLI:
        printf("%d %d %d",a,b,sc);
        break;
   case OP_ADD:
        printf("%d %d %d",a,b,c);
        break;
   case OP_SUB:
        printf("%d %d %d",a,b,c);
        break;
   case OP_MUL:
        printf("%d %d %d",a,b,c);
        break;
   case OP_MOD:
        printf("%d %d %d",a,b,c);
        break;
   case OP_POW:
        printf("%d %d %d",a,b,c);
        break;
   case OP_DIV:
        printf("%d %d %d",a,b,c);
        break;
   case OP_IDIV:
        printf("%d %d %d",a,b,c);
        break;
   case OP_BAND:
        printf("%d %d %d",a,b,c);
        break;
   case OP_BOR:
        printf("%d %d %d",a,b,c);
        break;
   case OP_BXOR:
        printf("%d %d %d",a,b,c);
        break;
   case OP_SHL:
        printf("%d %d %d",a,b,c);
        break;
   case OP_SHR:
        printf("%d %d %d",a,b,c);
        break;
   case OP_MMBIN:
        printf("%d %d %d",a,b,c);
        printf(COMMENT "%s",eventname(c));
        break;
   case OP_MMBINI:
        printf("%d %d %d %d",a,sb,c,isk);
        printf(COMMENT "%s",eventname(c));
        if (isk) printf(" flip");
        break;
   case OP_MMBINK:
        printf("%d %d %d %d",a,b,c,isk);
        printf(COMMENT "%s ",eventname(c)); PrintConstant(f,b);
        if (isk) printf(" flip");
        break;
   case OP_UNM:
        printf("%d %d",a,b);
        break;
   case OP_BNOT:
        printf("%d %d",a,b);
        break;
   case OP_NOT:
        printf("%d %d",a,b);
        break;
   case OP_LEN:
        printf("%d %d",a,b);
        break;
   case OP_CONCAT:
        printf("%d %d",a,b);
        break;
   case OP_CLOSE:
        printf("%d",a);
        break;
   case OP_TBC:
        printf("%d",a);
        break;
   case OP_JMP:
        printf("%d",GETARG_sJ(i));
        printf(COMMENT "to %d",GETARG_sJ(i)+pc+2);
        break;
   case OP_EQ:
        printf("%d %d %d",a,b,isk);
        break;
   case OP_LT:
        printf("%d %d %d",a,b,isk);
        break;
   case OP_LE:
        printf("%d %d %d",a,b,isk);
        break;
   case OP_EQK:
        printf("%d %d %d",a,b,isk);
        printf(COMMENT); PrintConstant(f,b);
        break;
   case OP_EQI:
        printf("%d %d %d",a,sb,isk);
        break;
   case OP_LTI:
        printf("%d %d %d",a,sb,isk);
        break;
   case OP_LEI:
        printf("%d %d %d",a,sb,isk);
        break;
   case OP_GTI:
        printf("%d %d %d",a,sb,isk);
        break;
   case OP_GEI:
        printf("%d %d %d",a,sb,isk);
        break;
   case OP_TEST:
        printf("%d %d",a,isk);
        break;
   case OP_TESTSET:
        printf("%d %d %d",a,b,isk);
        break;
   case OP_CALL:
        printf("%d %d %d",a,b,c);
        printf(COMMENT);
        if (b==0) printf("all in "); else printf("%d in ",b-1);
        if (c==0) printf("all out"); else printf("%d out",c-1);
        break;
   case OP_TAILCALL:
        printf("%d %d %d%s",a,b,c,ISK);
        printf(COMMENT "%d in",b-1);
        break;
   case OP_RETURN:
        printf("%d %d %d%s",a,b,c,ISK);
        printf(COMMENT);
        if (b==0) printf("all out"); else printf("%d out",b-1);
        break;
   case OP_RETURN0:
        break;
   case OP_RETURN1:
        printf("%d",a);
        break;
   case OP_FORLOOP:
        printf("%d %d",a,bx);
        printf(COMMENT "to %d",pc-bx+2);
        break;
   case OP_FORPREP:
        printf("%d %d",a,bx);
        printf(COMMENT "exit to %d",pc+bx+3);
        break;
   case OP_TFORPREP:
        printf("%d %d",a,bx);
        printf(COMMENT "to %d",pc+bx+2);
        break;
   case OP_TFORCALL:
        printf("%d %d",a,c);
        break;
   case OP_TFORLOOP:
        printf("%d %d",a,bx);
        printf(COMMENT "to %d",pc-bx+2);
        break;
   case OP_SETLIST:
        printf("%d %d %d",a,b,c);
        if (isk) printf(COMMENT "%d",c+EXTRAARGC);
        break;
   case OP_CLOSURE:
        printf("%d %d",a,bx);
        printf(COMMENT "%p",VOID(f->p[bx]));
        break;
   case OP_VARARG:
        printf("%d %d",a,c);
        printf(COMMENT);
        if (c==0) printf("all out"); else printf("%d out",c-1);
        break;
   case OP_VARARGPREP:
        printf("%d",a);
        break;
   case OP_EXTRAARG:
        printf("%d",ax);
        break;
#if 0
   default:
        printf("%d %d %d",a,b,c);
        printf(COMMENT "not handled");
        break;
#endif
  }
  printf("\n");
 }
}


#define SS(x)   ((x==1)?"":"s")
#define S(x)    (int)(x),SS(x)

static void PrintHeader(const Proto* f)
{
 const char* s=f->source ? getstr(f->source) : "=?";
 if (*s=='@' || *s=='=')
  s++;
 else if (*s==LUA_SIGNATURE[0])
  s="(bstring)";
 else
  s="(string)";
 printf("\n%s <%s:%d,%d> (%d instruction%s at %p)\n",
        (f->linedefined==0)?"main":"function",s,
        f->linedefined,f->lastlinedefined,
        S(f->sizecode),VOID(f));
 printf("%d%s param%s, %d slot%s, %d upvalue%s, ",
        (int)(f->numparams),f->is_vararg?"+":"",SS(f->numparams),
        S(f->maxstacksize),S(f->sizeupvalues));
 printf("%d local%s, %d constant%s, %d function%s\n",
        S(f->sizelocvars),S(f->sizek),S(f->sizep));
}

static void PrintDebug(const Proto* f)
{
 int i,n;
 n=f->sizek;
 printf("constants (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  printf("\t%d\t",i);
  PrintType(f,i);
  PrintConstant(f,i);
  printf("\n");
 }
 n=f->sizelocvars;
 printf("locals (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  printf("\t%d\t%s\t%d\t%d\n",
  i,getstr(f->locvars[i].varname),f->locvars[i].startpc+1,f->locvars[i].endpc+1);
 }
 n=f->sizeupvalues;
 printf("upvalues (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  printf("\t%d\t%s\t%d\t%d\n",
  i,UPVALNAME(i),f->upvalues[i].instack,f->upvalues[i].idx);
 }
}

static void PrintFunction(const Proto* f, int full)
{
 int i,n=f->sizep;
 PrintHeader(f);
 PrintCode(f);
 if (full) PrintDebug(f);
 for (i=0; i<n; i++) PrintFunction(f->p[i],full);
}

#endif




#endif


#ifndef privateluacembed_consts
#define privateluacembed_consts
//path: src/imports/../constants/consts.aplication.h
//mannaged by silver chain

#ifndef LUA_CEMBED_INDEX_DIF
#define LUA_CEMBED_INDEX_DIF 1
#endif

#define PRIVATE_LUA_EMBED_FILE_EVALUATION_TYPE 1
#define PRIVATE_LUA_EMBED_STRING_EVALUATION_TYPE 2
#define LUA_CEMBED_DEFAULT_TIMEOUT 10
#define LUA_CEMBED_DEFAULT_MEMORY_LIMIT 100

#define PRIVATE_LUA_CEMBED_ONE_MB (1024 * 1024)


//path: src/imports/../constants/consts.errors.h
//mannaged by silver chain



#define  PRIVATE_LUA_CEMBED_TIMEOUT_ERROR "timeout error"
#define PRIVATE_LUA_CEMBED_ARG_NOT_PROVIDED "arg %ld at function:%s not provided"
#define PRIVATE_LUA_CEMBED_ARG_WRONG_TYPE "arg %d at function:%s its %s instead of  %s"
#define PRIVATE_LUA_CEMBED_GLOBAL_WRONG_TYPE "global variable:%s its %s instead of %s"
#define PRIVATE_LUA_CEMBED_RESULT_EVALUATION_WRONG_TYPE "result evaluation its:%s instead of %s"
#define PRIVATE_LUA_CEMBED_RESULT_EVALUATION_ARG_WRONG_TYPE "result evaluation of function %s at index %d its:%s instead of %s"

#define PRIVATE_LUA_CEMBED_CODE_ITS_NOT_A_FUNCTION "value %s its not a function"
#define PRIVATE_LUA_CEMBED_WRONG_TYPE_PROPETY "propety %s at object %s its %s instead of %s"
#define PRIVATE_LUA_CEMBED_WRONG_TYPE_INDEX "index %d at object %s its %s instead of %s"
#define PRIVATE_LUA_CEMBED_ELEMENT_DOES_NOT_HAVE_KEY "index %d at object %s not have a key"
#define PRIVVATE_LUA_CEMBED_TIMEOUT_ERROR "timeout error"
#define PRIVATE_LUA_CEMBED_LIB_PROP_WRONG_TYPE "lib prop %s its %s insteadd of %s"

//path: src/imports/../constants/consts.lua_code.h
//mannaged by silver chain


#define PRIVATE_LUA_CEMBED_TOTAL_LIBS "private_lua_c_embed_total_running_libs"
#define PRIVATE_LUA_CEMBED_STACK_LEVEL "private_lua_c_embed_stack_level"

#define PRIVATE_LUA_CEMBED_EVALUATION_NAME "private_lua_c_embed_evaluation"
#define PRIVATE_LUA_CEMBED_DEL_PREFIX "__gc"
#define  PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE "%s = %s\n"




#define PRIVATE_LUA_CEMBED_SELFNAME "private_lua_c_embed_self"
#define PRIVATE_LUA_CEMBED_STAGE_AREA_TABLE "private_lua_c_embed_stage_area_table"
#define PRIVATE_LUA_CEMBED_TABLE_RETURN "private_lua_embed_table_return"

#define PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME__ "private_lua_c_embed_main_lib_table_%d"
#define PRIVATE_LUA_CEMBED_MAIN_LIB_SUB_TABLE "private_lua_c_embed_main_lib_table_%d_%s"

#define PRIVATE_LUA_CEMBED_MAIN_META_TABLE__ "private_lua_c_embed_main_meta_table_%d"
#define PRIVATE_LUA_CEMBED_ANONYMOUS_TABLE_ "private_lua_c_embed_anononymous_table_%d_%d"
#define PRIVATE_LUA_CEMBED_ANONYMOUS_FUNC_TABLE_ "private_lua_c_embed_anononymous_func_table_%d_%d"
#define PRIVATE_LUA_CEMBED_ARGS_ "private_lua_c_embed_args_%d_%d"
#define  PRIVATE_LUA_CEMBED_MULTIRETURN_ "private_lua_c_embed_multi_return_%d_%d"

//path: src/imports/../constants/consts.main_callback.h
//mannaged by silver chain


#define PRIVATE_LUACEMBED_NO_RETURN 0
#define PRIVATE_LUACEMBED_ONE_RETURN 1



//path: src/imports/../constants/consts.responses.h
//mannaged by silver chain

#define PRIVATE_LUA_CEMBED_STRING_RESPONSE 1
#define PRIVATE_LUA_CEMBED_LONG_RESPONSE 2
#define PRIVATE_LUA_CEMBED_DOUBLE_RESPONSE 3
#define PRIVATE_LUA_CEMBED_BOOL_RESPONSE 4
#define PRIVATE_LUA_CEMBED_TABLE_RESPONSE 5

#define PRIVATE_LUA_CEMBED_EVALUATION 6
#define PRIVATE_LUA_CEMBED_ERROR_RESPONSE 7
#define PRIVATE_LUA_CEMBED_MULTI_RESPONSE 8

#define LUA_CEMBED_OK 0
#define LUA_CEMBED_GENERIC_ERROR (-1)

//path: src/imports/../constants/consts.types.h
//mannaged by silver chain

#define LUA_CEMBED_NOT_FOUND LUA_TNONE
#define LUA_CEMBED_STRING LUA_TSTRING
#define LUA_CEMBED_NUMBER LUA_TNUMBER
#define LUA_CEMBED_BOOL LUA_TBOOLEAN
#define LUA_CEMBED_NIL LUA_TNIL
#define LUA_CEMBED_FUNCTION LUA_TFUNCTION
#define LUA_CEMBED_TABLE LUA_TTABLE

#define PRIVATE_LUA_CEMBED_NOT_FOUND_STRING "not Found"
#define PRIVATE_LUA_CEMBED_STRING_STRING "string"
#define PRIVATE_LUA_CEMBED_NUMBER_STRING "number"
#define PRIVATE_LUA_CEMBED_BOOL_STRING "boolean"
#define PRIVATE_LUA_CEMBED_NIL_STRING "nil"
#define PRIVATE_LUA_CEMBED_TABLE_STRING "table"
#define PRIVATE_LUA_CEMBED_FUNCTION_STRING "function"

#define PRIVATE_LUA_CEMBED_UNKNOW_STRING "unknow"

#endif


#ifndef privateluacembed_macros
#define privateluacembed_macros
//path: src/imports/../LuaCEmbed/macros.protection_macros.h
//mannaged by silver chain

#define PRIVATE_LUA_CEMBED_PROTECT_VOID \
    if(!self){                          \
    return;                              \
    }


#define PRIVATE_LUA_CEMBED_PROTECT_NUM \
    if(!self){                          \
    return LUA_CEMBED_GENERIC_ERROR;      \
    }

#define PRIVATE_LUA_CEMBED_PROTECT_BOOL \
    if(!self){                          \
    return false;      \
    }

#define PRIVATE_LUA_CEMBED_PROTECT_NULL \
    if(!self){                          \
    return NULL;      \
    }

#define PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK     if(lua_gettop(self->main_object->state)){lua_settop(self->main_object->state,0);}

#define PRIVATE_LUA_CEMBED_CLEAR_STACK     if(lua_gettop(self->state)){lua_settop(self->state,0);}


//path: src/imports/../LuaCEmbed/table/macros.protection_macros.h
//mannaged by silver chain

#define PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID \
    if(!self){                          \
    return;                              \
    }

#define PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM \
    if(!self){                          \
    return LUA_CEMBED_GENERIC_ERROR;      \
    }

#define PRIVATE_LUA_CEMBED_TABLE_PROTECT_BOOL \
    if(!self){                          \
    return false;      \
    }

#define PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL \
    if(!self){                          \
    return NULL;      \
    }


#endif


#ifndef privateluacembed_types
#define privateluacembed_types
//path: src/imports/../LuaCEmbed/types.LuaCEmbed.h
//mannaged by silver chain


typedef struct LuaCEmbed{
    lua_State *state;
    const char *current_function;
    bool is_lib;
    int total_args;
    char *error_msg;
    void (*delete_function)(struct  LuaCEmbed *self);
    void *global_tables;
    void *func_tables;
    int lib_identifier;
    int stack_leve;
    char *main_lib_table;
    bool field_protection;
}LuaCEmbed;

int lua_cembed_timeout = -1;
LuaCEmbed  *global_current_lua_embed_object;


//path: src/imports/../LuaCEmbedResponse/types.LuaCembedResponse.h
//mannaged by silver chain


typedef struct {

    int type;
    double double_val;
    lua_Integer int_val;
    bool its_string_ref;
    lua_Integer string_size;
    char *string_val;

}LuaCEmbedResponse;


//path: src/imports/../types.typedefs.h
//mannaged by silver chain

typedef lua_Integer private_lua_cembed_incremented_arg ;


#endif


#ifndef privateluacembed_typesB
#define privateluacembed_typesB
//path: src/imports/../LuaCEmbed/table/table/typesB.table.h
//mannaged by silver chain



typedef struct {
    LuaCEmbed  *main_object;
    char *prop_name;
    char  *global_name;
    void *sub_tables;
    long index;

}LuaCEmbedTable;


#endif


#ifndef privateluacembed_typesC
#define privateluacembed_typesC
//path: src/imports/../namespace/args/typesC.args.h
//mannaged by silver chain


typedef struct {


    lua_Integer  (*size)(LuaCEmbed *self);
    lua_Integer  (*get_type)(LuaCEmbed *self,lua_Integer index);
   lua_Integer (*get_long)(LuaCEmbed *self, lua_Integer index);
    double (*get_double)(LuaCEmbed *self, lua_Integer index);
    bool (*get_bool)(LuaCEmbed *self, lua_Integer index);
    char * (*get_raw_str)(LuaCEmbed *self, lua_Integer *size, lua_Integer index);
    char * (*get_str)(LuaCEmbed *self, lua_Integer index);
    LuaCEmbedTable  * (*get_table)(LuaCEmbed *self,lua_Integer index);
    lua_Integer  (*generate_arg_clojure_evalation)(LuaCEmbed *self,lua_Integer index,const char *code,...);
    lua_Integer  (*get_type_clojure_evalation)(LuaCEmbed *self,lua_Integer index,const char *code,...);
    lua_Integer  (*get_long_arg_clojure_evalation)(LuaCEmbed *self,lua_Integer index,const char *code,...);
    double (*get_double_arg_clojure_evalation)(LuaCEmbed *self,lua_Integer index,const char *code,...);
    bool (*get_bool_arg_clojure_evalation)(LuaCEmbed *self,lua_Integer index,const char *code,...);
    char* (*get_string_arg_clojure_evalation)(LuaCEmbed *self,lua_Integer index,const char *code,...);
    LuaCEmbedTable* (*run_lambda)(LuaCEmbed *self, lua_Integer index, LuaCEmbedTable *args_to_call,lua_Integer total_returns);




} LuaCembedArgsModule;


//path: src/imports/../namespace/globals/typesC.globals.h
//mannaged by silver chain



typedef struct {

    int (*ensure_type)(LuaCEmbed *self, const char *name,int expected_type);
    int (*get_type)(LuaCEmbed *self,const char *name);
    lua_Integer (*get_long)(LuaCEmbed *self,const char *name);
    double (*get_double)(LuaCEmbed *self,const char *name);
    bool (*get_bool)(LuaCEmbed *self,const char *name);
    char * (*get_string)(LuaCEmbed *self,const char *name);
    LuaCEmbedTable* (*run_global_lambda)(LuaCEmbed *self, const char *name, LuaCEmbedTable *args_to_call, int total_returns);
    void (*set_table)(LuaCEmbed *self, const char *name, LuaCEmbedTable *table);
    void (*set_raw_string)(LuaCEmbed *self, const char *name, const  char *value, long size);
    char * (*get_raw_string)(LuaCEmbed *self, const char *name, long *size);


    void (*set_string)(LuaCEmbed *self,const char *name,const  char *value);
    void (*set_long)(LuaCEmbed *self,const char *name,lua_Integer  value);
    void (*set_double)(LuaCEmbed *self,const char *name,double value);
    void (*set_bool)(LuaCEmbed *self,const char *name,bool value);

    LuaCEmbedTable * (*get_table)(LuaCEmbed *self, const char *name);
    LuaCEmbedTable * (*new_table)(LuaCEmbed *self, const char *name);


}LuaCEmbedGlobalModule;


//path: src/imports/../namespace/response/typesC.response.h
//mannaged by silver chain


typedef struct {
    LuaCEmbedResponse * (*send_multi_return)(LuaCEmbedTable *table);
    LuaCEmbedResponse * (*send_str)(const char *text);
    LuaCEmbedResponse * (*send_raw_string)(const char *text,lua_Integer size);
    LuaCEmbedResponse * (*send_str_reference)( const char *text);
    LuaCEmbedResponse * (*send_raw_string_reference)(const char *text,lua_Integer size);
    LuaCEmbedResponse * (*send_table)(LuaCEmbedTable *table);
    LuaCEmbedResponse * (*send_evaluation_function)(const char *function);
    LuaCEmbedResponse  * (*send_double)(double value);
    LuaCEmbedResponse  * (*send_bool)(bool value);
    LuaCEmbedResponse  * (*send_long)(lua_Integer value);
    LuaCEmbedResponse * (*send_error)(const char *format,...);

}LuaCEmbedResponseModule;


//path: src/imports/../namespace/table/typesC.table.h
//mannaged by silver chain

typedef struct {

    void  (*append_string)(LuaCEmbedTable *self,  const char *value);
    void  (*append_long)(LuaCEmbedTable *self, lua_Integer  value);
    void  (*append_double)(LuaCEmbedTable *self, double  value);
    void  (*append_bool)(LuaCEmbedTable *self,  bool value);
    void  (*append_table)(LuaCEmbedTable *self, LuaCEmbedTable *table);
    void  (*set_evaluation_prop)(LuaCEmbedTable *self, const char *name, const char *code, ...);
    void  (*append_evaluation)(LuaCEmbedTable *self, const char *code, ...);

    void  (*insert_string_at_index)(LuaCEmbedTable *self, lua_Integer index, const char *value);
    void  (*insert_bool_at_index)(LuaCEmbedTable *self, lua_Integer index,bool value);
    void  (*insert_long_at_index)(LuaCEmbedTable *self, lua_Integer  index,lua_Integer value);
    void  (*insert_double_at_index)(LuaCEmbedTable *self, lua_Integer index,double value);
    void  (*insert_table_at_index)(LuaCEmbedTable *self, lua_Integer index,LuaCEmbedTable *table);



    void (*set_sub_table_by_index)(LuaCEmbedTable *self, lua_Integer index,LuaCEmbedTable *sub_table);
    void  (*set_string_by_index)(LuaCEmbedTable *self, lua_Integer index, const char *value);
    void  (*set_raw_string_by_index)(LuaCEmbedTable *self, lua_Integer index, const char *value,lua_Integer size);
    void  (*set_long_by_index)(LuaCEmbedTable *self, lua_Integer index, lua_Integer  value);
    void  (*set_double_by_index)(LuaCEmbedTable *self, lua_Integer index, double  value);
    void  (*set_bool_by_index)(LuaCEmbedTable *self, lua_Integer index, bool value);
    void  (*set_evaluation_by_index)(LuaCEmbedTable *self, lua_Integer index, const char *code, ...);
    lua_Integer  (*get_listable_size)(LuaCEmbedTable *self);
    LuaCEmbedTable  *(*get_sub_table_by_index)(LuaCEmbedTable *self, lua_Integer index);

    LuaCEmbedTable  *(*new_sub_table_appending)(LuaCEmbedTable *self);

    void (*set_method)(LuaCEmbedTable *self , const char *name, LuaCEmbedResponse *(*callback)(LuaCEmbedTable  *self, LuaCEmbed *args));
    void  (*set_string_prop)(LuaCEmbedTable *self , const char *name, const char *value);
    void  (*set_raw_string_prop)(LuaCEmbedTable *self , const char *name, const char *value,lua_Integer size);

    void  (*set_long_prop)(LuaCEmbedTable *self , const char *name,lua_Integer   value);
    void  (*set_double_prop)(LuaCEmbedTable *self , const char *name, double  value);
    void  (*set_bool_prop)(LuaCEmbedTable *self , const char *name, bool value);
    int  (*get_type_prop)(LuaCEmbedTable *self, const char *name);
    char*  (*get_string_prop)(LuaCEmbedTable *self , const char *name);
    char*  (*get_raw_string_prop)(LuaCEmbedTable *self, const char *name,lua_Integer *size);
    lua_Integer  (*get_long_prop)(LuaCEmbedTable *self , const char *name);
    double  (*get_double_prop)(LuaCEmbedTable *self , const char *name);
    bool  (*get_bool_prop)(LuaCEmbedTable *self , const char *name);
    LuaCEmbedTable * (*new_anonymous_table)(LuaCEmbed *self);
    void (*destroy_prop)(LuaCEmbedTable *self, const char *name);

    LuaCEmbedTable  *(*get_sub_table_prop)(LuaCEmbedTable *self, const char *name);
    LuaCEmbedTable  *(*new_sub_table_prop)(LuaCEmbedTable *self, const char *name);
    void (*set_sub_table_prop)(LuaCEmbedTable *self, const char *name, LuaCEmbedTable *sub_table);
    char *(*get_key_by_index)(LuaCEmbedTable *self, lua_Integer index);
    bool (*has_key)(LuaCEmbedTable *self,lua_Integer index);

    lua_Integer  (*get_size)(LuaCEmbedTable *self);
    int (*get_type_by_index)(LuaCEmbedTable *self, lua_Integer index);
    lua_Integer (*get_long_by_index)(LuaCEmbedTable *self, lua_Integer index);
    double (*get_double_by_index)(LuaCEmbedTable *self, lua_Integer index);
    char * (*get_string_by_index)(LuaCEmbedTable *self, lua_Integer index);
    char * (*get_raw_string_by_index)(LuaCEmbedTable *self, lua_Integer index, lua_Integer *size);

    bool (*get_bool_by_index)(LuaCEmbedTable *self, lua_Integer index);

}LuaCembedTableModule;


//path: src/imports/../namespace/types/typesC.types.h
//mannaged by silver chain



typedef struct {
    int NOT_FOUND;
    int STRING;
    int NUMBER;
    int BOOL;
    int NILL;
    int TABLE;
    int FUNCTION;


}LuaCEmbedTypeModule;


#endif


#ifndef privateluacembed_typesD
#define privateluacembed_typesD
//path: src/imports/../namespace/LuaCEmbedNamespace/typesD.LuaCEmbedNamespace.h
//mannaged by silver chain


typedef struct{
    LuaCEmbedTypeModule  types;
    LuaCEmbedResponseModule  response;
    LuaCembedArgsModule args;
    LuaCEmbedGlobalModule  globals;
    LuaCembedTableModule tables;
    void (*clear_errors)(LuaCEmbed *self);
    LuaCEmbed * (*newLuaLib)(lua_State *state);
    void (*set_delete_function)(LuaCEmbed *self,void (*delelte_function)(struct  LuaCEmbed *self));
    LuaCEmbed * (*newLuaEvaluation)();
    void (*load_native_libs)(LuaCEmbed *self);
    void (*load_lib_from_c)(LuaCEmbed *self,int (*callback)(lua_State *l),const char *name);
    int (*perform)(LuaCEmbed *self);
    int (*send_self_as_lib)(LuaCEmbed *self);
    int (*send_global_as_lib)(LuaCEmbed *self,const char *global_name);

    const char * (*convert_arg_code)(int arg_code);
    void (*set_memory_limit)(LuaCEmbed *self, double limit);

    char * (*get_error_message)(LuaCEmbed *self);
    bool (*has_errors)(LuaCEmbed *self);
    void (*set_timeout)(int seconds);
    int (*evaluate)(LuaCEmbed *self, const char *code, ...);
    char * (*get_string_evaluation)(LuaCEmbed *self,const char *code, ...);
    int  (*get_evaluation_type)(LuaCEmbed *self,const char *code,...);
    long (*get_evaluation_size)(LuaCEmbed *self,const char *code,...);
    long long  (*get_evaluation_long)(LuaCEmbed *self,const char *code,...);
    double (*get_evaluation_double)(LuaCEmbed *self,const char *code,...);
    bool (*get_evaluation_bool)(LuaCEmbed *self, const char *code,...);

    void (*set_long_lib_prop)(LuaCEmbed *self,const char *name,lua_Integer value);

    void (*set_double_lib_prop)(LuaCEmbed *self,const char *name,double value);
    void (*set_bool_lib_prop)(LuaCEmbed *self,const char *name,bool value);
    void (*set_string_lib_prop)(LuaCEmbed *self,const char *name,const char * value);
    void (*set_table_lib_prop)(LuaCEmbed *self,const char *name,LuaCEmbedTable *value);

    lua_Integer  (*get_long_lib_prop)(LuaCEmbed *self,const char *name);
    double (*get_double_lib_prop)(LuaCEmbed *self,const char *name);
    bool (*get_bool_lib_prop)(LuaCEmbed *self,const char *name);
    char * (*get_string_lib_prop)(LuaCEmbed *self,const char *name);
    LuaCEmbedTable *(*get_table_lib_prop)(LuaCEmbed *self,const char *name);

    int (*evaluete_file)(LuaCEmbed *self, const char *file);
    void (*add_callback)(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) );
    void (*add_global_callback)(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) );

    void (*dangerous_raise_error_jumping)(LuaCEmbed *self,const char *error_msg,...);
    void (*dangerous_raise_self_error_jumping)(LuaCEmbed *self);

    void (*free)(LuaCEmbed *self);

} LuaCEmbedNamespace;


#endif


#ifndef privateluacembed_fdeclare
#define privateluacembed_fdeclare
//path: src/imports/../LuaCEmbed/args/args_error/fdeclare.args_error.h
//mannaged by silver chain



int LuaCEmbed_ensure_arg_exist(LuaCEmbed *self, private_lua_cembed_incremented_arg index);

int private_LuaCEmbed_ensure_top_stack_arg_type(LuaCEmbed *self, int index,int arg_type);


//path: src/imports/../LuaCEmbed/args/basic/fdeclare.basic.h
//mannaged by silver chain


lua_Integer privateLuaCEmbed_put_arg_on_top(LuaCEmbed *self, lua_Integer index);

lua_Integer  LuaCEmbed_get_total_args(LuaCEmbed *self);

lua_Integer  LuaCEmbed_get_arg_type(LuaCEmbed *self,lua_Integer index);



lua_Integer LuaCEmbed_get_long_arg(LuaCEmbed *self, lua_Integer index);

double LuaCEmbed_get_double_arg(LuaCEmbed *self, lua_Integer index);

bool LuaCEmbed_get_bool_arg(LuaCEmbed *self, lua_Integer index);

char * LuaCEmbed_get_str_arg(LuaCEmbed *self, lua_Integer index);

char * LuaCEmbed_get_raw_str_arg(LuaCEmbed *self,lua_Integer *size, lua_Integer index);

LuaCEmbedTable  * LuaCEmbed_get_arg_table(LuaCEmbed *self,lua_Integer index);

LuaCEmbedTable* LuaCEmbed_run_args_lambda(LuaCEmbed *self, lua_Integer index, LuaCEmbedTable *args_to_call, lua_Integer total_returns);


//path: src/imports/../LuaCEmbed/args/evaluation/fdeclare.evaluation.h
//mannaged by silver chain



lua_Integer private_LuaCembed_run_code_with_args(LuaCEmbed *self,lua_Integer index,const char *code,va_list args);

lua_Integer privateLuaCembed_ensure_arg_evaluation_type(LuaCEmbed *self,lua_Integer index,lua_Integer expected_type);

lua_Integer LuaCEmbed_get_type_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...);

lua_Integer LuaCEmbed_generate_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...);

lua_Integer LuaCEmbed_get_long_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...);


double LuaCEmbed_get_double_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...);

bool LuaCEmbed_get_bool_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...);

char* LuaCEmbed_get_string_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...);


//path: src/imports/../LuaCEmbed/basic/fdeclare.basic.h
//mannaged by silver chain




LuaCEmbed * newLuaCEmbedEvaluation();



void LuaCEmbed_load_lib_from_c(LuaCEmbed *self,int (*callback)(lua_State *l),const char *name);

void LuaCEmbed_load_native_libs(LuaCEmbed *self);

void LuaCembed_set_delete_function(LuaCEmbed *self,void (*delelte_function)(struct  LuaCEmbed *self));


void privateLuaCEmbd_get_field_protected(LuaCEmbed *self,const char *name);


void * privateLuaCEmbed_get_current_table_array(LuaCEmbed *self);


void LuaCEmbed_set_timeout(int seconds);



void  privata_LuaCEmbed_increment_stack_(LuaCEmbed *self);


void  privata_LuaCEmbed_decrement_stack(LuaCEmbed *self);

void LuaCEmbed_free(LuaCEmbed *self);


//path: src/imports/../LuaCEmbed/callback_handle/fdeclare.callback_handle.h
//mannaged by silver chain



int privateLuaCEmbed_main_callback_handler(lua_State  *L);

void private_LuaCEmbed_add_lib_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args),bool global_functions );

void private_LuaCEmbed_add_evaluation_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) );

void LuaCEmbed_add_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) );


void LuaCEmbed_add_global_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args));



//path: src/imports/../LuaCEmbed/errors/fdeclare.errors.h
//mannaged by silver chain



char * LuaCEmbed_get_error_message(LuaCEmbed *self);

bool LuaCEmbed_has_errors(LuaCEmbed *self);

void LuaCEmbed_clear_errors(LuaCEmbed *self);


void privateLuaCEmbed_raise_error_not_jumping(LuaCEmbed *self, const char *error, ...);

void LuaCEmbed_dangerous_raise_error_jumping(LuaCEmbed *self,const char *error_msg,...);

void LuaCEmbed_dangerous_raise_self_error_jumping(LuaCEmbed *self);


//path: src/imports/../LuaCEmbed/evaluation/fdeclare.evaluation.h
//mannaged by silver chain



#ifdef _WIN32
VOID CALLBACK TimerHandler(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
#else 

void private_LuaCembed_handle_timeout(int signum);

#endif 

int privateLuaCEmbed_start_func_evaluation(lua_State *state);

int LuaCEmbed_evaluate(LuaCEmbed *self, const char *code, ...);

int LuaCEmbed_evaluete_file(LuaCEmbed *self, const char *file);


int private_LuaCEmbed_ensure_evaluation_type(LuaCEmbed *self,int type);

int private_LuaCEmbed_evaluate_puting_on_top_of_stack(LuaCEmbed *self,const char *code, va_list args);

char * LuaCEmbed_get_evaluation_string(LuaCEmbed *self,const char *code, ...);


int  LuaCEmbed_get_evaluation_type(LuaCEmbed *self,const char *code, ...);


long LuaCEmbed_get_evaluation_table_size(LuaCEmbed *self,const char *code, ...);

long long  LuaCEmbed_get_evaluation_long(LuaCEmbed *self,const char *code, ...);

double LuaCEmbed_get_evaluation_double(LuaCEmbed *self,const char *code, ...);

bool LuaCEmbed_get_evaluation_bool(LuaCEmbed *self,const char *code, ...);




//path: src/imports/../LuaCEmbed/globals/errors/fdeclare.errors.h
//mannaged by silver chain




int LuaCEmbed_ensure_global_type(LuaCEmbed *self, const char *name,int expected_type);


//path: src/imports/../LuaCEmbed/globals/getters/fdeclare.getters.h
//mannaged by silver chain



int LuaCEmbed_get_global_type(LuaCEmbed *self,const char *name);

lua_Integer  LuaCEmbed_get_global_long(LuaCEmbed *self,const char *name);

double LuaCEmbed_get_global_double(LuaCEmbed *self,const char *name);

bool LuaCEmbed_get_global_bool(LuaCEmbed *self,const char *name);

char * LuaCEmbed_get_global_string(LuaCEmbed *self,const char *name);

char * LuaCEmbed_get_global_raw_string(LuaCEmbed *self,const char *name,long *size);


LuaCEmbedTable * LuaCembed_new_anonymous_table(LuaCEmbed *self);

LuaCEmbedTable* LuaCEmbed_run_global_lambda(LuaCEmbed *self, const char *name, LuaCEmbedTable *args_to_call, int total_returns);

LuaCEmbedTable * LuaCembed_get_global_table(LuaCEmbed *self, const char *name);

LuaCEmbedTable * LuaCembed_new_global_table(LuaCEmbed *self, const char *name);


//path: src/imports/../LuaCEmbed/globals/setters/fdeclare.setters.h
//mannaged by silver chain


void LuaCEmbed_set_global_string(LuaCEmbed *self, const char *name, const  char *value);

void LuaCEmbed_set_global_raw_string(LuaCEmbed *self, const char *name, const  char *value,long size);

void LuaCEmbed_set_global_long(LuaCEmbed *self, const char *name, lua_Integer  value);

void LuaCEmbed_set_global_double(LuaCEmbed *self, const char *name, double value);

void LuaCEmbed_set_global_bool(LuaCEmbed *self, const char *name, bool value);

void LuaCEmbed_set_global_table(LuaCEmbed *self, const char *name, LuaCEmbedTable *table);


//path: src/imports/../LuaCEmbed/lib_props/fdeclare.lib_props.h
//mannaged by silver chain



void LuaCEmbed_set_long_lib_prop(LuaCEmbed *self,const char *name,lua_Integer value);

void LuaCEmbed_set_double_lib_prop(LuaCEmbed *self,const char *name,double value);

void LuaCEmbed_set_bool_lib_prop(LuaCEmbed *self,const char *name,bool value);

void LuaCEmbed_set_string_lib_prop(LuaCEmbed *self,const char *name,const char * value);

void LuaCEmbed_set_table_lib_prop(LuaCEmbed *self,const char *name,LuaCEmbedTable *value);

int private_LuaCEmbed_ensure_lib_prop_type(LuaCEmbed *self,const char *name,int expected_type);;


lua_Integer LuaCEmbed_get_long_lib_prop(LuaCEmbed *self,const char *name);

double LuaCEmbed_get_double_lib_prop(LuaCEmbed *self,const char *name);

bool LuaCEmbed_get_bool_lib_prop(LuaCEmbed *self,const char *name);

char * LuaCEmbed_get_string_lib_prop(LuaCEmbed *self,const char *name);

LuaCEmbedTable *LuaCEmbed_get_table_lib_prop(LuaCEmbed *self,const char *name);


//path: src/imports/../LuaCEmbed/lib_start/fdeclare.lib_start.h
//mannaged by silver chain


int  private_LuaCemb_internal_free(lua_State *L);

LuaCEmbed * newLuaCEmbedLib(lua_State *state);

int  LuaCembed_perform(LuaCEmbed *self);

int LuaCembed_send_self_as_lib(LuaCEmbed *self);

int LuaCembed_send_global_as_lib(LuaCEmbed *self,const char *global_name);


//path: src/imports/../LuaCEmbed/memory/fdeclare.memory.h
//mannaged by silver chain



long lua_cembed_used_memory = 0;
long  lua_cembed_memory_limit = -1;

void LuaCEmbed_set_memory_limit(LuaCEmbed *self, double limit);


static void *private_LuaCembed_custom_allocator(void *ud, void *ptr, size_t osize, size_t nsize) ;



//path: src/imports/../LuaCEmbed/table/table/append/fdeclare.append.h
//mannaged by silver chain



void  LuaCEmbedTable_append_table(LuaCEmbedTable *self, LuaCEmbedTable *table);


void  LuaCEmbedTable_append_evaluation(LuaCEmbedTable *self, const char *code, ...);

void  LuaCEmbedTable_append_string(LuaCEmbedTable *self,  const char *value);

void  LuaCEmbedTable_append_long(LuaCEmbedTable *self, lua_Integer  value);

void  LuaCEmbedTable_append_double(LuaCEmbedTable *self, double  value);

void  LuaCEmbedTable_append_bool(LuaCEmbedTable *self,  bool value);


//path: src/imports/../LuaCEmbed/table/table/basic/fdeclare.basic.h
//mannaged by silver chain



LuaCEmbedTable * private_newLuaCembedTable(LuaCEmbed *main_embed, const char *format, ...);

int  private_lua_cEmbed_unpack(LuaCEmbedTable *self,const char * previews_function);

void privateLuaCEmbedTable_free(LuaCEmbedTable *self);

void privateLuaCEmbedTable_free_setting_nill(LuaCEmbedTable *self);

//path: src/imports/../LuaCEmbed/table/table/destroy/fdeclare.destroy.h
//mannaged by silver chain



void LuaCembedTable_destroy_prop(LuaCEmbedTable *self, const char *name);

void LuaCEmbedTable_destroy_by_index(LuaCEmbedTable *self, long index);

//path: src/imports/../LuaCEmbed/table/table/errors/fdeclare.errors.h
//mannaged by silver chain


int privateLuaCEmbedTable_ensure_type_with_key(LuaCEmbedTable *self, const char *name, int expected_type);

int privateLuaCEmbedTable_ensure_type_with_index(LuaCEmbedTable *self, long index, int expected_type);


//path: src/imports/../LuaCEmbed/table/table/getters/index/fdeclare.index.h
//mannaged by silver chain



char *LuaCembedTable_get_key_by_index(LuaCEmbedTable *self, lua_Integer index);

bool LuaCembedTable_has_key_at_index(LuaCEmbedTable *self, lua_Integer index);

lua_Integer  LuaCEmbedTable_get_full_size(LuaCEmbedTable *self);

lua_Integer  LuaCEmbedTable_get_listable_size(LuaCEmbedTable *self);


lua_Integer  privateLuaCEmbedTable_convert_index(LuaCEmbedTable *self, private_lua_cembed_incremented_arg index);

int LuaCEmbedTable_get_type_by_index(LuaCEmbedTable *self, lua_Integer index);

lua_Integer LuaCEmbedTable_get_long_by_index(LuaCEmbedTable *self, lua_Integer index);

double LuaCEmbedTable_get_double_by_index(LuaCEmbedTable *self, lua_Integer index);

char * LuaCEmbedTable_get_string_by_index(LuaCEmbedTable *self, lua_Integer index);

char * LuaCEmbedTable_get_raw_string_by_index(LuaCEmbedTable *self, lua_Integer index, lua_Integer *size);

bool LuaCEmbedTable_get_bool_by_index(LuaCEmbedTable *self, lua_Integer index);


//path: src/imports/../LuaCEmbed/table/table/getters/keys/fdeclare.keys.h
//mannaged by silver chain




char*  LuaCembedTable_get_string_prop(LuaCEmbedTable *self, const char *name);

char*  LuaCembedTable_get_raw_string_prop(LuaCEmbedTable *self, const char *name,lua_Integer *size);

lua_Integer  LuaCembedTable_get_long_prop(LuaCEmbedTable *self, const char *name);

double  LuaCembedTable_get_double_prop(LuaCEmbedTable *self, const char *name);

bool  LuaCembedTable_get_bool_prop(LuaCEmbedTable *self, const char *name);

int  LuaCEmbedTable_get_type_prop(LuaCEmbedTable *self, const char *name);


//path: src/imports/../LuaCEmbed/table/table/setters/index/fdeclare.index.h
//mannaged by silver chain


void  LuaCEmbedTable_set_string_by_index(LuaCEmbedTable *self, lua_Integer index, const char *value);

void  LuaCEmbedTable_set_raw_string_by_index(LuaCEmbedTable *self, lua_Integer index, const char *value,lua_Integer size);

void  LuaCEmbedTable_set_long_by_index(LuaCEmbedTable *self, lua_Integer index, lua_Integer  value);

void  LuaCEmbedTable_set_double_by_index(LuaCEmbedTable *self, lua_Integer index, double  value);

void  LuaCEmbedTable_set_bool_by_index(LuaCEmbedTable *self, lua_Integer index, bool value);


void  LuaCEmbedTable_set_evaluation_by_index(LuaCEmbedTable *self, lua_Integer index, const char *code, ...);


//path: src/imports/../LuaCEmbed/table/table/setters/insert/fdeclare.insert.h
//mannaged by silver chain



void  private_LuaCEmbedTable_add_space(LuaCEmbedTable *self, lua_Integer formatted_index);

void  LuaCEmbedTable_insert_string_at_index(LuaCEmbedTable *self, lua_Integer index, const char *value);

void  LuaCEmbedTable_insert_bool_at_index(LuaCEmbedTable *self, lua_Integer index,bool value);

void  LuaCEmbedTable_insert_long_at_index(LuaCEmbedTable *self,lua_Integer  index,lua_Integer value);

void  LuaCEmbedTable_insert_double_at_index(LuaCEmbedTable *self, lua_Integer index,double value);

void  LuaCEmbedTable_insert_table_at_index(LuaCEmbedTable *self, lua_Integer index,LuaCEmbedTable *table);


//path: src/imports/../LuaCEmbed/table/table/setters/keys/fdeclare.keys.h
//mannaged by silver chain






void LuaCEmbedTable_set_method(LuaCEmbedTable *self, const char *name, LuaCEmbedResponse *(*callback)(LuaCEmbedTable  *self, LuaCEmbed *args));

void  LuaCEmbedTable_set_string_prop(LuaCEmbedTable *self, const char *name, const char *value);

void  LuaCEmbedTable_set_raw_string_prop(LuaCEmbedTable *self , const char *name, const char *value,lua_Integer size);

void  LuaCEmbedTable_set_long_prop(LuaCEmbedTable *self, const char *name,lua_Integer   value);

void  LuaCEmbedTable_set_double_prop(LuaCEmbedTable *self, const char *name, double  value);

void  LuaCEmbedTable_set_bool_prop(LuaCEmbedTable *self, const char *name, bool value);


void  LuaCEmbedTable_set_evaluation_prop(LuaCEmbedTable *self, const char *name, const char *code, ...);


//path: src/imports/../LuaCEmbed/table/table/sub_tables/index/fdeclare.index.h
//mannaged by silver chain



LuaCEmbedTable  *LuaCEmbedTable_new_sub_table_appending(LuaCEmbedTable *self);


LuaCEmbedTable  *LuaCEmbedTable_get_sub_table_by_index(LuaCEmbedTable *self, lua_Integer index);


void LuaCEmbedTable_set_sub_table_by_index(LuaCEmbedTable *self, lua_Integer index,LuaCEmbedTable *sub_table);


//path: src/imports/../LuaCEmbed/table/table/sub_tables/key/fdeclare.key.h
//mannaged by silver chain


LuaCEmbedTable  *LuaCEmbedTable_new_sub_table_by_key(LuaCEmbedTable *self, const char *name);

LuaCEmbedTable  *LuaCEmbedTable_get_sub_table_by_key(LuaCEmbedTable *self, const char *name);

void LuaCEmbedTable_set_sub_table_prop(LuaCEmbedTable *self, const char *name, LuaCEmbedTable *sub_table);

//path: src/imports/../LuaCEmbed/table/table_array/fdeclare.table_array.h
//mannaged by silver chain



typedef struct {

    LuaCEmbedTable **tables;
    long size;

}privateLuaCEmbedTableArray;

privateLuaCEmbedTableArray *newprivateLuaCEmbedTableArray();

void privateLuaCEmbedTableArray_append(privateLuaCEmbedTableArray *self,LuaCEmbedTable *element);

LuaCEmbedTable  *privateLuaCEmbedTableArray_find_by_global_name(privateLuaCEmbedTableArray *self, const char *name);

LuaCEmbedTable  *privateLuaCEmbedTableArray_find_by_prop_name(privateLuaCEmbedTableArray *self, const char *name);

LuaCEmbedTable  *privateLuaCEmbedTableArray_find_by_internal_index(privateLuaCEmbedTableArray *self, lua_Integer internal_index);

void  privateLuaCEmbedTableArray_free(privateLuaCEmbedTableArray *self);


//path: src/imports/../LuaCEmbedResponse/fdeclare.LuaCEmbedResponse.h
//mannaged by silver chain


LuaCEmbedResponse *private_LuaCEmbedReturn_raw();


LuaCEmbedResponse * LuaCEmbed_send_str(const char *text);

LuaCEmbedResponse * LuaCEmbed_send_raw_string(const char *text,lua_Integer size);

LuaCEmbedResponse * LuaCEmbed_send_str_reference( const char *text);

LuaCEmbedResponse * LuaCEmbed_send_raw_string_reference(const char *text,lua_Integer size);

LuaCEmbedResponse * LuaCEmbed_send_error(const char *format,...);


LuaCEmbedResponse * LuaCEmbed_send_evaluation(const char *code);





LuaCEmbedResponse  * LuaCEmbed_send_double(double value);

LuaCEmbedResponse  * LuaCEmbed_send_bool(bool value);


LuaCEmbedResponse  * LuaCEmbed_send_long(lua_Integer value);


void private_LuaCEmbedResponse_free(LuaCEmbedResponse  *self);


LuaCEmbedResponse * LuaCEmbed_send_table(LuaCEmbedTable *table);


LuaCEmbedResponse * LuaCEmbed_send_multi_return(LuaCEmbedTable *table);


//path: src/imports/../extra/fdeclare.extra.h
//mannaged by silver chain



const char * LuaCembed_convert_arg_code(int arg_code);


char * private_LuaCembed_format_vaarg(const char *expresion, va_list args);

char * private_LuaCembed_format(const char *expresion, ...);




//path: src/imports/../namespace/LuaCEmbedNamespace/fdeclare.LuaCEmbedNamespace.h
//mannaged by silver chain



LuaCEmbedNamespace newLuaCEmbedNamespace();


//path: src/imports/../namespace/args/fdeclare.args.h
//mannaged by silver chain


LuaCembedArgsModule newLuaCembedArgsModule();


//path: src/imports/../namespace/globals/fdeclare.globals.h
//mannaged by silver chain


LuaCEmbedGlobalModule newLuaCEmbedGlobalModule();


//path: src/imports/../namespace/response/fdeclare.response.h
//mannaged by silver chain



LuaCEmbedResponseModule newLuaCEmbedResponseModule();


//path: src/imports/../namespace/table/fdeclare.table.h
//mannaged by silver chain



LuaCembedTableModule newLuaCembedTableModule();


//path: src/imports/../namespace/types/fdeclare.types.h
//mannaged by silver chain




LuaCEmbedTypeModule newLuaCEmbedTypeModule();


#endif


#ifndef privateluacembed_fdefine
#define privateluacembed_fdefine
//path: src/imports/../LuaCEmbed/args/args_error/fdefine.args_error.c
//mannaged by silver chain



int LuaCEmbed_ensure_arg_exist(LuaCEmbed *self, private_lua_cembed_incremented_arg index){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    if(index > self->total_args){

        privateLuaCEmbed_raise_error_not_jumping(self, PRIVATE_LUA_CEMBED_ARG_NOT_PROVIDED,index,self->current_function);
        return LUA_CEMBED_GENERIC_ERROR;
    }
    return LUA_CEMBED_OK;

}

int private_LuaCEmbed_ensure_top_stack_arg_type(LuaCEmbed *self, int index,int arg_type) {
    PRIVATE_LUA_CEMBED_PROTECT_NUM
    int type = lua_type(self->state, -1);
    if (type != arg_type) {

        privateLuaCEmbed_raise_error_not_jumping(self,
             PRIVATE_LUA_CEMBED_ARG_WRONG_TYPE,
             index + LUA_CEMBED_INDEX_DIF,
             self->current_function,
             LuaCembed_convert_arg_code(type),
             LuaCembed_convert_arg_code(arg_type)
        );
        return LUA_CEMBED_GENERIC_ERROR;
    }

    return LUA_CEMBED_OK;

}



//path: src/imports/../LuaCEmbed/args/basic/fdefine.basic.c
//mannaged by silver chain


lua_Integer  privateLuaCEmbed_put_arg_on_top(LuaCEmbed *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_PROTECT_NUM
    lua_Integer  formatted_index = index + LUA_CEMBED_INDEX_DIF;

    if(formatted_index > self->total_args){
        privateLuaCEmbed_raise_error_not_jumping(self,PRIVATE_LUA_CEMBED_ARG_NOT_PROVIDED,formatted_index,self->current_function);
        return LUA_CEMBED_GENERIC_ERROR;
     }

    char *formated_arg = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARGS_,self->stack_leve,formatted_index-1);
    lua_getglobal(self->state,formated_arg);
    int type = lua_type(self->state,-1);
    if(type == LUA_CEMBED_NOT_FOUND || type == LUA_CEMBED_NIL){
        privateLuaCEmbed_raise_error_not_jumping(self,PRIVATE_LUA_CEMBED_ARG_NOT_PROVIDED,formatted_index,self->current_function);
        free(formated_arg);
        return LUA_CEMBED_GENERIC_ERROR;
    }

    free(formated_arg);
    return LUA_OK;
}

lua_Integer  LuaCEmbed_get_total_args(LuaCEmbed *self){
    return self->total_args;
}



lua_Integer  LuaCEmbed_get_arg_type(LuaCEmbed *self,lua_Integer index){

    lua_Integer  formatted_index = index + LUA_CEMBED_INDEX_DIF;

    if(formatted_index > self->total_args){
        return LUA_CEMBED_NIL;
    }

    char *formated_arg = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARGS_,self->stack_leve,formatted_index-1);
    lua_getglobal(self->state,formated_arg);
    int type = lua_type(self->state, -1);
    free(formated_arg);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    return type;
}


lua_Integer LuaCEmbed_get_long_arg(LuaCEmbed *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_PROTECT_NUM


    if(privateLuaCEmbed_put_arg_on_top(self,index)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
       return (lua_Integer )LUA_CEMBED_NOT_FOUND;
    }

    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_NUMBER)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
        return (lua_Integer )LUA_CEMBED_NOT_FOUND;
    }

    lua_Integer result =  lua_tointeger(self->state,-1);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    return result;
}


double LuaCEmbed_get_double_arg(LuaCEmbed *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    if(privateLuaCEmbed_put_arg_on_top(self,index)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
       return LUA_CEMBED_NOT_FOUND;
    }
    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_NUMBER)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
        return LUA_CEMBED_NOT_FOUND;
    }
    double result = lua_tonumber(self->state,-1);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    return result;
}

bool LuaCEmbed_get_bool_arg(LuaCEmbed *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_PROTECT_BOOL

    if(privateLuaCEmbed_put_arg_on_top(self,index)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
       return LUA_CEMBED_NOT_FOUND;
    }
    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_BOOL)){
    PRIVATE_LUA_CEMBED_CLEAR_STACK
        return LUA_CEMBED_NOT_FOUND;
    }
    bool result = lua_toboolean(self->state,-1);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    return result;
}


char * LuaCEmbed_get_str_arg(LuaCEmbed *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_PROTECT_NULL
    PRIVATE_LUA_CEMBED_CLEAR_STACK

    if(privateLuaCEmbed_put_arg_on_top(self,index)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
        return NULL;
    }

    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_STRING)){
        return NULL;
    }
    char *result =  (char*)lua_tostring(self->state,-1);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    return result;
}
char * LuaCEmbed_get_raw_str_arg(LuaCEmbed *self,lua_Integer *size, lua_Integer index){
    PRIVATE_LUA_CEMBED_PROTECT_NULL
    PRIVATE_LUA_CEMBED_CLEAR_STACK

    if(privateLuaCEmbed_put_arg_on_top(self,index)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
       return NULL;
    }

    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_STRING)){
        return NULL;
    }
    char *result =  (char*)lua_tolstring(self->state,-1,(size_t*)size);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    return result;
}

LuaCEmbedTable  * LuaCEmbed_get_arg_table(LuaCEmbed *self,lua_Integer index){
    PRIVATE_LUA_CEMBED_PROTECT_NULL

    if(privateLuaCEmbed_put_arg_on_top(self,index)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
       return NULL;
    }

    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_TABLE)){
        return NULL;
    }

    int formatted_index = index + LUA_CEMBED_INDEX_DIF;
    char *formated_arg = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARGS_,self->stack_leve,formatted_index-1);
    LuaCEmbedTable  *created = LuaCembed_get_global_table(self,formated_arg);
    free(formated_arg);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    return created;

}

LuaCEmbedTable* LuaCEmbed_run_args_lambda(LuaCEmbed *self, lua_Integer index, LuaCEmbedTable *args_to_call, lua_Integer total_returns){

    long  formatted_index = index + LUA_CEMBED_INDEX_DIF;
    char *formatted_arg = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARGS_,self->stack_leve,formatted_index-1);

    if(privateLuaCEmbed_put_arg_on_top(self,index)){
        PRIVATE_LUA_CEMBED_CLEAR_STACK
        free(formatted_arg);
       return NULL;
    }

    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_FUNCTION)){
        free(formatted_arg);
        PRIVATE_LUA_CEMBED_CLEAR_STACK
        return  NULL;
    }

    int total_args = private_lua_cEmbed_unpack(args_to_call,formatted_arg);
    if(lua_pcall(self->state,total_args,total_returns,0)){

        privateLuaCEmbed_raise_error_not_jumping(self, lua_tostring(self->state,-1));
        free(formatted_arg);

        PRIVATE_LUA_CEMBED_CLEAR_STACK
        return  NULL;
    }


    for(int i = 0; i < total_returns; i++){
        char *formatted = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN_,self->stack_leve,i);
        int position = (i +1) * -1;
        lua_pushvalue(self->state,position);
        lua_setglobal(self->state,formatted);
        free(formatted);
    }



    LuaCEmbedTable  *result = LuaCembed_new_anonymous_table(self);
    for(int i = 0; i < total_returns; i++){
        lua_getglobal(self->state,result->global_name);
        lua_pushinteger(self->state,i+1);
        char *formatted = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN_,self->stack_leve,i);
        lua_getglobal(self->state,formatted);
        lua_settable(self->state,-3);
        free(formatted);
    }
    free(formatted_arg);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    return result;

}


//path: src/imports/../LuaCEmbed/args/evaluation/fdefine.evaluation.c
//mannaged by silver chain



lua_Integer private_LuaCembed_run_code_with_args(LuaCEmbed *self,lua_Integer index,const char *code,va_list args){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    int formatted_index = index + LUA_CEMBED_INDEX_DIF;

    if(LuaCEmbed_ensure_arg_exist(self,formatted_index)){
        return LUA_CEMBED_GENERIC_ERROR;
    }

    char *formated_expresion = private_LuaCembed_format_vaarg(code,args);

    char *buffer = private_LuaCembed_format(
            PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE,
            PRIVATE_LUA_CEMBED_EVALUATION_NAME,
            formated_expresion
    );

    if(LuaCEmbed_evaluate(self, buffer)){
        free(formated_expresion);
        free(buffer);
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    free(buffer);

    lua_getglobal(self->state,PRIVATE_LUA_CEMBED_EVALUATION_NAME);

    int type = lua_type(self->state,-1);

    if(type != LUA_CEMBED_FUNCTION){
        privateLuaCEmbed_raise_error_not_jumping(self,PRIVATE_LUA_CEMBED_CODE_ITS_NOT_A_FUNCTION,formated_expresion);
        free(formated_expresion);
        return LUA_CEMBED_GENERIC_ERROR;
    }

    free(formated_expresion);
    lua_getglobal(self->state,PRIVATE_LUA_CEMBED_EVALUATION_NAME);
    privateLuaCEmbed_put_arg_on_top(self,index);

    if(lua_pcall(self->state,1,1,0)){
        const char *generated_error = lua_tostring(self->state,-1);
        privateLuaCEmbed_raise_error_not_jumping(self, generated_error);
    }
    return LUA_CEMBED_OK;
}

lua_Integer privateLuaCembed_ensure_arg_evaluation_type(LuaCEmbed *self,lua_Integer index,lua_Integer expected_type){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    int actual_type = lua_type(self->state,-1);
    if(actual_type== expected_type){
        return LUA_CEMBED_OK;
    }

    privateLuaCEmbed_raise_error_not_jumping(self,
             PRIVATE_LUA_CEMBED_RESULT_EVALUATION_ARG_WRONG_TYPE,
             self->current_function,
             index,
             LuaCembed_convert_arg_code(actual_type),
             LuaCembed_convert_arg_code(expected_type)
     );
    return LUA_CEMBED_GENERIC_ERROR;
}
lua_Integer LuaCEmbed_get_type_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    lua_Integer possible_error = private_LuaCembed_run_code_with_args(self,index,code,args);
    va_end(args);
    if(possible_error){
        return possible_error;
    }
    return lua_type(self->state,-1);
}
lua_Integer LuaCEmbed_generate_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCembed_run_code_with_args(self,index,code,args);
    va_end(args);
    if(possible_error){
        return possible_error;
    }
    return LUA_CEMBED_OK;
}
lua_Integer LuaCEmbed_get_long_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCembed_run_code_with_args(self,index,code,args);
    va_end(args);
    if(possible_error){
        return possible_error;
    }
    if(privateLuaCembed_ensure_arg_evaluation_type(self,index,LUA_CEMBED_NUMBER)){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    return lua_tointeger(self->state,-1);

}


double LuaCEmbed_get_double_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const char *code,...){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCembed_run_code_with_args(self,index,code,args);
    va_end(args);
    if(possible_error){
        return possible_error;
    }
    if(privateLuaCembed_ensure_arg_evaluation_type(self,index,LUA_CEMBED_NUMBER)){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    return (double ) lua_tonumber(self->state,-1);

}

bool LuaCEmbed_get_bool_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const  char *code,...){
    PRIVATE_LUA_CEMBED_PROTECT_BOOL
    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCembed_run_code_with_args(self,index,code,args);
    va_end(args);
    if(possible_error){
        return possible_error;
    }
    if(privateLuaCembed_ensure_arg_evaluation_type(self,index,LUA_CEMBED_NUMBER)){
        return  false;
    }
    return lua_toboolean(self->state,-1);

}

char* LuaCEmbed_get_string_arg_clojure_evalation(LuaCEmbed *self,lua_Integer index,const  char *code,...){
    PRIVATE_LUA_CEMBED_PROTECT_NULL
    va_list args;
    va_start(args,code);
    lua_Integer possible_error = private_LuaCembed_run_code_with_args(self,index,code,args);
    va_end(args);
    if(possible_error){
        return  NULL;
    }
    if(privateLuaCembed_ensure_arg_evaluation_type(self,index,LUA_CEMBED_NUMBER)){
        return  NULL;
    }
    return (char*)lua_tostring(self->state,-1);

}


//path: src/imports/../LuaCEmbed/basic/fdefine.basic.c
//mannaged by silver chain



LuaCEmbed * newLuaCEmbedEvaluation(){
    LuaCEmbed  *self = (LuaCEmbed*) malloc(sizeof (LuaCEmbed));
    *self = (LuaCEmbed){0};
    self->state = luaL_newstate();

    self->global_tables = (void*)newprivateLuaCEmbedTableArray();

    return self;
}
void LuaCEmbed_load_lib_from_c(LuaCEmbed *self,int (*callback)(lua_State *l),const char *name){
    if(LuaCEmbed_has_errors(self)){
        return;
    }

    lua_pushcfunction(self->state, callback);
    int result  = lua_pcall(self->state,0,1,0);
    if(result){
        privateLuaCEmbed_raise_error_not_jumping(self,lua_tostring(self->state,-1));
        lua_settop(self->state,0);
        return;
    }
    lua_setglobal(self->state,name);
    lua_settop(self->state,0);
}
void LuaCEmbed_load_native_libs(LuaCEmbed *self) {
    luaL_openlibs(self->state);
}




void LuaCembed_set_delete_function(LuaCEmbed *self,void (*delelte_function)(struct  LuaCEmbed *self)){
    self->delete_function = delelte_function;
}




void LuaCEmbed_set_timeout(int seconds){
    lua_cembed_timeout = seconds;
}


void * privateLuaCEmbed_get_current_table_array(LuaCEmbed *self){
    if(self->current_function){
       return  self->func_tables;
    }
    return self->global_tables;
}







void privata_LuaCEmbed_increment_stack_(LuaCEmbed *self){
    lua_getglobal(self->state,PRIVATE_LUA_CEMBED_STACK_LEVEL);
    int value = 0;
    if(lua_type(self->state,-1) == LUA_CEMBED_NUMBER){
        value =  lua_tonumber(self->state,-1);
    }
    lua_Integer new_value = value+1;
    lua_pushinteger(self->state,new_value);
    lua_setglobal(self->state,PRIVATE_LUA_CEMBED_STACK_LEVEL);
    self->stack_leve =new_value;
}


void  privata_LuaCEmbed_decrement_stack(LuaCEmbed *self){
    lua_getglobal(self->state,PRIVATE_LUA_CEMBED_STACK_LEVEL);
    int value = 0;
    if(lua_type(self->state,-1) == LUA_CEMBED_NUMBER){
        value =  lua_tointeger(self->state,-1);
    }
    if(value> 0){
        int new_value = value -1;
        lua_pushinteger(self->state,new_value);
        lua_setglobal(self->state,PRIVATE_LUA_CEMBED_STACK_LEVEL);
        self->stack_leve = new_value;
    }

}

void privateLuaCEmbd_get_field_protected(LuaCEmbed *self,const char *name){
    bool old_field_proection = self->field_protection;
    self->field_protection = true;
    lua_getfield(self->state,-1,name);
    self->field_protection = old_field_proection;
}

void LuaCEmbed_free(LuaCEmbed *self){
    privateLuaCEmbedTableArray_free((privateLuaCEmbedTableArray*)self->global_tables);
    if(!self->is_lib){ //se for do próprio lua, o lua cuidará de limpar
        lua_close(self->state); // Fecha o estado Lua
    }
    if(self->error_msg){
        free(self->error_msg);
    }
    if(self->main_lib_table){
        free(self->main_lib_table);
    }
    free(self);
}


//path: src/imports/../LuaCEmbed/callback_handle/fdefine.callback_handle.c
//mannaged by silver chain



int privateLuaCEmbed_main_callback_handler(lua_State  *L){

    bool is_a_method = lua_toboolean(L, lua_upvalueindex(1));
    bool is_a_function = !is_a_method;
    LuaCEmbedResponse *possible_return = NULL;
    LuaCEmbed  *self = (LuaCEmbed*)lua_touserdata(L,lua_upvalueindex(2));
    int old_total_args = self->total_args;
    self->total_args =  lua_gettop(self->state);
    privata_LuaCEmbed_increment_stack_(self);

    lua_getglobal(self->state,PRIVATE_LUA_CEMBED_STACK_LEVEL);
    self->stack_leve = (int)lua_tonumber(self->state,-1);

    for(int i  = 0; i < self->total_args; i++){
        char *formated_arg = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARGS_,self->stack_leve,i);
        lua_pushvalue(L,i+1);
        lua_setglobal(L,formated_arg);
        free(formated_arg);
    }

    const char *func_name =  lua_tostring(L,lua_upvalueindex(3));
    self->current_function = func_name;
    void *old_funct_tables = self->func_tables;
    self->func_tables = (void*)newprivateLuaCEmbedTableArray();
    if(is_a_method){
        LuaCEmbedResponse *(*method_callback)(LuaCEmbedTable *tb, LuaCEmbed *self);

        //equivalent of PRIVATE_LUA_CEMBED_SELFNAME = index[4]
        lua_pushvalue(L, lua_upvalueindex(4));
        lua_setglobal(L,PRIVATE_LUA_CEMBED_SELFNAME);

        LuaCEmbedTable  *table = private_newLuaCembedTable(self, PRIVATE_LUA_CEMBED_SELFNAME);
        method_callback = (LuaCEmbedResponse *(*)(LuaCEmbedTable *tb, LuaCEmbed *self))lua_touserdata(L, lua_upvalueindex(5));
        possible_return = method_callback(table,self);
        privateLuaCEmbedTable_free(table);
    }

    if(is_a_function){
        LuaCEmbedResponse *(*function_callback)(LuaCEmbed *self);
        function_callback = (LuaCEmbedResponse *(*)(LuaCEmbed *self))lua_touserdata(L, lua_upvalueindex(4));
        possible_return = function_callback(self);
    }

    privateLuaCEmbedTableArray_free((privateLuaCEmbedTableArray*)self->func_tables);
    self->func_tables = old_funct_tables;
    self->total_args = old_total_args;
    privata_LuaCEmbed_decrement_stack(self);
    PRIVATE_LUA_CEMBED_CLEAR_STACK
    self->current_function = NULL;

    if(possible_return==NULL){
        return PRIVATE_LUACEMBED_NO_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_ERROR_RESPONSE){

        if(self->field_protection){
            private_LuaCEmbedResponse_free(possible_return);
            if(self->error_msg){
                free(self->error_msg);
                self->error_msg = NULL;
            }

            return PRIVATE_LUACEMBED_NO_RETURN;
        }

        lua_pushstring(L, possible_return->string_val);
        privateLuaCEmbed_raise_error_not_jumping(self,possible_return->string_val);
        private_LuaCEmbedResponse_free(possible_return);
        lua_error(L);
        return PRIVATE_LUACEMBED_NO_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_LONG_RESPONSE){
        lua_pushinteger(L, possible_return->int_val);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_BOOL_RESPONSE){
        lua_pushboolean(L, (bool)possible_return->int_val);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_DOUBLE_RESPONSE){
        lua_pushnumber(L, possible_return->double_val);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_STRING_RESPONSE){
        lua_pushlstring(self->state,possible_return->string_val,possible_return->string_size);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }


    if(possible_return->type == PRIVATE_LUA_CEMBED_TABLE_RESPONSE){
        lua_getglobal(self->state, PRIVATE_LUA_CEMBED_TABLE_RETURN);
        lua_pushvalue(self->state,-1);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_MULTI_RESPONSE){
        LuaCEmbedTable  *table = private_newLuaCembedTable(self,  PRIVATE_LUA_CEMBED_TABLE_RETURN);
        int size =private_lua_cEmbed_unpack(table,NULL);
        private_LuaCEmbedResponse_free(possible_return);
        privateLuaCEmbedTable_free(table);

        return  size;
    }


    if(possible_return->type == PRIVATE_LUA_CEMBED_EVALUATION){
        char *formated_function =private_LuaCembed_format(
                PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE,
                PRIVATE_LUA_CEMBED_EVALUATION_NAME,
                possible_return->string_val
        );

        int error_code = LuaCEmbed_evaluate(self, formated_function);
        free(formated_function);

        if(error_code){
            private_LuaCEmbedResponse_free(possible_return);
            return PRIVATE_LUACEMBED_NO_RETURN;
        }

        lua_getglobal(self->state, PRIVATE_LUA_CEMBED_EVALUATION_NAME);
        int v_type = lua_type(self->state,-1);
        if(v_type == LUA_CEMBED_FUNCTION){
            const int TOTAL_ARGS =0;
            const int TOTAL_RETURNS =1;
            if(lua_pcall(self->state,TOTAL_ARGS,TOTAL_RETURNS,0)){
                private_LuaCEmbedResponse_free(possible_return);
                return PRIVATE_LUACEMBED_NO_RETURN;
            }
        }

        lua_pushvalue(self->state,-1);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;

    }

    private_LuaCEmbedResponse_free(possible_return);
    return PRIVATE_LUACEMBED_NO_RETURN;

}


void private_LuaCEmbed_add_lib_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args),bool global_functions ){

    char *main_lib_table = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME__,self->lib_identifier);

    //get the table
    lua_getglobal(self->state,main_lib_table);
    lua_pushvalue(self->state,-1);
    //set the function name
    lua_pushstring(self->state,callback_name);

    //creating the clojure

    //creating the clojure
    lua_pushboolean(self->state,false);//is a method
    lua_pushlightuserdata(self->state,(void*)self); //self
    lua_pushstring(self->state,callback_name);//calback name
    lua_pushlightuserdata(self->state,(void*)callback);//calback

    lua_pushcclosure(self->state,privateLuaCEmbed_main_callback_handler,4);


    lua_settable(self->state,-3);
    if(global_functions){
        //it points the function to a global function
        //like: callback = private_lua_c_embed_main_lib_table.callback
        lua_getglobal(self->state, main_lib_table);
        lua_getfield(self->state,-1,callback_name);
        lua_setglobal(self->state, callback_name);
    }

    lua_settop(self->state, 0);
    free(main_lib_table);

}

void private_LuaCEmbed_add_evaluation_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) ){
    PRIVATE_LUA_CEMBED_PROTECT_VOID

    //creating the clojure
    lua_pushboolean(self->state,false);//is a method
    lua_pushlightuserdata(self->state,(void*)self); //self
    lua_pushstring(self->state,callback_name);//calback name
    lua_pushlightuserdata(self->state,(void*)callback);//calback

    lua_pushcclosure(self->state,privateLuaCEmbed_main_callback_handler,4);
    lua_setglobal(self->state, callback_name);
    lua_settop(self->state, 0);

}


void LuaCEmbed_add_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) ){
    PRIVATE_LUA_CEMBED_PROTECT_VOID

    if(self->is_lib){
        private_LuaCEmbed_add_lib_callback(self,callback_name,callback,false);
        return;
    }
    private_LuaCEmbed_add_evaluation_callback(self,callback_name,callback);

}

void LuaCEmbed_add_global_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args)){
    PRIVATE_LUA_CEMBED_PROTECT_VOID

    if(self->is_lib){
        private_LuaCEmbed_add_lib_callback(self,callback_name,callback,true);
        return;
    }
    private_LuaCEmbed_add_evaluation_callback(self,callback_name,callback);
}


//path: src/imports/../LuaCEmbed/errors/fdefine.errors.c
//mannaged by silver chain



char * LuaCEmbed_get_error_message(LuaCEmbed *self){
    if(!self){
        return NULL;
    }

    return self->error_msg;
}
void LuaCEmbed_clear_errors(LuaCEmbed *self){
    if(!self){
        return;
    }

    if(self->error_msg){
        free(self->error_msg);
        self->error_msg = NULL;
    }
}

void privateLuaCEmbed_raise_error_not_jumping(LuaCEmbed *self, const char *error_msg, ...){

    if(LuaCEmbed_has_errors(self)){
        return;
    }

    va_list args;
    va_start(args,error_msg);
    self->error_msg = private_LuaCembed_format_vaarg(error_msg, args);
    va_end(args);

}



bool LuaCEmbed_has_errors(LuaCEmbed *self){
    if(!self){
        return true;
    }

    if(self->error_msg){
        return  true;
    }

    return false;
}



void LuaCEmbed_dangerous_raise_error_jumping(LuaCEmbed *self,const char *error_msg,...){
    if(LuaCEmbed_has_errors(self)){
        return;
    }

    va_list args;
    va_start(args,error_msg);
    char *error_formmated = private_LuaCembed_format_vaarg(error_msg, args);
    va_end(args);
    lua_pushstring(self->state, error_formmated);
    free(error_formmated);
    lua_error(self->state);
}

void LuaCEmbed_dangerous_raise_self_error_jumping(LuaCEmbed *self){
    if(!LuaCEmbed_has_errors(self)){
        return;
    }
    char *msg = LuaCEmbed_get_error_message(self);
    lua_pushstring(self->state,msg);
    LuaCEmbed_clear_errors(self);
    lua_error(self->state);
}


//path: src/imports/../LuaCEmbed/evaluation/fdefine.evaluation.c
//mannaged by silver chain



#ifdef _WIN32
VOID CALLBACK TimerHandler(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {

    privateLuaCEmbed_raise_error_not_jumping(global_current_lua_embed_object, PRIVATE_LUA_CEMBED_TIMEOUT_ERROR);
    lua_pushstring(global_current_lua_embed_object->state,PRIVVATE_LUA_CEMBED_TIMEOUT_ERROR);
    lua_error(global_current_lua_embed_object->state);
}

#else 

void private_LuaCembed_handle_timeout(int signum) {

    privateLuaCEmbed_raise_error_not_jumping(global_current_lua_embed_object, PRIVATE_LUA_CEMBED_TIMEOUT_ERROR);
    lua_pushstring(global_current_lua_embed_object->state,PRIVVATE_LUA_CEMBED_TIMEOUT_ERROR);
    lua_error(global_current_lua_embed_object->state);
}

#endif 



int privateLuaCEmbed_start_func_evaluation(lua_State *state){

    int evaluation_type = lua_tointeger(state, lua_upvalueindex(1));
    char *text_value = (char*)lua_touserdata(state,lua_upvalueindex(2));
    LuaCEmbed  *self = (LuaCEmbed*)lua_touserdata(state,lua_upvalueindex(3));
    global_current_lua_embed_object = self;
        #ifdef _WIN32
            if (lua_cembed_timeout > 0) {
                SetTimer(NULL, 0,lua_cembed_timeout > 0 * 1000, TimerHandler);
            }
        #else
            if (lua_cembed_timeout > 0) {
                signal(SIGALRM, private_LuaCembed_handle_timeout);
                alarm(lua_cembed_timeout);
            }
        #endif

    int error  = 0;
    if(evaluation_type == PRIVATE_LUA_EMBED_FILE_EVALUATION_TYPE){
        error =luaL_dofile(self->state,text_value);
    }
    if(evaluation_type == PRIVATE_LUA_EMBED_STRING_EVALUATION_TYPE){
        error = luaL_dostring(self->state,text_value);
    }
    if(error){
        privateLuaCEmbed_raise_error_not_jumping(self,lua_tostring(self->state,-1));
    }
    lua_pushinteger(self->state,error);

    return 1;

}
int LuaCEmbed_evaluate(LuaCEmbed *self, const char *code, ...){

    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    char * formated_expresion = private_LuaCembed_format_vaarg(code,args);
    va_end(args);
    lua_pushinteger(self->state,PRIVATE_LUA_EMBED_STRING_EVALUATION_TYPE);
    lua_pushlightuserdata(self->state,(void*)formated_expresion); //code
    lua_pushlightuserdata(self->state,(void*)self); //code
    lua_pushcclosure(self->state,privateLuaCEmbed_start_func_evaluation,3);
    lua_pcall(self->state,0,1,0);
    int error = lua_tointeger(self->state,-1);
    free(formated_expresion);

    return error;

}

int LuaCEmbed_evaluete_file(LuaCEmbed *self, const char *file){
    PRIVATE_LUA_CEMBED_PROTECT_NUM


    lua_pushinteger(self->state,PRIVATE_LUA_EMBED_FILE_EVALUATION_TYPE);
    lua_pushlightuserdata(self->state,(void*)file); //code
    lua_pushlightuserdata(self->state,(void*)self); //code
    lua_pushcclosure(self->state,privateLuaCEmbed_start_func_evaluation,3);
    lua_pcall(self->state,0,1,0);
    int error = lua_tointeger(self->state,-1);
    return error;

}

int private_LuaCEmbed_evaluate_puting_on_top_of_stack(LuaCEmbed *self,const char *code, va_list args){

    char * formated_expresion =private_LuaCembed_format_vaarg(code,args);

    char * buffer =private_LuaCembed_format(
            PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE,
            PRIVATE_LUA_CEMBED_EVALUATION_NAME,
            formated_expresion
    );

    if(LuaCEmbed_evaluate(self, buffer)){
        free(formated_expresion);
        free(buffer);
        return  LUA_CEMBED_GENERIC_ERROR;
    }

    lua_getglobal(self->state,PRIVATE_LUA_CEMBED_EVALUATION_NAME);

    int type = lua_type(self->state,-1);

    if(type == LUA_CEMBED_FUNCTION){
        if(lua_pcall(self->state,0,1,0)){
            const char *generated_error = lua_tostring(self->state,-1);
            privateLuaCEmbed_raise_error_not_jumping(self, generated_error);
        }
    }
    free(formated_expresion);
    free(buffer);
    return LUA_CEMBED_OK;

}
int private_LuaCEmbed_ensure_evaluation_type(LuaCEmbed *self,int type){

    int actual_type = lua_type(self->state,-1);
    if(actual_type== type){
        return LUA_CEMBED_OK;
    }


    privateLuaCEmbed_raise_error_not_jumping(self,
         PRIVATE_LUA_CEMBED_RESULT_EVALUATION_WRONG_TYPE,
         LuaCembed_convert_arg_code(actual_type),
         LuaCembed_convert_arg_code(type)
    );

    return LUA_CEMBED_GENERIC_ERROR;
}

char * LuaCEmbed_get_evaluation_string(LuaCEmbed *self,const char *code, ...){
    PRIVATE_LUA_CEMBED_PROTECT_NULL

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCEmbed_evaluate_puting_on_top_of_stack(self,code,args);
    va_end(args);
    if(possible_error){
        return  NULL;
    }

    if(private_LuaCEmbed_ensure_evaluation_type(self,LUA_CEMBED_STRING)){
        return NULL;
    }
    return (char*)lua_tostring(self->state,-1);

}


int  LuaCEmbed_get_evaluation_type(LuaCEmbed *self,const char *code, ...){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCEmbed_evaluate_puting_on_top_of_stack(self,code,args);
    va_end(args);
    if(possible_error){
        return  LUA_CEMBED_GENERIC_ERROR;
    }

    return lua_type(self->state,-1);
}


long LuaCEmbed_get_evaluation_table_size(LuaCEmbed *self,const char *code, ...){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCEmbed_evaluate_puting_on_top_of_stack(self,code,args);
    va_end(args);
    if(possible_error){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    if(private_LuaCEmbed_ensure_evaluation_type(self,LUA_CEMBED_TABLE)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    

    return (long)lua_rawlen(self->state,-1);
}



long long  LuaCEmbed_get_evaluation_long(LuaCEmbed *self,const char *code, ...){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCEmbed_evaluate_puting_on_top_of_stack(self,code,args);
    va_end(args);
    if(possible_error){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    if(private_LuaCEmbed_ensure_evaluation_type(self,LUA_CEMBED_NUMBER)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    return (long)lua_tonumber(self->state,-1);

}

double LuaCEmbed_get_evaluation_double(LuaCEmbed *self,const char *code, ...){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCEmbed_evaluate_puting_on_top_of_stack(self,code,args);
    va_end(args);
    if(possible_error){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    if(private_LuaCEmbed_ensure_evaluation_type(self,LUA_CEMBED_NUMBER)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    return (double)lua_tonumber(self->state,-1);
}

bool LuaCEmbed_get_evaluation_bool(LuaCEmbed *self,const char *code, ...){
    PRIVATE_LUA_CEMBED_PROTECT_BOOL

    va_list args;
    va_start(args,code);
    int possible_error = private_LuaCEmbed_evaluate_puting_on_top_of_stack(self,code,args);
    va_end(args);
    if(possible_error){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    if(private_LuaCEmbed_ensure_evaluation_type(self,LUA_CEMBED_BOOL)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    return (bool)lua_toboolean(self->state,-1);
}



//path: src/imports/../LuaCEmbed/globals/errors/fdefine.errors.c
//mannaged by silver chain





int LuaCEmbed_ensure_global_type(LuaCEmbed *self, const char *name,int expected_type){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    lua_getglobal(self->state,name);
    int type = lua_type(self->state,-1);

    if(type != expected_type){
        privateLuaCEmbed_raise_error_not_jumping(self,
             PRIVATE_LUA_CEMBED_GLOBAL_WRONG_TYPE,
             name,
             LuaCembed_convert_arg_code(type),
             LuaCembed_convert_arg_code(expected_type)
        );
        return  LUA_CEMBED_GENERIC_ERROR;
    }

    return LUA_CEMBED_OK;

}

//path: src/imports/../LuaCEmbed/globals/getters/fdefine.getters.c
//mannaged by silver chain



int LuaCEmbed_get_global_type(LuaCEmbed *self,const char *name){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    lua_getglobal(self->state, name);
    return lua_type(self->state,-1);
}

lua_Integer  LuaCEmbed_get_global_long(LuaCEmbed *self,const char *name){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    if(LuaCEmbed_ensure_global_type(self,name,LUA_CEMBED_NUMBER)){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    lua_getglobal(self->state, name);
    return lua_tointeger(self->state,-1);
}

double LuaCEmbed_get_global_double(LuaCEmbed *self,const char *name){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    if(LuaCEmbed_ensure_global_type(self,name,LUA_CEMBED_NUMBER)){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    lua_getglobal(self->state, name);
    return (double )lua_tonumber(self->state,-1);
}

bool LuaCEmbed_get_global_bool(LuaCEmbed *self,const char *name){
    PRIVATE_LUA_CEMBED_PROTECT_BOOL

    if(LuaCEmbed_ensure_global_type(self,name,LUA_CEMBED_BOOL)){
        return  LUA_CEMBED_GENERIC_ERROR;
    }
    lua_getglobal(self->state, name);
    return lua_toboolean(self->state,-1);
}

char * LuaCEmbed_get_global_raw_string(LuaCEmbed *self,const char *name,long *size){
    PRIVATE_LUA_CEMBED_PROTECT_NULL

    if(LuaCEmbed_ensure_global_type(self,name,LUA_CEMBED_STRING)){
        return  NULL;
    }
    lua_getglobal(self->state, name);
    return (char*)lua_tolstring(self->state,-1,(size_t*)size);
}

char * LuaCEmbed_get_global_string(LuaCEmbed *self,const char *name){
    PRIVATE_LUA_CEMBED_PROTECT_NULL

    if(LuaCEmbed_ensure_global_type(self,name,LUA_CEMBED_STRING)){
        return  NULL;
    }
    lua_getglobal(self->state, name);
    return (char*)lua_tostring(self->state,-1);
}

LuaCEmbedTable * LuaCembed_new_anonymous_table(LuaCEmbed *self){
    PRIVATE_LUA_CEMBED_PROTECT_NULL


    const char *format_raw = PRIVATE_LUA_CEMBED_ANONYMOUS_TABLE_;
    if(self->current_function){
       format_raw  =PRIVATE_LUA_CEMBED_ANONYMOUS_FUNC_TABLE_;
    }
    privateLuaCEmbedTableArray *target = (privateLuaCEmbedTableArray*)privateLuaCEmbed_get_current_table_array(self);
    char *buffer= private_LuaCembed_format(format_raw, self->stack_leve, target->size);
    LuaCEmbedTable  *created_table =LuaCembed_new_global_table(self,buffer);

    free(buffer);
    return created_table;
}



LuaCEmbedTable * LuaCembed_get_global_table(LuaCEmbed *self, const char *name){
    PRIVATE_LUA_CEMBED_PROTECT_NULL

    if(LuaCEmbed_ensure_global_type(self,name,LUA_CEMBED_TABLE)){
        return  NULL;
    }

    privateLuaCEmbedTableArray *target = (privateLuaCEmbedTableArray*)privateLuaCEmbed_get_current_table_array(self);

    LuaCEmbedTable  *possible = privateLuaCEmbedTableArray_find_by_global_name(target,name);
    if(possible){
        return possible;
    }

    LuaCEmbedTable  *creaeted = private_newLuaCembedTable(self, "%s", name);

    privateLuaCEmbedTableArray_append(
            target,
            creaeted
    );
    return creaeted;
}

LuaCEmbedTable * LuaCembed_new_global_table(LuaCEmbed *self, const char *name){
    PRIVATE_LUA_CEMBED_PROTECT_NULL

    lua_newtable(self->state);
    lua_setglobal(self->state,name);
    privateLuaCEmbedTableArray *target = (privateLuaCEmbedTableArray*)privateLuaCEmbed_get_current_table_array(self);

    LuaCEmbedTable  *possible = privateLuaCEmbedTableArray_find_by_global_name(target,name);
    if(possible){
        return possible;
    }

    LuaCEmbedTable  *creaeted = private_newLuaCembedTable(self, "%s", name);

    privateLuaCEmbedTableArray_append(
            target,
            creaeted
    );
    return creaeted;
}

LuaCEmbedTable* LuaCEmbed_run_global_lambda(LuaCEmbed *self, const char *name, LuaCEmbedTable *args_to_call, int total_returns){

    PRIVATE_LUA_CEMBED_PROTECT_NULL

    if(LuaCEmbed_ensure_global_type(self,name,LUA_CEMBED_FUNCTION)){
        return  NULL;
    }

    int total_args = private_lua_cEmbed_unpack(args_to_call,name);
    if(lua_pcall(self->state,total_args,total_returns,0)){
        privateLuaCEmbed_raise_error_not_jumping(self, lua_tostring(self->state,-1));
        return  NULL;
    }

    for(int i = 0; i < total_returns; i++){
        char *formatted = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN_, self->stack_leve, i);
        int position = (i +1) * -1;
        lua_pushvalue(self->state,position);
        lua_setglobal(self->state,formatted);
        free(formatted);
    }

    LuaCEmbedTable  *result = LuaCembed_new_anonymous_table(self);
    for(int i = 0; i < total_returns; i++){
        lua_getglobal(self->state,result->global_name);
        lua_pushinteger(self->state,i+1);
        char *formatted = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN_,self->stack_leve,i);
        lua_getglobal(self->state,formatted);
        lua_settable(self->state,-3);
        free(formatted);
    }

    return result;
}


//path: src/imports/../LuaCEmbed/globals/setters/fdefine.setters.c
//mannaged by silver chain



void LuaCEmbed_set_global_string(LuaCEmbed *self, const char *name, const  char *value){
    PRIVATE_LUA_CEMBED_PROTECT_VOID

    lua_pushstring(self->state,value);
    lua_setglobal(self->state,name);
}
void LuaCEmbed_set_global_raw_string(LuaCEmbed *self, const char *name, const  char *value,long size){
    PRIVATE_LUA_CEMBED_PROTECT_VOID

    lua_pushlstring(self->state,value,size);
    lua_setglobal(self->state,name);
}


void LuaCEmbed_set_global_long(LuaCEmbed *self, const char *name, lua_Integer  value){
    PRIVATE_LUA_CEMBED_PROTECT_VOID

    lua_pushinteger(self->state,value);
    lua_setglobal(self->state,name);
}

void LuaCEmbed_set_global_double(LuaCEmbed *self, const char *name, double value){
    PRIVATE_LUA_CEMBED_PROTECT_VOID

    lua_pushnumber(self->state,(double )value);
    lua_setglobal(self->state,name);
}

void LuaCEmbed_set_global_bool(LuaCEmbed *self, const char *name, bool value){
    PRIVATE_LUA_CEMBED_PROTECT_VOID
    lua_pushboolean(self->state,value);
    lua_setglobal(self->state,name);
}


void LuaCEmbed_set_global_table(LuaCEmbed *self, const char *name, LuaCEmbedTable *table){
    PRIVATE_LUA_CEMBED_PROTECT_VOID

    lua_getglobal(self->state,table->global_name);
    lua_setglobal(self->state,name);
}


//path: src/imports/../LuaCEmbed/lib_props/fdefine.lib_props.c
//mannaged by silver chain



void LuaCEmbed_set_long_lib_prop(LuaCEmbed *self,const char *name,lua_Integer value){
    char *main_lib_table = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME__,self->lib_identifier);

    lua_getglobal(self->state,main_lib_table);
    lua_pushvalue(self->state,-1);
    //set the function name
    lua_pushstring(self->state,name);
    lua_pushinteger(self->state,value);
    lua_settable(self->state,-3);
    free(main_lib_table);
}

void LuaCEmbed_set_double_lib_prop(LuaCEmbed *self,const char *name,double value){

    lua_getglobal(self->state,self->main_lib_table);
    lua_pushvalue(self->state,-1);
    //set the function name
    lua_pushstring(self->state,name);
    lua_pushnumber(self->state,value);
    lua_settable(self->state,-3);

}

void LuaCEmbed_set_bool_lib_prop(LuaCEmbed *self,const char *name,bool value){

    lua_getglobal(self->state,self->main_lib_table);
    lua_pushvalue(self->state,-1);
    //set the function name
    lua_pushstring(self->state,name);
    lua_pushboolean(self->state,value);
    lua_settable(self->state,-3);

}

void LuaCEmbed_set_string_lib_prop(LuaCEmbed *self,const char *name,const char * value){

    lua_getglobal(self->state,self->main_lib_table);
    lua_pushvalue(self->state,-1);
    //set the function name
    lua_pushstring(self->state,name);
    lua_pushstring(self->state,value);
    lua_settable(self->state,-3);

}

void LuaCEmbed_set_table_lib_prop(LuaCEmbed *self,const char *name,LuaCEmbedTable *value){

    lua_getglobal(self->state,self->main_lib_table);
    lua_pushvalue(self->state,-1);
    lua_pushstring(self->state,name);
    lua_getglobal(self->state,value->global_name);
    lua_settable(self->state,-3);

}

int private_LuaCEmbed_ensure_lib_prop_type(LuaCEmbed *self,const char *name,int expected_type){
    int type = lua_type(self->state,-1);
    if(type!= expected_type){
        privateLuaCEmbed_raise_error_not_jumping(
                self,
                PRIVATE_LUA_CEMBED_LIB_PROP_WRONG_TYPE,
                name,
                LuaCembed_convert_arg_code(type),
                LuaCembed_convert_arg_code(expected_type)
        );
        return LUA_CEMBED_GENERIC_ERROR;
    }
    return LUA_CEMBED_OK;
}

lua_Integer LuaCEmbed_get_long_lib_prop(LuaCEmbed *self,const char *name){
    lua_getglobal(self->state,self->main_lib_table);
    lua_getfield(self->state, -1,name);
    if(private_LuaCEmbed_ensure_lib_prop_type(self,name,LUA_CEMBED_NUMBER)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    return lua_tointeger(self->state,-1);

}

double LuaCEmbed_get_double_lib_prop(LuaCEmbed *self,const char *name){

    lua_getglobal(self->state,self->main_lib_table);

    lua_getfield(self->state, -1,name);
    if(private_LuaCEmbed_ensure_lib_prop_type(self,name,LUA_CEMBED_NUMBER)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    return lua_tonumber(self->state,-1);
}

bool LuaCEmbed_get_bool_lib_prop(LuaCEmbed *self,const char *name){

    lua_getglobal(self->state,self->main_lib_table);

    lua_getfield(self->state, -1,name);
    if(private_LuaCEmbed_ensure_lib_prop_type(self,name,LUA_CEMBED_BOOL)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    return lua_toboolean(self->state,-1);
}

char * LuaCEmbed_get_string_lib_prop(LuaCEmbed *self,const char *name){
    lua_getglobal(self->state,self->main_lib_table);

    lua_getfield(self->state, -1,name);
    if(private_LuaCEmbed_ensure_lib_prop_type(self,name,LUA_CEMBED_STRING)){
        return NULL;
    }
    return (char*)lua_tostring(self->state,-1);
}

LuaCEmbedTable *LuaCEmbed_get_table_lib_prop(LuaCEmbed *self,const char *name){
;
    lua_getglobal(self->state,self->main_lib_table);

    lua_getfield(self->state, -1,name);
    if(private_LuaCEmbed_ensure_lib_prop_type(self,name,LUA_CEMBED_TABLE)){
        return NULL;
    }

    char *global_sub_table_name  = private_LuaCembed_format(
            PRIVATE_LUA_CEMBED_MAIN_LIB_SUB_TABLE,
            self->lib_identifier,
            name
            );
    lua_setglobal(self->state,global_sub_table_name);

    privateLuaCEmbedTableArray *target = (privateLuaCEmbedTableArray*)privateLuaCEmbed_get_current_table_array(self);

    LuaCEmbedTable  *possible = privateLuaCEmbedTableArray_find_by_global_name(target,global_sub_table_name);
    if(possible){
        free(global_sub_table_name);
        return possible;
    }

    LuaCEmbedTable  *creaeted = private_newLuaCembedTable(self, "%s",global_sub_table_name);
    free(global_sub_table_name);

    privateLuaCEmbedTableArray_append(
            target,
            creaeted
    );
    return creaeted;
}


//path: src/imports/../LuaCEmbed/lib_start/fdefine.lib_start.c
//mannaged by silver chain


int private_LuaCemb_internal_free(lua_State *L){
    LuaCEmbed  *self = (LuaCEmbed*)lua_touserdata(L, lua_upvalueindex(1));
    if(self->delete_function){
        self->delete_function(self);
    }
    LuaCEmbed_free(self);
    return 0;
}
LuaCEmbed * newLuaCEmbedLib(lua_State *state){
    LuaCEmbed  *self = (LuaCEmbed*) malloc(sizeof (LuaCEmbed));
    *self = (LuaCEmbed){0};

    self->state = state;

    self->lib_identifier = 0;

    lua_getglobal(self->state,PRIVATE_LUA_CEMBED_TOTAL_LIBS);
    if(lua_type(self->state,-1) != LUA_CEMBED_NIL){
        self->lib_identifier  = lua_tointeger(self->state,-1);
    }

    self->lib_identifier +=1;

    self->main_lib_table = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME__,self->lib_identifier);

    lua_pushinteger(self->state,self->lib_identifier);
    lua_setglobal(self->state,PRIVATE_LUA_CEMBED_TOTAL_LIBS);


    self->is_lib = true;
    self->global_tables = (void*)newprivateLuaCEmbedTableArray();


    UniversalGarbage  *garbage = newUniversalGarbage();
    char *lib_meta_table = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MAIN_META_TABLE__,self->lib_identifier);
    UniversalGarbage_add_simple(garbage,lib_meta_table);
    //creating the metatable
    luaL_newmetatable(self->state, lib_meta_table);
    //seting the clojure key
    lua_pushstring(self->state,PRIVATE_LUA_CEMBED_DEL_PREFIX);
    //set self as first clojure argument
    lua_pushlightuserdata(self->state,(void*)self);
    lua_pushcclosure(self->state,private_LuaCemb_internal_free,1);
    lua_settable(self->state, -3);


    char *lib_main_table = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME__,self->lib_identifier);
    UniversalGarbage_add_simple(garbage,lib_main_table);
    //creating the global table to store the elements
    lua_newtable(self->state);
    lua_setglobal(self->state,lib_main_table);
    luaL_setmetatable(self->state, lib_meta_table);

    UniversalGarbage_free(garbage);
    return  self;
}

int LuaCembed_perform(LuaCEmbed *self){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    char *lib_main_table = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME__,self->lib_identifier);
    lua_getglobal(self->state,lib_main_table);
    free(lib_main_table);
    return 1;
}

int LuaCembed_send_self_as_lib(LuaCEmbed *self){
    return LuaCembed_perform(self);
}

int LuaCembed_send_global_as_lib(LuaCEmbed *self,const char *global_name){
    if(global_name == NULL){
        return 0;
    }

    lua_getglobal(self->state,global_name);
    int type  = lua_type(self->state,-1);
    if(type == LUA_CEMBED_NIL){
        return 0;
    }
    return 1;
}


//path: src/imports/../LuaCEmbed/memory/fdefine.memory.c
//mannaged by silver chain



void LuaCEmbed_set_memory_limit(LuaCEmbed *self, double limit){
    lua_setallocf(self->state, private_LuaCembed_custom_allocator, &lua_cembed_used_memory);
    lua_cembed_memory_limit = limit;
}
static void *private_LuaCembed_custom_allocator(void *ud, void *ptr, size_t osize, size_t nsize) {
    int *used = (int *)ud;

    if (ptr == NULL) {
        osize = 0;
    }

    if (nsize == 0) {
        free(ptr);
        *used -= osize; 
        return NULL;
    } else {
        long  custom_limit = (long)(lua_cembed_memory_limit * PRIVATE_LUA_CEMBED_ONE_MB);
        if (*used + (nsize - osize) > custom_limit) 
            return NULL;
        ptr = realloc(ptr, nsize);
        if (ptr) 
            *used += (nsize - osize);
        return ptr;
    }
}



//path: src/imports/../LuaCEmbed/table/table/append/fdefine.append.c
//mannaged by silver chain

void  LuaCEmbedTable_append_table(LuaCEmbedTable *self, LuaCEmbedTable *table){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    long size = LuaCEmbedTable_get_listable_size(self);
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,size+1);
    lua_getglobal(self->main_object->state,table->global_name);
    lua_rawset(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_append_string(LuaCEmbedTable *self,  const char *value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    long size = LuaCEmbedTable_get_listable_size(self);
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,size+1);
    lua_pushstring(self->main_object->state,value);
    lua_rawset(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_append_long(LuaCEmbedTable *self,  lua_Integer value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    long size = LuaCEmbedTable_get_listable_size(self);
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,size+1);
    lua_pushinteger(self->main_object->state,value);
    lua_rawset(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_append_double(LuaCEmbedTable *self, double  value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    long size = LuaCEmbedTable_get_listable_size(self);
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,size+1);
    lua_pushnumber(self->main_object->state,value);
    lua_rawset(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_append_bool(LuaCEmbedTable *self,  bool value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    long size = LuaCEmbedTable_get_listable_size(self);
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,size+1);
    lua_pushboolean(self->main_object->state,value);
    lua_rawset(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_append_evaluation(LuaCEmbedTable *self, const char *code, ...){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    va_list  args;
    va_start(args,code);
     char *buffer = private_LuaCembed_format_vaarg(code,args);
    va_end(args);

    LuaCEmbed_evaluate(
            self->main_object,
            PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE,
            PRIVATE_LUA_CEMBED_EVALUATION_NAME,
            buffer
    );
    free(buffer);
    if(LuaCEmbed_has_errors(self->main_object)){
        return;
    }
    long size = LuaCEmbedTable_get_listable_size(self);
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,size+1);
    lua_getglobal(self->main_object->state,PRIVATE_LUA_CEMBED_EVALUATION_NAME);
    lua_rawset(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);
}


//path: src/imports/../LuaCEmbed/table/table/basic/fdefine.basic.c
//mannaged by silver chain





LuaCEmbedTable * private_newLuaCembedTable(LuaCEmbed *main_embed, const char *format, ...){

    LuaCEmbedTable  *self = (LuaCEmbedTable*)malloc(sizeof (LuaCEmbedTable));
    *self = (LuaCEmbedTable){0};
    self->main_object =main_embed;

    va_list  args;
    va_start(args,format);
    self->global_name = private_LuaCembed_format_vaarg(format,args);
    va_end(args);

    self->sub_tables = (void*)newprivateLuaCEmbedTableArray();

    return self;
}

 int  private_lua_cEmbed_unpack(LuaCEmbedTable *self,const char * previews_function){

    long size = LuaCEmbedTable_get_listable_size(self);
     lua_settop(self->main_object->state, 0);


     lua_getglobal(self->main_object->state,self->global_name);
     int table_index = lua_gettop(self->main_object->state);
     int total = 0;


     lua_pushnil(self->main_object->state);
     while(lua_next(self->main_object->state,table_index)){

         lua_pushvalue(self->main_object->state,-1);
         char *formated = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN_,self->main_object->stack_leve,total);

         lua_setglobal(self->main_object->state,formated);
         free(formated);
         lua_pop(self->main_object->state,1);
         total+=1;
     }
    if(previews_function){
        lua_getglobal(self->main_object->state,previews_function);
    }
    for(int i = 0; i < size; i++){
        char *formated = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN_,self->main_object->stack_leve,i);
        lua_getglobal(self->main_object->state,formated);
        free(formated);
    }

    return (int)size;
}
void privateLuaCEmbedTable_free(LuaCEmbedTable *self){

    free(self->global_name);

    if(self->prop_name){
        free(self->prop_name);
    }

    privateLuaCEmbedTableArray_free((privateLuaCEmbedTableArray*)self->sub_tables);
    free(self);
}



//path: src/imports/../LuaCEmbed/table/table/destroy/fdefine.destroy.c
//mannaged by silver chain



void LuaCembedTable_destroy_prop(LuaCEmbedTable *self, const char *name){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID


    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_pushnil(self->main_object->state);
    lua_settable(self->main_object->state,-3);
}
void LuaCEmbedTable_destroy_by_index(LuaCEmbedTable *self, long index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    if(LuaCembedTable_has_key_at_index(self,index)){
        char *key = LuaCembedTable_get_key_by_index(self,index);
        LuaCembedTable_destroy_prop(self,key);
        return;
    }

    lua_getglobal(self->main_object->state,self->global_name);

    long converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    int table_index = lua_gettop(self->main_object->state);
    int total = 0;

    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total >= converted_index){

            lua_getglobal(self->main_object->state,self->global_name);
            lua_pushnumber(self->main_object->state,(double)(total));
            lua_pushvalue(self->main_object->state,-3);
            lua_settable(self->main_object->state,-3);
            lua_pop(self->main_object->state,1);

        }
        lua_pop(self->main_object->state,1);
        total+=1;

    }

    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushnumber(self->main_object->state,total);
    lua_pushnil(self->main_object->state);
    lua_settable(self->main_object->state,-3);



}

//path: src/imports/../LuaCEmbed/table/table/errors/fdefine.errors.c
//mannaged by silver chain




int privateLuaCEmbedTable_ensure_type_with_key(LuaCEmbedTable *self, const char *name, int expected_type){
    if(!self){
        return LUA_CEMBED_GENERIC_ERROR;
    }

    int type = lua_type(self->main_object->state,-1);
    if(type == expected_type){
        return  LUA_CEMBED_OK;
    }
    privateLuaCEmbed_raise_error_not_jumping(
            self->main_object,
            PRIVATE_LUA_CEMBED_WRONG_TYPE_PROPETY,
            name,
            self->global_name,
            LuaCembed_convert_arg_code(type),
            LuaCembed_convert_arg_code(expected_type)
    );
    return LUA_CEMBED_GENERIC_ERROR;
}

int privateLuaCEmbedTable_ensure_type_with_index(LuaCEmbedTable *self, long index, int expected_type){
    if(!self){
        return LUA_CEMBED_GENERIC_ERROR;
    }

    int type = lua_type(self->main_object->state,-1);
    if(type == expected_type){
        return  LUA_CEMBED_OK;
    }
    privateLuaCEmbed_raise_error_not_jumping(
            self->main_object,
            PRIVATE_LUA_CEMBED_WRONG_TYPE_INDEX,
            index,
            self->global_name,
            LuaCembed_convert_arg_code(type),
            LuaCembed_convert_arg_code(expected_type)
    );
    return LUA_CEMBED_GENERIC_ERROR;
}

//path: src/imports/../LuaCEmbed/table/table/getters/index/fdefine.index.c
//mannaged by silver chain



lua_Integer  LuaCEmbedTable_get_full_size(LuaCEmbedTable *self){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM



    lua_getglobal(self->main_object->state,self->global_name);
    int index = lua_gettop(self->main_object->state);
    lua_Integer total = 0;

    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,index)){
        total+=1;
        lua_pop(self->main_object->state,1);
    }

    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return total;
}

lua_Integer  LuaCEmbedTable_get_listable_size(LuaCEmbedTable *self){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM

    lua_getglobal(self->main_object->state,self->global_name);
    return (long)lua_rawlen(self->main_object->state,-1);
}


lua_Integer  privateLuaCEmbedTable_convert_index(LuaCEmbedTable *self, private_lua_cembed_incremented_arg index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM

    if(index >= 0){
        return  index;
    }

    return LuaCEmbedTable_get_full_size(self) + index;
}

int LuaCEmbedTable_get_type_by_index(LuaCEmbedTable *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM

    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    lua_getglobal(self->main_object->state,self->global_name);
    int table_index = lua_gettop(self->main_object->state);
    long converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    int total = 1;
    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total == converted_index){
            int type = lua_type(self->main_object->state,-1);
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return type;
        }
        lua_pop(self->main_object->state,1);
        total+=1;

    }
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return LUA_CEMBED_NOT_FOUND;
}
char *LuaCembedTable_get_key_by_index(LuaCEmbedTable *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL

    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    lua_getglobal(self->main_object->state,self->global_name);
    int table_index = lua_gettop(self->main_object->state);
    long converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    lua_Integer total = 1;
    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){

        if(total == converted_index){
            if(lua_type(self->main_object->state,-2) != LUA_CEMBED_STRING ){
                lua_pop(self->main_object->state,1);
                PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
                return NULL;
            }
            char* result = (char*)lua_tostring(self->main_object->state,-2);
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return result;
        }

        lua_pop(self->main_object->state,1);
        total+=1;

    }
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return NULL;
}
bool LuaCembedTable_has_key_at_index(LuaCEmbedTable *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_BOOL
    lua_settop(self->main_object->state,0);

    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    lua_getglobal(self->main_object->state,self->global_name);
    int table_index = lua_gettop(self->main_object->state);
    long converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    int total = 1;
    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total == converted_index){
            bool has_key =lua_type(self->main_object->state,-2) == LUA_CEMBED_STRING;
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return has_key;
        }

        lua_pop(self->main_object->state,1);
        total+=1;
    }
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return false;
}

lua_Integer  LuaCEmbedTable_get_long_by_index(LuaCEmbedTable *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM
    lua_settop(self->main_object->state,0);

    int formatted_index = index + LUA_CEMBED_INDEX_DIF;

    lua_getglobal(self->main_object->state,self->global_name);
    int table_index = lua_gettop(self->main_object->state);
    lua_Integer converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    lua_Integer total = 1;

    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total == converted_index){
            if(privateLuaCEmbedTable_ensure_type_with_index(self,converted_index,LUA_CEMBED_NUMBER)){
                lua_pop(self->main_object->state,1);
                PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
                return LUA_CEMBED_GENERIC_ERROR;
            }
            long result = (long)lua_tonumber(self->main_object->state,-1);
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return result;
        }

        lua_pop(self->main_object->state,1);
        total+=1;

    }

    privateLuaCEmbed_raise_error_not_jumping(
            self->main_object,
            PRIVATE_LUA_CEMBED_WRONG_TYPE_INDEX,
            index,
            self->global_name,
            LuaCembed_convert_arg_code(LUA_CEMBED_NIL),
            LuaCembed_convert_arg_code(LUA_CEMBED_NUMBER)
    );
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return LUA_CEMBED_GENERIC_ERROR;
}

double LuaCEmbedTable_get_double_by_index(LuaCEmbedTable *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM

    int formatted_index = index + LUA_CEMBED_INDEX_DIF;

    lua_getglobal(self->main_object->state,self->global_name);
    int table_index = lua_gettop(self->main_object->state);
    long converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    lua_Integer total = 1;
    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total == converted_index){
            if(privateLuaCEmbedTable_ensure_type_with_index(self,converted_index,LUA_CEMBED_NUMBER)){
                lua_pop(self->main_object->state,1);
                PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
                return LUA_CEMBED_GENERIC_ERROR;
            }
            double result = (double )lua_tonumber(self->main_object->state,-1);
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return result;
        }
        lua_pop(self->main_object->state,1);
        total+=1;

    }

    privateLuaCEmbed_raise_error_not_jumping(
            self->main_object,
            PRIVATE_LUA_CEMBED_WRONG_TYPE_INDEX,
            index,
            self->global_name,
            LuaCembed_convert_arg_code(LUA_CEMBED_NIL),
            LuaCembed_convert_arg_code(LUA_CEMBED_NUMBER)
    );
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return LUA_CEMBED_GENERIC_ERROR;
}

char * LuaCEmbedTable_get_string_by_index(LuaCEmbedTable *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL

    int formatted_index = index + LUA_CEMBED_INDEX_DIF;

    lua_getglobal(self->main_object->state,self->global_name);
    int table_index = lua_gettop(self->main_object->state);
    long converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    lua_Integer total = 1;
    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total == converted_index){
            if(privateLuaCEmbedTable_ensure_type_with_index(self,converted_index,LUA_CEMBED_STRING)){
                lua_pop(self->main_object->state,1);
                PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
                return NULL;
            }
            char * result = (char*)lua_tostring(self->main_object->state,-1);
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return result;
        }
        lua_pop(self->main_object->state,1);
        total+=1;

    }

    privateLuaCEmbed_raise_error_not_jumping(
            self->main_object,
            PRIVATE_LUA_CEMBED_WRONG_TYPE_INDEX,
            index,
            self->global_name,
            LuaCembed_convert_arg_code(LUA_CEMBED_NIL),
            LuaCembed_convert_arg_code(LUA_CEMBED_STRING)
    );
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return NULL;
}

char * LuaCEmbedTable_get_raw_string_by_index(LuaCEmbedTable *self, lua_Integer index, lua_Integer *size){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL

    int formatted_index = index + LUA_CEMBED_INDEX_DIF;

    lua_getglobal(self->main_object->state,self->global_name);
    int table_index = lua_gettop(self->main_object->state);
    long converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    int total = 1;
    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total == converted_index){
            if(privateLuaCEmbedTable_ensure_type_with_index(self,converted_index,LUA_CEMBED_STRING)){
                lua_pop(self->main_object->state,1);
                PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
                return NULL;
            }
            char * result = (char*)lua_tolstring(self->main_object->state,-1,(size_t*)size);
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return result;
        }
        lua_pop(self->main_object->state,1);
        total+=1;

    }

    privateLuaCEmbed_raise_error_not_jumping(
            self->main_object,
            PRIVATE_LUA_CEMBED_WRONG_TYPE_INDEX,
            index,
            self->global_name,
            LuaCembed_convert_arg_code(LUA_CEMBED_NIL),
            LuaCembed_convert_arg_code(LUA_CEMBED_STRING)
    );
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return NULL;
}
bool LuaCEmbedTable_get_bool_by_index(LuaCEmbedTable *self, lua_Integer index){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_BOOL

    int formatted_index = index + LUA_CEMBED_INDEX_DIF;
    lua_getglobal(self->main_object->state,self->global_name);
    int table_index = lua_gettop(self->main_object->state);
    long converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    int total = 1;
    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total == converted_index){
            if(privateLuaCEmbedTable_ensure_type_with_index(self,converted_index,LUA_CEMBED_BOOL)){
                lua_pop(self->main_object->state,1);
                PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
                return LUA_CEMBED_GENERIC_ERROR;
            }
            bool result = lua_toboolean(self->main_object->state,-1);
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return result;
        }
        lua_pop(self->main_object->state,1);
        total+=1;

    }

    privateLuaCEmbed_raise_error_not_jumping(
            self->main_object,
            PRIVATE_LUA_CEMBED_WRONG_TYPE_INDEX,
            converted_index,
            self->global_name,
            LuaCembed_convert_arg_code(LUA_CEMBED_NIL),
            LuaCembed_convert_arg_code(LUA_CEMBED_BOOL)
    );
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return LUA_CEMBED_GENERIC_ERROR;
}


//path: src/imports/../LuaCEmbed/table/table/getters/keys/fdefine.keys.c
//mannaged by silver chain


int  LuaCEmbedTable_get_type_prop(LuaCEmbedTable *self, const char *name){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM
    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);
    return lua_type(self->main_object->state,-1);

}

char*  LuaCembedTable_get_string_prop(LuaCEmbedTable *self , const char *name){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL



    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);
    if(privateLuaCEmbedTable_ensure_type_with_key(self, name, LUA_CEMBED_STRING)){
        return NULL;
    }

    char *value = (char*)lua_tostring(self->main_object->state,-1);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return value;
}

char*  LuaCembedTable_get_raw_string_prop(LuaCEmbedTable *self, const char *name,lua_Integer *size){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL

    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);
    if(privateLuaCEmbedTable_ensure_type_with_key(self, name, LUA_CEMBED_STRING)){
        return NULL;
    }

    char *value = (char*)lua_tolstring(self->main_object->state,-1,(size_t*)size);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return value;
}

lua_Integer LuaCembedTable_get_long_prop(LuaCEmbedTable *self , const char *name){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM

    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);
    if(privateLuaCEmbedTable_ensure_type_with_key(self, name, LUA_CEMBED_NUMBER)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    lua_Integer value = lua_tointeger(self->main_object->state,-1);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return value;
}

double  LuaCembedTable_get_double_prop(LuaCEmbedTable *self , const char *name){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NUM



    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);

    if(privateLuaCEmbedTable_ensure_type_with_key(self, name, LUA_CEMBED_NUMBER)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    double value  = (double )lua_tonumber(self->main_object->state,-1);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return value;
}

bool  LuaCembedTable_get_bool_prop(LuaCEmbedTable *self , const char *name){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_BOOL



    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);
    if(privateLuaCEmbedTable_ensure_type_with_key(self, name, LUA_CEMBED_BOOL)){
        return LUA_CEMBED_GENERIC_ERROR;
    }
    bool value =  lua_toboolean(self->main_object->state,-1);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return value;
}


//path: src/imports/../LuaCEmbed/table/table/setters/index/fdefine.index.c
//mannaged by silver chain



void  LuaCEmbedTable_set_string_by_index(LuaCEmbedTable *self, lua_Integer index, const char *value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    char *possible_key = LuaCembedTable_get_key_by_index(self,index);
    if(possible_key){
        LuaCEmbedTable_set_string_prop(self,possible_key,value);
        return;
    }
    long formatted_index = index + LUA_CEMBED_INDEX_DIF;
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushnumber(self->main_object->state,(double)formatted_index);
    lua_pushstring(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);
}


void  LuaCEmbedTable_set_raw_string_by_index(LuaCEmbedTable *self, lua_Integer index, const char *value,lua_Integer size){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    char *possible_key = LuaCembedTable_get_key_by_index(self,index);
    if(possible_key){
        LuaCEmbedTable_set_string_prop(self,possible_key,value);
        return;
    }
    long formatted_index = index + LUA_CEMBED_INDEX_DIF;
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushnumber(self->main_object->state,(double)formatted_index);
    lua_pushlstring(self->main_object->state,value,size);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);
}



void  LuaCEmbedTable_set_long_by_index(LuaCEmbedTable *self, lua_Integer  index, lua_Integer  value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    char *possible_key = LuaCembedTable_get_key_by_index(self,index);
    if(possible_key){
        LuaCEmbedTable_set_long_prop(self,possible_key,value);
        return;
    }
    lua_Integer formatted_index = index + LUA_CEMBED_INDEX_DIF;
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,formatted_index);
    lua_pushinteger(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_set_double_by_index(LuaCEmbedTable *self, lua_Integer index, double  value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    char *possible_key = LuaCembedTable_get_key_by_index(self,index);
    if(possible_key){
        LuaCEmbedTable_set_double_prop(self,possible_key,value);
        return;
    }

    long formatted_index = index + LUA_CEMBED_INDEX_DIF;
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,formatted_index);
    lua_pushnumber(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_set_bool_by_index(LuaCEmbedTable *self, lua_Integer index, bool value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    char *possible_key = LuaCembedTable_get_key_by_index(self,index);
    if(possible_key){
        LuaCEmbedTable_set_bool_prop(self,possible_key,value);
        return;
    }

    long formatted_index = index + LUA_CEMBED_INDEX_DIF;
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,formatted_index);
    lua_pushboolean(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}


void  LuaCEmbedTable_set_evaluation_by_index(LuaCEmbedTable *self, lua_Integer index, const char *code, ...){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    long formatted_index = index + LUA_CEMBED_INDEX_DIF;
    va_list  args;
    va_start(args,code);
    char *  buffer = private_LuaCembed_format_vaarg(code,args);
    va_end(args);

    LuaCEmbed_evaluate(self->main_object,
                       PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE,
                       PRIVATE_LUA_CEMBED_EVALUATION_NAME,
                       buffer
    );
    free(buffer);
    if(LuaCEmbed_has_errors(self->main_object)){
        return;
    }
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,formatted_index);
    lua_getglobal(self->main_object->state,PRIVATE_LUA_CEMBED_EVALUATION_NAME);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}


//path: src/imports/../LuaCEmbed/table/table/setters/insert/fdefine.insert.c
//mannaged by silver chain


void  private_LuaCEmbedTable_add_space(LuaCEmbedTable *self, lua_Integer formatted_index){


    lua_newtable(self->main_object->state);
    lua_setglobal(self->main_object->state,PRIVATE_LUA_CEMBED_STAGE_AREA_TABLE);

    lua_getglobal(self->main_object->state, self->global_name);
    lua_Integer table_index = lua_gettop(self->main_object->state);
    lua_Integer total = 1;
    lua_pushnil(self->main_object->state); // Empilhando o primeiro par chave-valor
    while (lua_next(self->main_object->state, table_index)) {

        if (total >= formatted_index) {
            lua_getglobal(self->main_object->state, PRIVATE_LUA_CEMBED_STAGE_AREA_TABLE);
            lua_pushinteger(self->main_object->state, total + 1);
            lua_pushvalue(self->main_object->state, -3);
            lua_settable(self->main_object->state, -3);
            lua_pop(self->main_object->state, 1);
        }
        lua_pop(self->main_object->state, 1); // Removendo o valor atual
        total+=1;
    }

    lua_getglobal(self->main_object->state, PRIVATE_LUA_CEMBED_STAGE_AREA_TABLE);
    table_index = lua_gettop(self->main_object->state);
    lua_pushnil(self->main_object->state);

    while (lua_next(self->main_object->state, table_index)) {
        lua_getglobal(self->main_object->state,self->global_name);
        lua_pushvalue(self->main_object->state,-3); //table[index] =  stage_area[index]
        lua_pushvalue(self->main_object->state,-3);  //table[index] =  stage_area[index]
        lua_settable(self->main_object->state,-3);
        lua_pop(self->main_object->state, 2); // Removendo o valor atual
    }

    lua_getglobal(self->main_object->state, "stage_arrea");
    lua_pushnil(self->main_object->state);
}

void LuaCEmbedTable_insert_string_at_index(LuaCEmbedTable *self, lua_Integer index, const char *value) {
    // Movendo os elementos existentes para frente
    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    private_LuaCEmbedTable_add_space(self,formatted_index);
    // Inserindo o novo valor na posição especificada
    lua_getglobal(self->main_object->state, self->global_name);
    lua_pushinteger(self->main_object->state, formatted_index);
    lua_pushstring(self->main_object->state, value);
    lua_settable(self->main_object->state, -3);

    // Limpando a pilha
    lua_settop(self->main_object->state, 0);
}


void  LuaCEmbedTable_insert_bool_at_index(LuaCEmbedTable *self, lua_Integer index,bool value){

    // Movendo os elementos existentes para frente
    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    private_LuaCEmbedTable_add_space(self,formatted_index);
    // Inserindo o novo valor na posição especificada
    lua_getglobal(self->main_object->state, self->global_name);
    lua_pushinteger(self->main_object->state, formatted_index);
    lua_pushboolean(self->main_object->state, value);
    lua_settable(self->main_object->state, -3);

    // Limpando a pilha
    lua_settop(self->main_object->state, 0);
}

void  LuaCEmbedTable_insert_long_at_index(LuaCEmbedTable *self, lua_Integer  index,lua_Integer value){

    // Movendo os elementos existentes para frente
    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    private_LuaCEmbedTable_add_space(self,formatted_index);
    // Inserindo o novo valor na posição especificada
    lua_getglobal(self->main_object->state, self->global_name);
    lua_pushinteger(self->main_object->state, formatted_index);
    lua_pushinteger(self->main_object->state, value);
    lua_settable(self->main_object->state, -3);

    // Limpando a pilha
    lua_settop(self->main_object->state, 0);
}

void  LuaCEmbedTable_insert_double_at_index(LuaCEmbedTable *self, lua_Integer index,double value){

    // Movendo os elementos existentes para frente
    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    private_LuaCEmbedTable_add_space(self,formatted_index);
    // Inserindo o novo valor na posição especificada
    lua_getglobal(self->main_object->state, self->global_name);
    lua_pushinteger(self->main_object->state, formatted_index);
    lua_pushnumber(self->main_object->state, value);
    lua_settable(self->main_object->state, -3);

    // Limpando a pilha
    lua_settop(self->main_object->state, 0);
}

void  LuaCEmbedTable_insert_table_at_index(LuaCEmbedTable *self, lua_Integer index,LuaCEmbedTable *table){

    // Movendo os elementos existentes para frente
    lua_Integer formatted_index = index + LUA_CEMBED_INDEX_DIF;

    private_LuaCEmbedTable_add_space(self,formatted_index);
    // Inserindo o novo valor na posição especificada
    lua_getglobal(self->main_object->state, self->global_name);
    lua_pushinteger(self->main_object->state, formatted_index);
    lua_getglobal(self->main_object->state,table->global_name);
    lua_settable(self->main_object->state, -3);
    // Limpando a pilha
    lua_settop(self->main_object->state, 0);
}


//path: src/imports/../LuaCEmbed/table/table/setters/keys/fdefine.keys.c
//mannaged by silver chain





void LuaCEmbedTable_set_method(LuaCEmbedTable *self , const char *name, LuaCEmbedResponse *(*callback)(LuaCEmbedTable  *self, LuaCEmbed *args)){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID


    bool is_meta = false;

    if(strlen(name) > 3){
        if(name[0] == '_' && name[1] == '_' ){
            is_meta = true;
        }
    }
    lua_getglobal(self->main_object->state,self->global_name);

    if(is_meta){


        int found =lua_getmetatable(self->main_object->state,-1);
        if(!found){

            //equivalent of meta ={} ;setmetatable(table,meta)
            lua_getglobal(self->main_object->state,self->global_name);
            lua_newtable(self->main_object->state);
            lua_setmetatable(self->main_object->state,-2);

            lua_getglobal(self->main_object->state,self->global_name);
            lua_getmetatable(self->main_object->state,-1);
        }
    }


    lua_pushstring(self->main_object->state,name);
    //creating the clojure

    lua_pushboolean(self->main_object->state,true);//is a method

    lua_pushlightuserdata(self->main_object->state,(void*)self->main_object); //self
    lua_pushstring(self->main_object->state,name);//calback name
    lua_getglobal(self->main_object->state,self->global_name);//table
    lua_pushlightuserdata(self->main_object->state,(void*)callback);

    //add these clojure to be handled by the callbacks
    lua_pushcclosure(self->main_object->state,privateLuaCEmbed_main_callback_handler,5);
    lua_settable(self->main_object->state,-3);

    if(is_meta){
        //equivalent of meta ={} ;setmetatable(table,meta)
        lua_getglobal(self->main_object->state,self->global_name);
       lua_getmetatable(self->main_object->state,-1);
        lua_setmetatable(self->main_object->state,-2);
    }
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_set_string_prop(LuaCEmbedTable *self , const char *name, const char *value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_pushstring(self->main_object->state,value);

    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}
void  LuaCEmbedTable_set_raw_string_prop(LuaCEmbedTable *self , const char *name, const char *value,lua_Integer size){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_pushlstring(self->main_object->state,value,size);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_set_long_prop(LuaCEmbedTable *self , const char *name,lua_Integer   value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_pushinteger(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_set_double_prop(LuaCEmbedTable *self , const char *name, double  value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_pushnumber(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_set_bool_prop(LuaCEmbedTable *self , const char *name, bool value){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_pushboolean(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}

void  LuaCEmbedTable_set_evaluation_prop(LuaCEmbedTable *self, const char *name, const char *code, ...){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID


    va_list  args;
    va_start(args,code);
    char *buffer = private_LuaCembed_format_vaarg(code,args);
    va_end(args);

    LuaCEmbed_evaluate(self->main_object,
                       PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE,
                       PRIVATE_LUA_CEMBED_EVALUATION_NAME,
                       buffer
    );
    free(buffer);
    if(LuaCEmbed_has_errors(self->main_object)){
        return;
    }
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_getglobal(self->main_object->state,PRIVATE_LUA_CEMBED_EVALUATION_NAME);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

}


//path: src/imports/../LuaCEmbed/table/table/sub_tables/index/fdefine.index.c
//mannaged by silver chain


LuaCEmbedTable  *LuaCEmbedTable_new_sub_table_appending(LuaCEmbedTable *self){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL



    //equivalent of: full_sub_table_name = {}
    long index= LuaCEmbedTable_get_listable_size(self) +1;
    char *full_sub_table_name = private_LuaCembed_format("%s_%d", self->global_name, index);
    lua_newtable(self->main_object->state);
    lua_setglobal(self->main_object->state,full_sub_table_name);


    //equivalent of: table[index] = full_sub_table_name
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,index);
    lua_getglobal(self->main_object->state,full_sub_table_name);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);

    LuaCEmbedTable  *possible = privateLuaCEmbedTableArray_find_by_internal_index(
            (privateLuaCEmbedTableArray *) self->sub_tables,
            index
    );

    if(possible){
        free(full_sub_table_name);
        PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
        return possible;
    }

    LuaCEmbedTable  *created = private_newLuaCembedTable(self->main_object, full_sub_table_name);
    created->index = index;

    privateLuaCEmbedTableArray_append(
            (privateLuaCEmbedTableArray*)self->sub_tables,
            created
    );

    free(full_sub_table_name);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return created;
}

LuaCEmbedTable  *LuaCEmbedTable_get_sub_table_by_index(LuaCEmbedTable *self, lua_Integer index){

    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL


    long formatted_index = index + LUA_CEMBED_INDEX_DIF;

    lua_getglobal(self->main_object->state,self->global_name);
    lua_Integer table_index = lua_gettop(self->main_object->state);
    lua_Integer converted_index = privateLuaCEmbedTable_convert_index(self,formatted_index);
    lua_Integer total = 1;

    lua_pushnil(self->main_object->state);
    while(lua_next(self->main_object->state,table_index)){
        if(total == converted_index){

            if(privateLuaCEmbedTable_ensure_type_with_index(self,converted_index,LUA_CEMBED_TABLE)){
                lua_pop(self->main_object->state,1);
                PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
                return NULL;
            }

            //equivalent of full_sub_table_name = table[index]
            char *full_sub_table_name = private_LuaCembed_format("%s_%d", self->global_name, index);
            lua_setglobal(self->main_object->state,full_sub_table_name);

            LuaCEmbedTable  *possible = privateLuaCEmbedTableArray_find_by_internal_index(
                    (privateLuaCEmbedTableArray *) self->sub_tables,
                    index
            );

            if(possible){
                free(full_sub_table_name);
                lua_pop(self->main_object->state,1);
                PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
                return possible;
            }

            LuaCEmbedTable  *created = private_newLuaCembedTable(self->main_object, full_sub_table_name);
            created->index = index;

            privateLuaCEmbedTableArray_append(
                    (privateLuaCEmbedTableArray*)self->sub_tables,
                    created
            );

            free(full_sub_table_name);
            lua_pop(self->main_object->state,1);
            PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
            return created;
        }
        lua_pop(self->main_object->state,1);
        total+=1;

    }

    privateLuaCEmbed_raise_error_not_jumping(
            self->main_object,
            PRIVATE_LUA_CEMBED_WRONG_TYPE_INDEX,
            index,
            self->global_name,
            LuaCembed_convert_arg_code(LUA_CEMBED_NIL),
            LuaCembed_convert_arg_code(LUA_CEMBED_TABLE)
    );
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return NULL;
}

void LuaCEmbedTable_set_sub_table_by_index(LuaCEmbedTable *self, lua_Integer index,LuaCEmbedTable *sub_table){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID


    char *possible_key = LuaCembedTable_get_key_by_index(self,index);
    if(possible_key){
        LuaCEmbedTable_set_sub_table_prop(self,possible_key,sub_table);
        PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
        return;
    }

    lua_Integer formatted_index = index + LUA_CEMBED_INDEX_DIF;
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushinteger(self->main_object->state,formatted_index);
    lua_getglobal(self->main_object->state,sub_table->global_name);
    lua_settable(self->main_object->state,-3);
    lua_settop(self->main_object->state, 0);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
}


//path: src/imports/../LuaCEmbed/table/table/sub_tables/key/fdefine.key.c
//mannaged by silver chain


LuaCEmbedTable  *LuaCEmbedTable_new_sub_table_by_key(LuaCEmbedTable *self, const char *name){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL


    //equivalent of: table.sub_table = {}
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_newtable(self->main_object->state);
    lua_settable(self->main_object->state,-3);


    //equivalent of full_sub_table_name = table.sub_table
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state, name);
    lua_gettable(self->main_object->state, -2);
    char *full_sub_table_name = private_LuaCembed_format("%s_%s", self->global_name, name);
    lua_setglobal(self->main_object->state,full_sub_table_name);


    LuaCEmbedTable  *possible = privateLuaCEmbedTableArray_find_by_prop_name(
            (privateLuaCEmbedTableArray *) self->sub_tables,
            name
    );


    if(possible){
        free(full_sub_table_name);
        PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
        return possible;
    }

    LuaCEmbedTable  *created = private_newLuaCembedTable(self->main_object, full_sub_table_name);
    created->prop_name = strdup(name);

    privateLuaCEmbedTableArray_append(
            (privateLuaCEmbedTableArray*)self->sub_tables,
            created
    );

    free(full_sub_table_name);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return created;
}


LuaCEmbedTable  *LuaCEmbedTable_get_sub_table_by_key(LuaCEmbedTable *self, const char *name){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_NULL


    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);
    if(privateLuaCEmbedTable_ensure_type_with_key(self, name, LUA_CEMBED_TABLE)){
        PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
        return NULL;
    }
    //equivalent of full_sub_table_name = table.sub_table
    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);
    char *full_sub_table_name = private_LuaCembed_format("%s_%s", self->global_name, name);
    lua_setglobal(self->main_object->state,full_sub_table_name);


    LuaCEmbedTable  *possible = privateLuaCEmbedTableArray_find_by_prop_name(
            (privateLuaCEmbedTableArray *) self->sub_tables,
            name
    );


    if(possible){
        free(full_sub_table_name);
        PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
        return possible;
    }

    LuaCEmbedTable  *created = private_newLuaCembedTable(self->main_object, full_sub_table_name);
    created->prop_name = strdup(name);

    privateLuaCEmbedTableArray_append(
            (privateLuaCEmbedTableArray*)self->sub_tables,
            created
    );

    free(full_sub_table_name);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
    return created;

}



void LuaCEmbedTable_set_sub_table_prop(LuaCEmbedTable *self, const char *name, LuaCEmbedTable *sub_table){
    PRIVATE_LUA_CEMBED_TABLE_PROTECT_VOID

    //equivalent of  table.name = sub_table;
    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushstring(self->main_object->state,name);
    lua_getglobal(self->main_object->state,sub_table->global_name);
    lua_settable(self->main_object->state,-3);


    //equivalent of full_sub_table_name = table.sub_table
    lua_getglobal(self->main_object->state,self->global_name);
    privateLuaCEmbd_get_field_protected(self->main_object,name);

    char *full_sub_table_name = private_LuaCembed_format("%s_%s", self->global_name, name);
    lua_setglobal(self->main_object->state,full_sub_table_name);
    free(full_sub_table_name);
    PRIVATE_LUA_CEMBED_TABLE_CLEAR_STACK
}

//path: src/imports/../LuaCEmbed/table/table_array/fdefine.table_array.c
//mannaged by silver chain



privateLuaCEmbedTableArray *newprivateLuaCEmbedTableArray(){
    privateLuaCEmbedTableArray *self = (privateLuaCEmbedTableArray*) malloc(sizeof (privateLuaCEmbedTableArray));
    *self = (privateLuaCEmbedTableArray){0};
    self->tables = (LuaCEmbedTable**)malloc(0);
    return self;
}

void privateLuaCEmbedTableArray_append(privateLuaCEmbedTableArray *self,LuaCEmbedTable *element){

    self->tables = (LuaCEmbedTable **)realloc(
            self->tables,
           (self->size+1)* sizeof (LuaCEmbedTable**)
    );
    self->tables[self->size] = element;
    self->size+=1;
}


LuaCEmbedTable  *privateLuaCEmbedTableArray_find_by_prop_name(privateLuaCEmbedTableArray *self, const char *name){
    for(int i = 0; i < self->size;i++){
        LuaCEmbedTable  *current_table = self->tables[i];
        if(current_table->prop_name){
            if(strcmp(current_table->prop_name,name) ==0){
                return  current_table;
            }
        }
    }
    return NULL;
}

LuaCEmbedTable  *privateLuaCEmbedTableArray_find_by_global_name(privateLuaCEmbedTableArray *self, const char *name){
    for(int i = 0; i < self->size;i++){
        LuaCEmbedTable  *current_table = self->tables[i];
            if(strcmp(current_table->global_name,name) ==0){
                return  current_table;
            }
    }
    return NULL;
}

LuaCEmbedTable  *privateLuaCEmbedTableArray_find_by_internal_index(privateLuaCEmbedTableArray *self, lua_Integer internal_index){
    for(lua_Integer i = 0; i < self->size;i++){
        LuaCEmbedTable  *current_table = self->tables[i];
        if(current_table->index == internal_index){
            return current_table;
        }
    }
    return NULL;
}

void  privateLuaCEmbedTableArray_free(privateLuaCEmbedTableArray *self){

    for(int i = 0; i < self->size;i++){
        LuaCEmbedTable  *current_table = self->tables[i];
        privateLuaCEmbedTable_free(current_table);
    }

    free(self->tables);
    free(self);
}


//path: src/imports/../LuaCEmbedResponse/fdefine.LuaCEmbedResponse.c
//mannaged by silver chain




LuaCEmbedResponse *private_LuaCEmbedReturn_raw(){
    LuaCEmbedResponse *self = (LuaCEmbedResponse*)malloc(sizeof(LuaCEmbedResponse));
    *self = (LuaCEmbedResponse){0};
    return self;
}

LuaCEmbedResponse  * LuaCEmbed_send_bool(bool value){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_BOOL_RESPONSE;
    self->int_val = value;
    return self;
}


LuaCEmbedResponse * LuaCEmbed_send_str(const char *text){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_STRING_RESPONSE;
    self->string_size = (long)strlen(text);
    self->string_val  =  (char*)malloc(sizeof(char) * self->string_size +1);
    memcpy(self->string_val,text,self->string_size);
    self->string_val[self->string_size] = '\0';
    return self;
}

LuaCEmbedResponse * LuaCEmbed_send_raw_string(const char *text,lua_Integer size){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_STRING_RESPONSE;
    self->string_size = size;
    self->string_val  =  (char*)malloc(sizeof(char) * self->string_size +1);
    memcpy(self->string_val,text,self->string_size);
    self->string_val[self->string_size] = '\0';
    return self;
}

LuaCEmbedResponse * LuaCEmbed_send_str_reference(const char *text){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_STRING_RESPONSE;
    self->string_size = (long)strlen(text);
    self->string_val  = (char*)text;
    self->its_string_ref = true;
    return self;
}

LuaCEmbedResponse * LuaCEmbed_send_raw_string_reference(const  char *text,lua_Integer size){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_STRING_RESPONSE;
    self->string_size = size;
    self->string_val  = (char*)text;
    self->its_string_ref = true;
    return  self;
}



LuaCEmbedResponse * LuaCEmbed_send_error(const char *format,...){

    va_list args;
    va_start(args,format);
    char *error = private_LuaCembed_format_vaarg(format,args);
    va_end(args);

    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_ERROR_RESPONSE;
    self->string_val  = error;
    return self;
}


LuaCEmbedResponse * LuaCEmbed_send_multi_return(LuaCEmbedTable *table){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_MULTI_RESPONSE;
    lua_getglobal(table->main_object->state,table->global_name);
    lua_setglobal(table->main_object->state,PRIVATE_LUA_CEMBED_TABLE_RETURN);
    return self;
}

LuaCEmbedResponse * LuaCEmbed_send_table(LuaCEmbedTable *table){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_TABLE_RESPONSE;
    lua_getglobal(table->main_object->state,table->global_name);
    lua_setglobal(table->main_object->state,PRIVATE_LUA_CEMBED_TABLE_RETURN);
    return self;
}

LuaCEmbedResponse * LuaCEmbed_send_evaluation(const char *code){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_EVALUATION;
    self->string_val  = strdup(code);
    return self;
}

LuaCEmbedResponse  * LuaCEmbed_send_double(double value){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_DOUBLE_RESPONSE;
    self->double_val = value;
    return self;
}

LuaCEmbedResponse  * LuaCEmbed_send_long(lua_Integer value){
    LuaCEmbedResponse * self= private_LuaCEmbedReturn_raw();
    self->type = PRIVATE_LUA_CEMBED_LONG_RESPONSE;
    self->int_val = value;
    return self;
}



void private_LuaCEmbedResponse_free(LuaCEmbedResponse  *self){

    if(self->string_val && self->its_string_ref == false){
        free(self->string_val);
    }
    free(self);
}


//path: src/imports/../extra/fdefine.extra.c
//mannaged by silver chain



const char * LuaCembed_convert_arg_code(int arg_code){
    if(arg_code == LUA_CEMBED_NOT_FOUND){
        return PRIVATE_LUA_CEMBED_NOT_FOUND_STRING;
    }

    if(arg_code == LUA_CEMBED_STRING){
        return PRIVATE_LUA_CEMBED_STRING_STRING;

    }
    if(arg_code == LUA_CEMBED_NUMBER){
        return PRIVATE_LUA_CEMBED_NUMBER_STRING;
    }
    if(arg_code == LUA_CEMBED_BOOL){
        return  PRIVATE_LUA_CEMBED_BOOL_STRING;
    }
    if(arg_code == LUA_CEMBED_NIL){
        return  PRIVATE_LUA_CEMBED_NIL_STRING;
    }
    if(arg_code == LUA_CEMBED_TABLE){
        return  PRIVATE_LUA_CEMBED_TABLE_STRING;
    }
    if(arg_code == LUA_CEMBED_FUNCTION){
        return PRIVATE_LUA_CEMBED_FUNCTION_STRING;
    }

    return PRIVATE_LUA_CEMBED_UNKNOW_STRING;
}

char * private_LuaCembed_format_vaarg(const char *expresion, va_list args){

    va_list args_copy;
    va_copy(args_copy, args);
    long required_size = vsnprintf(NULL, 0,expresion,args_copy);
    va_end(args_copy);
    char *buffer = (char*)malloc(sizeof(char) * required_size + 2);
    vsnprintf(buffer,sizeof (char) * required_size+1,expresion,args);
    return buffer;
}
char * private_LuaCembed_format(const char *expresion, ...){
    va_list  args;
    va_start(args,expresion);
    char *result = private_LuaCembed_format_vaarg(expresion,args);
    va_end(args);
    return  result;
}

//path: src/imports/../namespace/LuaCEmbedNamespace/fdefine.LuaCEmbedNamespace.c
//mannaged by silver chain




LuaCEmbedNamespace newLuaCEmbedNamespace(){
    LuaCEmbedNamespace self = {0};
    self.newLuaLib = newLuaCEmbedLib;
    self.load_lib_from_c = LuaCEmbed_load_lib_from_c;
    self.newLuaEvaluation = newLuaCEmbedEvaluation;
    self.set_delete_function = LuaCembed_set_delete_function;
    self.perform = LuaCembed_perform;
    self.send_self_as_lib = LuaCembed_send_self_as_lib;
    self.send_global_as_lib = LuaCembed_send_global_as_lib;

    self.load_native_libs = LuaCEmbed_load_native_libs;
    self.set_memory_limit = LuaCEmbed_set_memory_limit;
    self.clear_errors = LuaCEmbed_clear_errors;
    self.convert_arg_code = LuaCembed_convert_arg_code;
    self.tables = newLuaCembedTableModule();
    self.args = newLuaCembedArgsModule();
    self.types = newLuaCEmbedTypeModule();
    self.globals = newLuaCEmbedGlobalModule();
    self.response = newLuaCEmbedResponseModule();
    self.get_error_message = LuaCEmbed_get_error_message;
    self.has_errors = LuaCEmbed_has_errors;
    self.set_timeout = LuaCEmbed_set_timeout;
    self.evaluate = LuaCEmbed_evaluate;
    self.get_string_evaluation = LuaCEmbed_get_evaluation_string;
    self.get_evaluation_type = LuaCEmbed_get_evaluation_type;
    self.get_evaluation_size = LuaCEmbed_get_evaluation_table_size;
    self.get_evaluation_long = LuaCEmbed_get_evaluation_long;
    self.get_evaluation_double  = LuaCEmbed_get_evaluation_double;
    self.get_evaluation_bool = LuaCEmbed_get_evaluation_bool;
    self.evaluete_file = LuaCEmbed_evaluete_file;
    self.add_callback = LuaCEmbed_add_callback;
    self.add_global_callback = LuaCEmbed_add_global_callback;
    self.set_bool_lib_prop = LuaCEmbed_set_bool_lib_prop;
    self.set_long_lib_prop = LuaCEmbed_set_long_lib_prop;
    self.set_double_lib_prop = LuaCEmbed_set_double_lib_prop;
    self.set_string_lib_prop = LuaCEmbed_set_string_lib_prop;
    self.set_table_lib_prop = LuaCEmbed_set_table_lib_prop;
    self.get_long_lib_prop = LuaCEmbed_get_long_lib_prop;
    self.get_double_lib_prop = LuaCEmbed_get_double_lib_prop;
    self.get_bool_lib_prop  = LuaCEmbed_get_bool_lib_prop;
    self.get_string_lib_prop = LuaCEmbed_get_string_lib_prop;
    self.get_table_lib_prop = LuaCEmbed_get_table_lib_prop;
    self.dangerous_raise_error_jumping = LuaCEmbed_dangerous_raise_error_jumping;
    self.dangerous_raise_self_error_jumping = LuaCEmbed_dangerous_raise_self_error_jumping;

    self.free = LuaCEmbed_free;
    return self;
}


//path: src/imports/../namespace/args/fdefine.args.c
//mannaged by silver chain



LuaCembedArgsModule newLuaCembedArgsModule(){
    LuaCembedArgsModule self = {0};
    self.get_bool = LuaCEmbed_get_bool_arg;
    self.get_long = LuaCEmbed_get_long_arg;
    self.get_double = LuaCEmbed_get_double_arg;
    self.get_str = LuaCEmbed_get_str_arg;
    self.get_raw_str = LuaCEmbed_get_raw_str_arg;
    self.get_type = LuaCEmbed_get_arg_type;
    self.get_table = LuaCEmbed_get_arg_table;
    self.size = LuaCEmbed_get_total_args;
    self.get_type_clojure_evalation = LuaCEmbed_get_type_clojure_evalation;
    self.generate_arg_clojure_evalation = LuaCEmbed_generate_arg_clojure_evalation;
    self.get_bool_arg_clojure_evalation = LuaCEmbed_get_bool_arg_clojure_evalation;
    self.get_long_arg_clojure_evalation = LuaCEmbed_get_long_arg_clojure_evalation;
    self.get_double_arg_clojure_evalation = LuaCEmbed_get_double_arg_clojure_evalation;
    self.get_string_arg_clojure_evalation = LuaCEmbed_get_string_arg_clojure_evalation;
    self.run_lambda = LuaCEmbed_run_args_lambda;
    return self;
}

//path: src/imports/../namespace/globals/fdefine.globals.c
//mannaged by silver chain



LuaCEmbedGlobalModule newLuaCEmbedGlobalModule(){
    LuaCEmbedGlobalModule self = {0};
    self.ensure_type = LuaCEmbed_ensure_global_type;
    self.get_double = LuaCEmbed_get_global_double;
    self.get_string = LuaCEmbed_get_global_string;
    self.get_long = LuaCEmbed_get_global_long;
    self.get_raw_string = LuaCEmbed_get_global_raw_string;
    self.set_raw_string = LuaCEmbed_set_global_raw_string;
    self.get_bool = LuaCEmbed_get_global_bool;
    self.get_type = LuaCEmbed_get_global_type;
    self.run_global_lambda = LuaCEmbed_run_global_lambda;
    self.set_table = LuaCEmbed_set_global_table;
    self.set_string = LuaCEmbed_set_global_string;
    self.set_bool = LuaCEmbed_set_global_bool;
    self.set_double = LuaCEmbed_set_global_double;
    self.set_long = LuaCEmbed_set_global_long;
    self.new_table = LuaCembed_new_global_table;
    self.get_table = LuaCembed_get_global_table;

    return self;
}

//path: src/imports/../namespace/response/fdefine.response.c
//mannaged by silver chain



LuaCEmbedResponseModule newLuaCEmbedResponseModule(){
    LuaCEmbedResponseModule self = {0};
    self.send_multi_return = LuaCEmbed_send_multi_return;
    self.send_str = LuaCEmbed_send_str;
    self.send_str_reference = LuaCEmbed_send_str_reference;
    self.send_raw_string = LuaCEmbed_send_raw_string;
    self.send_raw_string_reference = LuaCEmbed_send_raw_string_reference;
    self.send_bool = LuaCEmbed_send_bool;
    self.send_double = LuaCEmbed_send_double;
    self.send_evaluation_function =  LuaCEmbed_send_evaluation;
    self.send_long = LuaCEmbed_send_long;
    self.send_table = LuaCEmbed_send_table;
    self.send_error = LuaCEmbed_send_error;
    return  self;
}


//path: src/imports/../namespace/table/fdefine.table.c
//mannaged by silver chain



LuaCembedTableModule newLuaCembedTableModule(){
    LuaCembedTableModule self ={0};

    self.append_string = LuaCEmbedTable_append_string;
    self.append_bool = LuaCEmbedTable_append_bool;
    self.append_double = LuaCEmbedTable_append_double;
    self.append_long = LuaCEmbedTable_append_long;
    self.append_table = LuaCEmbedTable_append_table;
    self.append_evaluation = LuaCEmbedTable_append_evaluation;

    self.insert_string_at_index = LuaCEmbedTable_insert_string_at_index;
    self.insert_double_at_index = LuaCEmbedTable_insert_double_at_index;
    self.insert_long_at_index = LuaCEmbedTable_insert_long_at_index;
    self.insert_table_at_index = LuaCEmbedTable_insert_table_at_index;

    self.set_evaluation_prop = LuaCEmbedTable_set_evaluation_prop;
    self.set_method = LuaCEmbedTable_set_method;
    self.set_method = LuaCEmbedTable_set_method;
    self.set_bool_prop = LuaCEmbedTable_set_bool_prop;
    self.set_double_prop = LuaCEmbedTable_set_double_prop;
    self.set_long_prop = LuaCEmbedTable_set_long_prop;
    self.set_string_prop = LuaCEmbedTable_set_string_prop;
    self.set_raw_string_prop = LuaCEmbedTable_set_raw_string_prop;
    self.get_sub_table_prop = LuaCEmbedTable_get_sub_table_by_key;
    self.new_sub_table_prop = LuaCEmbedTable_new_sub_table_by_key;
    self.set_sub_table_prop = LuaCEmbedTable_set_sub_table_prop;
    self.new_anonymous_table = LuaCembed_new_anonymous_table;

    self.set_sub_table_by_index = LuaCEmbedTable_set_sub_table_by_index;
    self.get_bool_prop = LuaCembedTable_get_bool_prop;
    self.get_double_prop = LuaCembedTable_get_double_prop;
    self.get_long_prop = LuaCembedTable_get_long_prop;
    self.get_string_prop = LuaCembedTable_get_string_prop;
    self.get_raw_string_prop=  LuaCembedTable_get_raw_string_prop;
    self.get_type_prop = LuaCEmbedTable_get_type_prop;
    self.destroy_prop = LuaCembedTable_destroy_prop;

    self.set_long_by_index = LuaCEmbedTable_set_long_by_index;
    self.set_bool_by_index= LuaCEmbedTable_set_bool_by_index;
    self.set_double_by_index =LuaCEmbedTable_set_double_by_index;
    self.set_string_by_index = LuaCEmbedTable_set_string_by_index;
    self.set_raw_string_by_index = LuaCEmbedTable_set_raw_string_by_index;
    self.set_evaluation_by_index = LuaCEmbedTable_set_evaluation_by_index;

    self.get_listable_size = LuaCEmbedTable_get_listable_size;
    self.new_sub_table_appending = LuaCEmbedTable_new_sub_table_appending;
    self.has_key = LuaCembedTable_has_key_at_index;
    self.get_key_by_index = LuaCembedTable_get_key_by_index;
    self.get_size = LuaCEmbedTable_get_full_size;
    self.get_type_by_index = LuaCEmbedTable_get_type_by_index;
    self.get_long_by_index = LuaCEmbedTable_get_long_by_index;
    self.get_double_by_index = LuaCEmbedTable_get_double_by_index;
    self.get_bool_by_index = LuaCEmbedTable_get_bool_by_index;
    self.get_string_by_index= LuaCEmbedTable_get_string_by_index;
    self.get_raw_string_by_index =LuaCEmbedTable_get_raw_string_by_index;
    self.get_sub_table_by_index = LuaCEmbedTable_get_sub_table_by_index;
    return self;
}


//path: src/imports/../namespace/types/fdefine.types.c
//mannaged by silver chain


LuaCEmbedTypeModule newLuaCEmbedTypeModule(){
    LuaCEmbedTypeModule  self = {0};
    self.NOT_FOUND = LUA_CEMBED_NOT_FOUND;
    self.STRING = LUA_CEMBED_STRING;
    self.NUMBER = LUA_CEMBED_NUMBER;
    self.BOOL = LUA_CEMBED_BOOL;
    self.NILL = LUA_CEMBED_NIL;
    self.TABLE = LUA_CEMBED_TABLE;
    self.FUNCTION = LUA_CEMBED_FUNCTION;
    return self;
}

#endif




#define LUACEMBED_H
#endif
