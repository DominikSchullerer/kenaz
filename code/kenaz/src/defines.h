#ifndef _DEFINES_H_
    #define _DEFINES_H_

    /* ============================================================
    Basic types
    ============================================================ */

    #include <stdint.h>

    typedef float  float32_t;
    typedef double float64_t;

    typedef uint32_t bool32_t;
    typedef uint8_t  bool8_t;

    #define TRUE  1
    #define FALSE 0

    /* ============================================================
    Static assert
    ============================================================ */

    #if defined(__clang__) || defined(__gcc__)
        #define STATIC_ASSERT _Static_assert
    #else
        #define STATIC_ASSERT static_assert
    #endif

    /* ============================================================
    Platform detection
    ============================================================ */

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        #define K_PLATFORM_WINDOWS 1
        #ifndef _WIN64
            #error "64-bit Windows required!"
        #endif
    #else
        #error "Unsupported platform!"
    #endif

    /* ============================================================
    Build configuration
    ============================================================ */

    /*
    K_ENGINE     → Engine executable
    K_GAME_DLL   → Game dynamic library
    */

    #if defined(K_ENGINE) && defined(K_GAME_DLL)
        #error "K_ENGINE and K_GAME_DLL cannot both be defined!"
    #endif

    /* ============================================================
    API export / visibility
    ============================================================ */

    #if defined(K_GAME_DLL)
        #if defined(_MSC_VER)
            #define K_API __declspec(dllexport)
        #else
            #define K_API __attribute__((visibility("default")))
        #endif
    #else
        #define K_API
    #endif

#endif  // _DEFINES_H_