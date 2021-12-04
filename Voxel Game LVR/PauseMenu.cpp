#include "PauseMenu.h"

PauseMenu::PauseMenu(Entity* aligner, glm::quat orientation, float scale)
	: _orientation(orientation)
	, _position(aligner->getPosition())
	, _aligner(aligner)
	, _scale(scale)
{
	_textures.push_back(new Texture("textures/banner.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	_textures.push_back(new Texture("textures/button_resume.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	_textures.push_back(new Texture("textures/button_controls.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	_textures.push_back(new Texture("textures/button_menu.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	_textures.push_back(new Texture("textures/button_exit.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));

	_materials.push_back(new Material(_textures[0]));
	_materials.push_back(new Material(_textures[1]));
	_materials.push_back(new Material(_textures[2]));
	_materials.push_back(new Material(_textures[3]));
	_materials.push_back(new Material(_textures[4]));

	_draw();
}

void PauseMenu::_draw() {
	for (int i = 0; i < _entities.size(); ++i)
		delete _entities[i];

	_entities.clear();

	for (int i = 0; i < _meshes.size(); ++i)
		delete _meshes[i];

	_meshes.clear();

	_meshes.push_back(CreateTexturedQuad(0.5f  * _scale, 0.125f  * _scale, 1.0, 1.0)); // Banner
	_meshes.push_back(CreateTexturedQuad(0.4f  * _scale, 0.1f  * _scale, 1.0, 1.0)); // Button
	_meshes.push_back(CreateTexturedQuad(0.1f  * _scale, 0.1f  * _scale, 1.0, 1.0)); // Red X

	// Model
	_aligner->translateLocal(0, 0, 0.25 * _scale);
	_entities.push_back(new Entity(_meshes[0], _materials[0], Transform(_aligner->getPosition()))); // Banner
	_aligner->setPosition(_position);

	_aligner->translateLocal(0, 0, 0.10 * _scale);
	_entities.push_back(new Entity(_meshes[1], _materials[1], Transform(_aligner->getPosition()))); // Resume
	_aligner->setPosition(_position); 

	_aligner->translateLocal(0, 0, - 0.025 * _scale);
	_entities.push_back(new Entity(_meshes[1], _materials[2], Transform(_aligner->getPosition()))); // Controls
	_aligner->setPosition(_position);

	_aligner->translateLocal(0, 0, -0.15 * _scale);
	_entities.push_back(new Entity(_meshes[1], _materials[3], Transform(_aligner->getPosition()))); // Main Menu
	_aligner->setPosition(_position);

	_aligner->translateLocal(0.5 * _scale, 0, 0.35 * _scale);
	_entities.push_back(new Entity(_meshes[2], _materials[4], Transform(_aligner->getPosition()))); // Red X
	_aligner->setPosition(_position);

	for (int i = 0; i < _entities.size(); ++i) {
		_entities[i]->setOrientation(_orientation);
	}
}


void PauseMenu::update(glm::quat orientation, float scale) {
	_position = _aligner->getPosition();
	_orientation = orientation;
	_scale = scale;

	_draw();
}
