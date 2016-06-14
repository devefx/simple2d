#ifndef __PLATFORM_DEFINE_LINUX_H__
#define __PLATFORM_DEFINE_LINUX_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_LINUX

#include <string.h>

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

#endif // TARGET_PLATFORM == PLATFORM_LINUX

#endif // !__PLATFORM_DEFINE_LINUX_H__
