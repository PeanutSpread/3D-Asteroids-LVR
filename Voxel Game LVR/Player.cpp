#include "Player.h"

Player::Player(glm::vec3 location)
	:textureLocation("textures/mogus.tga")
	,texture(NULL)
	,material(NULL)
	,mesh(NULL)
	,entity(NULL)
	,position(location)
{
	this->texture = new Texture(textureLocation, GL_REPEAT, GL_LINEAR);
	this->material = new Material(this->texture);
	this->mesh = CreateChunkyCone(1,6,6);
	this->entity = new Entity(mesh, material, Transform(position.x, position.y, position.z));

	this->entity->rotate(-90, 1, 0, 0);
}

void Player::headLook(float yaw, float pitch, float dt) {
	float radYaw = (3.14159265f / 180.0f) * yaw;
	float radPitch = (3.14159265f / 180.0f) * pitch;

	glm::vec3 rotEul = glm::vec3(radPitch - 1.5, radYaw, 0);

	entity->setOrientation(glm::quat(rotEul));

}

void Player::bodyMove(const Keyboard * kb, float dt) {
	
	float speed = 3;
	float disp = speed * dt;
	
	// move forward and back
	if (kb->isKeyDown(KC_W))
		entity->translateLocal(0, disp, 0);
	if (kb->isKeyDown(KC_S))
		entity->translateLocal(0, -disp, 0);

	// strafe left and right
	if (kb->isKeyDown(KC_A))
		entity->translateLocal(-disp, 0, 0);
	if (kb->isKeyDown(KC_D))
		entity->translateLocal(disp, 0, 0);
}
