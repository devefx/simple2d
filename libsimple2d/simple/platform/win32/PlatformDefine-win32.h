#ifndef __PLATFORM_DEFINE_WIN32_H__
#define __PLATFORM_DEFINE_WIN32_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_WIN32

#ifdef __MINGW32__
    #include <string.h>
#endif // !__MINGW32__

#if defined(STATIC)
    #define DLL
#else
    #if defined(_USRDLL)
        #define DLL     __declspec(dllexport)
    #else
        #define DLL     __declspec(dllimport)
    #endif
#endif // STATIC

#if DISABLE_ASSERT
    #define ASSERT(cond)
#else
    #include <assert.h>
    #define ASSERT(cond)    assert(cond)
#endif // DISABLE_ASSERT

#ifndef NULL
    #ifdef __cplusplus
        #define NULL    0
    #else
        #define NULL    ((void *)0)
    #endif
#endif // !NULL

#ifdef _DEBUG
#define SIMPLE2D_DEBUG 1
#endif // _DEBUG

#if _MSC_VER > 1800
#pragma comment(lib,"glfw3-2015.lib")
#else
#pragma comment(lib,"glfw3.lib")
#endif

#endif // TARGET_PLATFORM == PLATFORM_WIN32

#endif // !__PLATFORM_DEFINE_WIN32_H__
