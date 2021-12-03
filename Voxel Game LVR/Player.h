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
	std::vector<Entity*>	_lights;
	std::vector<glm::vec3>	_litOffsets;
	
	Entity*					_aligner;
	glm::vec3				_position;
	glm::quat				_orientation;
	glm::vec3				_aim;
	float					_acceleration = 0;
	float					t = 0;
	bool					_throttle = false;

	glm::vec3				_splinePointOnCurve(float dt, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	Settings				s;

public:
	Player(glm::vec3 location);

	std::vector<Entity*> getEntities();

	std::vector<Entity*> getHitboxes() const
	{ return _hitboxes; }

	std::vector<Entity*> getLights() const
	{ return _lights; }

	glm::vec3 getPosition() const 
	{ return _position; }

	glm::quat getOrientation() const
	{ return _orientation; }

	glm::vec3 getAim() const
	{ return _aim; }

	float getAcceleration() const
	{ return _acceleration; }

	bool getThrottle() const
	{ return _throttle; }
	
	void headLook(float yaw, float pitch, float dt);
	void bodyMove(const Keyboard* kb, float dt);
	void death(float dt);
	std::vector<Projectile*> shoot();
	bool hasCollision(std::vector<Entity*> otherHitboxes, CollisionType check);

};

