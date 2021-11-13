#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "glshell.h"
#include <string>

class Texture {
    GLuint mTexId;

public:
    Texture();
    Texture(const std::string& fname, GLint wrapMode = GL_REPEAT, GLint filteringMode = GL_LINEAR);

    ~Texture();

    GLuint id() const           { return mTexId; }

    bool isValid() const        { return mTexId > 0; }
};

#endif
