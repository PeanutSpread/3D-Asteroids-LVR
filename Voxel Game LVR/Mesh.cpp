#include "Mesh.h"
#include "common.h"

#include <fstream>      // file I/O
#include <iostream>     // console I/O

Mesh::Mesh()
    : mVBO(0)
    , mFormat(NULL)
    , mMode(0)
    , mNumVertices(0)
{
}

Mesh::~Mesh()
{
    if (mVBO)
        glDeleteBuffers(1, &mVBO);
}

bool Mesh::loadFromData(const void* data,
                        GLsizei numVertices,
                        GLsizei vertexSize,
                        GLenum mode,
                        const VertexFormat* format)
{
    // if there is no buffer, generate one
    if (!mVBO) {
        glGenBuffers(1, &mVBO);
    }

    // upload the data to device RAM
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mMode = mode;
    mNumVertices = numVertices;

    mFormat = format;

    return true;
}

void Mesh::activate() const
{
    // bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    // activate the vertex format
    mFormat->activate();
}

void Mesh::deactivate() const
{
    // unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // deactivate the vertex format
    mFormat->deactivate();
}

void Mesh::draw() const
{
    glDrawArrays(mMode, 0, mNumVertices);
}


struct TriFace {
    int a, b, c;
};


Mesh* LoadMesh(const std::string& path)
{
    std::cout << "Loading mesh from '" << path << "'" << std::endl;

    std::ifstream file(path.c_str());

    if (!file) {
        std::cerr << "ERROR: Failed to open " << path << std::endl;
        return NULL;
    }

    std::vector<glm::vec3> positions;
    std::vector<TriFace> faces;

    std::string line;
    int lineno = 0;

    for (;;) {

        std::getline(file, line);

        if (!file) {
            if (!file.eof()) {
                std::cerr << "ERROR: Failed to read from " << path << std::endl;
                return NULL;
            }
            break;
        }

        ++lineno;

        std::vector<std::string> tokens = Tokenize(line);

        // make sure it's not an empty line
        if (tokens.size() > 0) {

            // check if it's a comment
            char firstChar = tokens[0][0];
            if (firstChar == '#')
                continue;  // move on to next line

            if (tokens[0] == "v") {
                if (tokens.size() < 4) {
                    std::cerr << "ERROR: Incorrect number of vertex position components on line " << lineno << std::endl;
                    return NULL;
                }

                GLfloat x = FromString<GLfloat>(tokens[1]);
                GLfloat y = FromString<GLfloat>(tokens[2]);
                GLfloat z = FromString<GLfloat>(tokens[3]);

                positions.push_back(glm::vec3(x, y, z));

            } else if (tokens[0] == "f") {

                // only allow 3 vertices per face (triangles only!)
                if (tokens.size() != 4) {
                    std::cerr << "ERROR: Incorrect number of face elements" << lineno << std::endl;
                    return NULL;
                }

                // create a triangular face
                TriFace face;
                face.a = FromString<int>(tokens[1]);
                face.b = FromString<int>(tokens[2]);
                face.c = FromString<int>(tokens[3]);

                // add to our growing list of faces
                faces.push_back(face);
            }
        }
    }

    std::cout << "  Loaded " << positions.size() << " vertices" << std::endl;
    std::cout << "  Loaded " << faces.size() << " faces" << std::endl;

    //
    // Now build the vertex buffer!
    //

    std::vector<VertexPositionNormal> vertices;

    vertices.resize(3 * faces.size());

    for (unsigned i = 0; i < faces.size(); i++) {

        int a = faces[i].a - 1;
        int b = faces[i].b - 1;
        int c = faces[i].c - 1;

        // assumes abc define counter-clockwise triangle
        glm::vec3 v1 = positions[b] - positions[a];
        glm::vec3 v2 = positions[c] - positions[a];
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        //std::cout << "normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;

        vertices[i * 3].x = positions[a].x;
        vertices[i * 3].y = positions[a].y;
        vertices[i * 3].z = positions[a].z;
        vertices[i * 3].nx = normal.x;
        vertices[i * 3].ny = normal.y;
        vertices[i * 3].nz = normal.z;

        vertices[i * 3 + 1].x = positions[b].x;
        vertices[i * 3 + 1].y = positions[b].y;
        vertices[i * 3 + 1].z = positions[b].z;
        vertices[i * 3 + 1].nx = normal.x;
        vertices[i * 3 + 1].ny = normal.y;
        vertices[i * 3 + 1].nz = normal.z;

        vertices[i * 3 + 2].x = positions[c].x;
        vertices[i * 3 + 2].y = positions[c].y;
        vertices[i * 3 + 2].z = positions[c].z;
        vertices[i * 3 + 2].nx = normal.x;
        vertices[i * 3 + 2].ny = normal.y;
        vertices[i * 3 + 2].nz = normal.z;
    }

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}
