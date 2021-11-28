#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include <vector>

class Projectile {
	std::vector<Entity*>	entities;
	std::vector<glm::vec3>	offsets;

	Entity*					aligner;
	glm::vec3				position;
	glm::quat				orientation;

	void					adjustOrientation();

public:
	Projectile(glm::vec3 location, glm::quat orientation);

	std::vector<Entity*> getEntities() const
	{ return entities; }

	glm::vec3 getPosition() const
	{ return position; }

	glm::quat getOrientation() const
	{ return orientation; }

	void update(float dt);

};

