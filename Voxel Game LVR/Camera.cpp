#include "Camera.h"

Camera::Camera(GLApp* app)
    : mApp(app)
    , mPosition(0.0f, 0.0f, 0.0f)
    , mForward(0.0f, 0.0f, -1.0f)
    , mRight(1.0f, 0.0f, 0.0f)
    , mUp(0.0f, 1.0f, 0.0f)
    , mFOV(50.0f)
	, dx(0.0f)
	, dy(0.0f)
    , mYaw(0.0f)
    , mPitch(0.0f)
    , mSpeed(5)                         // world units / second
    , mMouseSpeed(180.0f / 1000.0f)     // degrees / pixel
    , mOrientationChanged(false)
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
    if (mPitch > 80.0f)
        mPitch = 80.0f;
    else if (mPitch < -80.0f)
        mPitch = -80.0f;
    mOrientationChanged = true;
}

void Camera::update(float deltaT)
{
    const Keyboard* kb = mApp->getKeyboard();
    const Mouse* mouse = mApp->getMouse();

    bool orientationChanged = false;

    // freelook with mouse
	dx += mouse->getDeltaX();
    dy += mouse->getDeltaY();
	if (dx != 0)
        yaw(-dx * mMouseSpeed);
	if (dy != 0)
        pitch(-dy * mMouseSpeed);

    // scroll wheel controls FOV
    int wd = mouse->getWheelDelta();
    if (wd > 0) {
        mFOV -= wd * 10.0f;
        if (mFOV < 20)
            mFOV = 20;
    } else if (wd < 0) {
        mFOV += wd * 10.0f;
        if (mFOV > 180)
            mFOV = 180;
    }

    // recompute forward, right, and up vectors if needed
    if (mOrientationChanged) {
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

        mOrientationChanged = false;
    }

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
