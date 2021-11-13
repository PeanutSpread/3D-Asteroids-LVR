#include "Image.h"

#include <iostream>
#include <fstream>

enum TargaFileType {
    TARGA_RGB               = 2,
    TARGA_GRAYSCALE         = 3,
    TARGA_RLE_RGB           = 10,
    TARGA_RLE_GRAYSCALE     = 11
};

struct TargaHeader {
    unsigned char idLength;
    unsigned char colorMapType;
    unsigned char imageTypeCode;
    unsigned char colorMapSpec[5];
    unsigned short xOrigin;
    unsigned short yOrigin;
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char imageDesc;
};


Image::Image()
    : mWidth(0)
    , mHeight(0)
    , mBytesPerPixel(0)
    , mData(NULL)
{
}

Image::~Image()
{
    Deallocate();
}

bool Image::Allocate(int width, int height, int bytesPerPixel)
{
    // make sure to delete old memory if reallocating
    Deallocate();

    // allocate new memory for the image
    mData = new char[width * height * bytesPerPixel];
    mWidth = width;
    mHeight = height;
    mBytesPerPixel = bytesPerPixel;

    return true;
}

void Image::Deallocate()
{
    delete [] mData;
    mData = NULL;
    mWidth = 0;
    mHeight = 0;
    mBytesPerPixel = 0;
}

bool Image::LoadTarga(const std::string& path)
{
    // open the file in binary mode
    std::ifstream file(path.c_str(), std::ios::binary);

    // make sure the file opened correctly
    if (!file.good()) {
        std::cerr << "*** Failed to open file '" << path << "'" << std::endl;
        return false;
    }

    // get the file size
    file.seekg(0, std::ios::end);          // seek to the end of the file
    size_t len = (size_t)file.tellg();     // get the file pointer offset in bytes
    file.seekg(0, std::ios::beg);          // rewind back to the beginning

    // allocate memory for the file contents
    char* buf = new char[len];

    // read entire file into memory
    file.read(buf, len);
    if (!file.good()) {
        std::cerr << "*** Failed to read file '" << path << "'" << std::endl;
        delete [] buf;  // avoid a leak
        return false;
    }

    // the header is at the beginning of the file contents; use a cast to reinterpret that chunk of memory
    TargaHeader* hdr = reinterpret_cast<TargaHeader*>(buf);

    //std::cout << "Loading '" << path << "': " << hdr->width << "x" << hdr->height << ", " << (unsigned)hdr->bpp << " bpp" << std::endl;

    // check if the image type is supported
    switch (hdr->imageTypeCode) {
    case TARGA_RGB:
    case TARGA_GRAYSCALE:
    case TARGA_RLE_RGB:
    case TARGA_RLE_GRAYSCALE:
        // ok, these are supported types
        break;
    default:
        // anything else (like indexed formats) is unsupported
        std::cerr << "*** Unsuported TGA format" << std::endl;
        delete [] buf;  // avoid a leak
        return false;
    }

    // bit 4 of image descriptor indicates right-to-left pixel ordering, which we don't support
    if (hdr->imageDesc & 0x10) {
        std::cerr << "*** Oopsy doodle, right-to-left TGA files are not supported" << std::endl;
        delete [] buf;  // avoid a leak
        return false;
    }

    // allocate memory for the image data
    if (!Allocate(hdr->width, hdr->height, hdr->bpp / 8)) {
        std::cerr << "*** Failed to allocate memory for image" << std::endl;
        delete [] buf;  // avoid a leak
        return false;
    }

    // jump to the start of the image data (skip past header and optional variable-length id field)
    char* imgData = buf + sizeof(TargaHeader) + hdr->idLength;

    // decide how to load the image depending on type
    switch (hdr->imageTypeCode) {
    case TARGA_RGB:
    case TARGA_GRAYSCALE:
        // load an uncompressed image
        LoadTargaUncompressed(hdr, imgData);
        break;
    case TARGA_RLE_RGB:
    case TARGA_RLE_GRAYSCALE:
        // load RLE-compressed image
        LoadTargaRLE(hdr, imgData);
        break;
    default:
        // we should never get here
        std::cerr << "*** Oops, don't know how to load this format: fire the programmer" << std::endl;
        delete [] buf;
        Deallocate();
        return false;
    }

    // done loading, so deallocate the file contents
    delete [] buf;

    // all good, yay
    return true;
}

void Image::LoadTargaUncompressed(const TargaHeader* hdr, const char* imgData)
{
    int rowlen = (hdr->bpp / 8) * hdr->width;  // bytes per row
    int rowstep;
    char* dstRow;
    // check bit 5 of image descriptor to determine row ordering
    if (hdr->imageDesc & 0x20) {
        // bottom-to-top
        rowstep = -rowlen;
        dstRow = mData + rowlen * (hdr->height - 1);
    } else {
        // top-to-bottom
        rowstep = rowlen;
        dstRow = mData;
    }

    switch (hdr->bpp) {
    case 24:
        //std::cout << "~~ Loading 24 bpp" << std::endl;
        for (unsigned short j = 0; j < hdr->height; j++) {
            char* p = dstRow;
            for (unsigned short i = 0; i < hdr->width; i++) {
                char b = *imgData++;
                char g = *imgData++;
                char r = *imgData++;
                *p++ = r;
                *p++ = g;
                *p++ = b;
            }
            dstRow += rowstep;
        }
        break;

    case 32:
        //std::cout << "~~ Loading 32 bpp" << std::endl;
        for (unsigned short j = 0; j < hdr->height; j++) {
            char* p = dstRow;
            for (unsigned short i = 0; i < hdr->width; i++) {
                char b = *imgData++;
                char g = *imgData++;
                char r = *imgData++;
                char a = *imgData++;
                *p++ = r;
                *p++ = g;
                *p++ = b;
                *p++ = a;
            }
            dstRow += rowstep;
        }
        break;

    case 8:
        //std::cout << "~~ Loading 8 bpp" << std::endl;
        for (unsigned short j = 0; j < hdr->height; j++) {
            char* p = dstRow;
            for (unsigned short i = 0; i < hdr->width; i++) {
                *p++ = *imgData++;
            }
            dstRow += rowstep;
        }
        break;
    }
}

void Image::LoadTargaRLE(const TargaHeader* hdr, const char* imgData)
{
    int rowlen = (hdr->bpp / 8) * hdr->width;  // bytes per row
    int rowstep;
    char* dstRow;
    // check bit 5 of image descriptor to determine row ordering
    if (hdr->imageDesc & 0x20) {
        // bottom-to-top
        rowstep = -rowlen;
        dstRow = mData + rowlen * (hdr->height - 1);
    } else {
        // top-to-bottom
        rowstep = rowlen;
        dstRow = mData;
    }

    const unsigned numPixels = hdr->width * hdr->height;
    unsigned numPixelsRead = 0;
    unsigned short numPixelsInRow = 0;
    char* p = dstRow;

    switch (hdr->bpp) {
    case 24:
        //std::cout << "~~ Loading 24 bpp, RLE" << std::endl;
        while (numPixelsRead < numPixels) {
            unsigned char count = (unsigned char)*imgData++;
            if (count > 127) {
                // RLE packet
                count -= 127;
                char b = *imgData++;
                char g = *imgData++;
                char r = *imgData++;
                for (unsigned char i = 0; i < count; i++) {
                    *p++ = r;
                    *p++ = g;
                    *p++ = b;
                    if (++numPixelsInRow == hdr->width) {
                        // advance to next row
                        dstRow += rowstep;
                        p = dstRow;
                        numPixelsInRow = 0;
                    }
                }
            } else {
                // raw packet
                ++count;
                for (unsigned char i = 0; i < count; i++) {
                    char b = *imgData++;
                    char g = *imgData++;
                    char r = *imgData++;
                    *p++ = r;
                    *p++ = g;
                    *p++ = b;
                    if (++numPixelsInRow == hdr->width) {
                        // advance to next row
                        dstRow += rowstep;
                        p = dstRow;
                        numPixelsInRow = 0;
                    }
                }
            }
            numPixelsRead += count;
        }
        break;

    case 32:
        //std::cout << "~~ Loading 24 bpp, RLE" << std::endl;
        while (numPixelsRead < numPixels) {
            unsigned char count = (unsigned char)*imgData++;
            if (count > 127) {
                // RLE packet
                count -= 127;
                char b = *imgData++;
                char g = *imgData++;
                char r = *imgData++;
                char a = *imgData++;
                for (unsigned char i = 0; i < count; i++) {
                    *p++ = r;
                    *p++ = g;
                    *p++ = b;
                    *p++ = a;
                    if (++numPixelsInRow == hdr->width) {
                        // advance to next row
                        dstRow += rowstep;
                        p = dstRow;
                        numPixelsInRow = 0;
                    }
                }
            } else {
                // raw packet
                ++count;
                for (unsigned char i = 0; i < count; i++) {
                    char b = *imgData++;
                    char g = *imgData++;
                    char r = *imgData++;
                    char a = *imgData++;
                    *p++ = r;
                    *p++ = g;
                    *p++ = b;
                    *p++ = a;
                    if (++numPixelsInRow == hdr->width) {
                        // advance to next row
                        dstRow += rowstep;
                        p = dstRow;
                        numPixelsInRow = 0;
                    }
                }
            }
            numPixelsRead += count;
        }
        break;

    case 8:
        //std::cout << "~~ Loading 8 bpp, RLE" << std::endl;
        while (numPixelsRead < numPixels) {
            unsigned char count = (unsigned char)*imgData++;
            if (count > 127) {
                // RLE packet
                count -= 127;
                char val = *imgData++;
                for (unsigned char i = 0; i < count; i++) {
                    *p++ = val;
                    if (++numPixelsInRow == hdr->width) {
                        // advance to next row
                        dstRow += rowstep;
                        p = dstRow;
                        numPixelsInRow = 0;
                    }
                }
            } else {
                // raw packet
                ++count;
                for (unsigned char i = 0; i < count; i++) {
                    *p++ = *imgData++;
                    if (++numPixelsInRow == hdr->width) {
                        // advance to next row
                        dstRow += rowstep;
                        p = dstRow;
                        numPixelsInRow = 0;
                    }
                }
            }
            numPixelsRead += count;
        }
        break;
    }
}
