#include "Vertex.h"

//
// Define static vertex formats
//

const VertexFormat VertexPosition::Format(
        VertexAttrib(VA_POSITION, 3, GL_FLOAT, 3 * sizeof(GLfloat), 0)
    );

const VertexFormat VertexPositionColor::Format(
        VertexAttrib(VA_POSITION, 3, GL_FLOAT, 7 * sizeof(GLfloat), 0),
        VertexAttrib(VA_COLOR,    4, GL_FLOAT, 7 * sizeof(GLfloat), 3 * sizeof(GLfloat))
    );

const VertexFormat VertexPositionNormal::Format(
        VertexAttrib(VA_POSITION, 3, GL_FLOAT, 6 * sizeof(GLfloat), 0),
        VertexAttrib(VA_NORMAL,   3, GL_FLOAT, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat))
    );

const VertexFormat VertexPositionTexture::Format(
        VertexAttrib(VA_POSITION, 3, GL_FLOAT, 5 * sizeof(GLfloat), 0),
        VertexAttrib(VA_TEXCOORD, 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat))
    );

const VertexFormat VertexPositionNormalTexture::Format(
        VertexAttrib(VA_POSITION, 3, GL_FLOAT, 8 * sizeof(GLfloat), 0),
        VertexAttrib(VA_NORMAL,   3, GL_FLOAT, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat)),
        VertexAttrib(VA_TEXCOORD, 2, GL_FLOAT, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat))
    );

//
// Implementation of VertexFormat class
//

VertexFormat::VertexFormat()
{
    // empty format (invalid)
}

VertexFormat::VertexFormat(const VertexAttrib& va)
{
    mAttribs.push_back(va);
}

VertexFormat::VertexFormat(const VertexAttrib& va1, const VertexAttrib& va2)
{
    mAttribs.push_back(va1);
    mAttribs.push_back(va2);
}

VertexFormat::VertexFormat(const VertexAttrib& va1, const VertexAttrib& va2, const VertexAttrib& va3)
{
    mAttribs.push_back(va1);
    mAttribs.push_back(va2);
    mAttribs.push_back(va3);
}

VertexFormat::VertexFormat(const VertexAttrib& va1, const VertexAttrib& va2, const VertexAttrib& va3, const VertexAttrib& va4)
{
    mAttribs.push_back(va1);
    mAttribs.push_back(va2);
    mAttribs.push_back(va3);
    mAttribs.push_back(va4);
}

void VertexFormat::activate() const
{
    for (unsigned i = 0; i < mAttribs.size(); i++) {
        const VertexAttrib* va = &mAttribs[i];
        glVertexAttribPointer(va->index, va->size, va->type, GL_FALSE, va->stride, va->offset);
        glEnableVertexAttribArray(va->index);
    }
}

void VertexFormat::deactivate() const
{
    for (unsigned i = 0; i < mAttribs.size(); i++) {
        const VertexAttrib* va = &mAttribs[i];
        glDisableVertexAttribArray(va->index);
    }
}
