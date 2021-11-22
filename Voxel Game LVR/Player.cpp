#include "Player.h"

Player::Player(glm::vec3 location)
	: position(location)
	, orientation(glm::vec3(NULL))
	, aim(NULL)
	, aligner(new Entity(NULL, NULL, location))
	, offsets(NULL)
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
	meshes.push_back(CreateChunkyCone(1.5, 6, 10));
	meshes.push_back(CreateTexturedCube(2));
	meshes.push_back(CreateSmoothCylinder(1, 6, 6));
	meshes.push_back(CreateChunkyCone(0.75, 4, 6));
	meshes.push_back(CreateTexturedCube(1));
	meshes.push_back(CreateTexturedQuad(0.6, 2, 1, 1));

	// Hitbox
	meshes.push_back(CreateWireframeBox(4, 6, 1.5));
	meshes.push_back(CreateWireframeCube(1));
	meshes.push_back(CreateWireframeBox(0.25, 2, 0.25));


	// Model
	entities.push_back(new Entity(meshes[0], materials[0], Transform(position.x, position.y, position.z))); // Front Cone
	entities.push_back(new Entity(meshes[5], materials[3], Transform(position.x, position.y - 2, position.z + 1.195))); // Window
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y - 3.5, position.z - 0.25))); // Body
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y - 5.4, position.z - 0.25)));
	entities.push_back(new Entity(meshes[2], materials[1], Transform(position.x - 1.5, position.y - 3.5, position.z))); // Tube
	entities.push_back(new Entity(meshes[2], materials[1], Transform(position.x + 1.5, position.y - 3.5, position.z))); 
	entities.push_back(new Entity(meshes[3], materials[2], Transform(position.x - 1.5, position.y - 5.5, position.z))); // Rocket Exhaust
	entities.push_back(new Entity(meshes[3], materials[2], Transform(position.x + 1.5, position.y - 5.5, position.z)));
	entities.push_back(new Entity(meshes[4], materials[0], Transform(position.x, position.y - 3.5, position.z + 1))); // Top
	entities.push_back(new Entity(meshes[4], materials[0], Transform(position.x, position.y - 4.5, position.z + 1)));

	// Hitbox
	hitboxes.push_back(new Entity(meshes[8], materials[4], Transform(position.x, position.y + 1.4, position.z)));
	hitboxes.push_back(new Entity(meshes[7], materials[4], Transform(position.x, position.y, position.z)));
	hitboxes.push_back(new Entity(meshes[6], materials[4], Transform(position.x, position.y - 3.5, position.z)));

	glm::vec3 relatedLoc(0, 0, 0);

	for (int i = 0; i < entities.size(); i++) {
		relatedLoc.x = entities[i]->getPosition().x - position.x;
		relatedLoc.y = entities[i]->getPosition().y - position.y;
		relatedLoc.z = entities[i]->getPosition().z - position.z;

		offsets.push_back(relatedLoc);
	}

	for (int i = 0; i < hitboxes.size(); i++) {
		relatedLoc.x = entities[i]->getPosition().x - position.x;
		relatedLoc.y = entities[i]->getPosition().y - position.y;
		relatedLoc.z = entities[i]->getPosition().z - position.z;

		hbOffsets.push_back(relatedLoc);
	}

}

void Player::headLook(float yaw, float pitch, float dt) {
	float radYaw = (3.14159265f / 180.0f) * yaw;
	float radPitch = (3.14159265f / 180.0f) * pitch;

	glm::vec3 rotEul = glm::vec3(radPitch - 1.5, radYaw, 0);
	aligner->setOrientation(glm::quat(rotEul));
	

	// rotating the ship
	for (int i = 0; i < entities.size(); i++) {
		aligner->setPosition(position);
		aligner->translateLocal(offsets[i]);
		entities[i]->setPosition(aligner->getPosition());
		entities[i]->setOrientation(glm::quat(rotEul));
	}
	entities[1]->rotate(-13.5, glm::vec3(1, 0, 0));

	// rotating the hitboxes
	for (int i = 0; i < hitboxes.size(); i++) {
		aligner->setPosition(position);
		aligner->translateLocal(hbOffsets[i]);
		hitboxes[i]->setPosition(aligner->getPosition());
		hitboxes[i]->setOrientation(glm::quat(rotEul));
	}

	aligner->setPosition(position);
	aligner->setOrientation(entities[0]->getOrientation());
	aligner->translateLocal(0, 0, 3);
	aim = aligner->getPosition();

	orientation = entities[0]->getOrientation();
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

std::vector<Projectile*> Player::shoot() {
	std::vector<Projectile*> rockets;
	rockets.push_back(new Projectile(aim, orientation));
	
	return  (rockets);
}

bool Player::hasCollision(std::vector<Entity*> otherHitboxes) {
	bool collide = false;

	return collide;
}
