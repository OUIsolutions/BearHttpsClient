
#if defined(__EMSCRIPTEN__)


#include <string.h>
// these its just for intelisense stop showing errors
#if defined(__EMSCRIPTEN_DISABLE_ERRORS__)
#undef __EMSCRIPTEN__
#endif 
#define C2WASM_ALLOW_ASYNC
#include "../../../dependencies/c2wasm.c"

#if defined(__EMSCRIPTEN_DISABLE_ERRORS__)
#define __EMSCRIPTEN__
#endif 


#endif