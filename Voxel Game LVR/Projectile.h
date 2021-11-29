#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "CollisionType.h"
#include <vector>

class Projectile {
	std::vector<Entity*>	_entities;
	std::vector<glm::vec3>	_offsets;
	std::vector<Entity*>	_hitboxes;
	std::vector<glm::vec3>	_hbOffsets;

	Entity*					_aligner;
	glm::vec3				_position;
	glm::quat				_orientation;

	void					_adjustOrientation();

public:
	Projectile(glm::vec3 location, glm::quat orientation);

	std::vector<Entity*> getEntities() const
	{ return _entities; }

	std::vector<Entity*> getHitboxes() const
	{ return _hitboxes; }

	glm::vec3 getPosition() const
	{ return _position; }

	glm::quat getOrientation() const
	{ return _orientation; }

	void update(float dt);
	bool hasCollision(std::vector<Entity*> otherHitboxes, CollisionType check);

};

