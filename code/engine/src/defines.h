#ifndef _DEFINES_H_
    #define _DEFINES_H_

    #include <stdint.h>

    typedef float float32_t;
    typedef double float64_t;

    // Boolean types
    typedef uint32_t bool32_t;
    typedef uint8_t bool8_t;

    #define TRUE 1
    #define FALSE 0

    // Properly define static assertions.
    #if defined(__clang__) || defined(__gcc__)
        #define STATIC_ASSERT _Static_assert
    #else
        #define STATIC_ASSERT static_assert
    #endif

    // Platform detection
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        #define K_PLATFORM_WINDOWS 1
        #ifndef _WIN64
            #error "64-bit is required on Windows!"
        #endif
    #else
        #error "Unknown platform!"
    #endif

    #ifdef K_EXPORT
        // Exports
        #ifdef _MSC_VER
            #define K_API __declspec(dllexport)
        #else
            #define K_API __attribute__((visibility("default")))
        #endif
    #else
        // Imports
        #ifdef _MSC_VER
            #define K_API __declspec(dllimport)
        #else
            #define K_API
        #endif
    #endif

#endif  // _DEFINES_H_