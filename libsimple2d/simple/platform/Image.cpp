#include "platform/Image.h"

#include "unzip.h"
#include "png.h"
#include "jpeglib.h"

#define RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))

NS_BEGIN

Image::Image()
: _data(nullptr)
, _dataLen(0)
, _width(0)
, _height(0)
, _unpack(false)
, _fileType(Format::UNKNOWN)
{

}

Image::~Image()
{

}

bool Image::initWithImageData(const unsigned char * data, size_t dataLen)
{
    bool ret = false;

    do 
    {
        BREAK_IF(! data || dataLen <= 0);

        unsigned char* unpackedData = nullptr;
        size_t unpackedLen = 0;

        //
        unpackedData = const_cast<unsigned char*>(data);
        unpackedLen = dataLen;

        _fileType = detectFormat(unpackedData, unpackedLen);
        switch (_fileType)
        {
        case Image::Format::JPG:
            ret = initWithJpgData(unpackedData, unpackedLen);
            break;
        case Image::Format::PNG:
            ret = initWithPngData(unpackedData, unpackedLen);
            break;
        default:
            break;
        }
        if(unpackedData != data)
        {
            free(unpackedData);
        }
    } while (0);

    return ret;
}

bool Image::isPng(const unsigned char * data, size_t dataLen)
{
    if (dataLen <= 8)
    {
        return false;
    }

    static const unsigned char PNG_SIGNATURE[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};

    return memcmp(PNG_SIGNATURE, data, sizeof(PNG_SIGNATURE)) == 0;
}

bool Image::isJpg(const unsigned char * data, size_t dataLen)
{
    if (dataLen <= 4)
    {
        return false;
    }

    static const unsigned char JPG_SOI[] = {0xFF, 0xD8};

    return memcmp(data, JPG_SOI, 2) == 0;
}

Image::Format Image::detectFormat(const unsigned char * data, size_t dataLen)
{
    if (isPng(data, dataLen))
    {
        return Format::PNG;
    }
    else if (isJpg(data, dataLen))
    {
        return Format::JPG;
    }
    LOG("simple2d: can't detect image format");
    return Format::UNKNOWN;
}

//---------------------------------------------------------------------------------
//
// JPEG
//
//---------------------------------------------------------------------------------

struct MyErrorMgr
{
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

typedef struct MyErrorMgr * MyErrorPtr;

METHODDEF(void) myErrorExit(j_common_ptr cinfo)
{
    MyErrorPtr myerr = (MyErrorPtr) cinfo->err;

    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message) (cinfo, buffer);
    LOG("jpeg error: %s", buffer);

    longjmp(myerr->setjmp_buffer, 1);
}

bool Image::initWithJpgData(const unsigned char * data, size_t dataLen)
{
    
    struct jpeg_decompress_struct cinfo;
    struct MyErrorMgr jerr;

    JSAMPROW row_pointer[1] = {0};
    unsigned long location = 0;
    
    bool ret = false;
    do 
    {
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = myErrorExit;

        if (setjmp(jerr.setjmp_buffer))
        {
            jpeg_destroy_decompress(&cinfo);
            break;
        }

        jpeg_create_decompress(&cinfo);

        jpeg_mem_src(&cinfo, const_cast<unsigned char*>(data), dataLen);

        jpeg_read_header(&cinfo, TRUE);

        if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
        {
            //_renderFormat = Texture2D::PixelFormat::I8;
        }
        else
        {
            cinfo.out_color_space = JCS_RGB;
            //_renderFormat = Texture2D::PixelFormat::RGB888;
        }

        jpeg_start_decompress(&cinfo);

        _width  = cinfo.output_width;
        _height = cinfo.output_height;

        _dataLen = cinfo.output_width * cinfo.output_height * cinfo.output_components;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));

        BREAK_IF(!_data);

        while (cinfo.output_scanline < cinfo.output_height)
        {
            row_pointer[0] = _data + location;
            location += cinfo.output_width * cinfo.output_components;
            jpeg_read_scanlines(&cinfo, row_pointer, 1);
        }

        jpeg_destroy_decompress(&cinfo);

        ret = true;
    } while (0);
    
    return ret;
}
//---------------------------------------------------------------------------------
//
// PNG
//
//---------------------------------------------------------------------------------
#define PNGSIGSIZE  8

typedef struct 
{
    const unsigned char * data;
    size_t size;
    int offset;
} tImageSource;

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

    if((int)(isource->offset + length) <= isource->size)
    {
        memcpy(data, isource->data+isource->offset, length);
        isource->offset += length;
    }
    else
    {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

bool Image::initWithPngData(const unsigned char * data, size_t dataLen)
{
    png_byte        header[PNGSIGSIZE]  = {0}; 
    png_structp     png_ptr             = 0;
    png_infop       info_ptr            = 0;

    bool ret = false;
    
    do 
    {
        // png header len is 8 bytes
        BREAK_IF(dataLen < PNGSIGSIZE);
        // check the data is png or not
        memcpy(header, data, PNGSIGSIZE);
        BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));
        // init png_struct
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        BREAK_IF(!png_ptr);
        // init png_info
        info_ptr = png_create_info_struct(png_ptr);
        BREAK_IF(!info_ptr);

        BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
        // set the read call back function
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)data;
        imageSource.size    = dataLen;
        imageSource.offset  = 0;
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

        // read png file info
        png_read_info(png_ptr, info_ptr);

        _width = png_get_image_width(png_ptr, info_ptr);
        _height = png_get_image_height(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }
        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        {
            bit_depth = 8;
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }
        // reduce images with 16-bit samples to 8 bits
        if (bit_depth == 16)
        {
            png_set_strip_16(png_ptr);            
        }
        // Expanded earlier for grayscale, now take care of palette and rgb
        if (bit_depth < 8)
        {
            png_set_packing(png_ptr);
        }
        // update info
        png_read_update_info(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);

        switch (color_type)
        {
        case PNG_COLOR_TYPE_GRAY:
            //_renderFormat = Texture2D::PixelFormat::I8;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            //_renderFormat = Texture2D::PixelFormat::AI88;
            break;
        case PNG_COLOR_TYPE_RGB:
            //_renderFormat = Texture2D::PixelFormat::RGB888;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            //_renderFormat = Texture2D::PixelFormat::RGBA8888;
            break;
        default:
            break;
        }
        // read png data
        png_size_t rowbytes;
        png_bytep* row_pointers = (png_bytep* )malloc(sizeof(png_bytep) * _height);

        rowbytes = png_get_rowbytes(png_ptr, info_ptr);

        _dataLen = rowbytes * _height;
        _data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
        if (!_data)
        {
            if (row_pointers != nullptr)
            {
                free(row_pointers);
            }
            break;
        }

        for (unsigned short i = 0; i < _height; ++i)
        {
            row_pointers[i] = _data + i*rowbytes;
        }
        png_read_image(png_ptr, row_pointers);

        png_read_end(png_ptr, nullptr);

        // premultiplied alpha for RGBA8888
        if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        {
            premultipliedAlpha();
        }
        else
        {
            _hasPremultipliedAlpha = false;
        }

        if (row_pointers != nullptr)
        {
            free(row_pointers);
        }

        ret = true;

    } while (0);

    if (png_ptr)
    {
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    
    return ret;
}

void Image::premultipliedAlpha()
{
    //ASSERT(_renderFormat == Texture2D::PixelFormat::RGBA8888, "The pixel format should be RGBA8888!");

    unsigned int* fourBytes = (unsigned int*)_data;
    for(int i = 0; i < _width * _height; i++)
    {
        unsigned char* p = _data + i * 4;
        fourBytes[i] = RGB_PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
    }

    _hasPremultipliedAlpha = true;
}



NS_END