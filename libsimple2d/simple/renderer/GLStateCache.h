#ifndef __GLSTATECACHE_H__
#define __GLSTATECACHE_H__

#include <stdint.h>
#if defined(__APPLE__)
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <OpenGL/glext.h>
#elif defined(_WIN32)
#include "GL/glew.h"
#endif
#if defined(__APPLE__)
#define DLL __attribute__ ((visibility("default")))
#elif defined(_WIN32)
#ifdef _USRDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
#endif

namespace GL
{

    enum
    {
        VERTEX_ATTRIB_FLAG_NONE         = 0,

        VERTEX_ATTRIB_FLAG_POSITION     = 1 << 0,
        VERTEX_ATTRIB_FLAG_COLOR        = 1 << 1,
        VERTEX_ATTRIB_FLAG_TEX_COORD    = 1 << 2,
        VERTEX_ATTRIB_FLAG_NORMAL       = 1 << 3,
        VERTEX_ATTRIB_FLAG_BLEND_WEIGHT = 1 << 4,
        VERTEX_ATTRIB_FLAG_BLEND_INDEX  = 1 << 5,

        VERTEX_ATTRIB_FLAG_POS_COLOR_TEX    = VERTEX_ATTRIB_FLAG_POSITION | VERTEX_ATTRIB_FLAG_COLOR | VERTEX_ATTRIB_FLAG_TEX_COORD
    };

    void DLL invalidateStateCache(void);

    void DLL useProgram(GLuint program);

    void DLL deleteProgram(GLuint program);

    void DLL blendFunc(GLenum sfactor, GLenum dfactor);

    void DLL blendResetToCache(void);

    void DLL setProjectionMatrixDirty(void);

    void DLL enableVertexAttribs(uint32_t flags);

    void DLL bindTexture2D(GLuint textureId);

    void DLL bindTexture2DN(GLuint textureUnit, GLuint textureId);

    void DLL bindTextureN(GLuint textureUnit, GLuint textureId, GLuint textureType = GL_TEXTURE_2D);

    void DLL deleteTexture(GLuint textureId);

    void DLL activeTexture(GLenum texture);

    void DLL bindVAO(GLuint vaoId);
}

#endif // !__GLSTATECACHE_H__
