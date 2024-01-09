#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/console.h>
#endif

#ifndef EXPORT_FUNC
    #ifdef __EMSCRIPTEN__
        #define EXPORT_FUNC EMSCRIPTEN_KEEPALIVE
    #else
        #define EXPORT_FUNC
    #endif
#endif
