#ifndef ENTITY_H_
#define ENTITY_H_

#include "glshell.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"

class Entity {
    Transform           mTransform;
    const Mesh*         mMesh;
    const Material*     mMaterial;

public:
    Entity(const Mesh* mesh, const Material* material, const Transform& transform)
        : mTransform(transform)
        , mMesh(mesh)
        , mMaterial(material)
    { }

    //
    // a bunch of useful getters
    //

    const Mesh*         getMesh() const         { return mMesh; }
    const Material*     getMaterial() const     { return mMaterial; }
    const Transform&    getTransform() const    { return mTransform; }
    const glm::vec3&    getPosition() const     { return mTransform.position; }
    const glm::quat&    getOrientation() const  { return mTransform.orientation; }

    //
    // The renderer can use the getWorldMatrix method to obtain the modeling matrix for this entity.
    // The modeling matrix combines the position and orientation transformations.
    //
    glm::mat4           getWorldMatrix() const  { return mTransform.toMatrix(); }

    //
    // setters for position and orientation
    //

    void setTransform(const Transform& transform)
    {
        mTransform = transform;
    }

    void setPosition(const glm::vec3& pos)
    {
        mTransform.position = pos;
    }

    void setPosition(float x, float y, float z)
    {
        mTransform.position.x = x;
        mTransform.position.y = z;
        mTransform.position.z = z;
    }

    void setOrientation(const glm::quat& orientation)
    {
        mTransform.orientation = orientation;
    }

    //
    // rotation about an arbitrary axis
    //

    void rotate(float angle, const glm::vec3& axis)
    {
        // combine rotation with existing orientation using quaternion multiplication
        glm::quat q = glm::angleAxis(glm::radians(angle), axis);
        mTransform.orientation = mTransform.orientation * q;
    }

    void rotate(float angle, float x, float y, float z)
    {
        // combine rotation with existing orientation using quaternion multiplication
        glm::quat q = glm::angleAxis(glm::radians(angle), glm::vec3(x, y, z));
        mTransform.orientation = mTransform.orientation * q;
    }

    //
    // translation along world axes
    //

    void translate(const glm::vec3& disp)
    {
        mTransform.position += disp;
    }

    void translate(float dx, float dy, float dz)
    {
        translate(glm::vec3(dx, dy, dz));
    }

    //
    // translation along local axes
    //

    void translateLocal(const glm::vec3& disp)
    {
        // multiply the displacement by our orientation quaternion
        translate(mTransform.orientation * disp);
    }

    void translateLocal(float dx, float dy, float dz)
    {
        translateLocal(glm::vec3(dx, dy, dz));
    }
};

#endif
