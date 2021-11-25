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
	float				xWidth;
	float				yHeight;
	float				zDepth;

public:
    Entity(const Mesh* mesh, const Material* material, const Transform& transform)
        : mTransform(transform)
        , mMesh(mesh)
        , mMaterial(material)
    { 
		if (mesh != NULL) {
			xWidth = mesh->getXWidth();
			yHeight = mesh->getYHeight();
			zDepth = mesh->getZDepth();
		}
		else {
			xWidth, yHeight, zDepth = 0;
		}
	}

    //
    // a bunch of useful getters
    //

    const Mesh*         getMesh() const         { return mMesh; }
    const Material*     getMaterial() const     { return mMaterial; }
    const Transform&    getTransform() const    { return mTransform; }
    const glm::vec3&    getPosition() const     { return mTransform.position; }
    const glm::quat&    getOrientation() const  { return mTransform.orientation; }
	
	float getXWidth() const
	{ return xWidth; }
	float getYHeight() const
	{ return yHeight; }
	float getZDepth() const
	{ return zDepth; }

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

	//
	// Collision with other entities
	//
	
	bool doesIntersectAABB_Point(glm::vec3 point) {
		glm::vec3 selfPosition = mTransform.position;
		glm::vec3 selfAligner(xWidth/2, yHeight/2, zDepth/2);
		selfAligner = selfAligner * mTransform.orientation;

		bool xPlane, yPlane, zPlane = false;

		// xPlane
		float selfXMin = selfPosition.x - selfAligner.x;
		float selfXMax = selfPosition.x + selfAligner.x;

		if (selfXMin <= point.x && selfXMax >= point.x)
			xPlane = true;

		// yPlane
		float selfYMin = selfPosition.y - selfAligner.y;
		float selfYMax = selfPosition.y + selfAligner.y;

		if (selfYMin <= point.y && selfYMax >= point.y)
			yPlane = true;

		// zPlane
		float selfZMin = selfPosition.z - selfAligner.z;
		float selfZMax = selfPosition.z + selfAligner.z;

		if (selfZMin <= point.z && selfZMax >= point.z)
			zPlane = true;

		return xPlane && yPlane && zPlane;
	}

	bool doesIntersectAABB_AABB (Entity entity) {
		glm::vec3 selfPosition = mTransform.position;
		glm::vec3 selfAligner(xWidth / 2, yHeight / 2, zDepth / 2);
		selfAligner = selfAligner * mTransform.orientation;

		glm::vec3 itsPosition = entity.getPosition();
		glm::vec3 itsAligner(entity.getXWidth / 2, entity.getYHeight / 2, entity.getZDepth / 2);
		itsAligner = itsAligner * entity.getOrientation;
		
		bool xPlane, yPlane, zPlane = false;

		// xPlane
		float selfXMin = selfPosition.x - selfAligner.x;
		float selfXMax = selfPosition.x + selfAligner.x;
		float itsXMin = itsPosition.x - itsAligner.x;
		float itsXMax = itsPosition.x + itsAligner.x;

		if (selfXMin <= itsXMax && selfXMax >= itsXMin)
			xPlane = true;

		// yPlane
		float selfYMin = selfPosition.y - selfAligner.y;
		float selfYMax = selfPosition.y + selfAligner.y;
		float itsYMin = itsPosition.y - itsAligner.y;
		float itsYMax = itsPosition.y + itsAligner.y;

		if (selfYMin <= itsYMax && selfYMax >= itsYMin)
			yPlane = true;

		// zPlane
		float selfZMin = selfPosition.z - selfAligner.z;
		float selfZMax = selfPosition.z + selfAligner.z;
		float itsZMin = itsPosition.z - itsAligner.z;
		float itsZMax = itsPosition.z + itsAligner.z;

		if (selfZMin <= itsZMax && selfZMax >= itsZMin)
			zPlane = true;

		return xPlane && yPlane && zPlane;
	}

	// TODO: implement rest of collision scenarios
	bool doesIntersectAABB_Sphere(Entity entity) {
		return false;
	}
};

#endif
