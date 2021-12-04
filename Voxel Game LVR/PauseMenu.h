#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "Settings.h"
#include "Projectile.h"
#include "CollisionType.h"
#include <vector>

class PauseMenu {
	std::vector<Entity*>	_entities;

	std::vector<Texture*>	_textures;
	std::vector<Material*>	_materials;
	std::vector<Mesh*>		_meshes;

	Entity*					_aligner;
	glm::quat				_orientation;
	glm::vec3				_position;
	float					_scale;

	void					_draw();

public:
	PauseMenu(Entity* aligner, glm::quat orientation, float scale);

	void					update(glm::quat orientation, float scale);
	void					interaction();

	std::vector<Entity*> getMenu() const 
	{ return _entities; }

	Entity* getResumeButton() const
	{ return _entities[1]; }

	Entity* getControlsButton() const
	{ return _entities[2]; }

	Entity* getMenuButton() const
	{ return _entities[3]; }

	Entity* getExitButton() const
	{ return _entities[4]; }
};

