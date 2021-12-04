#include "Projectile.h"

Projectile::Projectile(glm::vec3 location, glm::quat orientation)
	: _position(location)
	, _orientation(orientation)
	, _aligner(new Entity(NULL, NULL, location))
	, _offsets(NULL)
{
	std::vector<Texture*> textures;
	textures.push_back(new Texture("textures/paint_red.tga", GL_REPEAT, GL_LINEAR));
	textures.push_back(new Texture("textures/rusted_gold.tga", GL_REPEAT, GL_LINEAR));
	textures.push_back(new Texture("textures/debug2.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));

	std::vector<Material*> materials;
	materials.push_back(new Material(textures[0]));
	materials.push_back(new Material(textures[1]));
	materials.push_back(new Material(textures[2]));

	std::vector<Mesh*> meshes;
	meshes.push_back(CreateChunkyCone(0.5f, 0.75f, 8.f));
	meshes.push_back(CreateChunkyTexturedCylinder(0.5f, 1.5f, 8.f));
	meshes.push_back(CreateChunkyTexturedCylinder(0.4f, 0.5f, 8.f));

	// Hitbox
	meshes.push_back(CreateWireframeBox(1.f, 3.f, 1.f));

	_entities.push_back(new Entity(meshes[0], materials[0], Transform(_position.x, _position.y, _position.z))); // Front Cone
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x, _position.y - 1.1f, _position.z)));
	_entities.push_back(new Entity(meshes[2], materials[1], Transform(_position.x, _position.y - 2.0f, _position.z)));

	// Hitbox
	_hitboxes.push_back(new Entity(meshes[3], materials[2], Transform(_position.x, _position.y - 1.f, _position.z)));

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

	_adjustOrientation();
}

void Projectile::_adjustOrientation() {

	_aligner->setOrientation(_orientation);
	for (int i = 0; i < _entities.size(); i++) {
		_aligner->setPosition(_position);
		_aligner->translateLocal(_offsets[i]);
		_entities[i]->setPosition(_aligner->getPosition());
		_entities[i]->setOrientation(_orientation);
	}

	for (int i = 0; i < _hitboxes.size(); i++) {
		_aligner->setPosition(_position);
		_aligner->translateLocal(_hbOffsets[i]);
		_hitboxes[i]->setPosition(_aligner->getPosition());
		_hitboxes[i]->setOrientation(_orientation);
	}

}

void Projectile::update(float dt) {
	float speed = 150;
	float disp = speed * dt;

	glm::vec3 displacement(0, 0, 0);

	displacement.y = disp;
	_entities[0]->translateLocal(displacement);

	_position = _entities[0]->getPosition();
	_adjustOrientation();
}

bool Projectile::hasCollision(std::vector<Entity*> otherHitboxes, CollisionType check)
{
	for (int i = 0; i < _hitboxes.size(); i++) {
		for (int j = 0; j < otherHitboxes.size(); j++) {

			switch (check) {
			case AABB_AABB:
				if (_hitboxes[i]->doesIntersectAABB_AABB(otherHitboxes[j]))
					return true;
				break;
			case AABB_Sphere:
				if (_hitboxes[i]->doesIntersectAABB_Sphere(otherHitboxes[j]))
					return true;
				break;
			default:
				break;
			}
		}
	}

	return false;
}

void Projectile::destroy() {
	for (int i = 0; i < _entities.size(); ++i)
		delete _entities[i];
	for (int i = 0; i < _hitboxes.size(); ++i)
		delete _hitboxes[i];

	delete _aligner;
}
