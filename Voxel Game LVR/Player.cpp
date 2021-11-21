#include "Player.h"

Player::Player(glm::vec3 location)
	: position(location)
	, orientation(glm::vec3(NULL))
	, aim(NULL)
	, aligner(new Entity(NULL, NULL, location))
	, offsets(NULL)
{
	std::vector<Texture*> textures;
	textures.push_back(new Texture("textures/mogus.tga", GL_REPEAT, GL_LINEAR));

	std::vector<Material*> materials;
	materials.push_back(new Material(textures[0]));

	std::vector<Mesh*> meshes;
	meshes.push_back(CreateChunkyCone(1.5, 6, 10));
	meshes.push_back(CreateTexturedCube(2));
	meshes.push_back(CreateSmoothCylinder(1, 6, 6));
	meshes.push_back(CreateChunkyCone(0.75, 4, 6));
	meshes.push_back(CreateTexturedCube(1));

	entities.push_back(new Entity(meshes[0], materials[0], Transform(position.x, position.y, position.z))); // Front Cone
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y - 3.5, position.z - 0.25))); // Body
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y - 5.4, position.z - 0.25)));
	entities.push_back(new Entity(meshes[2], materials[0], Transform(position.x - 1.5, position.y - 3.5, position.z))); // Tube
	entities.push_back(new Entity(meshes[2], materials[0], Transform(position.x + 1.5, position.y - 3.5, position.z))); 
	entities.push_back(new Entity(meshes[3], materials[0], Transform(position.x - 1.5, position.y - 5.5, position.z))); // Rocket Exhaust
	entities.push_back(new Entity(meshes[3], materials[0], Transform(position.x + 1.5, position.y - 5.5, position.z)));
	entities.push_back(new Entity(meshes[4], materials[0], Transform(position.x, position.y - 3.5, position.z + 1))); // Top
	entities.push_back(new Entity(meshes[4], materials[0], Transform(position.x, position.y - 4.5, position.z + 1)));

	glm::vec3 relatedLoc(0, 0, 0);

	for (int i = 0; i < entities.size(); i++) {
		relatedLoc.x = entities[i]->getPosition().x - position.x;
		relatedLoc.y = entities[i]->getPosition().y - position.y;
		relatedLoc.z = entities[i]->getPosition().z - position.z;

		offsets.push_back(relatedLoc);
	}

}

void Player::headLook(float yaw, float pitch, float dt) {
	float radYaw = (3.14159265f / 180.0f) * yaw;
	float radPitch = (3.14159265f / 180.0f) * pitch;

	glm::vec3 rotEul = glm::vec3(radPitch - 1.5, radYaw, 0);
	aligner->setOrientation(glm::quat(rotEul));
	
	for (int i = 0; i < entities.size(); i++) {
		aligner->setPosition(position);
		aligner->translateLocal(offsets[i]);
		entities[i]->setPosition(aligner->getPosition());
		entities[i]->setOrientation(glm::quat(rotEul));
	}

	aligner->setPosition(position);
	aligner->setOrientation(entities[0]->getOrientation());
	aligner->translateLocal(0, 0, 3);
	aim = aligner->getPosition();
}

void Player::bodyMove(const Keyboard * kb, float dt) {
	
	float speed = 15;
	float disp = speed * dt;

	glm::vec3 displacement(0, 0, 0);
	
	// move forward and back
	if (kb->isKeyDown(KC_W))
		displacement.y = disp;
	if (kb->isKeyDown(KC_S))
		displacement.y = -disp;

	// strafe left and right
	if (kb->isKeyDown(KC_A))
		displacement.x = -disp;
	if (kb->isKeyDown(KC_D))
		displacement.x = disp;

	for (int i = 0; i < entities.size(); i++) {
		entities[i]->translateLocal(displacement);
	}

	position = entities[0]->getPosition();
}

Projectile* Player::shoot() {
	Projectile rocket(position, orientation);
	
	return &rocket;
}
