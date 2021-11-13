#include "Texture.h"
#include "Image.h"

Texture::Texture()
    : mTexId(0)
{
}

Texture::Texture(const std::string& fname, GLint wrapMode, GLint filteringMode)
    : mTexId(0)
{
    Image img;
    if (img.LoadTarga(fname)) {
        // create texture object
        glGenTextures(1, &mTexId);

        // activate this texture
        glBindTexture(GL_TEXTURE_2D, mTexId);

        // the Image class does not pad rows, so set most flexible alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GetTextureType(img), img.getWidth(), img.getHeight(),
                                    0, GetTextureType(img), GL_UNSIGNED_BYTE, img.getData());

        // configure wrap mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

        // configure filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringMode);
    }
}

Texture::~Texture()
{
    if (mTexId)
        glDeleteTextures(1, &mTexId);
}
