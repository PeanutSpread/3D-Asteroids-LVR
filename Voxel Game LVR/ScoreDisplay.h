#pragma once
#include "Image.h"
#include "Prefabs.h"
#include "Entity.h"
#include "Settings.h"
#include "Projectile.h"
#include <vector>
class ScoreDisplay {
	std::vector<Texture*>	_textures;
	std::vector<Material*>	_materials;
	Mesh*					_number;
	int						_redOffest = 10;

	void					_destroy();

public:
	ScoreDisplay();
	void					addScoreEntities(glm::vec3 position, glm::quat orientation, int scale, std::vector<Entity*> &entities, int score);
	void					addEndScoreEntities(glm::vec3 position, glm::quat orientation, int scale, std::vector<Entity*> &entities, int score);

	void destroy()
	{ _destroy(); }
};

