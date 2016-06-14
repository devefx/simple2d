#ifndef __PLATFORM_GL_MAC_H__
#define __PLATFORM_GL_MAC_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_MAC

#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <OpenGL/glext.h>

#ifdef  GL_DEPTH24_STENCIL8
#undef  GL_DEPTH24_STENCIL8
#endif
#define GL_DEPTH24_STENCIL8 -1

#define glDeleteVertexArrays            glDeleteVertexArraysAPPLE
#define glGenVertexArrays               glGenVertexArraysAPPLE
#define glBindVertexArray               glBindVertexArrayAPPLE
#define glClearDepthf                   glClearDepth
#define glDepthRangef                   glDepthRange
#define glReleaseShaderCompiler(xxx)

#endif // TARGET_PLATFORM == PLATFORM_MAC

#endif // !__PLATFORM_GL_MAC_H__
