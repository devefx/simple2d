#ifndef __PLATFORM_DEFINE_ANDROID_H__
#define __PLATFORM_DEFINE_ANDROID_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_ANDROID

#define DLL

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

#endif // TARGET_PLATFORM == PLATFORM_ANDROID

#endif // !__PLATFORM_DEFINE_ANDROID_H__
