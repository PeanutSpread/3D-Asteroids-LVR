#ifndef CAMERA_H_
#define CAMERA_H_

#include "glshell.h"
#include "Settings.h"
#include "Player.h"

class Camera {
    GLApp* mApp;

    glm::vec3 mPosition;
    glm::vec3 mForward, mRight, mUp;   // orientation vectors

	Player*	mPlayer;

    float mFOV;  // field of view

	float dx, dy;
    float mYaw;
    float mPitch;
	float mDistance;

    float mSpeed;
    float mMouseSpeed;

    bool mOrientationChanged;
	void orientationChange();

	bool isFocused();
	bool freeLook;

	void thirdPerson();
	void localMove(float deltaT);
	void localMoveTo(glm::vec3 movement);

	Settings s;
	bool startFix = true;

public:
    Camera(GLApp* app, Player* player);

    const glm::vec3& getPosition() const
    { return mPosition; }

    void setPosition(const glm::vec3& position)
    { mPosition = position; }

    void setPosition(float x, float y, float z)
    { mPosition = glm::vec3(x, y, z); }

	void lookAt(const glm::vec3& target);

    void lookAt(float x, float y, float z)
    { lookAt(glm::vec3(x, y, z)); }

    float getSpeed() const
    { return mSpeed; }

    void setSpeed(float speed)
    { mSpeed = speed; }

    const glm::vec3& getForward() const
    { return mForward; }

    const glm::vec3& getRight() const
    { return mRight; }

    const glm::vec3& getUp() const
    { return mUp; }

    glm::mat4 getViewMatrix() const
    { return glm::lookAt(mPosition, mPosition + mForward, mUp); }

    float getFOV() const
    { return mFOV; }

    void yaw(float degrees);
    void pitch(float degrees);
    
	float getYaw() const
	{ return mYaw; }
	float getPitch() const
	{ return mPitch; }

    void update(float deltaT);

	void toggleFreelook();
	bool getFreeLook() const 
	{ return freeLook; }

};

#endif
