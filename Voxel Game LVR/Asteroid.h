#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "Settings.h"
#include "Projectile.h"
#include <vector>

class Asteroid {
	std::vector<Entity*>	entities;
	std::vector<glm::vec3>	offsets;
	std::vector<Entity*>	hitboxes;
	std::vector<glm::vec3>	hbOffsets;

	Entity*					aligner;
	glm::vec3				position;
	glm::quat				orientation;

	float					selfSpeed;
	glm::vec3				selfDirection;
	int						stage;

	void					adjustOrientation(glm::quat orientation);
	void					spin();

public:
	Asteroid(glm::vec3 location, glm::vec3 vectr, int size);

	std::vector<Entity*> getEntities() const
	{ return entities; }

	std::vector<Entity*> getHitboxes() const
	{ return hitboxes; }

	glm::vec3 getPosition() const 
	{ return position; }

	glm::quat getOrientation() const
	{ return orientation; }

	void changeDirection();

	void update();
};

