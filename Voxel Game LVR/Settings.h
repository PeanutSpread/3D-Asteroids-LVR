#pragma once
#include <math.h>

// Hacked way to set constants since the screensize built in constant was wrong
class Settings {
public:
	const int SCREEN_WIDTH = 800; //NOT FINAL
	const int SCREEN_HEIGHT = 600; //NOT FINAL
	
	// Game Constants
	const float ROOM_SIZE = 550;
	const float BUFFER = 10;
	const float SAFE_DISTANCE = 25;
	const int MIN_ASTEROIDS = 20;
	const int VARIANCE_ASTEROIDS = 16;
	const int MIN_XYZ = -5;
	const int VARIANCE_XYZ = 11;
	const int ASTEROID_SCALES = 3;
	const int ASTEROID_MIN_ANGLE = -5;
	const int ASTEROID_ANGLE = 11;
	const int ASTEROID_SPLITS = 2;
	const int ASTEROID_SPEED_SPLIT = 2;
};

