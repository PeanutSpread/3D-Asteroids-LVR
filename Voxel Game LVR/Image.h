#ifndef IMAGE_H_
#define IMAGE_H_

#include "glshell.h"

#include <string>

// a forward declaration
struct TargaHeader;

class Image {
private:
    int             mWidth, mHeight;
    int             mBytesPerPixel;
    char*           mData;

public:
                    Image();
                    ~Image();

    bool            isGood() const                          { return mData != 0; }
    int             getWidth() const                        { return mWidth; }
    int             getHeight() const                       { return mHeight; }
    int             getBytesPerPixel() const                { return mBytesPerPixel; }
    int             getBitsPerPixel() const                 { return 8 * mBytesPerPixel; }
    const char*     getData() const                         { return mData; }
    char*           getData()                               { return mData; }

    bool            Allocate(int width, int height, int bytesPerPixel);
    void            Deallocate();

    bool            LoadTarga(const std::string& path);

private:
                    //
                    // helper methods for loading TGA images
                    //
    void            LoadTargaUncompressed(const TargaHeader* hdr, const char* imgData);
    void            LoadTargaRLE(const TargaHeader* hdr, const char* imgData);
};

//
// return GL texture type depending on image bytes-per-pixel (GL_RGB, GL_RGBA, or GL_LUMINANCE)
//
inline GLenum GetTextureType(const Image& img)
{
    switch (img.getBytesPerPixel()) {
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    case 1:
        return GL_LUMINANCE;
    default:
        return 0;   // hmmm, not a valid type
    }
}

#endif
