#include "GameEndMenu.h"

GameEndMenu::GameEndMenu(Entity* aligner, glm::quat orientation, float scale)
	: _orientation(orientation)
	, _position(aligner->getPosition())
	, _aligner(aligner)
	, _scale(scale)
{
	_textures.push_back(new Texture("textures/banner_end.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	_textures.push_back(new Texture("textures/button_end_restart.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	_textures.push_back(new Texture("textures/button_end_menu.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	_textures.push_back(new Texture("textures/button_exit.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	_textures.push_back(new Texture("textures/cursor_end.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));

	_materials.push_back(new Material(_textures[0]));
	_materials.push_back(new Material(_textures[1]));
	_materials.push_back(new Material(_textures[2]));
	_materials.push_back(new Material(_textures[3]));
	_materials.push_back(new Material(_textures[4]));

	_mouse = new Entity(NULL, NULL, Transform(_position));
	_draw();
}

void GameEndMenu::_draw() {
	for (int i = 0; i < _entities.size(); ++i)
		delete _entities[i];

	_entities.clear();

	for (int i = 0; i < _meshes.size(); ++i)
		delete _meshes[i];

	_meshes.clear();

	_meshes.push_back(CreateTexturedQuad(0.5f  * _scale, 0.125f  * _scale, 1.0, 1.0)); // Banner
	_meshes.push_back(CreateTexturedQuad(0.4f  * _scale, 0.1f  * _scale, 1.0, 1.0)); // Button
	_meshes.push_back(CreateTexturedQuad(0.1f  * _scale, 0.1f  * _scale, 1.0, 1.0)); // Red X
	_meshes.push_back(CreateTexturedQuad(0.1f * _scale, 0.1f * _scale, 1.0, 1.0)); // Cursor

	// Menu
	_aligner->translateLocal(0, 0.25 * _scale, 0);
	_entities.push_back(new Entity(_meshes[0], _materials[0], Transform(_aligner->getPosition()))); // Banner
	_aligner->setPosition(_position);

	_aligner->translateLocal(0, -0.025 * _scale, 0);
	_entities.push_back(new Entity(_meshes[1], _materials[1], Transform(_aligner->getPosition()))); // Restart
	_aligner->setPosition(_position);

	_aligner->translateLocal(0, -0.15 * _scale, 0);
	_entities.push_back(new Entity(_meshes[1], _materials[2], Transform(_aligner->getPosition()))); // Main Menu
	_aligner->setPosition(_position);

	_aligner->translateLocal(0.5 * _scale, 0.35 * _scale, 0);
	_entities.push_back(new Entity(_meshes[2], _materials[3], Transform(_aligner->getPosition()))); // Red X
	_aligner->setPosition(_position);

	for (int i = 0; i < _entities.size(); ++i) {
		_entities[i]->setOrientation(_orientation);
	}

	delete _cursor;
	_cursor = new Entity(_meshes[3], _materials[4], Transform(_mouse->getPosition())); // Cursor
	_cursor->setOrientation(_orientation);

}

void GameEndMenu::_mouseLocator(const Mouse * mouse) {
	_mouse->setPosition(_aligner->getPosition());

	float mouseX = (float)(mouse->getX() - s.SCREEN_WIDTH() / 2) / (s.SCREEN_HEIGHT() / _scale);
	float mouseY = -(float)(mouse->getY() - s.SCREEN_HEIGHT() / 2) / (s.SCREEN_HEIGHT() / _scale);
	float mouseZ = _aligner->getPosition().z;

	glm::vec3 mousePos(mouseX, mouseY, mouseZ);
	mousePos.z = mouseZ;

	_mouse->translateLocal(mousePos);
	_mouse->setOrientation(_orientation);
}

void GameEndMenu::update(glm::quat orientation, float scale) {
	_position = _aligner->getPosition();
	_orientation = orientation;
	_scale = scale;

	_draw();
}

void GameEndMenu::interaction(const Mouse* mouse) {

	_restart = false;
	_menu = false;
	_exit = false;

	if (mouse->buttonPressed(MOUSE_BUTTON_LEFT))
		_onClick();

	_mouseLocator(mouse);
	_cursor->setPosition(_mouse->getPosition());

}

void GameEndMenu::_onClick() {
	glm::vec3 mousePos(_cursor->getPosition());

	if (_entities[1]->doesIntersectAABB_Point(mousePos))
		_restart = true;
	if (_entities[2]->doesIntersectAABB_Point(mousePos))
		_menu = true;
	if (_entities[3]->doesIntersectAABB_Point(mousePos))
		_exit = true;
}