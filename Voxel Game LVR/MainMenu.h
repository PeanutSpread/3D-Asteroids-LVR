#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "Settings.h"
#include "Projectile.h"
#include "CollisionType.h"
#include <vector>

class MainMenu {
	std::vector<Entity*>	_entities;

	std::vector<Texture*>	_textures;
	std::vector<Material*>	_materials;
	std::vector<Mesh*>		_meshes;

	Entity*					_aligner;
	Entity*					_mouse;
	Entity*					_cursor;
	glm::quat				_orientation;
	glm::vec3				_position;
	float					_scale;

	bool					_start = false;
	bool					_controls = false;
	bool					_exit = false;

	void					_mouseLocator(const Mouse* mouse);
	void					_draw();
	void					_onClick();
	void					_destroy();
	Settings				s;


public:
	MainMenu(Entity* aligner, glm::quat orientation, float scale);

	void					update(glm::quat orientation, float scale);
	void					interaction(const Mouse* mouse);

	std::vector<Entity*> getMenu() const
	{ return _entities; }

	Entity* getCursor() const
	{ return _cursor; }

	bool getStartButton() const
	{ return _start; }

	bool getControlsButton() const
	{ return _controls; }

	bool getExitButton() const
	{ return _exit; }

	void destroy()
	{ _destroy(); }
};

