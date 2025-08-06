
#if !defined(__EMSCRIPTEN__)

// Discoment these to allo emcc on your intelisense
// never Discoment these for building the lib
// these disable code errors on intelisense //but also make the code not works on browser
// so to work on browser compile with -D__EMSCRIPTEN__
//#define __EMSCRIPTEN_DISABLE_ERRORS__
//#define  __EMSCRIPTEN__
#endif 
#if !defined(_WIN32) 
// Discoment these to allo emcc on your intelisense
// never Discoment these for building the lib
//#define _WIN32
#endif