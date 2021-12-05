#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "Settings.h"
#include "Projectile.h"
#include <vector>

class Asteroid {
	std::vector<Entity*>	_entities; // Collection of enitities for visuals
	std::vector<glm::vec3>	_offsets; // Keeps track of relative positions of models for visuals
	std::vector<Entity*>	_hitboxes; // Collection of entities for collision reasons
	std::vector<glm::vec3>	_hbOffsets; // Keeps track of relative positions of hitboxes for collision

	Entity*					_aligner; // Do rotations and translations then assign resulting value to other entities
	glm::vec3				_position;
	glm::quat				_orientation;

	float					_speed;
	glm::vec3				_direction;
	int						_stage; // Help determine if it splits into more asteroids

	void					_adjustOrientation(glm::quat orientation);
	void					_spin(); // Animate
	void					_destroy(); // Memory Management

	Settings				s;

public:
	Asteroid(glm::vec3 location, glm::vec3 velocity, int scale);

	std::vector<Entity*> getEntities() const
	{ return _entities; }

	std::vector<Entity*> getHitboxes() const
	{ return _hitboxes; }

	glm::vec3 getPosition() const 
	{ return _position; }

	glm::quat getOrientation() const
	{ return _orientation; }

	int getStage() const
	{ return _stage; }

	void adjustSpeed(float value)
	{ _speed += value; }

	void setPosition(glm::vec3 value);

	std::vector<Asteroid*> explode();

	void destroy()
	{ _destroy(); }

	void update(float dt); // game loop function
};

