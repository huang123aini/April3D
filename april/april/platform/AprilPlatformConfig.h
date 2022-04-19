#pragma once

/**
 * 目前IOS、Android 、Linux  使用的是OpenGL ES 3.2版本
 */ 

#define APRIL_OS_UNKNOWN            0
#define APRIL_OS_IOS                1
#define APRIL_OS_ANDROID            2
#define APRIL_OS_WIN32              3
#define APRIL_OS_LINUX              4
#define APRIL_OS_MAC                5


#define APRIL_OS           APRIL_OS_UNKNOWN

#if defined(__APPLE__) && !defined(ANDROID) 
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE 
        #undef  APRIL_OS
        #define APRIL_OS         APRIL_OS_IOS
    #elif TARGET_OS_MAC
        #undef  APRIL_OS
        #define APRIL_OS         APRIL_OS_MAC
    #endif
#endif

// android
#if defined(ANDROID)
    #undef  APRIL_OS
    #define APRIL_OS         APRIL_OS_ANDROID
#endif

// win32
#if defined(_WIN32) && defined(_WINDOWS)
    #undef  APRIL_OS
    #define APRIL_OS         APRIL_OS_WIN32
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
    #undef  APRIL_OS
    #define APRIL_OS         APRIL_OS_LINUX
#endif


// check user set platform
#if ! APRIL_OS
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

#if (APRIL_OS == APRIL_OS_WIN32)
#ifndef __MINGW32__
#pragma warning (disable:4127)
#endif
#endif  //OS_WIN32

#if ((APRIL_OS == APRIL_OS_ANDROID) || (APRIL_OS == APRIL_OS_IOS))
    #define APRIL_OS_MOBILE
#else
    #define APRIL_OS_PC
#endif

#if  (APRIL_OS == APRIL_OS_IOS)
#define APRIL_USE_GLES

#elif (APRIL_OS == APRIL_OS_ANDROID)
    #define APRIL_USE_GLES
#else
    #define APRIL_USE_GL
#endif
