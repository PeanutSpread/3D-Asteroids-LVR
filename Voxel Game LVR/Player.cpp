#include "Player.h"
#include "CollisionType.h"

Player::Player(glm::vec3 location)
	: _position(location)
	, _orientation(glm::vec3(NULL))
	, _aim(NULL)
	, _aligner(new Entity(NULL, NULL, location))
	, _offsets(NULL)
{
	std::vector<Texture*> textures;
	textures.push_back(new Texture("textures/metal_panel.tga", GL_REPEAT, GL_LINEAR));
	textures.push_back(new Texture("textures/metal_panel_corrugated.tga", GL_REPEAT, GL_LINEAR));
	textures.push_back(new Texture("textures/metal_panel_dark.tga", GL_REPEAT, GL_LINEAR));
	textures.push_back(new Texture("textures/metal_window.tga", GL_REPEAT, GL_LINEAR));
	textures.push_back(new Texture("textures/debug2.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));

	std::vector<Material*> materials;
	materials.push_back(new Material(textures[0]));
	materials.push_back(new Material(textures[1]));
	materials.push_back(new Material(textures[2]));
	materials.push_back(new Material(textures[3]));
	materials.push_back(new Material(textures[4]));

	std::vector<Mesh*> meshes;
	// Model
	meshes.push_back(CreateChunkyCone(1.5f, 6.f, 10));
	meshes.push_back(CreateTexturedCube(2.0125f));
	meshes.push_back(CreateSmoothCylinder(1.f, 6.f, 6));
	meshes.push_back(CreateChunkyCone(0.75f, 4.f, 6));
	meshes.push_back(CreateTexturedCube(1.f));
	meshes.push_back(CreateTexturedQuad(0.6f, 2.f, 1.f, 1.f));

	// Hitbox
	meshes.push_back(CreateWireframeBox(4.f, 6.f, 1.5f));
	meshes.push_back(CreateWireframeCube(1.f));
	meshes.push_back(CreateWireframeBox(0.25f, 2.f, 0.25f));

	// Lights
	meshes.push_back(CreateTexturedCube(0.5f));

	// Model
	_entities.push_back(new Entity(meshes[0], materials[0], Transform(_position.x, _position.y, _position.z))); // Front Cone
	_entities.push_back(new Entity(meshes[5], materials[3], Transform(_position.x, _position.y - 2.f, _position.z + 1.195f))); // Window
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x, _position.y - 3.5f, _position.z - 0.25f))); // Body
	_entities.push_back(new Entity(meshes[1], materials[0], Transform(_position.x, _position.y - 5.51f, _position.z - 0.25f)));
	_entities.push_back(new Entity(meshes[2], materials[1], Transform(_position.x - 1.5f, _position.y - 3.5f, _position.z))); // Tube
	_entities.push_back(new Entity(meshes[2], materials[1], Transform(_position.x + 1.5f, _position.y - 3.5f, _position.z)));
	_entities.push_back(new Entity(meshes[3], materials[2], Transform(_position.x - 1.5f, _position.y - 5.5f, _position.z))); // Rocket Exhaust
	_entities.push_back(new Entity(meshes[3], materials[2], Transform(_position.x + 1.5f, _position.y - 5.5f, _position.z)));
	_entities.push_back(new Entity(meshes[4], materials[0], Transform(_position.x, _position.y - 3.5f, _position.z + 1.f))); // Top
	_entities.push_back(new Entity(meshes[4], materials[0], Transform(_position.x, _position.y - 4.5f, _position.z + 1.f)));

	// Hitbox
	_hitboxes.push_back(new Entity(meshes[8], materials[4], Transform(_position.x, _position.y + 1.4f, _position.z)));
	_hitboxes.push_back(new Entity(meshes[7], materials[4], Transform(_position.x, _position.y, _position.z)));
	_hitboxes.push_back(new Entity(meshes[6], materials[4], Transform(_position.x, _position.y - 3.5f, _position.z)));

	// Lights
	_lights.push_back(new Entity(meshes[9], NULL, Transform(_position.x - 1.5f, _position.y - 0.25f, _position.z)));
	_lights.push_back(new Entity(meshes[9], NULL, Transform(_position.x + 1.5f, _position.y - 0.25f, _position.z)));
	_lights.push_back(new Entity(meshes[9], NULL, Transform(_position.x - 1.5f, _position.y - 1.25f, _position.z)));
	_lights.push_back(new Entity(meshes[9], NULL, Transform(_position.x + 1.5f, _position.y - 1.25f, _position.z)));
	_lights.push_back(new Entity(meshes[9], NULL, Transform(_position.x - 1.5f, _position.y - 2.25f, _position.z)));
	_lights.push_back(new Entity(meshes[9], NULL, Transform(_position.x + 1.5f, _position.y - 2.25f, _position.z)));
	_lights.push_back(new Entity(meshes[9], NULL, Transform(_position.x - 1.5f, _position.y - 8.f, _position.z)));
	_lights.push_back(new Entity(meshes[9], NULL, Transform(_position.x + 1.5f, _position.y - 8.f, _position.z)));

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

	for (int i = 0; i < _lights.size(); i++) {
		relatedLoc.x = _lights[i]->getPosition().x - _position.x;
		relatedLoc.y = _lights[i]->getPosition().y - _position.y;
		relatedLoc.z = _lights[i]->getPosition().z - _position.z;

		_litOffsets.push_back(relatedLoc);
	}

}

void Player::headLook(float yaw, float pitch, float dt) {
	float radYaw = (3.14159265f / 180.0f) * yaw;
	float radPitch = (3.14159265f / 180.0f) * pitch;

	glm::vec3 rotEul = glm::vec3(radPitch - 1.5f, radYaw, 0.f);
	_aligner->setOrientation(glm::quat(rotEul));
	

	// rotating the ship
	for (int i = 0; i < _entities.size(); i++) {
		_aligner->setPosition(_position);
		_aligner->translateLocal(_offsets[i]);
		_entities[i]->setPosition(_aligner->getPosition());
		_entities[i]->setOrientation(glm::quat(rotEul));
	}
	_entities[1]->rotate(-13.5, glm::vec3(1, 0, 0));

	// rotating the hitboxes
	for (int i = 0; i < _hitboxes.size(); i++) {
		_aligner->setPosition(_position);
		_aligner->translateLocal(_hbOffsets[i]);
		_hitboxes[i]->setPosition(_aligner->getPosition());
		_hitboxes[i]->setOrientation(glm::quat(rotEul));
	}

	// rotating the lights
	for (int i = 0; i < _lights.size(); i++) {
		_aligner->setPosition(_position);
		_aligner->translateLocal(_litOffsets[i]);
		_lights[i]->setPosition(_aligner->getPosition());
		_lights[i]->setOrientation(glm::quat(rotEul));
	}

	_aligner->setPosition(_position);
	_aligner->setOrientation(_entities[0]->getOrientation());
	_aligner->translateLocal(0, 0, 3);
	_aim = _aligner->getPosition();

	_orientation = _entities[0]->getOrientation();
}

void Player::bodyMove(const Keyboard * kb, float dt) {
	printf("%f\n", _acceleration);
	float speed = 0.5f;
	if (_acceleration <= 0)
		_acceleration = 0;

	float disp = 0;
	_aligner->setPosition(_entities[0]->getPosition());

	glm::vec3 displacement(0, 0, 0);

	// move forward and back
	if (kb->isKeyDown(KC_W)) {
		_acceleration += 1;
		_throttle = true;
		if (_acceleration > 50)
			_acceleration = 50;
		disp = _acceleration * speed * dt;
		displacement.y = disp;
	} else {
		_acceleration -= 0.5;
		_throttle = false;
		if (_acceleration < 0)
			_acceleration = 0;
		disp = _acceleration * speed * dt;
		displacement.y = disp;
	}
	/*
	if (kb->isKeyDown(KC_S))
		displacement.y = -disp;

	// strafe left and right
	if (kb->isKeyDown(KC_A))
		displacement.x = -disp;
	if (kb->isKeyDown(KC_D))
		displacement.x = disp;
	*/

	_aligner->translateLocal(displacement);

	/*
	for (int i = 0; i < _entities.size(); i++) {
		_entities[i]->translateLocal(displacement);
	}
	*/

	_position = _aligner->getPosition();
}

std::vector<Projectile*> Player::shoot() {
	std::vector<Projectile*> rockets;
	rockets.push_back(new Projectile(_position, _orientation));
	
	return  (rockets);
}

bool Player::hasCollision(std::vector<Entity*> otherHitboxes, CollisionType check) {
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
