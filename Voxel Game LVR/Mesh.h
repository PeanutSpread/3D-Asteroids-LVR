#ifndef MESH_H_
#define MESH_H_

#include "glshell.h"
#include "Vertex.h"

class Mesh {
    GLuint              mVBO;           // id of vertex buffer containing vertex data

    const VertexFormat* mFormat;        // format of vertices in the vertex buffer

    GLenum              mMode;          // drawing mode
    GLsizei             mNumVertices;   // number of vertices

	float				xWidth;
	float				yHeight;
	float				zDepth;

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

	float setXWidth(float value)
	{ xWidth = value; }
	float setYHeight(float value)
	{ yHeight = value; }
	float setZDepth(float value)
	{ zDepth = value; }

	float getXWidth() const
	{ return xWidth; }
	float getYHeight() const
	{ return yHeight; }
	float getZDepth() const
	{ return zDepth; }
};

//
// Load mesh from a Wavefront OBJ file
//
Mesh* LoadMesh(const std::string& path);

#endif
