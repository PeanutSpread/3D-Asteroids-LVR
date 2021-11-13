#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "glshell.h"

// quaternion magic
#include <glm/gtx/quaternion.hpp>

//
// A simple Transform class that includes position and orientation
//
struct Transform {
    glm::vec3 position;
    glm::quat orientation;

    Transform()
        : position(0.0f, 0.0f, 0.0f)
        , orientation(1.0f, 0.0f, 0.0f, 0.0f)  // identity quaternion
    { }

    Transform(const glm::vec3& position, const glm::quat& orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        : position(position)
        , orientation(orientation)
    { }

    Transform(float x, float y, float z, const glm::quat& orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        : position(x, y, z)
        , orientation(orientation)
    { }

    //
    // Combine the position and orientation into a single 4x4 matrix.
    // The renderer will use this as the modeling matrix for entities.
    //
    glm::mat4 toMatrix() const
    {
        // construct the translation matrix
        glm::mat4 T(1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    position.x, position.y, position.z, 1.0f);

        // compute the rotation matrix from the quaternion (thanks, glm!)
        glm::mat4 R = glm::toMat4(orientation);

        // return combined transform as a 4x4 matrix
        return T * R;
    }
};

#endif
