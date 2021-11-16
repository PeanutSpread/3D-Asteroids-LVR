#include "Player.h"

Player::Player(float x, float y, float z)
	:textureLocation("textures/mogus.tga")
	,texture(NULL)
	,material(NULL)
	,mesh(NULL)
	,entity(NULL)
	,position(x, y, z)
{
	this->texture = new Texture(textureLocation, GL_CLAMP_TO_EDGE, GL_LINEAR);
	this->material = new Material(this->texture);
	this->mesh = CreateTexturedCube(1);
	this->entity = new Entity(mesh, material, Transform(position.x, position.y, position.z));

	this->entity->rotate(180, 0, 1, 0);
}

void Player::headLook(int x, int y, float dt) {

	int mouseChangeX = x;
	int mouseChangeY = y;

	int mouseChangeAvg = (abs(mouseChangeX) + abs(mouseChangeY)) / 2;
	if (abs(mouseChangeX) == 0 || abs(mouseChangeY) == 0) {
		mouseChangeAvg = (abs(mouseChangeX) + abs(mouseChangeY));
	}

	float rotSpeed = 50 * abs(mouseChangeAvg);
	float rotAmount = rotSpeed * dt;
	if (mouseChangeX < 0)
		entity->rotate(rotAmount, 0, 1, 0);
	if (mouseChangeX > 0)
		entity->rotate(-rotAmount, 0, 1, 0);
	if (mouseChangeY < 0)
		entity->rotate(rotAmount, 1, 0, 0);
	if (mouseChangeY > 0)
		entity->rotate(-rotAmount, 1, 0, 0);

}

void Player::bodyMove(const Keyboard * kb, float dt) {
	
	float speed = 3;
	float disp = speed * dt;
	
	// move forward and back
	if (kb->isKeyDown(KC_W))
		entity->translateLocal(0, 0, -disp);
	if (kb->isKeyDown(KC_S))
		entity->translateLocal(0, 0, disp);

	// strafe left and right
	if (kb->isKeyDown(KC_A))
		entity->translateLocal(-disp, 0, 0);
	if (kb->isKeyDown(KC_D))
		entity->translateLocal(disp, 0, 0);
}
