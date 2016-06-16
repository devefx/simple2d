#include "renderer/GLStateCache.h"

#include "base/Director.h"

NS_BEGIN

static const int MAX_ATTRIBUTES = 16;
static const int MAX_ACTIVE_TEXTURE = 16;

#define ENABLE_GL_STATE_CACHE 1

namespace
{
    static GLuint s_currentProjectionMatrix = -1;
    static uint32_t s_attributeFlags = 0;

#if ENABLE_GL_STATE_CACHE

    static GLuint    s_currentShaderProgram = -1;
    static GLuint    s_currentBoundTexture[MAX_ACTIVE_TEXTURE] =  {(GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, };
    static GLenum    s_blendingSource = -1;
    static GLenum    s_blendingDest = -1;
    static int       s_GLServerState = 0;
    static GLuint    s_VAO = 0;
    static GLenum    s_activeTexture = -1;

#endif

}

// GL State Cache functions

namespace GL
{
    void invalidateStateCache(void)
    {
        Matrix::getInstance()->resetMatrixStack();
        s_currentProjectionMatrix = -1;
        s_attributeFlags = 0;

#if ENABLE_GL_STATE_CACHE
        s_currentShaderProgram = -1;
        for( int i=0; i < MAX_ACTIVE_TEXTURE; i++ )
        {
            s_currentBoundTexture[i] = -1;
        }
        s_blendingSource = -1;
        s_blendingDest = -1;
        s_GLServerState = 0;
        s_VAO = 0;
#endif
    }

    void useProgram(GLuint program)
    {
#if ENABLE_GL_STATE_CACHE
        if (program != s_currentShaderProgram)
        {
            s_currentShaderProgram = program;
            glUseProgram(program);
        }
#else
        glUseProgram(program);
#endif
    }

    void deleteProgram(GLuint program)
    {
#if ENABLE_GL_STATE_CACHE
        if(program == s_currentShaderProgram)
        {
            s_currentShaderProgram = -1;
        }
#endif
        glDeleteProgram(program);
    }

    static void setBlending(GLenum sfactor, GLenum dfactor)
    {
        if (sfactor == GL_ONE && dfactor == GL_ZERO)
        {
            glDisable(GL_BLEND);
        }
        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(sfactor, dfactor);
        }
    }

    void blendFunc(GLenum sfactor, GLenum dfactor)
    {
#if ENABLE_GL_STATE_CACHE
        if (sfactor != s_blendingSource || dfactor != s_blendingDest)
        {
            s_blendingSource = sfactor;
            s_blendingDest = dfactor;
            setBlending(sfactor, dfactor);
        }
#else
        setBlending(sfactor, dfactor);
#endif
    }

    void blendResetToCache(void)
    {
        glBlendEquation(GL_FUNC_ADD);
#if ENABLE_GL_STATE_CACHE
        setBlending(s_blendingSource, s_blendingDest);
#else
        setBlending(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif
    }

    void setProjectionMatrixDirty(void)
    {
        s_currentProjectionMatrix = -1;
    }

    void enableVertexAttribs(uint32_t flags)
    {
        bindVAO(0);

        // hardcoded!
        for(int i=0; i < MAX_ATTRIBUTES; i++) {
            unsigned int bit = 1 << i;
            // FIXME:Cache is disabled, try to enable cache as before
            bool enabled = (flags & bit) != 0;
            bool enabledBefore = (s_attributeFlags & bit) != 0;
            if(enabled != enabledBefore) 
            {
                if( enabled )
                    glEnableVertexAttribArray(i);
                else
                    glDisableVertexAttribArray(i);
            }
        }
        s_attributeFlags = flags;
    }

    void bindTexture2D(GLuint textureId)
    {
        GL::bindTexture2DN(0, textureId);
    }

    void bindTexture2DN(GLuint textureUnit, GLuint textureId)
    {
#if ENABLE_GL_STATE_CACHE
        if (s_currentBoundTexture[textureUnit] != textureId)
        {
            s_currentBoundTexture[textureUnit] = textureId;
            activeTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureId);
        }
#else
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureId);
#endif
    }

    void bindTextureN(GLuint textureUnit, GLuint textureId, GLuint textureType)
    {
#if ENABLE_GL_STATE_CACHE
        if (s_currentBoundTexture[textureUnit] != textureId)
        {
            s_currentBoundTexture[textureUnit] = textureId;
            activeTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(textureType, textureId);
        }
#else
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(textureType, textureId);
#endif
    }

    void deleteTexture(GLuint textureId)
    {
#if ENABLE_GL_STATE_CACHE
        for (size_t i = 0; i < MAX_ACTIVE_TEXTURE; ++i)
        {
            if (s_currentBoundTexture[i] == textureId)
            {
                s_currentBoundTexture[i] = -1;
            }
        }
#endif
        glDeleteTextures(1, &textureId);
    }

    void activeTexture(GLenum texture)
    {
#if ENABLE_GL_STATE_CACHE
        if(s_activeTexture != texture) {
            s_activeTexture = texture;
            glActiveTexture(s_activeTexture);
        }
#else
        glActiveTexture(texture);
#endif
    }

    void bindVAO(GLuint vaoId)
    {

    }
}


NS_END