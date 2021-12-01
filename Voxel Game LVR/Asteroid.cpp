#include "Asteroid.h"
#include "Prefabs.h"

Asteroid::Asteroid(glm::vec3 location, glm::vec3 velocity, int size)
	: _position(location)
	, _orientation(glm::vec3(NULL))
	, _aligner(new Entity(NULL, NULL, location))
	, _offsets(NULL)
	, _direction(NULL)
	, _speed((float) velocity.length())
	, _stage(size)
{
	std::vector<Texture*> textures;
	textures.push_back(new Texture("textures/rocky.tga", GL_REPEAT, GL_LINEAR));
	textures.push_back(new Texture("textures/debug2.tga", GL_CLAMP_TO_EDGE, GL_LINEAR)); // Hitbox Debug Texture

	std::vector<Material*> materials;
	materials.push_back(new Material(textures[0]));
	materials.push_back(new Material(textures[1]));

	std::vector<Mesh*> meshes;
	// Model
	meshes.push_back(CreateTexturedCube(4.f * size));
	meshes.push_back(CreateTexturedCube(2.f * size));

	// Hitbox
	meshes.push_back(CreateWireframeBox(4.5f * size, 4.5f * size, 4.5f * size));

	// Model
	_entities.push_back(new Entity(meshes[0], materials[0], Transform(_position.x, _position.y, _position.z))); // Base
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x + 1.5f * size, _position.y, _position.z))); // Bumps
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x - 1.5f * size, _position.y, _position.z)));
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x, _position.y + 1.5f * size, _position.z)));
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x, _position.y - 1.5f * size, _position.z)));
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x, _position.y, _position.z + 1.5f * size)));
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x, _position.y, _position.z - 1.5f * size)));
	
	// Hitbox
	_hitboxes.push_back(new Entity(meshes[2], materials[1], Transform(_position.x, _position.y, _position.z)));

	// Getting offsets
	glm::vec3 relatedLoc(0, 0, 0);

	for (int i = 0; i < _entities.size(); i++) {
		relatedLoc.x = _entities[i]->getPosition().x - _position.x;
		relatedLoc.y = _entities[i]->getPosition().y - _position.y;
		relatedLoc.z = _entities[i]->getPosition().z - _position.z;

		_offsets.push_back(relatedLoc);
	}

	for (int i = 0; i < _hitboxes.size(); i++) {
		relatedLoc.x = _hitboxes[i]->getPosition().x - _position.x;
		relatedLoc.y = _hitboxes[i]->getPosition().y - _position.y;
		relatedLoc.z = _hitboxes[i]->getPosition().z - _position.z;

		_hbOffsets.push_back(relatedLoc);
	}

	// Set direction as a unit vector
	_direction = glm::vec3(velocity.x / velocity.length(), velocity.y / velocity.length(), velocity.z / velocity.length());
}

void Asteroid::_adjustOrientation(glm::quat orientation) {

	// rotating the asteroid's individual entities
	_aligner->setOrientation(orientation);
	for (int i = 0; i < _entities.size(); i++) {
		_aligner->setPosition(_position);
		_aligner->translateLocal(_offsets[i]);
		_entities[i]->setPosition(_aligner->getPosition());
		_entities[i]->setOrientation(orientation);
	}

	// rotating the hitboxes
	for (int i = 0; i < _hitboxes.size(); i++) {
		_aligner->setPosition(_position);
		_aligner->translateLocal(_hbOffsets[i]);
		_hitboxes[i]->setPosition(_aligner->getPosition());
		_hitboxes[i]->setOrientation(orientation);
	}

	_orientation = _entities[0]->getOrientation();

}

// Rotation Animation
void Asteroid::_spin() {
	float speed = _speed / 4;
	_aligner->setOrientation(_entities[0]->getOrientation());
	_aligner->rotate(_direction.x * speed, glm::vec3(1, 0, 0));
	_aligner->rotate(_direction.y * speed, glm::vec3(0, 1, 0));
	_aligner->rotate(_direction.z * speed, glm::vec3(0, 0, 1));
	_adjustOrientation(_aligner->getOrientation());
}

void Asteroid::setPosition(glm::vec3 value) {
	_position = value;
	_adjustOrientation(_orientation);
}

void Asteroid::explode() {
	// TODO: memory management
	// Splintering
}

void Asteroid::update(float dt) {
	float disp = _speed * dt;

	glm::vec3 displacement(_direction);
	displacement.x *= disp;
	displacement.y *= disp;
	displacement.z *= disp;

	_entities[0]->translateLocal(displacement);

	_position = _entities[0]->getPosition();
	_spin();
}
