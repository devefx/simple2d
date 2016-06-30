#ifndef __PLATFORM_IMAGE_H__
#define __PLATFORM_IMAGE_H__

#include "base/Ref.h"
#include "renderer/Texture2D.h"

#include <string>
#include <ctype.h>

// premultiply alpha, or the effect will wrong when want to use other pixel format in Texture2D,
// such as RGB888, RGB5A1
#define RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))

/**
 * @addtogroup platform
 * @{
 */
NS_BEGIN

class DLL Image : public Ref
{
public:

    Image();

    virtual ~Image();

    /** Supported formats for Image */
    enum class Format
    {
        //! JPEG
        JPG,
        //! PNG
        PNG,
        //! TIFF
        TIFF,
        //! WebP
        WEBP,
        //! PVR
        PVR,
        //! ETC
        ETC,
        //! S3TC
        S3TC,
        //! ATITC
        ATITC,
        //! TGA
        TGA,
        //! Raw Data
        RAW_DATA,
        //! Unknown format
        UNKNOWN
    };

    bool initWithImageData(const unsigned char * data, size_t dataLen);


    // Getters
    inline unsigned char *  getData()               { return _data; }
    inline size_t           getDataLen()            { return _dataLen; }
    inline Format           getFileType()           { return _fileType; }
    inline Texture2D::PixelFormat getRenderFormat() { return _renderFormat; }
    inline int              getWidth()              { return _width; }
    inline int              getHeight()             { return _height; }
    inline bool             hasPremultipliedAlpha() { return _hasPremultipliedAlpha; }

    int                      getBitPerPixel();
    bool                     hasAlpha();
    bool                     isCompressed();

    bool saveToFile(const std::string &filename, bool isToRGB = true);

protected:

    bool initWithJpgData(const unsigned char * data, size_t dataLen);
    bool initWithPngData(const unsigned char * data, size_t dataLen);

    bool saveImageToPNG(const std::string& filePath, bool isToRGB = true);
    bool saveImageToJPG(const std::string& filePath);

    void premultipliedAlpha();
protected:

    unsigned char *_data;
    size_t _dataLen;
    int _width;
    int _height;
    bool _unpack;
    Format _fileType;
    Texture2D::PixelFormat _renderFormat;

    bool _hasPremultipliedAlpha;
protected:

    Format detectFormat(const unsigned char * data, size_t dataLen);
    bool isPng(const unsigned char * data, size_t dataLen);
    bool isJpg(const unsigned char * data, size_t dataLen);

};

NS_END
/**
 enf of platform group
 @}
 */

#endif // __PLATFORM_IMAGE_H__