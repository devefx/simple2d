#ifndef __MACROS_H__
#define __MACROS_H__

#include "platform/PlatformDefine.h"


#ifndef SIMPLE2D_ASSERT
#if SIMPLE2D_DEBUG
//
#define SIMPLE2D_ASSERT(cond, msg) ASSERT(cond)
#else
#define SIMPLE2D_ASSERT(cond, msg)
#endif

#define GP_ASSERT(cond) SIMPLE2D_ASSERT(cond, "")
#endif // !SIMPLE2D_ASSERT



#ifdef SIMPLE2D_DEBUG

    #define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if (__error) { \
            \
        } \
    } while (false) 
#else
    #define CHECK_GL_ERROR_DEBUG()
#endif





#endif // !__MACROS_H__
