#include "ScoreDisplay.h"
ScoreDisplay::ScoreDisplay() {
	std::string texChange = "";
	texChange = "end_";

	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < 10; ++i)
			_textures.push_back(new Texture("textures/number_" + texChange + std::to_string(i) + ".tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
		texChange = "end_";
	}
	
	for (int i = 0; i < _textures.size(); ++i)
		_materials.push_back(new Material(_textures[i]));
}

void ScoreDisplay::addScoreEntities(glm::vec3 position, glm::quat orientation, int scale, std::vector<Entity*> &entities, int score) {
	
	delete _number;
	_number = (CreateTexturedQuad(0.025 * scale, 0.05 * scale, 1.0, 1.0));

	Entity aligner(NULL, NULL, position);
	aligner.setOrientation(orientation);

	std::string scoreString = std::to_string(score);
	aligner.translateLocal(-(0.025 * scale) * (scoreString.size() / 2), 0, 0);
	glm::vec3 base(aligner.getPosition());

	for (int i = 0; i < scoreString.size(); ++i) {
		aligner.translateLocal((0.025 * i) * scale, 0, 0);
		entities.push_back(new Entity(_number, _materials[scoreString[i] - '0'], Transform(aligner.getPosition())));
		entities[entities.size() - 1]->setOrientation(orientation);
		aligner.setPosition(base);
	}
}

void ScoreDisplay::addEndScoreEntities(glm::vec3 position, glm::quat orientation, int scale, std::vector<Entity*> &entities, int score) {

	delete _number;
	_number = (CreateTexturedQuad(0.025 * scale, 0.05 * scale, 1.0, 1.0));

	Entity aligner(NULL, NULL, position);
	aligner.setOrientation(orientation);

	std::string scoreString = std::to_string(score);
	aligner.translateLocal(-(0.025 * scale) * (scoreString.size() / 2), 0, 0);
	glm::vec3 base(aligner.getPosition());

	for (int i = 0; i < scoreString.size(); ++i) {
		aligner.translateLocal((0.025 * i) * scale, 0, 0);
		entities.push_back(new Entity(_number, _materials[(scoreString[i] - '0') + _redOffest], Transform(aligner.getPosition())));
		entities[entities.size() - 1]->setOrientation(orientation);
		aligner.setPosition(base);
	}
}

void ScoreDisplay::_destroy() {
	for (int i = 0; i < _textures.size(); ++i)
		delete _textures[i];
	_textures.clear();

	for (int i = 0; i < _materials.size(); ++i)
		delete _materials[i];
	_materials.clear();

	delete _number;
	_number = NULL;
}

