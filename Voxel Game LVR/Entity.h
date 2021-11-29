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
	float				xWidth; //used to keep shape sizes for collision
	float				yHeight; //used to keep shape sizes for collision
	float				zDepth; //used to keep shape sizes for collision

public:
    Entity(const Mesh* mesh, const Material* material, const Transform& transform)
        : mTransform(transform)
        , mMesh(mesh)
        , mMaterial(material)
    { 
		//used to keep shape sizes for collision
		if (mesh != NULL) {
			xWidth = mesh->getXWidth();
			yHeight = mesh->getYHeight();
			zDepth = mesh->getZDepth();
		}
		else {
			xWidth = 0, yHeight = 0, zDepth = 0;
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
	
	// Compares this entity as a box with another entity, which is treated as a point
	bool doesIntersectAABB_Point(glm::vec3 point) {
		glm::vec3 selfPosition = mTransform.position;
		glm::vec3 selfAligner(xWidth/2, yHeight/2, zDepth/2);
		Entity aligner(NULL, NULL, selfPosition);

		bool xPlane = false, yPlane = false, zPlane = false;

		// xPlane
		aligner.translateLocal(-selfAligner.x, 0, 0);
		float selfXMin = aligner.getPosition().x;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(selfAligner.x, 0, 0);
		float selfXMax = aligner.getPosition().x;
		aligner.setPosition(selfPosition);

		if (selfXMin <= point.x && selfXMax >= point.x)
			xPlane = true;

		// yPlane
		aligner.translateLocal(0, -selfAligner.y, 0);
		float selfYMin = aligner.getPosition().y;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(0, selfAligner.y, 0);
		float selfYMax = aligner.getPosition().y;
		aligner.setPosition(selfPosition);

		if (selfYMin <= point.y && selfYMax >= point.y)
			yPlane = true;

		// zPlane
		aligner.translateLocal(0, 0, -selfAligner.z);
		float selfZMin = aligner.getPosition().z;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(0, 0, selfAligner.z);
		float selfZMax = aligner.getPosition().z;
		aligner.setPosition(selfPosition);

		if (selfZMin <= point.z && selfZMax >= point.z)
			zPlane = true;

		return xPlane && yPlane && zPlane;
	}

	// Compares this entity as box with another entity, which is treated as a box
	bool doesIntersectAABB_AABB (Entity* entity) {
		glm::vec3 selfPosition = mTransform.position;
		glm::vec3 selfAligner(xWidth / 2, yHeight / 2, zDepth / 2);

		glm::vec3 itsPosition = entity->getPosition();
		glm::vec3 itsAligner(entity->getXWidth() / 2, entity->getYHeight() / 2, entity->getZDepth() / 2);

		Entity aligner(NULL, NULL, selfPosition);
		
		bool xPlane = false, yPlane = false, zPlane = false;

		// xPlane
		aligner.translateLocal(-selfAligner.x, 0, 0);
		float selfXMin = aligner.getPosition().x;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(selfAligner.x, 0, 0);
		float selfXMax = aligner.getPosition().x;
		aligner.setPosition(itsPosition);

		aligner.translateLocal(-itsAligner.x, 0, 0);
		float itsXMin = aligner.getPosition().x;
		aligner.setPosition(itsPosition);

		aligner.translateLocal(itsAligner.x, 0, 0);
		float itsXMax = aligner.getPosition().x;
		aligner.setPosition(selfPosition);

		if (selfXMin <= itsXMax && selfXMax >= itsXMin)
			xPlane = true;

		// yPlane
		aligner.translateLocal(0, -selfAligner.y, 0);
		float selfYMin = aligner.getPosition().y;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(0, selfAligner.y, 0);
		float selfYMax = aligner.getPosition().y;
		aligner.setPosition(itsPosition);

		aligner.translateLocal(0, -itsAligner.y, 0);
		float itsYMin = aligner.getPosition().y;
		aligner.setPosition(itsPosition);

		aligner.translateLocal(0, itsAligner.y, 0);
		float itsYMax = aligner.getPosition().y;
		aligner.setPosition(selfPosition);

		if (selfYMin <= itsYMax && selfYMax >= itsYMin)
			yPlane = true;

		// zPlane
		aligner.translateLocal(0, 0, -selfAligner.z);
		float selfZMin = aligner.getPosition().z;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(0, 0, selfAligner.z);
		float selfZMax = aligner.getPosition().z;
		aligner.setPosition(itsPosition);

		aligner.translateLocal(0, 0, -itsAligner.z);
		float itsZMin = aligner.getPosition().z;
		aligner.setPosition(itsPosition);

		aligner.translateLocal(0, 0, itsAligner.z);
		float itsZMax = aligner.getPosition().z;
		aligner.setPosition(selfPosition);

		if (selfZMin <= itsZMax && selfZMax >= itsZMin)
			zPlane = true;

		return xPlane && yPlane && zPlane;
	}

	// Compares this entity as box with another entity, which is treated as a sphere
	bool doesIntersectAABB_Sphere(Entity* entity) {
		glm::vec3 selfPosition = mTransform.position;
		glm::vec3 selfAligner(xWidth / 2, yHeight / 2, zDepth / 2);
		Entity aligner(NULL, NULL, selfPosition);

		glm::vec3 itsPosition = entity->getPosition();
		glm::vec3 holder(0, 0, 0);
		float distance = 0;

		// xPlane
		aligner.translateLocal(-selfAligner.x, 0, 0);
		float selfXMin = aligner.getPosition().x;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(selfAligner.x, 0, 0);
		float selfXMax = aligner.getPosition().x;
		aligner.setPosition(selfPosition);
		
		holder.x = std::fmax(selfXMin, std::fmin(itsPosition.x, selfXMax));

		// yPlane
		aligner.translateLocal(0, -selfAligner.y, 0);
		float selfYMin = aligner.getPosition().y;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(0, selfAligner.y, 0);
		float selfYMax = aligner.getPosition().y;
		aligner.setPosition(selfPosition);

		holder.y = std::fmax(selfYMin, std::fmin(itsPosition.y, selfYMax));

		// zPlane
		aligner.translateLocal(0, 0, -selfAligner.z);
		float selfZMin = aligner.getPosition().z;
		aligner.setPosition(selfPosition);

		aligner.translateLocal(0, 0, selfAligner.z);
		float selfZMax = aligner.getPosition().z;
		aligner.setPosition(selfPosition);

		holder.z = std::fmax(selfZMin, std::fmin(itsPosition.z, selfZMax));

		distance = std::sqrtf(pow(holder.x - itsPosition.x, 2) + pow(holder.y - itsPosition.y, 2) + pow(holder.z - itsPosition.z, 2));
		return distance < (entity->getXWidth()/2);
	}

	// Compares this entity as a sphere with another entity, which is treated as a point
	bool doesIntersectSphere_Point(glm::vec3 point) {
		glm::vec3 selfPosition = mTransform.position;
		float distance = std::sqrtf(pow(point.x - selfPosition.x, 2) + pow(point.y - selfPosition.y, 2) + pow(point.z - selfPosition.z, 2));
		return distance < (xWidth/2);
	}

	// Compares this entity as a sphere with another entity, which is treated as a box
	bool doesIntersectSphere_AABB (Entity* entity) {
		{
			glm::vec3 selfPosition = mTransform.position;

			glm::vec3 itsPosition = entity->getPosition();
			glm::vec3 itsAligner(entity->getXWidth() / 2, entity->getYHeight() / 2, entity->getZDepth() / 2);

			Entity aligner(NULL, NULL, selfPosition);

			glm::vec3 holder(0, 0, 0);
			float distance = 0;

			// xPlane
			aligner.translateLocal(-itsAligner.x, 0, 0);
			float itsXMin = aligner.getPosition().x;
			aligner.setPosition(itsPosition);

			aligner.translateLocal(itsAligner.x, 0, 0);
			float itsXMax = aligner.getPosition().x;
			aligner.setPosition(itsPosition);

			holder.x = std::fmax(itsXMin, std::fmin(selfPosition.x, itsXMax));

			// yPlane
			aligner.translateLocal(0, -itsAligner.y, 0);
			float itsYMin = aligner.getPosition().y;
			aligner.setPosition(itsPosition);

			aligner.translateLocal(0, itsAligner.y, 0);
			float itsYMax = aligner.getPosition().y;
			aligner.setPosition(itsPosition);

			holder.y = std::fmax(itsYMin, std::fmin(selfPosition.y, itsYMax));

			// zPlane
			aligner.translateLocal(0, 0, -itsAligner.z);
			float itsZMin = aligner.getPosition().z;
			aligner.setPosition(itsPosition);

			aligner.translateLocal(0, 0, itsAligner.z);
			float itsZMax = aligner.getPosition().z;
			aligner.setPosition(selfPosition);

			holder.z = std::fmax(itsZMin, std::fmin(selfPosition.z, itsZMax));

			distance = std::sqrtf(pow(holder.x - selfPosition.x, 2) + pow(holder.y - selfPosition.y, 2) + pow(holder.z - selfPosition.z, 2));
			return distance < (xWidth / 2);
		}
	}

	// Compares this entity as a sphere with another entity, which is treated as a sphere
	bool doesIntersectSphere_Sphere(Entity* entity) {
		glm::vec3 selfPosition = mTransform.position;
		glm::vec3 itsPosition = entity->getPosition();

		float distance = std::sqrtf(pow(selfPosition.x - itsPosition.x, 2) + pow(selfPosition.y - itsPosition.y, 2) + pow(selfPosition.z - itsPosition.z, 2));
		return distance < (xWidth / 2 + entity->getXWidth() / 2);
	}
};

#endif
