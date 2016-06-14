#ifndef __PLATFORM_GL_IOS_H__
#define __PLATFORM_GL_IOS_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_IOS

#define	glClearDepth				glClearDepthf
#define glDeleteVertexArrays		glDeleteVertexArraysOES
#define glGenVertexArrays			glGenVertexArraysOES
#define glBindVertexArray			glBindVertexArrayOES
#define glMapBuffer					glMapBufferOES
#define glUnmapBuffer				glUnmapBufferOES

#define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES
#define GL_WRITE_ONLY				GL_WRITE_ONLY_OES

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif // TARGET_PLATFORM == PLATFORM_IOS

#endif // !__PLATFORM_GL_IOS_H__
