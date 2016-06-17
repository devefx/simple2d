#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include <map>

#include "base/Ref.h"
#include "math/Geometry.h"
#include "platform/GL.h"
#include "renderer/GLProgram.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
    #include <BaseTsd.h>
    #include <WinSock2.h>
    #ifndef __SSIZE_T
        #define __SSIZE_T
        typedef SSIZE_T ssize_t;
    #endif
#else
    #include <sys/select.h>
#endif

/**
 * @addtogroup base
 * @{
 */
NS_BEGIN

typedef struct MipmapInfo
{
    unsigned char* address;
    int len;
    MipmapInfo():address(NULL), len(0) {}
} MipmapInfo;

class DLL Texture2D : public Ref
{
public:
    /** @typedef Texture2D::PixelFormat
     Possible texture pixel formats
     */
    enum class PixelFormat
    {
        //! auto detect the type
        AUTO,
        //! 32-bit texture: BGRA8888
        BGRA8888,
        //! 32-bit texture: RGBA8888
        RGBA8888,
        //! 24-bit texture: RGBA888
        RGB888,
        //! 16-bit texture without Alpha channel
        RGB565,
        //! 8-bit textures used as masks
        A8,
        //! 8-bit intensity texture
        I8,
        //! 16-bit textures used as masks
        AI88,
        //! 16-bit textures: RGBA4444
        RGBA4444,
        //! 16-bit textures: RGB5A1
        RGB5A1,
        //! 4-bit PVRTC-compressed texture: PVRTC4
        PVRTC4,
        //! 4-bit PVRTC-compressed texture: PVRTC4 (has alpha channel)
        PVRTC4A,
        //! 2-bit PVRTC-compressed texture: PVRTC2
        PVRTC2,
        //! 2-bit PVRTC-compressed texture: PVRTC2 (has alpha channel)
        PVRTC2A,
        //! ETC-compressed texture: ETC
        ETC,
        //! S3TC-compressed texture: S3TC_Dxt1
        S3TC_DXT1,
        //! S3TC-compressed texture: S3TC_Dxt3
        S3TC_DXT3,
        //! S3TC-compressed texture: S3TC_Dxt5
        S3TC_DXT5,
        //! ATITC-compressed texture: ATC_RGB
        ATC_RGB,
        //! ATITC-compressed texture: ATC_EXPLICIT_ALPHA
        ATC_EXPLICIT_ALPHA,
        //! ATITC-compresed texture: ATC_INTERPOLATED_ALPHA
        ATC_INTERPOLATED_ALPHA,
        //! Default texture format: AUTO
        DEFAULT = AUTO,
        
        NONE = -1
    };

    struct PixelFormatInfo {

        PixelFormatInfo(GLenum anInternalFormat, GLenum aFormat, GLenum aType, int aBpp, bool aCompressed, bool anAlpha)
            : internalFormat(anInternalFormat)
            , format(aFormat)
            , type(aType)
            , bpp(aBpp)
            , compressed(aCompressed)
            , alpha(anAlpha)
        {}

        GLenum internalFormat;
        GLenum format;
        GLenum type;
        int bpp;
        bool compressed;
        bool alpha;
    };

    typedef std::map<Texture2D::PixelFormat, const PixelFormatInfo> PixelFormatInfoMap;
public:

    Texture2D();

    virtual ~Texture2D();

    void releaseGLTexture();

    bool initWithData(const void *data, ssize_t dataLen, Texture2D::PixelFormat pixelFormat, int pixelsWide, int pixelsHigh);

    bool initWithMipmaps(MipmapInfo* mipmaps, int mipmapsNum, Texture2D::PixelFormat pixelFormat, int pixelsWide, int pixelsHigh);

    bool updateWithData(const void *data,int offsetX,int offsetY,int width,int height);

    //

    /** Get content size. */
    const Size& getContentSizeInPixels();

    /** Whether or not the texture has their Alpha premultiplied. */
    bool hasPremultipliedAlpha() const;

    /** Whether or not the texture has mip maps.*/
    bool hasMipmaps() const;

    /** Gets the pixel format of the texture. */
    Texture2D::PixelFormat getPixelFormat() const;

    /** Gets the width of the texture in pixels. */
    int getPixelsWide() const;

    /** Gets the height of the texture in pixels. */
    int getPixelsHigh() const;

    /** Gets the texture name. */
    GLuint getName() const;

    /** Gets max S. */
    GLfloat getMaxS() const;
    /** Sets max S. */
    void setMaxS(GLfloat maxS);

    /** Gets max T. */
    GLfloat getMaxT() const;
    /** Sets max T. */
    void setMaxT(GLfloat maxT);

    /** Get the texture content size.*/
    Size getContentSize() const;

    /** Set a shader program to the texture.

     It's used by drawAtPoint and drawInRect
     */
    void setGLProgram(GLProgram* program);

    /** Get a shader program from the texture.*/
    GLProgram* getGLProgram() const;

public:
    /** Get pixel info map, the key-value pairs is PixelFormat and PixelFormatInfo.*/
    static const PixelFormatInfoMap& getPixelFormatInfoMap();

protected:
    /** pixel format of the texture */
    Texture2D::PixelFormat _pixelFormat;

    /** width in pixels */
    int _pixelsWide;

    /** height in pixels */
    int _pixelsHigh;

    /** texture name */
    GLuint _name;

    /** texture max S */
    GLfloat _maxS;

    /** texture max T */
    GLfloat _maxT;

    /** content size */
    Size _contentSize;

    /** whether or not the texture has their Alpha premultiplied */
    bool _hasPremultipliedAlpha;

    /** whether or not the texture has mip maps*/
    bool _hasMipmaps;

    /** shader program used by drawAtPoint and drawInRect */
    GLProgram* _shaderProgram;

    static const PixelFormatInfoMap _pixelFormatInfoTables;

    bool _antialiasEnabled;
};

NS_END
/**
 end of base group
 @}
 */

#endif // !__TEXTURE2D_H__
