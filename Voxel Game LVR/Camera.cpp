#include "Camera.h"

Camera::Camera(GLApp* app, Player* player)
	: mApp(app)
	, mPosition(0.0f, 0.0f, 0.0f)
	, mForward(0.0f, 0.0f, -1.0f)
	, mRight(1.0f, 0.0f, 0.0f)
	, mUp(0.0f, 1.0f, 0.0f)
	, mFOV(50.0f)
	, _dx(0.0f)
	, _dy(0.0f)
	, mYaw(0.0f)
	, mPitch(0.0f)
	, mSpeed(0.5)                         // world units / second
	, mMouseSpeed(180.0f / 1000.0f)     // degrees / pixel
	, mOrientationChanged(false)
	, _freeLook(false)
	, mPlayer(player)
	,mDistance(10.0f)
{
}

void Camera::lookAt(const glm::vec3& target)
{
    glm::vec3 toTarget(target - mPosition);
    mYaw = -glm::degrees(std::atan2(toTarget.x, -toTarget.z));
    mPitch = glm::degrees(glm::asin(toTarget.y / glm::length(toTarget)));
    mOrientationChanged = true;
}

void Camera::yaw(float degrees)
{
	mYaw += degrees;

    // keep angle in standard range
    if (mYaw >= 180.0f)
        mYaw -= 360.0f;
    else if (mYaw < -180.0f)
        mYaw += 360.0f;
    mOrientationChanged = true;
}

void Camera::pitch(float degrees)
{
    mPitch += degrees;
    // limit pitch range
	if (mPitch >= 180.0f)
		mPitch -= 360.0f;
	else if (mPitch < -180.0f)
		mPitch += 360.0f;
    mOrientationChanged = true;
}

void Camera::update(float deltaT)
{
    const Keyboard* kb = mApp->getKeyboard();
    const Mouse* mouse = mApp->getMouse();

	bool orientationChanged = false;

	if (mPitch <= 90 && mPitch > -90)
		_inverse = false;
	else
		_inverse = true;

    // freelook with mouse
	if (_isFocused()) {

		// Had issues with actual center of screen, and this gives a cool delayed affect to the camera
		_dx += (mouse->getX() - s.SCREEN_WIDTH / 2);
		_dy += (mouse->getY() - s.SCREEN_HEIGHT / 2);

		if (_startFix) {
			if (mouse->getX() != 0.0 and mouse->getY() != 0.0) {
				_startFix = false;
			}
			_dx = 0;
			_dy = 0;
		}

		if (_dx != 0.0) {

			if (_inverse) 
				yaw(_dx * mMouseSpeed);
			else 
				yaw(-_dx * mMouseSpeed);

			_dx += -_dx / 5;
		}
		if (_dy != 0.0) {
			pitch(-_dy * mMouseSpeed);
			_dy += -_dy / 5;
		}

		float dw = mDistance - mouse->getWheelDelta();
			if (dw >= 6 && dw <= 18) {
				mDistance -= mouse->getWheelDelta();
			}
	}

    // recompute forward, right, and up vectors if needed
    if (mOrientationChanged) {
		_orientationChange();
        mOrientationChanged = false;
    }

	if (!_freeLook)
		_thirdPerson();
	else
		_localMove(deltaT);
}

void Camera::toggleFreelook() {
	_freeLook = !_freeLook;
}

void Camera::_thirdPerson() {
	mPosition = mPlayer->getAim();
	_localMoveTo(glm::vec3(0.0f, 0.0f, -mDistance));
}

void Camera::_localMove(float deltaT) {
	const Keyboard* kb = mApp->getKeyboard();
	
	// move vector determined from key states
	glm::vec3 localMoveVec(0.0f, 0.0f, 0.0f);

	// move forward and back
	if (kb->isKeyDown(KC_W))
		localMoveVec.z += 1;
	if (kb->isKeyDown(KC_S))
		localMoveVec.z -= 1;

	// move left and right
	if (kb->isKeyDown(KC_A))
		localMoveVec.x -= 1;
	if (kb->isKeyDown(KC_D))
		localMoveVec.x += 1;

	// move up and down
	if (kb->isKeyDown(KC_E))
		localMoveVec.y += 1;
	if (kb->isKeyDown(KC_C))
		localMoveVec.y -= 1;


	float localMoveLen = glm::length(localMoveVec);

	if (localMoveLen > 0) {

		// determine motion speed
		float speed = mSpeed;
		if (kb->isKeyDown(KC_SHIFT))
			speed *= 5;

		// normalize and scale the local move vector
		localMoveVec *= speed * deltaT / localMoveLen;

		// apply translation in world space (relative to camera orientation)
		mPosition += localMoveVec.z * mForward + localMoveVec.x * mRight + localMoveVec.y * mUp;
	}
}

void Camera::_localMoveTo(glm::vec3 movement) {

	glm::vec3 localMoveVec = movement;

	float localMoveLen = glm::length(localMoveVec);

	if (localMoveLen > 0) {

		// apply translation in world space (relative to camera orientation)
		mPosition += localMoveVec.z * mForward + localMoveVec.x * mRight + localMoveVec.y * mUp;
	}
}

void Camera::_orientationChange() {
	// sin and cos functions eat radians, not degrees
	float radYaw = (3.14159265f / 180.0f) * mYaw;
	float radPitch = (3.14159265f / 180.0f) * mPitch;

	// some angle measures needed to compute the orientation vectors
	float sinYaw = std::sin(radYaw);
	float cosYaw = std::cos(radYaw);
	float sinPitch = std::sin(radPitch);
	float cosPitch = std::cos(radPitch);

	mForward.x = -sinYaw * cosPitch;
	mForward.y = sinPitch;
	mForward.z = -cosYaw * cosPitch;
	mForward = glm::normalize(mForward);

	mRight.x = cosYaw;
	mRight.y = 0;
	mRight.z = -sinYaw;
	mRight = glm::normalize(mRight);

	// the Up vector is the cross product of the Right and Forward vectors
	mUp = glm::cross(mRight, mForward);
	mUp = glm::normalize(mUp);
}

bool Camera::_isFocused() {

	if (GetActiveWindow()) {
		return true;
	}
	else {
		return false;
	}
}
