#include "GameState.h"
#include "Image.h"
#include "Prefabs.h"
#include "CollisionType.h"
#include "LightingModel.h"

#include <time.h>
#include <iostream>

GameState::GameState()
	: mLightingModel(BLINN_PHONG_PER_FRAGMENT_MULTI_LIGHT)
	, mCamera(NULL)
	, mProjMatrix(1.0f)
	, mDbgProgram(NULL)
	, mAxes(NULL)
	, mVisualizePointLights(true)
	, _player(NULL)
	, _toBeDrawn()
{
}

void GameState::initialize()
{
    // print usage instructions
    std::cout << "Usage:" << std::endl;
    std::cout << "  Camera control:           WASD + Mouse" << std::endl;
    std::cout << "  Rotate active entity:     Arrow keys" << std::endl;
    std::cout << "  Reset entity orientation: R" << std::endl;
    std::cout << "  Translate active entity:  IJKL (world space)" << std::endl;
    std::cout << "  Translate active entity:  TFGH (local space)" << std::endl;
    std::cout << "  Cycle active entity:      X/Z" << std::endl;
    std::cout << "  Toggle point light vis.:  Tab" << std::endl;

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enable blending (needed for textures with alpha channel)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mPrograms.resize(NUM_LIGHTING_MODELS);

	mPrograms[PER_VERTEX_DIR_LIGHT] = new ShaderProgram("shaders/PerVertexDirLight-vs.glsl",
														"shaders/PerVertexDirLight-fs.glsl");
    
    mPrograms[BLINN_PHONG_PER_FRAGMENT_DIR_LIGHT] = new ShaderProgram("shaders/BlinnPhongPerFragment-vs.glsl",
                                                                      "shaders/BlinnPhongPerFragmentDirLight-fs.glsl");

    mPrograms[BLINN_PHONG_PER_FRAGMENT_POINT_LIGHT] = new ShaderProgram("shaders/BlinnPhongPerFragment-vs.glsl",
                                                                        "shaders/BlinnPhongPerFragmentPointLight-fs.glsl");

    mPrograms[BLINN_PHONG_PER_FRAGMENT_MULTI_LIGHT] = new ShaderProgram("shaders/BlinnPhongPerFragment-vs.glsl",
                                                                        "shaders/BlinnPhongPerFragmentMultiLight-fs.glsl");

    //
    // Create meshes
    //

    mMeshes.push_back(CreateTexturedCube(1));
    mMeshes.push_back(CreateChunkyTexturedCylinder(0.5f, 1, 8));
    mMeshes.push_back(CreateSmoothTexturedCylinder(0.5f, 1, 15));

    float roomWidth = s.ROOM_SIZE;
    float roomHeight = s.ROOM_SIZE;
    float roomDepth = s.ROOM_SIZE;
    float roomTilesPerUnit = 0.05f;

    // front and back walls
    Mesh* fbMesh = CreateTexturedQuad(roomWidth, roomHeight, roomWidth * roomTilesPerUnit, roomHeight * roomTilesPerUnit);
    mMeshes.push_back(fbMesh);
    // left and right walls
    Mesh* lrMesh = CreateTexturedQuad(roomDepth, roomHeight, roomDepth * roomTilesPerUnit, roomHeight * roomTilesPerUnit);
    mMeshes.push_back(lrMesh);
    // ceiling and floor
    Mesh* cfMesh = CreateTexturedQuad(roomWidth, roomDepth, roomWidth * roomTilesPerUnit, roomDepth * roomTilesPerUnit);
    mMeshes.push_back(cfMesh);

    //
    // Load textures
    //

    std::vector<std::string> texNames;
    texNames.push_back("textures/white.tga");
    texNames.push_back("textures/black.tga");
	texNames.push_back("textures/space.tga");

    for (unsigned i = 0; i < texNames.size(); i++)
        mTextures.push_back(new Texture(texNames[i], GL_REPEAT, GL_LINEAR));

    //
    // Create materials
    //

    // add a material for each loaded texture (with default tint)
    for (unsigned i = 0; i < texNames.size(); i++)
        mMaterials.push_back(new Material(mTextures[i]));

    //
    // set extra material properties
    //

    // white
    mMaterials[0]->specular = glm::vec3(0.75f, 0.75f, 0.75f);
    mMaterials[0]->shininess = 64;

    // black
    mMaterials[1]->specular = glm::vec3(1.0f, 0.5f, 0.0f);  // orange hightlights
    mMaterials[1]->shininess = 16;

	// space
	mMaterials[2]->specular = glm::vec3(0.1f, 0.1f, 0.1f);
	mMaterials[2]->shininess = 100;
	mMaterials[2]->emissive = glm::vec3(0.025f, 0.025f, 0.025f);

	//
	// Setup Random
	//
	srand(time(NULL));

    //
    // Create permanent entities
    //

	// create Player
	_player = new Player(glm::vec3(0, 0, 0));

    //
    // Create room
    //

    // back wall
    _boundries.push_back(new Entity(fbMesh, mMaterials[2], Transform(0, 0, -0.5f * roomDepth)));
    // front wall
    _boundries.push_back(new Entity(fbMesh, mMaterials[2], Transform(0, 0, 0.5f * roomDepth, glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)))));
    // left wall
    _boundries.push_back(new Entity(lrMesh, mMaterials[2], Transform(-0.5f * roomWidth, 0, 0, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)))));
    // right wall
    _boundries.push_back(new Entity(lrMesh, mMaterials[2], Transform(0.5f * roomWidth, 0, 0, glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)))));
    // floor
    _boundries.push_back(new Entity(cfMesh, mMaterials[2], Transform(0, -0.5f * roomHeight, 0, glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))));
    // ceiling
    _boundries.push_back(new Entity(cfMesh, mMaterials[2], Transform(0, 0.5f * roomHeight, 0, glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))));

	_addEntities(_boundries);

    //
    // create the camera
    //

    mCamera = new Camera(this, _player);
    mCamera->setSpeed(2);

	glutWarpPointer(s.SCREEN_WIDTH() / 2, s.SCREEN_HEIGHT() / 2);

	// 2D Elements
	_menuAligner = new Entity(NULL, NULL, Transform(0, 0, 0));
	_pauseMenu = new PauseMenu(_menuAligner, _menuAligner->getOrientation(), 0);

    // create shader program for debug geometry
    mDbgProgram = new ShaderProgram("shaders/vpc-vs.glsl",
                                    "shaders/vcolor-fs.glsl");

    // create geometry for axes
    mAxes = CreateAxes(2);

    CHECK_GL_ERRORS("initialization");
}

void GameState::shutdown()
{
    for (unsigned i = 0; i < mPrograms.size(); i++)
        delete mPrograms[i];
    mPrograms.clear();

    delete mDbgProgram;
    mDbgProgram = NULL;

    delete mCamera;
    mCamera = NULL;

    for (unsigned i = 0; i < mEntities.size(); i++)
        delete mEntities[i];
    mEntities.clear();

    for (unsigned i = 0; i < mMeshes.size(); i++)
        delete mMeshes[i];
    mMeshes.clear();

    for (unsigned i = 0; i < mMaterials.size(); i++)
        delete mMaterials[i];
    mMaterials.clear();
    
    for (unsigned i = 0; i < mTextures.size(); i++)
        delete mTextures[i];
    mTextures.clear();

    delete mDbgProgram;
    mDbgProgram = NULL;
    
    delete mAxes;
    mAxes = NULL;
}

void GameState::resize(int width, int height)
{
    glViewport(0, 0, width, height);

    // compute new projection matrix
    mProjMatrix = glm::perspective(glm::radians(50.f), width / (float)height, 0.1f, 1000.0f);
}

void GameState::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate current program
    ShaderProgram* prog = mPrograms[mLightingModel];
    prog->activate();

    // send projection matrix
    prog->sendUniform("u_ProjectionMatrix", mProjMatrix);

    // send the texture sampler id to shader
    prog->sendUniformInt("u_TexSampler", 0);

    // get the view matrix from the camera
    glm::mat4 viewMatrix = mCamera->getViewMatrix();

	_toBeDrawn.clear();
	_drawEntities(mEntities);
	_drawEntities(_flattenAsteroids());
	if (!_pause) {
		_drawEntities(_flattenProjectiles());
		_drawEntities(_player->getEntities());
		if (_visualHitboxes)
			_drawEntities(_player->getHitboxes());
	}

	// Lighting

	prog->sendUniform("u_AmbientLightColor", glm::vec3(0.1f, 0.1f, 0.1f));
	prog->sendUniformInt("u_NumPointLights", _player->getLights().size());
	prog->sendUniformInt("u_NumDirLights", 1);
	glm::vec3 lightColor = glm::vec3(1.0f, 0.9f, 0.8f);
	glm::vec3 engineColor = glm::vec3(0.6f, 0.6f, 1.0f);
	glm::vec3 spawnColor = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 dieingColor = glm::vec3(1.0f, 0.0f, 0.0f);

	// direction light
	if (!_pause) {
		glm::vec4 lightDir = glm::normalize(glm::vec4(_player->getAim().x, _player->getAim().y, _player->getAim().z, 0));
		prog->sendUniform("u_DirLights[0].dir", glm::vec3(viewMatrix * lightDir));
		prog->sendUniform("u_DirLights[0].color", glm::vec3(0.0f, 0.0f, 0.05f));
	} else {
		glm::vec4 lightDir = glm::normalize(glm::vec4(mCamera->getPosition(), 0));
		prog->sendUniform("u_DirLights[0].dir", glm::vec3(viewMatrix * lightDir));
		prog->sendUniform("u_DirLights[0].color", glm::vec3(0.5f, 0.5f, 0.5f));
	}
	static bool cond;
	bool flip = _timerIntervalCheck(_respawnTimer, s.LIGHT_FLASH_INTERVAL);
	if (_pause)
		flip = cond;
	cond = flip;

	std::string text;
	for (int i = 0; i < _player->getLights().size(); i++) {
		// point light position
		glm::vec3 lightPos = _player->getLights()[i]->getPosition();

		text = "u_PointLights[" + std::to_string(i) + "]";

		// send light position in eye space
		prog->sendUniform(text + ".pos", glm::vec3(viewMatrix * glm::vec4(lightPos, 1)));

		// send light color/intensity
		if (_dieing  && flip) {
			prog->sendUniform(text + ".color", dieingColor);
			prog->sendUniform(text + ".attQuat", 0.75f);
			prog->sendUniform(text + ".attLin", 0.75f);
			prog->sendUniform(text + ".attConst", 0.75f);

		} else if (_spawnSafety && flip) {
			prog->sendUniform(text + ".color", spawnColor);
			prog->sendUniform(text + ".attQuat", 0.75f);
			prog->sendUniform(text + ".attLin", 0.75f);
			prog->sendUniform(text + ".attConst", 0.75f);

		} else if (_player->getThrottle() && i > _player->getLights().size() - 3) {
			prog->sendUniform(text + ".color", engineColor);
			prog->sendUniform(text + ".attQuat", 0.725f);
			prog->sendUniform(text + ".attLin", 0.25f);
			prog->sendUniform(text + ".attConst", 0.025f);
		} else if (i > 5) {
			prog->sendUniform(text + ".color", lightColor);
			prog->sendUniform(text + ".attQuat", 1.0f);
			prog->sendUniform(text + ".attLin", 1.0f);
			prog->sendUniform(text + ".attConst", 1.0f);
		} else {
			prog->sendUniform(text + ".color", lightColor);
			prog->sendUniform(text + ".attQuat", 1.0f);
			prog->sendUniform(text + ".attLin", 0.35f);
			prog->sendUniform(text + ".attConst", 0.025f);
		}

	}

    // render all entities
	_render(prog, viewMatrix, _toBeDrawn);

	_drawHUD(prog, viewMatrix);

    mDbgProgram->activate();
    mDbgProgram->sendUniform("u_ProjectionMatrix", mProjMatrix);

    mDbgProgram->sendUniform("u_ModelviewMatrix", viewMatrix * _player->getEntities()[0]->getWorldMatrix());

	if (_visualHitboxes) {
		mAxes->activate();
		mAxes->draw();
	}

    CHECK_GL_ERRORS("drawing");
}

void GameState::_render(ShaderProgram* prog, glm::mat4 viewMatrix, std::vector<Entity*> entities) {
	for (unsigned i = 0; i < entities.size(); i++) {

		Entity* ent = entities[i];
		// use the entity's material
		const Material* mat = ent->getMaterial();
		glBindTexture(GL_TEXTURE_2D, mat->tex->id());   // bind texture
		prog->sendUniform("u_Tint", mat->tint);     // send tint color

		// send the Blinn-Phong parameters, if required
		if (mLightingModel > PER_VERTEX_DIR_LIGHT) {
			prog->sendUniform("u_MatEmissiveColor", mat->emissive);
			prog->sendUniform("u_MatSpecularColor", mat->specular);
			prog->sendUniform("u_MatShininess", mat->shininess);
		}

		// compute modelview matrix
		glm::mat4 modelview = viewMatrix * ent->getWorldMatrix();

		// send the entity's modelview and normal matrix
		prog->sendUniform("u_ModelviewMatrix", modelview);
		prog->sendUniform("u_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(modelview))));

		// use the entity's mesh
		const Mesh* mesh = ent->getMesh();
		mesh->activate();
		mesh->draw();
	}
}

bool GameState::update(float dt) // GAME LOOP
{
	const Keyboard* kb = getKeyboard();
	const Mouse* mouse = getMouse();


	// Pause Toggle
	if (kb->keyPressed(KC_ESCAPE)) {
		_pause = !_pause;
		glutWarpPointer(s.SCREEN_WIDTH() / 2, s.SCREEN_HEIGHT() / 2);
	}

	if (kb->keyPressed(KC_SPACE) && _pause)
		return false;

	if (!_pause) {
		if (_dieing) {
			_player->death(dt);
			if (_timerCheck(_respawnTimer, s.RESPAWN_INTERVAL)) {
				_spawnSafety = true;
				_dieing = false;
				_respawnTimer = clock();
			}
		}

		if (_spawnSafety)
			if (_timerCheck(_respawnTimer, s.RESPAWN_INTERVAL))
				_spawnSafety = false;

		if (!_gameOver) {
			if (!mCamera->getFreeLook() && !_dieing) {
				_player->headLook(mCamera->getYaw(), mCamera->getPitch(), dt);
				_player->bodyMove(kb, dt);
				if (!_spawnSafety) {
					if (_player->hasCollision(_getDangersTo(_player->getPosition(), _asteroids), AABB_AABB)) {
						_playerDeath(dt);
					}
				}
				if (!_projectileReady)
					if (_timerCheck(_projectileTimer, s.PROJECTILE_INTERVAL))
						_projectileReady = true;

				if (mouse->buttonPressed(MOUSE_BUTTON_LEFT) && _projectileReady) {
					_projectiles.push_back(_player->shoot()[0]);
					_projectileReady = false;
					_projectileTimer = clock();
				}
			}
		}

		if (_projectiles.size() > 0) {
			for (int i = 0; i < _projectiles.size(); i++) {
				_projectiles[i]->update(dt);
				_projectileCheck(i);
			}
		}

		if (_asteroids.size() == 0) {
			_createAsteroids();
		}

		if (_asteroids.size() > 0) {
			for (int i = 0; i < _asteroids.size(); i++) {
				_asteroids[i]->update(dt);
				_asteroidCheck(i);
			}
		}

		_cleanUpProjectiles();


		// Commiting Suicide
		if (kb->keyPressed(KC_RETURN))
			_playerDeath(dt);

		// Show Hitboxes
		if (kb->keyPressed(KC_TILDE))
			_visualHitboxes = !_visualHitboxes;

		// update the camera
		if (kb->keyPressed(KC_P)) {
			// Freelook (i.e. not attatched to ship) Will be removed or hidden
			mCamera->toggleFreelook();
		}

	} else {
		mCamera->setPosition(0, -s.ROOM_SIZE/2, 0);
		mCamera->lookAt(0, 0, 0);

		_pauseMenu->interaction(mouse);

		// Menu Operations
		if (_pauseMenu->getExitButton())
			return false;
	}

	if (isFocused()) {
		// Stop mouse tracking when out of window
		if (!_pause)
			glutWarpPointer(s.SCREEN_WIDTH() / 2, s.SCREEN_HEIGHT() / 2);
		glutSetCursor(GLUT_CURSOR_NONE);
		mCamera->update(dt);
	} else {
		// Display mouse cursor over screen when not focused
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}

    return true;
}

bool GameState::isFocused() {
	if (GetActiveWindow()) {
		return true;
	} else {
		return false;
	}
}

void GameState::_addEntities(std::vector<Entity*> entities) {
	for (int i = 0; i < entities.size(); i++) {
		mEntities.push_back(entities[i]);
    }

}

void GameState::_drawEntities(std::vector<Entity*> entities) {
	for (int i = 0; i < entities.size(); i++) {
		_toBeDrawn.push_back(entities[i]);
	}
}

void GameState::_cleanUpProjectiles() {
	int index = 0;
	for (int i = _projectileIndexBin.size() - 1; i >= 0; --i) {
		index = _projectileIndexBin[i];
		_projectiles[index]->destroy();
		delete _projectiles[index];
		_projectiles.erase(_projectiles.begin() + index);
	}
	_projectileIndexBin.clear();
}

void GameState::_playerDeath(float dt) {
	//TODO: Special animation or something
	// Delete Player
	if (_lives > 0) {
		--_lives;
		_dieing = true;
		_respawnTimer = clock();
	}
	
	_player->death(dt);
}

void GameState::_projectileCheck(int index) {
	// Check projectiles for collisions

	glm::vec3 position(_projectiles[index]->getPosition());
	std::vector<Entity*> dangers = _getDangersTo(_projectiles[index]->getPosition(), _asteroids);
	if (_projectiles[index]->hasCollision(dangers, AABB_Sphere)) {
		_destroyAsteroid(_projectiles[index]);
		_projectileIndexBin.push_back(index);
	}

	int boundry = (s.ROOM_SIZE / 2) + s.BUFFER*2;
	if (position.x > boundry || position.y > boundry || position.z > boundry || position.x < -boundry || position.y < -boundry || position.z < -boundry)
		_projectileIndexBin.push_back(index);
}

void GameState::_asteroidCheck(int index) {
	// Repositioning asteroids so that they stay inside boundries
	glm::vec3 position(_asteroids[index]->getPosition());
	int boundry = (s.ROOM_SIZE / 2) + s.BUFFER * 2;
	bool hasChanged = true;

	if (position.x > boundry)
		_asteroids[index]->setPosition(glm::vec3(-boundry, position.y, position.z));
	else if (position.y > boundry)
		_asteroids[index]->setPosition(glm::vec3(position.x, -boundry, position.z));
	else if (position.z > boundry)
		_asteroids[index]->setPosition(glm::vec3(position.x, position.y, -boundry));
	else if (position.x < -boundry)
		_asteroids[index]->setPosition(glm::vec3(boundry, position.y, position.z));
	else if (position.y < -boundry)
		_asteroids[index]->setPosition(glm::vec3(position.x, boundry, position.z));
	else if (position.z < -boundry)
		_asteroids[index]->setPosition(glm::vec3(position.x, position.y, boundry));
	else
		hasChanged = false;

	if (hasChanged) {
		_asteroids[index]->adjustSpeed(0.5);
	}
}

void GameState::_destroyAsteroid(Projectile* projectile) {
	
	int asteroidHolder = 0;
	for (int i = 0; i < _asteroids.size(); i++) {
		if (projectile->hasCollision(_asteroids[i]->getHitboxes(), AABB_Sphere)) {
			asteroidHolder = i;
		}
	}
	std::vector<Asteroid*> splits = _asteroids[asteroidHolder]->explode();
	for (int i = 0; i < splits.size(); ++i)
		_asteroids.push_back(splits[i]);
	delete _asteroids[asteroidHolder];
	_asteroids.erase(_asteroids.begin() + asteroidHolder);
}

void GameState::_createAsteroids() {
	int totalAsteroids = rand() % s.VARIANCE_ASTEROIDS + s.MIN_ASTEROIDS;
	int size = 0;
	glm::vec3 location, velocity;
	for (int i = 0; i < totalAsteroids; ++i) {
		location = glm::vec3(rand() % (int)(s.ROOM_SIZE + 1) - s.ROOM_SIZE/2, rand() % (int)(s.ROOM_SIZE + 1) - s.ROOM_SIZE / 2, rand() % (int)(s.ROOM_SIZE + 1) - s.ROOM_SIZE/2);
		velocity = glm::vec3(rand() % s.VARIANCE_XYZ + s.MIN_XYZ, rand() % s.VARIANCE_ASTEROIDS + s.MIN_XYZ, rand() % s.VARIANCE_XYZ + s.MIN_XYZ);
		size = rand() % s.ASTEROID_SCALES + 1;

		// Remove asteroids that are too close too the player
		if (sqrt(pow(_player->getPosition().x - location.x, 2)) > s.SAFE_DISTANCE)
			if (sqrt(pow(_player->getPosition().y - location.y, 2)) > s.SAFE_DISTANCE)
				if (sqrt(pow(_player->getPosition().z - location.z, 2)) > s.SAFE_DISTANCE)
					_asteroids.push_back(new Asteroid(location, velocity, size));
	}
}

bool GameState::_timerCheck(clock_t timer, float elapsedTime) {
	timer = clock() - timer;
	if ((float)timer / CLOCKS_PER_SEC >= elapsedTime)
		return true;
	return false;
}

bool GameState::_timerIntervalCheck(clock_t timer, int interval) {
	bool toggle = false;
	timer = clock() - timer;
	if ((timer / CLOCKS_PER_SEC) % interval == 0)
		toggle = !toggle;
	return toggle;
}

std::vector<Entity*> GameState::_flattenProjectiles()
{
	std::vector<Entity*> entities;
	for (int i = 0; i < _projectiles.size(); ++i) {
		for (int j = 0; j < _projectiles[i]->getEntities().size(); ++j) {
			entities.push_back(_projectiles[i]->getEntities()[j]);
			if (_visualHitboxes)
				for (int j = 0; j < _projectiles[i]->getHitboxes().size(); ++j) {
					entities.push_back(_projectiles[i]->getHitboxes()[j]);
				}
		}

	}
	return entities;
}

std::vector<Entity*> GameState::_flattenAsteroids()
{
	std::vector<Entity*> entities;
	for (int i = 0; i < _asteroids.size(); ++i) {
		for (int j = 0; j < _asteroids[i]->getEntities().size(); ++j) {
			entities.push_back(_asteroids[i]->getEntities()[j]);
		}
		if (_visualHitboxes)
			for (int j = 0; j < _asteroids[i]->getHitboxes().size(); ++j) {
				entities.push_back(_asteroids[i]->getHitboxes()[j]);
			}

	}
	return entities;
}

std::vector<Entity*> GameState::_getDangersTo(glm::vec3 point, std::vector<Asteroid*> entities) {
	std::vector<Entity*> dangers;
	glm::vec3 distance;
	for (int i = 0; i < entities.size(); ++i) {
		distance.x = pow(point.x - entities[i]->getPosition().x, 2);
		distance.y = pow(point.y - entities[i]->getPosition().y, 2);
		distance.z = pow(point.z - entities[i]->getPosition().z, 2);

		if (distance.length() < 50) {
			for (int j = 0; j < entities[i]->getHitboxes().size(); ++j) {
				dangers.push_back(entities[i]->getHitboxes()[j]);
			}
		}
	}
	return dangers;
}

void GameState::_drawHUD(ShaderProgram* prog, glm::mat4 viewMatrix) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_DST_ALPHA, GL_ONE);

	glm::vec3 vP = mCamera->getPosition();
	_menuAligner->setPosition(vP);
	float radYaw = (3.14159265f / 180.0f) * mCamera->getYaw();
	float radPitch = (3.14159265f / 180.0f) * mCamera->getPitch();
	_menuAligner->setOrientation(glm::quat(glm::vec3(radPitch - 1.5f, radYaw, 0.f)));
	_menuAligner->setPosition(_player->getEntities()[0]->getPosition());
	_menuAligner->translateLocal(0, 0, 3);
	vP = _menuAligner->getPosition();
	float scale = mCamera->getZoom() + (_player->getAcceleration() / 20);
	glm::quat viewOrientation(glm::vec3(radPitch, radYaw, 0.f));
	_pauseMenu->update(viewOrientation, scale);

	std::vector<Entity*> entities;
	if (!_pause) {

		std::vector<Texture*> textures;
		textures.push_back(new Texture("textures/crosshair.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
		textures.push_back(new Texture("textures/life.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));

		std::vector<Material*> materials;
		materials.push_back(new Material(textures[0]));
		materials.push_back(new Material(textures[1]));

		std::vector<Mesh*> meshes;
		meshes.push_back(CreateTexturedQuad(0.05f * scale, 0.025f * scale, 1.f, 1.f)); // Crosshair
		meshes.push_back(CreateTexturedQuad(0.05f * scale, 0.1f * scale, 1.f, 1.f)); // Life


		if (!_dieing) {
			_menuAligner->translateLocal(-0.035 * scale, 0, 0);
			entities.push_back(new Entity(meshes[0], materials[0], Transform(_menuAligner->getPosition())));
			_menuAligner->setPosition(vP);

			_menuAligner->translateLocal(0.035 * scale, 0, 0);
			entities.push_back(new Entity(meshes[0], materials[0], Transform(_menuAligner->getPosition())));
			_menuAligner->setPosition(vP);
		}

		for (int i = 0; i < _lives; ++i) {
			_menuAligner->translateLocal((0.25 + 0.07 * i)* scale, 0, 0.38 * scale);
			entities.push_back(new Entity(meshes[1], materials[1], Transform(_menuAligner->getPosition())));
			_menuAligner->setPosition(vP);
		}

	}

	for (int i = 0; i < entities.size(); ++i) {
		entities[i]->setOrientation(viewOrientation);
		if (i == 1 && !_pause && !_dieing) {
			entities[1]->rotate(180, 0, 0, 1);
		}
	}



	if (_pause) {
		mCamera->orientationChange();
		glm::vec3 vP = mCamera->getPosition();
		_menuAligner->setPosition(vP);
		float radYaw = (3.14159265f / 180.0f) * mCamera->getYaw();
		float radPitch = (3.14159265f / 180.0f) * mCamera->getPitch();
		glm::quat viewOrientation(glm::vec3(radPitch, radYaw, 0.f));
		_menuAligner->setOrientation(viewOrientation);
		_menuAligner->translateLocal(0, 0, -10);
		vP = _menuAligner->getPosition();

		_pauseMenu->update(viewOrientation, 10);


		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		std::vector<Entity*> menuEntities = _pauseMenu->getMenu();
		for (int i = 0; i < menuEntities.size(); ++i)
			entities.push_back(menuEntities[i]);
	}

	glDepthMask(GL_FALSE);
	_render(prog, viewMatrix, entities);

	if (_pause) {
		glBlendFunc(GL_DST_ALPHA, GL_ONE);
		std::vector<Entity*> cursorAdapter;
		cursorAdapter.push_back(_pauseMenu->getCursor());
		_render(prog, viewMatrix, cursorAdapter);
	}

	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
