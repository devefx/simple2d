#ifndef __BASE_TYPES_H_
#define __BASE_TYPES_H_

#include "math/Vec3.h"
#include "platform/GL.h"

NS_BEGIN

struct Color4B;
struct Color4F;

/**
 * RGB color composed of bytes 3 bytes.
 * @since v3.0
 */
struct DLL Color3B
{
    Color3B();
    Color3B(GLubyte _r, GLubyte _g, GLubyte _b);
    explicit Color3B(const Color4B& color);
    explicit Color3B(const Color4F& color);

    bool operator==(const Color3B& right) const;
    bool operator==(const Color4B& right) const;
    bool operator==(const Color4F& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4B& right) const;
    bool operator!=(const Color4F& right) const;

    bool equals(const Color3B& other)
    {
        return (*this == other);
    }

    GLubyte r;
    GLubyte g;
    GLubyte b;

    static const Color3B WHITE;
    static const Color3B YELLOW;
    static const Color3B BLUE;
    static const Color3B GREEN;
    static const Color3B RED;
    static const Color3B MAGENTA;
    static const Color3B BLACK;
    static const Color3B ORANGE;
    static const Color3B GRAY;
};

/**
 * RGBA color composed of 4 bytes.
 * @since v3.0
 */
struct DLL Color4B
{
    Color4B();
    Color4B(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a);
    explicit Color4B(const Color3B& color);
    explicit Color4B(const Color4F& color);

    bool operator==(const Color4B& right) const;
    bool operator==(const Color3B& right) const;
    bool operator==(const Color4F& right) const;
    bool operator!=(const Color4B& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4F& right) const;

    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;

    static const Color4B WHITE;
    static const Color4B YELLOW;
    static const Color4B BLUE;
    static const Color4B GREEN;
    static const Color4B RED;
    static const Color4B MAGENTA;
    static const Color4B BLACK;
    static const Color4B ORANGE;
    static const Color4B GRAY;
};


/**
 * RGBA color composed of 4 floats.
 * @since v3.0
 */
struct DLL Color4F
{
    Color4F();
    Color4F(float _r, float _g, float _b, float _a);
    explicit Color4F(const Color3B& color);
    explicit Color4F(const Color4B& color);

    bool operator==(const Color4F& right) const;
    bool operator==(const Color3B& right) const;
    bool operator==(const Color4B& right) const;
    bool operator!=(const Color4F& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4B& right) const;

    bool equals(const Color4F &other)
    {
        return (*this == other);
    }

    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;

    static const Color4F WHITE;
    static const Color4F YELLOW;
    static const Color4F BLUE;
    static const Color4F GREEN;
    static const Color4F RED;
    static const Color4F MAGENTA;
    static const Color4F BLACK;
    static const Color4F ORANGE;
    static const Color4F GRAY;
};

/** @struct Tex2F
 * A TEXCOORD composed of 2 floats: u, y
 * @since v3.0
 */
struct Tex2F {
    Tex2F(float _u, float _v): u(_u), v(_v) {}

    Tex2F(): u(0.f), v(0.f) {}

    GLfloat u;
    GLfloat v;
};

/** @struct V3F_C4B_T2F
 * A Vec2 with a vertex point, a tex coord point and a color 4B.
 */
struct V3F_C4B_T2F
{
    /// vertices (3F)
    Vec3     vertices;            // 12 bytes

    /// colors (4B)
    Color4B      colors;              // 4 bytes

    // tex coords (2F)
    Tex2F        texCoords;           // 8 bytes
};

/** @struct V3F_C4B_T2F_Quad
 * 4 Vertex3FTex2FColor4B.
 */
struct DLL V3F_C4B_T2F_Quad
{
    /// top left
    V3F_C4B_T2F    tl;
    /// bottom left
    V3F_C4B_T2F    bl;
    /// top right
    V3F_C4B_T2F    tr;
    /// bottom right
    V3F_C4B_T2F    br;
};

/** @struct BlendFunc
 * Blend Function used for textures.
 */
struct DLL BlendFunc
{
    /** source blend function */
    GLenum src;
    /** destination blend function */
    GLenum dst;

    /** Blending disabled. Uses {GL_ONE, GL_ZERO} */
    static const BlendFunc DISABLE;
    /** Blending enabled for textures with Alpha premultiplied. Uses {GL_ONE, GL_ONE_MINUS_SRC_ALPHA} */
    static const BlendFunc ALPHA_PREMULTIPLIED;
    /** Blending enabled for textures with Alpha NON premultiplied. Uses {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA} */
    static const BlendFunc ALPHA_NON_PREMULTIPLIED;
    /** Enables Additive blending. Uses {GL_SRC_ALPHA, GL_ONE} */
    static const BlendFunc ADDITIVE;

    bool operator==(const BlendFunc &a) const
    {
        return src == a.src && dst == a.dst;
    }

    bool operator!=(const BlendFunc &a) const
    {
        return src != a.src || dst != a.dst;
    }

    bool operator<(const BlendFunc &a) const
    {
        return src < a.src || (src == a.src && dst < a.dst);
    }
};

NS_END

#endif // !__BASE_TYPES_H_
