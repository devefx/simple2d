#include "platform/Image.h"

#include "unzip.h"
#include "png.h"
#include "jpeglib.h"
#include <stdio.h>

NS_BEGIN

Image::Image()
: _data(nullptr)
, _dataLen(0)
, _width(0)
, _height(0)
, _unpack(false)
, _fileType(Format::UNKNOWN)
, _renderFormat(Texture2D::PixelFormat::NONE)
, _hasPremultipliedAlpha(true)
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

int Image::getBitPerPixel()
{
    return Texture2D::getPixelFormatInfoMap().at(_renderFormat).bpp;
}

bool Image::hasAlpha()
{
    return Texture2D::getPixelFormatInfoMap().at(_renderFormat).alpha;
}

bool Image::isCompressed()
{
    return Texture2D::getPixelFormatInfoMap().at(_renderFormat).compressed;
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
#define PNGSIGSIZE  8

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
            _renderFormat = Texture2D::PixelFormat::I8;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            _renderFormat = Texture2D::PixelFormat::AI88;
            break;
        case PNG_COLOR_TYPE_RGB:
            _renderFormat = Texture2D::PixelFormat::RGB888;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            _renderFormat = Texture2D::PixelFormat::RGBA8888;
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

bool Image::saveToFile(const std::string& filename, bool isToRGB)
{
    //only support for Texture2D::PixelFormat::RGB888 or Texture2D::PixelFormat::RGBA8888 uncompressed data
    if (isCompressed() || (_renderFormat != Texture2D::PixelFormat::RGB888 && _renderFormat != Texture2D::PixelFormat::RGBA8888))
    {
        LOG("simple2d: Image: saveToFile is only support for Texture2D::PixelFormat::RGB888 or Texture2D::PixelFormat::RGBA8888 uncompressed data for now");
        return false;
    }

    bool ret = false;

    do 
    {
        BREAK_IF(filename.size() <= 4);

        std::string strLowerCasePath(filename);
        for (unsigned int i = 0; i < strLowerCasePath.length(); ++i)
        {
            strLowerCasePath[i] = tolower(filename[i]);
        }

        if (std::string::npos != strLowerCasePath.find(".png"))
        {
            ret = saveImageToPNG(filename, isToRGB);
        }
        else if (std::string::npos != strLowerCasePath.find(".jpg"))
        {
            ret = saveImageToJPG(filename);
        }

    } while (0);

    return ret;
}

bool Image::saveImageToPNG(const std::string& filePath, bool isToRGB)
{
    bool ret = false;

    do 
    {
        FILE *fp;
        png_structp png_ptr;
        png_infop info_ptr;
        png_colorp palette;
        png_bytep *row_pointers;

        fp = fopen(filePath.c_str(), "wb");
        BREAK_IF(nullptr == fp);

        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

        if (nullptr == png_ptr)
        {
            fclose(fp);
            break;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (nullptr == info_ptr)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, nullptr);
            break;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }

        png_init_io(png_ptr, fp);

        if (!isToRGB && hasAlpha())
        {
            png_set_IHDR(png_ptr, info_ptr, _width, _height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        } 
        else
        {
            png_set_IHDR(png_ptr, info_ptr, _width, _height, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        }

        palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
        png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);

        png_write_info(png_ptr, info_ptr);

        png_set_packing(png_ptr);

        row_pointers = (png_bytep *)malloc(_height * sizeof(png_bytep));
        if(row_pointers == nullptr)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }

        if (!hasAlpha())
        {
            for (int i = 0; i < (int)_height; i++)
            {
                row_pointers[i] = (png_bytep)_data + i * _width * 3;
            }

            png_write_image(png_ptr, row_pointers);

            free(row_pointers);
            row_pointers = nullptr;
        }
        else
        {
            if (isToRGB)
            {
                unsigned char *tempData = static_cast<unsigned char*>(malloc(_width * _height * 3 * sizeof(unsigned char)));
                if (nullptr == tempData)
                {
                    fclose(fp);
                    png_destroy_write_struct(&png_ptr, &info_ptr);

                    free(row_pointers);
                    row_pointers = nullptr;
                    break;
                }

                for (int i = 0; i < _height; ++i)
                {
                    for (int j = 0; j < _width; ++j)
                    {
                        tempData[(i * _width + j) * 3] = _data[(i * _width + j) * 4];
                        tempData[(i * _width + j) * 3 + 1] = _data[(i * _width + j) * 4 + 1];
                        tempData[(i * _width + j) * 3 + 2] = _data[(i * _width + j) * 4 + 2];
                    }
                }

                for (int i = 0; i < (int)_height; i++)
                {
                    row_pointers[i] = (png_bytep)tempData + i * _width * 3;
                }

                png_write_image(png_ptr, row_pointers);

                free(row_pointers);
                row_pointers = nullptr;

                if (tempData != nullptr)
                {
                    free(tempData);
                }
            } 
            else
            {
                for (int i = 0; i < (int)_height; i++)
                {
                    row_pointers[i] = (png_bytep)_data + i * _width * 4;
                }

                png_write_image(png_ptr, row_pointers);

                free(row_pointers);
                row_pointers = nullptr;
            }
        }

        png_write_end(png_ptr, info_ptr);

        png_free(png_ptr, palette);
        palette = nullptr;

        png_destroy_write_struct(&png_ptr, &info_ptr);

        fclose(fp);

        ret = true;

    } while (0);
    
    return ret;
}

bool Image::saveImageToJPG(const std::string& filePath)
{
    bool ret = false;

    do 
    {
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        FILE * outfile;                 /* target file */
        JSAMPROW row_pointer[1];        /* pointer to JSAMPLE row[s] */
        int     row_stride;             /* physical row width in image buffer */

        cinfo.err = jpeg_std_error(&jerr);
        /* Now we can initialize the JPEG compression object. */
        jpeg_create_compress(&cinfo);

        BREAK_IF((outfile = fopen(filePath.c_str(), "wb")) == nullptr);

        jpeg_stdio_dest(&cinfo, outfile);

        cinfo.image_width = _width;           /* image width and height, in pixels */
        cinfo.image_height = _height;
        cinfo.input_components = 3;           /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB;       /* colorspace of input image */

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 90, TRUE);

        jpeg_start_compress(&cinfo, TRUE);

        row_stride = _width * 3;              /* JSAMPLEs per row in image_buffer */

        if (hasAlpha())
        {
            unsigned char *tempData = static_cast<unsigned char*>(malloc(_width * _height * 3 * sizeof(unsigned char)));
            if (nullptr == tempData)
            {
                jpeg_finish_compress(&cinfo);
                jpeg_destroy_compress(&cinfo);
                fclose(outfile);
                break;
            }

            for (int i = 0; i < _height; ++i)
            {
                for (int j = 0; j < _width; ++j)

                {
                    tempData[(i * _width + j) * 3] = _data[(i * _width + j) * 4];
                    tempData[(i * _width + j) * 3 + 1] = _data[(i * _width + j) * 4 + 1];
                    tempData[(i * _width + j) * 3 + 2] = _data[(i * _width + j) * 4 + 2];
                }
            }

            while (cinfo.next_scanline < cinfo.image_height)
            {
                row_pointer[0] = & tempData[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }

            if (tempData != nullptr)
            {
                free(tempData);
            }
        } 
        else
        {
            while (cinfo.next_scanline < cinfo.image_height) {
                row_pointer[0] = & _data[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }
        }

        jpeg_finish_compress(&cinfo);
        fclose(outfile);
        jpeg_destroy_compress(&cinfo);

        ret = true;
    } while (0);

    return ret;
}

void Image::premultipliedAlpha()
{
    unsigned int* fourBytes = (unsigned int*)_data;
    for(int i = 0; i < _width * _height; i++)
    {
        unsigned char* p = _data + i * 4;
        fourBytes[i] = RGB_PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
    }

    _hasPremultipliedAlpha = true;
}

NS_END