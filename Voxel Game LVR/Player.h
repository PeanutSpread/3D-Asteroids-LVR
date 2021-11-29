#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "Settings.h"
#include "Projectile.h"
#include "CollisionType.h"
#include <vector>

class Player {
	std::vector<Entity*>	_entities;
	std::vector<glm::vec3>	_offsets;
	std::vector<Entity*>	_hitboxes;
	std::vector<glm::vec3>	_hbOffsets;
	
	Entity*					_aligner;
	glm::vec3				_position;
	glm::quat				_orientation;
	glm::vec3				_aim;

	Settings				s;

public:
	Player(glm::vec3 location);

	std::vector<Entity*> getEntities() const
	{ return _entities; }

	std::vector<Entity*> getHitboxes() const
	{ return _hitboxes; }

	glm::vec3 getPosition() const 
	{ return _position; }

	glm::quat getOrientation() const
	{ return _orientation; }

	glm::vec3 getAim() const
	{ return _aim; }
	
	void headLook(float yaw, float pitch, float dt);
	void bodyMove(const Keyboard* kb, float dt);
	std::vector<Projectile*> shoot();
	bool hasCollision(std::vector<Entity*> otherHitboxes, CollisionType check);

};

