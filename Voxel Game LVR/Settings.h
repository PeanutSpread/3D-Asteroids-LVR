#pragma once
#include <math.h>

class Settings {
public:
	int SCREEN_WIDTH() const 
	{ return glutGet(GLUT_WINDOW_WIDTH); }

	int SCREEN_HEIGHT() const 
	{ return glutGet(GLUT_WINDOW_HEIGHT); }
	
	// Game Constants
	const float ROOM_SIZE = 550;
	const float BUFFER = 10;
	const float SAFE_DISTANCE = 25;
	const float PROJECTILE_INTERVAL = 0.25;
	const float RESPAWN_INTERVAL = 5.0;
	const float BREAK_OFF = 100.0;
	const int LIGHT_FLASH_INTERVAL = 2;
	const int MIN_ASTEROIDS = 20;
	const int VARIANCE_ASTEROIDS = 16;
	const int MIN_XYZ = -5;
	const int VARIANCE_XYZ = 11;
	const int ASTEROID_SCALES = 3;
	const int ASTEROID_MIN_ANGLE = -5;
	const int ASTEROID_ANGLE = 11;
	const int ASTEROID_SPLITS = 2;
	const int ASTEROID_SPEED_SPLIT = 2;
	const int ANGLE_MIN = -2;
	const int ANGLE = 5;
};

