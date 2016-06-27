#ifndef __PLATFORM_IMAGE_H__
#define __PLATFORM_IMAGE_H__

#include "base/Ref.h"

#include <string>
#include <ctype.h>
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

    inline int               getWidth()              { return _width; }
    inline int               getHeight()             { return _height; }


protected:

    bool initWithJpgData(const unsigned char * data, size_t dataLen);
    bool initWithPngData(const unsigned char * data, size_t dataLen);

    void premultipliedAlpha();
protected:

    unsigned char *_data;
    size_t _dataLen;
    int _width;
    int _height;
    bool _unpack;
    Format _fileType;


    bool _hasPremultipliedAlpha;
protected:

    Format detectFormat(const unsigned char * data, size_t dataLen);
    bool isPng(const unsigned char * data, size_t dataLen);
    bool isJpg(const unsigned char * data, size_t dataLen);

};

NS_END

#endif // __PLATFORM_IMAGE_H__