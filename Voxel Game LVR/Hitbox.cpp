#include "Hitbox.h"

Hitbox::Hitbox(Entity* box, float length, float width, float height) 
	: entity(box)
	, position(NULL)
	, orientation(glm::quat(glm::vec3(0,0,0)))
	, xSize(length)
	, ySize(width)
	, zSize(height)
{
	position = entity->getPosition();
	orientation = entity->getOrientation();
}

Hitbox::Hitbox(Entity* cube, float length) 
	: entity(cube)
	, position(NULL)
	, orientation(glm::quat(glm::vec3(0, 0, 0)))
	, xSize(length)
	, ySize(length)
	, zSize(length)
{
	position = entity->getPosition();
	orientation = entity->getOrientation();
}
