#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "Settings.h"
#include "Projectile.h"
#include <vector>

class Player {
	std::vector<Entity*>	entities;
	std::vector<glm::vec3>	offsets;
	std::vector<Entity*>	hitboxes;
	std::vector<glm::vec3>	hbOffsets;
	
	Entity*					aligner;
	glm::vec3				position;
	glm::quat				orientation;
	glm::vec3				aim;

	Settings				s;

public:
	Player(glm::vec3 location);

	std::vector<Entity*> getEntities() const
	{ return entities; }

	std::vector<Entity*> getHitboxes() const
	{ return hitboxes; }

	glm::vec3 getPosition() const 
	{ return position; }

	glm::quat getOrientation() const
	{ return orientation; }

	glm::vec3 getAim() const
	{ return aim; }
	
	void headLook(float yaw, float pitch, float dt);
	void bodyMove(const Keyboard* kb, float dt);
	std::vector<Projectile*> shoot();
	bool hasCollision(std::vector<Entity*> otherHitboxes);

};

