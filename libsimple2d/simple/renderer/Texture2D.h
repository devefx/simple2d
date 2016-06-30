#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include <string>
#include <map>

#include "base/Ref.h"
#include "base/Types.h"

NS_BEGIN

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



public:
    /** Get pixel info map, the key-value pairs is PixelFormat and PixelFormatInfo.*/
    static const PixelFormatInfoMap& getPixelFormatInfoMap();

private:



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
    //Size _contentSize;

    /** whether or not the texture has their Alpha premultiplied */
    bool _hasPremultipliedAlpha;

    /** whether or not the texture has mip maps*/
    bool _hasMipmaps;

    /** shader program used by drawAtPoint and drawInRect */
    //GLProgram* _shaderProgram;

    static const PixelFormatInfoMap _pixelFormatInfoTables;

    bool _antialiasEnabled;
};

NS_END

#endif // !__TEXTURE2D_H__
