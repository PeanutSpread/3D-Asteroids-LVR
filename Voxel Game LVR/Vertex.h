#ifndef VERTEX_H_
#define VERTEX_H_

#include "glshell.h"

//
// Vertex attribute locations.
// All vertex shaders must follow this convention (use layout(location=X) qualifiers)
//
enum VertexAttribIndex {
    VA_POSITION  = 0,
    VA_NORMAL    = 1,
    VA_COLOR     = 2,
    VA_TEXCOORD  = 3,
};


//
// A structure that holds the arguments to glVertexAttribPointer.
// In essence, it defines the layout of a vertex attribute in a VBO.
//
struct VertexAttrib {
    GLuint index;
    GLint size;
    GLenum type;
    GLsizei stride;
    const GLvoid* offset;

    // default constructor initializes everything to 0 (meaningless values)
    VertexAttrib()
        : index(0), size(0), type(0), stride(0), offset(0)
    { }

    VertexAttrib(GLuint index, GLint size, GLenum type, GLsizei stride, size_t bufOffset)
        : index(index), size(size), type(type), stride(stride), offset((const GLvoid*)bufOffset)
    { }
};


//
// A VertexFormat is defined as a list of vertex attributes and their characteristics
//
class VertexFormat {
    std::vector<VertexAttrib> mAttribs;  // list of attributes

public:
    // default constructor (creates empty attribute list)
    VertexFormat();

    // constructors for easily creating VertexFormats with a few attributes
    VertexFormat(const VertexAttrib& va);
    VertexFormat(const VertexAttrib& va1, const VertexAttrib& va2);
    VertexFormat(const VertexAttrib& va1, const VertexAttrib& va2, const VertexAttrib& va3);
    VertexFormat(const VertexAttrib& va1, const VertexAttrib& va2, const VertexAttrib& va3, const VertexAttrib& va4);

    // activate/deactivate this vertex format with OpenGL
    void activate() const;
    void deactivate() const;
};


//
// a structure that stores vertex position information
//
struct VertexPosition {

    // the following floats will be laid out consecutively in memory
    GLfloat x, y, z;

    // default constructor initializes all members to 0
    VertexPosition()
        : x(0), y(0), z(0)
    { }

    // fully parameterized constructor initializes all members from arguments
    VertexPosition(GLfloat x, GLfloat y, GLfloat z)
        : x(x), y(y), z(z)
    { }

    // the vertex format corresponding to this structure (defined in .cpp file)
    static const VertexFormat Format;

    // instance method for obtaining the vertex format
    const VertexFormat* getFormat() const
    { return &Format; }
};


//
// a structure that stores vertex position and color
//
struct VertexPositionColor {

    // the following floats will be laid out consecutively in memory like this: { x, y, z, r, g, b, a }
    GLfloat x, y, z;      // position
    GLfloat r, g, b, a;   // color

    // default constructor initializes all members to 0 (except alpha, which defaults to 1)
    VertexPositionColor()
        : x(0), y(0), z(0), r(0), g(0), b(0), a(1)
    { }

    // fully parameterized constructor initializes all members from arguments
    VertexPositionColor(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
        : x(x), y(y), z(z), r(r), g(g), b(b), a(a)
    { }

    // the vertex format corresponding to this structure (defined in .cpp file)
    static const VertexFormat Format;

    // instance method for obtaining the vertex format
    const VertexFormat* getFormat() const
    { return &Format; }
};


//
// a structure that stores vertex position and normal
//
struct VertexPositionNormal {

    // the following floats will be laid out consecutively in memory like this: { x, y, z, nx, ny, nz }
    GLfloat x, y, z;      // position
    GLfloat nx, ny, nz;   // normal

    // default constructor initializes all members to 0 (CAUTION: this creates an invalid normal!)
    VertexPositionNormal()
        : x(0), y(0), z(0), nx(0), ny(0), nz(0)
    { }

    // fully parameterized constructor initializes all members from arguments
    VertexPositionNormal(GLfloat x, GLfloat y, GLfloat z, GLfloat nx, GLfloat ny, GLfloat nz)
        : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz)
    { }

    // the vertex format corresponding to this structure (defined in .cpp file)
    static const VertexFormat Format;

    // instance method for obtaining the vertex format
    const VertexFormat* getFormat() const
    { return &Format; }
};

//
// a structure that stores vertex position and texture coordinate
//
struct VertexPositionTexture {

    // the following floats will be laid out consecutively in memory like this: { x, y, z, u, v }
    GLfloat x, y, z;      // position
    GLfloat u, v;         // texcoord

    // default constructor initializes all members to 0
    VertexPositionTexture()
        : x(0), y(0), z(0), u(0), v(0)
    { }

    // fully parameterized constructor initializes all members from arguments
    VertexPositionTexture(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
        : x(x), y(y), z(z), u(u), v(v)
    { }

    // the vertex format corresponding to this structure (defined in .cpp file)
    static const VertexFormat Format;

    // instance method for obtaining the vertex format
    const VertexFormat* getFormat() const
    { return &Format; }
};

//
// a structure that stores vertex position, normal, and texture coordinate (PNT)
//
struct VertexPositionNormalTexture {

    // the following floats will be laid out consecutively in memory like this: { x, y, z, nx, ny, nz, u, v }
    GLfloat x, y, z;      // position
    GLfloat nx, ny, nz;   // normal
    GLfloat u, v;         // texcoord

    // default constructor initializes all members to 0 (CAUTION: this creates an invalid normal!)
    VertexPositionNormalTexture()
        : x(0), y(0), z(0), nx(0), ny(0), nz(0), u(0), v(0)
    { }

    // fully parameterized constructor initializes all members from arguments
    VertexPositionNormalTexture(GLfloat x, GLfloat y, GLfloat z, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat u, GLfloat v)
        : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(u), v(v)
    { }

    // the vertex format corresponding to this structure (defined in .cpp file)
    static const VertexFormat Format;

    // instance method for obtaining the vertex format
    const VertexFormat* getFormat() const
    { return &Format; }
};

//
// Short forms for vertex types (avoids some typing)
//
typedef VertexPositionColor             VertexPC;
typedef VertexPositionNormal            VertexPN;
typedef VertexPositionTexture           VertexPT;
typedef VertexPositionNormalTexture     VertexPNT;

#endif
