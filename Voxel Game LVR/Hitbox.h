#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
class Hitbox {
	Entity*		entity;
	
	glm::vec3	position;
	glm::quat	orientation;

	float		xSize;
	float		ySize;
	float		zSize;

public:
	Hitbox(Entity* box, float length, float width, float height);
	Hitbox(Entity* cube, float length);

};

