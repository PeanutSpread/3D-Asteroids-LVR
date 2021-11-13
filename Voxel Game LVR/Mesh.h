#ifndef MESH_H_
#define MESH_H_

#include "glshell.h"
#include "Vertex.h"

class Mesh {
    GLuint              mVBO;           // id of vertex buffer containing vertex data

    const VertexFormat* mFormat;        // format of vertices in the vertex buffer

    GLenum              mMode;          // drawing mode
    GLsizei             mNumVertices;   // number of vertices

public:
    Mesh();
    ~Mesh();

    bool loadFromData(const void* data,
                      GLsizei numVertices,
                      GLsizei vertexSize,
                      GLenum mode,
                      const VertexFormat* format);

    void activate() const;
    void deactivate() const;

    void draw() const;
};

//
// Load mesh from a Wavefront OBJ file
//
Mesh* LoadMesh(const std::string& path);

#endif
