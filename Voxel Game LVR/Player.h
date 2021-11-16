#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "Player.h"
#include "Settings.h"
#include <vector>

class Player {
	std::string		textureLocation;
	Texture*		texture;
	Material*		material;
	Entity*			entity;
	Mesh*			mesh;
	
	glm::vec3		position;
	glm::vec3		aim;

	Settings		s;

public:
	Player(glm::vec3 location);

	Entity* getEntity() const
	{ return entity; }

	glm::vec3 getPosition() const 
	{ return entity->getPosition(); }

	glm::quat getOrientation() const
	{ return entity->getOrientation(); }
	
	void headLook(float yaw, float pitch, float dt);
	void bodyMove(const Keyboard* kb, float dt);
	glm::vec3 getAim() {}

};

