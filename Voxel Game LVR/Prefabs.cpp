#include "Prefabs.h"


Mesh* CreateSolidBox_Nolight(float width, float height, float depth)
{
    std::vector<VertexPosition> vertices;

    float w = 0.5f * width;
    float h = 0.5f * height;
    float d = 0.5f * depth;

    // front face
    vertices.push_back(VertexPosition(-w, -h,  d));
    vertices.push_back(VertexPosition( w, -h,  d));
    vertices.push_back(VertexPosition( w,  h,  d));

    vertices.push_back(VertexPosition(-w,  h,  d));
    vertices.push_back(VertexPosition(-w, -h,  d));
    vertices.push_back(VertexPosition( w,  h,  d));

    // back face
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition(-w,  h, -d));
    vertices.push_back(VertexPosition( w,  h, -d));

    vertices.push_back(VertexPosition( w, -h, -d));
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition( w,  h, -d));

    // right face
    vertices.push_back(VertexPosition( w, -h, -d));
    vertices.push_back(VertexPosition( w,  h, -d));
    vertices.push_back(VertexPosition( w,  h,  d));

    vertices.push_back(VertexPosition( w, -h,  d));
    vertices.push_back(VertexPosition( w, -h, -d));
    vertices.push_back(VertexPosition( w,  h,  d));

    // left face
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition(-w, -h,  d));
    vertices.push_back(VertexPosition(-w,  h,  d));

    vertices.push_back(VertexPosition(-w,  h, -d));
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition(-w,  h,  d));

    // top face
    vertices.push_back(VertexPosition(-w,  h, -d));
    vertices.push_back(VertexPosition(-w,  h,  d));
    vertices.push_back(VertexPosition( w,  h,  d));

    vertices.push_back(VertexPosition( w,  h, -d));
    vertices.push_back(VertexPosition(-w,  h, -d));
    vertices.push_back(VertexPosition( w,  h,  d));

    // bottom face
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition( w, -h, -d));
    vertices.push_back(VertexPosition( w, -h,  d));
 
    vertices.push_back(VertexPosition(-w, -h,  d));
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition( w, -h,  d));

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}

Mesh* CreateSolidBox(float width, float height, float depth)
{
    std::vector<VertexPositionNormal> vertices;

    float w = 0.5f * width;
    float h = 0.5f * height;
    float d = 0.5f * depth;

    // front face
    vertices.push_back(VertexPositionNormal(-w, -h,  d,  0, 0, 1));
    vertices.push_back(VertexPositionNormal( w, -h,  d,  0, 0, 1));
    vertices.push_back(VertexPositionNormal( w,  h,  d,  0, 0, 1));

    vertices.push_back(VertexPositionNormal(-w,  h,  d,  0, 0, 1));
    vertices.push_back(VertexPositionNormal(-w, -h,  d,  0, 0, 1));
    vertices.push_back(VertexPositionNormal( w,  h,  d,  0, 0, 1));

    // back face
    vertices.push_back(VertexPositionNormal(-w, -h, -d,  0, 0, -1));
    vertices.push_back(VertexPositionNormal(-w,  h, -d,  0, 0, -1));
    vertices.push_back(VertexPositionNormal( w,  h, -d,  0, 0, -1));

    vertices.push_back(VertexPositionNormal( w, -h, -d,  0, 0, -1));
    vertices.push_back(VertexPositionNormal(-w, -h, -d,  0, 0, -1));
    vertices.push_back(VertexPositionNormal( w,  h, -d,  0, 0, -1));

    // right face
    vertices.push_back(VertexPositionNormal( w, -h, -d,  1, 0, 0));
    vertices.push_back(VertexPositionNormal( w,  h, -d,  1, 0, 0));
    vertices.push_back(VertexPositionNormal( w,  h,  d,  1, 0, 0));

    vertices.push_back(VertexPositionNormal( w, -h,  d,  1, 0, 0));
    vertices.push_back(VertexPositionNormal( w, -h, -d,  1, 0, 0));
    vertices.push_back(VertexPositionNormal( w,  h,  d,  1, 0, 0));

    // left face
    vertices.push_back(VertexPositionNormal(-w, -h, -d,  -1, 0, 0));
    vertices.push_back(VertexPositionNormal(-w, -h,  d,  -1, 0, 0));
    vertices.push_back(VertexPositionNormal(-w,  h,  d,  -1, 0, 0));

    vertices.push_back(VertexPositionNormal(-w,  h, -d,  -1, 0, 0));
    vertices.push_back(VertexPositionNormal(-w, -h, -d,  -1, 0, 0));
    vertices.push_back(VertexPositionNormal(-w,  h,  d,  -1, 0, 0));

    // top face
    vertices.push_back(VertexPositionNormal(-w,  h, -d,  0, 1, 0));
    vertices.push_back(VertexPositionNormal(-w,  h,  d,  0, 1, 0));
    vertices.push_back(VertexPositionNormal( w,  h,  d,  0, 1, 0));

    vertices.push_back(VertexPositionNormal( w,  h, -d,  0, 1, 0));
    vertices.push_back(VertexPositionNormal(-w,  h, -d,  0, 1, 0));
    vertices.push_back(VertexPositionNormal( w,  h,  d,  0, 1, 0));

    // bottom face
    vertices.push_back(VertexPositionNormal(-w, -h, -d,  0, -1, 0));
    vertices.push_back(VertexPositionNormal( w, -h, -d,  0, -1, 0));
    vertices.push_back(VertexPositionNormal( w, -h,  d,  0, -1, 0));
 
    vertices.push_back(VertexPositionNormal(-w, -h,  d,  0, -1, 0));
    vertices.push_back(VertexPositionNormal(-w, -h, -d,  0, -1, 0));
    vertices.push_back(VertexPositionNormal( w, -h,  d,  0, -1, 0));

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}

Mesh* CreateWireframeBox(float width, float height, float depth)
{
    std::vector<VertexPosition> vertices;

    float w = 0.5f * width;
    float h = 0.5f * height;
    float d = 0.5f * depth;

    // sides
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition(-w,  h, -d));
    vertices.push_back(VertexPosition(-w, -h,  d));
    vertices.push_back(VertexPosition(-w,  h,  d));
    vertices.push_back(VertexPosition( w, -h, -d));
    vertices.push_back(VertexPosition( w,  h, -d));
    vertices.push_back(VertexPosition( w, -h,  d));
    vertices.push_back(VertexPosition( w,  h,  d));

    // top
    vertices.push_back(VertexPosition(-w,  h, -d));
    vertices.push_back(VertexPosition( w,  h, -d));
    vertices.push_back(VertexPosition(-w,  h,  d));
    vertices.push_back(VertexPosition( w,  h,  d));
    vertices.push_back(VertexPosition(-w,  h, -d));
    vertices.push_back(VertexPosition(-w,  h,  d));
    vertices.push_back(VertexPosition( w,  h, -d));
    vertices.push_back(VertexPosition( w,  h,  d));

    // bottom
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition( w, -h, -d));
    vertices.push_back(VertexPosition(-w, -h,  d));
    vertices.push_back(VertexPosition( w, -h,  d));
    vertices.push_back(VertexPosition(-w, -h, -d));
    vertices.push_back(VertexPosition(-w, -h,  d));
    vertices.push_back(VertexPosition( w, -h, -d));
    vertices.push_back(VertexPosition( w, -h,  d));

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_LINES,                   // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}


Mesh* CreateSolidCube_Nolight(float width)
{
    return CreateSolidBox_Nolight(width, width, width);
}

Mesh* CreateSolidCube(float width)
{
    return CreateSolidBox(width, width, width);
}

Mesh* CreateWireframeCube(float width)
{
    return CreateWireframeBox(width, width, width);
}


Mesh* CreateSmoothCylinder(float radius, float height, int numSegments)
{
    std::vector<VertexPositionNormal> vertices;

    float top = 0.5f * height;
    float bottom = -0.5f * height;

    float angStep = 2 * 3.14159f / numSegments;
    float angle = 0;
    for (int i = 0; i < numSegments; i++) {
        float x1 = radius * std::cos(angle);
        float z1 = radius * std::sin(angle);
        float x2 = radius * std::cos(angle + angStep);
        float z2 = radius * std::sin(angle + angStep);

        // triangle at the base
        vertices.push_back(VertexPositionNormal(0, bottom, 0,    0, -1, 0));
        vertices.push_back(VertexPositionNormal(x1, bottom, z1,  0, -1, 0));
        vertices.push_back(VertexPositionNormal(x2, bottom, z2,  0, -1, 0));

        // triangle at the top
        vertices.push_back(VertexPositionNormal(0, top, 0,    0, 1, 0));
        vertices.push_back(VertexPositionNormal(x2, top, z2,  0, 1, 0));
        vertices.push_back(VertexPositionNormal(x1, top, z1,  0, 1, 0));

        // compute normals for triangles on the side
        glm::vec3 n1 = glm::normalize(glm::vec3(x1, 0.0f, z1));
        glm::vec3 n2 = glm::normalize(glm::vec3(x2, 0.0f, z2));

        vertices.push_back(VertexPositionNormal(x1, top, z1,     n1.x, n1.y, n1.z));
        vertices.push_back(VertexPositionNormal(x2, bottom, z2,  n2.x, n2.y, n2.z));
        vertices.push_back(VertexPositionNormal(x1, bottom, z1,  n1.x, n1.y, n1.z));

        vertices.push_back(VertexPositionNormal(x1, top, z1,     n1.x, n1.y, n1.z));
        vertices.push_back(VertexPositionNormal(x2, top, z2,     n2.x, n2.y, n2.z));
        vertices.push_back(VertexPositionNormal(x2, bottom, z2,  n2.x, n2.y, n2.z));

        angle += angStep;
    }

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}

Mesh* CreateChunkyCylinder(float radius, float height, int numSegments)
{
    std::vector<VertexPositionNormal> vertices;

    float top = 0.5f * height;
    float bottom = -0.5f * height;

    float angStep = 2 * 3.14159f / numSegments;
    float angle = 0;
    for (int i = 0; i < numSegments; i++) {
        float x1 = radius * std::cos(angle);
        float z1 = radius * std::sin(angle);
        float x2 = radius * std::cos(angle + angStep);
        float z2 = radius * std::sin(angle + angStep);

        // triangle at the base
        vertices.push_back(VertexPositionNormal(0, bottom, 0,    0, -1, 0));
        vertices.push_back(VertexPositionNormal(x1, bottom, z1,  0, -1, 0));
        vertices.push_back(VertexPositionNormal(x2, bottom, z2,  0, -1, 0));

        // triangle at the top
        vertices.push_back(VertexPositionNormal(0, top, 0,    0, 1, 0));
        vertices.push_back(VertexPositionNormal(x2, top, z2,  0, 1, 0));
        vertices.push_back(VertexPositionNormal(x1, top, z1,  0, 1, 0));

        // compute normal for triangles on the side
        glm::vec3 a(x1, top, z1);
        glm::vec3 b(x1, bottom, z1);
        glm::vec3 c(x2, bottom, z2);
        glm::vec3 u = b - a;
        glm::vec3 v = c - a;
        glm::vec3 n = glm::normalize(glm::cross(v, u));

        vertices.push_back(VertexPositionNormal(x1, top, z1,     n.x, n.y, n.z));
        vertices.push_back(VertexPositionNormal(x2, bottom, z2,  n.x, n.y, n.z));
        vertices.push_back(VertexPositionNormal(x1, bottom, z1,  n.x, n.y, n.z));

        vertices.push_back(VertexPositionNormal(x1, top, z1,     n.x, n.y, n.z));
        vertices.push_back(VertexPositionNormal(x2, top, z2,     n.x, n.y, n.z));
        vertices.push_back(VertexPositionNormal(x2, bottom, z2,  n.x, n.y, n.z));

        angle += angStep;
    }

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}


Mesh* CreateChunkyCone(float radius, float height, int numSegments)
{
    std::vector<VertexPositionNormal> vertices;

    float top = 0.5f * height;
    float bottom = -0.5f * height;

    float angStep = 2 * 3.14159f / numSegments;
    float angle = 0;
    for (int i = 0; i < numSegments; i++) {
        float x1 = radius * std::cos(angle);
        float z1 = radius * std::sin(angle);
        float x2 = radius * std::cos(angle + angStep);
        float z2 = radius * std::sin(angle + angStep);

        // triangle at the base
        vertices.push_back(VertexPositionNormal(0, bottom, 0,    0, -1, 0));
        vertices.push_back(VertexPositionNormal(x1, bottom, z1,  0, -1, 0));
        vertices.push_back(VertexPositionNormal(x2, bottom, z2,  0, -1, 0));

        // compute normal for triangle on the side
        glm::vec3 a(0, top, 0);
        glm::vec3 b(x1, bottom, z1);
        glm::vec3 c(x2, bottom, z2);
        glm::vec3 u = b - a;
        glm::vec3 v = c - a;
        glm::vec3 n = glm::normalize(glm::cross(v, u));

        // triangle on the side
        vertices.push_back(VertexPositionNormal(0, top, 0,       n.x, n.y, n.z));
        vertices.push_back(VertexPositionNormal(x2, bottom, z2,  n.x, n.y, n.z));
        vertices.push_back(VertexPositionNormal(x1, bottom, z1,  n.x, n.y, n.z));

        angle += angStep;
    }

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}


Mesh* CreateTexturedCube(float width)
{
    std::vector<VertexPNT> vertices;

    float w = 0.5f * width;
    float h = 0.5f * width;
    float d = 0.5f * width;

    // front face
    vertices.push_back(VertexPNT(-w, -h,  d,  0, 0, 1,  0, 0));
    vertices.push_back(VertexPNT( w, -h,  d,  0, 0, 1,  1, 0));
    vertices.push_back(VertexPNT( w,  h,  d,  0, 0, 1,  1, 1));

    vertices.push_back(VertexPNT(-w,  h,  d,  0, 0, 1,  0, 1));
    vertices.push_back(VertexPNT(-w, -h,  d,  0, 0, 1,  0, 0));
    vertices.push_back(VertexPNT( w,  h,  d,  0, 0, 1,  1, 1));

    // back face
    vertices.push_back(VertexPNT(-w, -h, -d,  0, 0, -1,  1, 0));
    vertices.push_back(VertexPNT(-w,  h, -d,  0, 0, -1,  1, 1));
    vertices.push_back(VertexPNT( w,  h, -d,  0, 0, -1,  0, 1));

    vertices.push_back(VertexPNT( w, -h, -d,  0, 0, -1,  0, 0));
    vertices.push_back(VertexPNT(-w, -h, -d,  0, 0, -1,  1, 0));
    vertices.push_back(VertexPNT( w,  h, -d,  0, 0, -1,  0, 1));

    // right face
    vertices.push_back(VertexPNT( w, -h, -d,  1, 0, 0,  1, 0));
    vertices.push_back(VertexPNT( w,  h, -d,  1, 0, 0,  1, 1));
    vertices.push_back(VertexPNT( w,  h,  d,  1, 0, 0,  0, 1));

    vertices.push_back(VertexPNT( w, -h,  d,  1, 0, 0,  0, 0));
    vertices.push_back(VertexPNT( w, -h, -d,  1, 0, 0,  1, 0));
    vertices.push_back(VertexPNT( w,  h,  d,  1, 0, 0,  0, 1));

    // left face
    vertices.push_back(VertexPNT(-w, -h, -d,  -1, 0, 0,  0, 0));
    vertices.push_back(VertexPNT(-w, -h,  d,  -1, 0, 0,  1, 0));
    vertices.push_back(VertexPNT(-w,  h,  d,  -1, 0, 0,  1, 1));

    vertices.push_back(VertexPNT(-w,  h, -d,  -1, 0, 0,  0, 1));
    vertices.push_back(VertexPNT(-w, -h, -d,  -1, 0, 0,  0, 0));
    vertices.push_back(VertexPNT(-w,  h,  d,  -1, 0, 0,  1, 1));

    // top face
    vertices.push_back(VertexPNT(-w,  h, -d,  0, 1, 0,  0, 1));
    vertices.push_back(VertexPNT(-w,  h,  d,  0, 1, 0,  0, 0));
    vertices.push_back(VertexPNT( w,  h,  d,  0, 1, 0,  1, 0));

    vertices.push_back(VertexPNT( w,  h, -d,  0, 1, 0,  1, 1));
    vertices.push_back(VertexPNT(-w,  h, -d,  0, 1, 0,  0, 1));
    vertices.push_back(VertexPNT( w,  h,  d,  0, 1, 0,  1, 0));

    // bottom face
    vertices.push_back(VertexPNT(-w, -h, -d,  0, -1, 0,  0, 0));
    vertices.push_back(VertexPNT( w, -h, -d,  0, -1, 0,  1, 0));
    vertices.push_back(VertexPNT( w, -h,  d,  0, -1, 0,  1, 1));
 
    vertices.push_back(VertexPNT(-w, -h,  d,  0, -1, 0,  0, 1));
    vertices.push_back(VertexPNT(-w, -h, -d,  0, -1, 0,  0, 0));
    vertices.push_back(VertexPNT( w, -h,  d,  0, -1, 0,  1, 1));

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}


Mesh* CreateSmoothTexturedCylinder(float radius, float height, int numSegments)
{
    std::vector<VertexPNT> vertices;

    float top = 0.5f * height;
    float bottom = -0.5f * height;

    float angStep = 2 * 3.14159f / numSegments;
    float angle = 0;
    float uStep = -2.0f / numSegments;  // wrap u coordinate twice around cylinder
    float u = 0;
    for (int i = 0; i < numSegments; i++) {
        float x1 = radius * std::cos(angle);
        float z1 = radius * std::sin(angle);
        float x2 = radius * std::cos(angle + angStep);
        float z2 = radius * std::sin(angle + angStep);

        // uv coordiates for top and bottom (normalized from[-1, 1] to [0, 1])
        float u1 = 0.5f * (1 + std::cos(angle));
        float v1 = 0.5f * (1 + std::sin(angle));
        float u2 = 0.5f * (1 + std::cos(angle + angStep));
        float v2 = 0.5f * (1 + std::sin(angle + angStep));

        // triangle at the base
        vertices.push_back(VertexPNT(0, bottom, 0,    0, -1, 0,  0.5f, 0.5f));
        vertices.push_back(VertexPNT(x1, bottom, z1,  0, -1, 0,  u1, v1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  0, -1, 0,  u2, v2));

        // triangle at the top
        vertices.push_back(VertexPNT(0, top, 0,    0, 1, 0,  0.5f, 0.5f));
        vertices.push_back(VertexPNT(x2, top, z2,  0, 1, 0,  u2, 1 - v2));
        vertices.push_back(VertexPNT(x1, top, z1,  0, 1, 0,  u1, 1 - v1));

        // compute normals for triangles on the side
        glm::vec3 n1 = glm::normalize(glm::vec3(x1, 0.0f, z1));
        glm::vec3 n2 = glm::normalize(glm::vec3(x2, 0.0f, z2));

        vertices.push_back(VertexPNT(x1, top, z1,     n1.x, n1.y, n1.z,  u, 1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  n2.x, n2.y, n2.z,  u + uStep, 0));
        vertices.push_back(VertexPNT(x1, bottom, z1,  n1.x, n1.y, n1.z,  u, 0));

        vertices.push_back(VertexPNT(x1, top, z1,     n1.x, n1.y, n1.z,  u, 1));
        vertices.push_back(VertexPNT(x2, top, z2,     n2.x, n2.y, n2.z,  u + uStep, 1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  n2.x, n2.y, n2.z,  u + uStep, 0));

        angle += angStep;
        u += uStep;
    }

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}


Mesh* CreateChunkyTexturedCylinder(float radius, float height, int numSegments)
{
    std::vector<VertexPNT> vertices;

    float top = 0.5f * height;
    float bottom = -0.5f * height;

    float angStep = 2 * 3.14159f / numSegments;
    float angle = 0;
    float uStep = -2.0f / numSegments;  // wrap u coordinate twice around cylinder
    float u = 0;
    for (int i = 0; i < numSegments; i++) {
        float x1 = radius * std::cos(angle);
        float z1 = radius * std::sin(angle);
        float x2 = radius * std::cos(angle + angStep);
        float z2 = radius * std::sin(angle + angStep);

        // uv coordiates for top and bottom (normalized from[-1, 1] to [0, 1])
        float u1 = 0.5f * (1 + std::cos(angle));
        float v1 = 0.5f * (1 + std::sin(angle));
        float u2 = 0.5f * (1 + std::cos(angle + angStep));
        float v2 = 0.5f * (1 + std::sin(angle + angStep));

        // triangle at the base
        vertices.push_back(VertexPNT(0, bottom, 0,    0, -1, 0,  0.5f, 0.5f));
        vertices.push_back(VertexPNT(x1, bottom, z1,  0, -1, 0,  u1, v1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  0, -1, 0,  u2, v2));

        // triangle at the top
        vertices.push_back(VertexPNT(0, top, 0,    0, 1, 0,  0.5f, 0.5f));
        vertices.push_back(VertexPNT(x2, top, z2,  0, 1, 0,  u2, 1 - v2));
        vertices.push_back(VertexPNT(x1, top, z1,  0, 1, 0,  u1, 1 - v1));

        // compute normal for triangles on the side
        glm::vec3 a(x1, top, z1);
        glm::vec3 b(x1, bottom, z1);
        glm::vec3 c(x2, bottom, z2);
        glm::vec3 ba = b - a;
        glm::vec3 ca = c - a;
        glm::vec3 n = glm::normalize(glm::cross(ca, ba));

        vertices.push_back(VertexPNT(x1, top, z1,     n.x, n.y, n.z,  u, 1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  n.x, n.y, n.z,  u + uStep, 0));
        vertices.push_back(VertexPNT(x1, bottom, z1,  n.x, n.y, n.z,  u, 0));

        vertices.push_back(VertexPNT(x1, top, z1,     n.x, n.y, n.z,  u, 1));
        vertices.push_back(VertexPNT(x2, top, z2,     n.x, n.y, n.z,  u + uStep, 1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  n.x, n.y, n.z,  u + uStep, 0));

        angle += angStep;
        u += uStep;
    }

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;

    /*
    std::vector<VertexPNT> vertices;

    float top = 0.5f * height;
    float bottom = -0.5f * height;

    float angStep = 2 * 3.14159f / numSegments;
    float angle = 0;
    float uStep = -2.0f / numSegments;  // wrap u coordinate twice around cylinder
    float u = 0;
    for (int i = 0; i < numSegments; i++) {
        float x1 = radius * std::cos(angle);
        float z1 = radius * std::sin(angle);
        float x2 = radius * std::cos(angle + angStep);
        float z2 = radius * std::sin(angle + angStep);

        // uv coordiates for top and bottom (normalized from[-1, 1] to [0, 1])
        float u1 = 0.5f * (1 + std::cos(angle));
        float v1 = 0.5f * (1 + std::sin(angle));
        float u2 = 0.5f * (1 + std::cos(angle + angStep));
        float v2 = 0.5f * (1 + std::sin(angle + angStep));

        // triangle at the base
        vertices.push_back(VertexPNT(0, bottom, 0,    0, -1, 0,  0.5f, 0.5f));
        vertices.push_back(VertexPNT(x1, bottom, z1,  0, -1, 0,  u1, v1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  0, -1, 0,  u2, v2));

        // triangle at the top
        vertices.push_back(VertexPNT(0, top, 0,    0, 1, 0,  0.5f, 0.5f));
        vertices.push_back(VertexPNT(x2, top, z2,  0, 1, 0,  u2, 1 - v2));
        vertices.push_back(VertexPNT(x1, top, z1,  0, 1, 0,  u1, 1 - v1));

        // compute normals for triangles on the side
        glm::vec3 n1 = glm::normalize(glm::vec3(x1, 0.0f, z1));
        glm::vec3 n2 = glm::normalize(glm::vec3(x2, 0.0f, z2));

        vertices.push_back(VertexPNT(x1, top, z1,     n1.x, n1.y, n1.z,  u, 1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  n2.x, n2.y, n2.z,  u + uStep, 0));
        vertices.push_back(VertexPNT(x1, bottom, z1,  n1.x, n1.y, n1.z,  u, 0));

        vertices.push_back(VertexPNT(x1, top, z1,     n1.x, n1.y, n1.z,  u, 1));
        vertices.push_back(VertexPNT(x2, top, z2,     n2.x, n2.y, n2.z,  u + uStep, 1));
        vertices.push_back(VertexPNT(x2, bottom, z2,  n2.x, n2.y, n2.z,  u + uStep, 0));

        angle += angStep;
        u += uStep;
    }

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLES,               // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
    */
}


Mesh* CreateTexturedQuad(float width, float height, float uTile, float vTile)
{
    std::vector<VertexPNT> vertices;

    float x = 0.5f * width;
    float y = 0.5f * height;

    vertices.push_back(VertexPNT(-x, -y, 0,  0, 0, 1,  0, 0));
    vertices.push_back(VertexPNT( x, -y, 0,  0, 0, 1,  uTile, 0));
    vertices.push_back(VertexPNT(-x,  y, 0,  0, 0, 1,  0, vTile));
    vertices.push_back(VertexPNT( x,  y, 0,  0, 0, 1,  uTile, vTile));

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_TRIANGLE_STRIP,          // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}

Mesh* CreateAxes(float scale)
{
    std::vector<VertexPC> vertices;

    // x-axis
    vertices.push_back(VertexPC(0, 0, 0,      1, 0, 0, 1));
    vertices.push_back(VertexPC(scale, 0, 0,  1, 0, 0, 1));

    // y-axis
    vertices.push_back(VertexPC(0, 0, 0,      0, 1, 0, 1));
    vertices.push_back(VertexPC(0, scale, 0,  0, 1, 0, 1));

    // z-axis
    vertices.push_back(VertexPC(0, 0, 0,      0, 0, 1, 1));
    vertices.push_back(VertexPC(0, 0, scale,  0, 0, 1, 1));

    Mesh* mesh = new Mesh;
    mesh->loadFromData(&vertices[0],               // address of data in memory
                       vertices.size(),            // number of vertices
                       sizeof(vertices[0]),        // size of each vertex
                       GL_LINES,                   // drawing mode
                       vertices[0].getFormat());   // vertex format

    return mesh;
}
