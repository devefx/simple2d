#include "renderer/Texture2D.h"

#include "renderer/GLStateCache.h"

NS_BEGIN

namespace {
    typedef Texture2D::PixelFormatInfoMap::value_type PixelFormatInfoMapValue;
    static const PixelFormatInfoMapValue TexturePixelFormatInfoTablesValue[] =
    {
        PixelFormatInfoMapValue(Texture2D::PixelFormat::BGRA8888, Texture2D::PixelFormatInfo(GL_BGRA, GL_BGRA, GL_UNSIGNED_BYTE, 32, false, true)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::RGBA8888, Texture2D::PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 32, false, true)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::RGBA4444, Texture2D::PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, 16, false, true)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::RGB5A1, Texture2D::PixelFormatInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 16, false, true)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::RGB565, Texture2D::PixelFormatInfo(GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 16, false, false)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::RGB888, Texture2D::PixelFormatInfo(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 24, false, false)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::A8, Texture2D::PixelFormatInfo(GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, 8, false, false)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::I8, Texture2D::PixelFormatInfo(GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, 8, false, false)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::AI88, Texture2D::PixelFormatInfo(GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 16, false, true)),

#ifdef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
        PixelFormatInfoMapValue(Texture2D::PixelFormat::PVRTC2, Texture2D::PixelFormatInfo(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 2, true, false)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::PVRTC2A, Texture2D::PixelFormatInfo(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 2, true, true)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::PVRTC4, Texture2D::PixelFormatInfo(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, false)),
        PixelFormatInfoMapValue(Texture2D::PixelFormat::PVRTC4A, Texture2D::PixelFormatInfo(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, true)),
#endif

#ifdef GL_ETC1_RGB8_OES
        PixelFormatInfoMapValue(Texture2D::PixelFormat::ETC, Texture2D::PixelFormatInfo(GL_ETC1_RGB8_OES, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, false)),
#endif

#ifdef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
        PixelFormatInfoMapValue(Texture2D::PixelFormat::S3TC_DXT1, Texture2D::PixelFormatInfo(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 0xFFFFFFFF, 0xFFFFFFFF, 4, true, false)),
#endif

#ifdef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
        PixelFormatInfoMapValue(Texture2D::PixelFormat::S3TC_DXT3, Texture2D::PixelFormatInfo(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 0xFFFFFFFF, 0xFFFFFFFF, 8, true, false)),
#endif

#ifdef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
        PixelFormatInfoMapValue(Texture2D::PixelFormat::S3TC_DXT5, Texture2D::PixelFormatInfo(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0xFFFFFFFF, 0xFFFFFFFF, 8, true, false)),
#endif

#ifdef GL_ATC_RGB_AMD
        PixelFormatInfoMapValue(Texture2D::PixelFormat::ATC_RGB, Texture2D::PixelFormatInfo(GL_ATC_RGB_AMD,
        0xFFFFFFFF, 0xFFFFFFFF, 4, true, false)),
#endif

#ifdef GL_ATC_RGBA_EXPLICIT_ALPHA_AMD
        PixelFormatInfoMapValue(Texture2D::PixelFormat::ATC_EXPLICIT_ALPHA, Texture2D::PixelFormatInfo(GL_ATC_RGBA_EXPLICIT_ALPHA_AMD,
        0xFFFFFFFF, 0xFFFFFFFF, 8, true, false)),
#endif

#ifdef GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD
        PixelFormatInfoMapValue(Texture2D::PixelFormat::ATC_INTERPOLATED_ALPHA, Texture2D::PixelFormatInfo(GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD,
        0xFFFFFFFF, 0xFFFFFFFF, 8, true, false)),
#endif
    };
}

const Texture2D::PixelFormatInfoMap Texture2D::_pixelFormatInfoTables(TexturePixelFormatInfoTablesValue,
                                                                      TexturePixelFormatInfoTablesValue + sizeof(TexturePixelFormatInfoTablesValue) / sizeof(TexturePixelFormatInfoTablesValue[0]));

Texture2D::Texture2D()
: _pixelFormat(Texture2D::PixelFormat::DEFAULT)
, _pixelsWide(0)
, _pixelsHigh(0)
, _name(0)
//
, _antialiasEnabled(true)
{
}

Texture2D::~Texture2D()
{
    LOGINFO("deallocing Texture2D: %p - id=%u", this, _name);
    SAFE_RELEASE(_shaderProgram);

    if (_name)
    {
        GL::deleteTexture(_name);
    }
}

void Texture2D::releaseGLTexture()
{
    if(_name)
    {
        GL::deleteTexture(_name);
    }
    _name = 0;
}

Texture2D::PixelFormat Texture2D::getPixelFormat() const
{
    return _pixelFormat;
}

int Texture2D::getPixelsWide() const
{
    return _pixelsWide;
}

int Texture2D::getPixelsHigh() const
{
    return _pixelsHigh;
}

GLuint Texture2D::getName() const
{
    return _name;
}

Size Texture2D::getContentSize() const
{
    Size ret;
    //
    return ret;
}

const Size& Texture2D::getContentSizeInPixels()
{
    return _contentSize;
}

GLfloat Texture2D::getMaxS() const
{
    return _maxS;
}

void Texture2D::setMaxS(GLfloat maxS)
{
    _maxS = maxS;
}

GLfloat Texture2D::getMaxT() const
{
    return _maxT;
}

void Texture2D::setMaxT(GLfloat maxT)
{
    _maxT = maxT;
}

GLProgram* Texture2D::getGLProgram() const
{
    return _shaderProgram;
}

void Texture2D::setGLProgram(GLProgram* shaderProgram)
{
    SAFE_RETAIN(shaderProgram);
    SAFE_RELEASE(_shaderProgram);
    _shaderProgram = shaderProgram;
}

bool Texture2D::hasPremultipliedAlpha() const
{
    return _hasPremultipliedAlpha;
}

bool Texture2D::hasMipmaps() const
{
    return _hasMipmaps;
}

bool Texture2D::initWithData(const void *data, ssize_t dataLen, Texture2D::PixelFormat pixelFormat, int pixelsWide, int pixelsHigh)
{
    MipmapInfo mipmap;
    mipmap.address = (unsigned char*) data;
    mipmap.len = static_cast<int>(dataLen);
    return initWithMipmaps(&mipmap, 1, pixelFormat, pixelsWide, pixelsHigh);
}

bool Texture2D::initWithMipmaps(MipmapInfo* mipmaps, int mipmapsNum, PixelFormat pixelFormat, int pixelsWide, int pixelsHigh)
{
    //the pixelFormat must be a certain value 
    SIMPLE2D_ASSERT(pixelFormat != PixelFormat::NONE && pixelFormat != PixelFormat::AUTO, "the \"pixelFormat\" param must be a certain value!");
    SIMPLE2D_ASSERT(pixelsWide > 0 && pixelsHigh > 0, "Invalid size");

    if (mipmapsNum < 0)
    {
        LOG("WARNING: mipmap number is less than 1");
        return false;
    }

    if(_pixelFormatInfoTables.find(pixelFormat) == _pixelFormatInfoTables.end())
    {
        LOG("WARNING: unsupported pixelformat: %lx", (unsigned long) pixelFormat);
        return false;
    }

    const PixelFormatInfo& info = _pixelFormatInfoTables.at(pixelFormat);

    if (info.compressed/* && !Configuration::getInstance()->supportsPVRTC()
                        && !Configuration::getInstance()->supportsETC()
                        && !Configuration::getInstance()->supportsS3TC()
                        && !Configuration::getInstance()->supportsATITC()*/)
    {
        LOG("WARNING: PVRTC/ETC images are not supported");
        return false;
    }

    if (mipmapsNum == 1 && !info.compressed)
    {
        unsigned int bytesPerRow = pixelsWide * info.bpp / 8;

        if(bytesPerRow % 8 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
        }
        else if(bytesPerRow % 4 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
        else if(bytesPerRow % 2 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
        }
        else
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    if(_name != 0)
    {
        GL::deleteTexture(_name);
        _name = 0;
    }

    glGenTextures(1, &_name);
    GL::bindTexture2D(_name);

    if (mipmapsNum == 1)
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _antialiasEnabled ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST);
    }

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    //

    // clean possible GL error
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        LOG("OpenGL error 0x%04X in %s %s %d\n", err, __FILE__, __FUNCTION__, __LINE__);
    }

    // Specify OpenGL texture image
    int width = pixelsWide;
    int height = pixelsHigh;

    for (int i = 0; i < mipmapsNum; ++i)
    {
        unsigned char *data = mipmaps[i].address;
        GLsizei datalen = mipmaps[i].len;

        if (info.compressed)
        {
            glCompressedTexImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, datalen, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, info.format, info.type, data);
        }

        //
    }

    _contentSize = Size((float)pixelsWide, (float)pixelsHigh);
    _pixelsWide = pixelsWide;
    _pixelsHigh = pixelsHigh;
    _pixelFormat = pixelFormat;
    _maxS = 1;
    _maxT = 1;

    _hasPremultipliedAlpha = false;
    _hasMipmaps = mipmapsNum > 1;

    //
    return true;
}

bool Texture2D::updateWithData(const void *data,int offsetX,int offsetY,int width,int height)
{
    if (_name)
    {
        GL::bindTexture2D(_name);
        const PixelFormatInfo& info = _pixelFormatInfoTables.at(_pixelFormat);
        glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, info.format, info.type, data);
        return true;
    }
    return false;
}

//

const Texture2D::PixelFormatInfoMap& Texture2D::getPixelFormatInfoMap()
{
	return _pixelFormatInfoTables;
}

//

NS_END