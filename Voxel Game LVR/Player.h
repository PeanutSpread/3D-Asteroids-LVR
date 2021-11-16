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

	Settings		s;

public:
	Player(float x, float y, float z);

	Entity* getEntity() const
	{ return entity; }
	
	glm::vec3 getPosition() const 
	{ return entity->getPosition(); }

	glm::quat getOrientation() const
	{ return entity->getOrientation(); }
	
	void headLook(int x, int y, float dt);
	void bodyMove(const Keyboard* kb, float dt);

};

