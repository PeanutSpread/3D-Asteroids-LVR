#include "MenuState.h"
#include "Image.h"
#include "Prefabs.h"
#include "CollisionType.h"
#include "LightingModel.h"

#include <time.h>
#include <iostream>

MenuState::MenuState()
	: mLightingModel(BLINN_PHONG_PER_FRAGMENT_MULTI_LIGHT)
	, mCamera(NULL)
	, mProjMatrix(1.0f)
	, mDbgProgram(NULL)
	, mAxes(NULL)
	, mVisualizePointLights(true)
	, _toBeDrawn()
	, _player()
{
}

void MenuState::initialize()
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
	// Create entities
	//
	_player = new Player(glm::vec3(0, 0, 0));

	_createAsteroids();

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
	mCamera->toggleCameraMovement();

	glutWarpPointer(s.SCREEN_WIDTH() / 2, s.SCREEN_HEIGHT() / 2);

	// 2D Elements
	_menuAligner = new Entity(NULL, NULL, Transform(0, 0, 0));
	_mainMenu = new MainMenu(_menuAligner, _menuAligner->getOrientation(), 0);

	// create shader program for debug geometry
	mDbgProgram = new ShaderProgram("shaders/vpc-vs.glsl",
		"shaders/vcolor-fs.glsl");

	// create geometry for axes
	mAxes = CreateAxes(2);

	CHECK_GL_ERRORS("initialization");
}

void MenuState::shutdown()
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

void MenuState::resize(int width, int height)
{
	glViewport(0, 0, width, height);

	// compute new projection matrix
	mProjMatrix = glm::perspective(glm::radians(50.f), width / (float)height, 0.1f, 1000.0f);
}

void MenuState::draw()
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

	// Lighting

	prog->sendUniform("u_AmbientLightColor", glm::vec3(0.1f, 0.1f, 0.1f));
	prog->sendUniformInt("u_NumDirLights", 1);
	glm::vec3 lightColor = glm::vec3(1.0f, 0.9f, 0.8f);
	glm::vec3 engineColor = glm::vec3(0.6f, 0.6f, 1.0f);
	glm::vec3 spawnColor = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 dieingColor = glm::vec3(1.0f, 0.0f, 0.0f);

	// direction light
		glm::vec4 lightDir = glm::normalize(glm::vec4(mCamera->getPosition(), 0));
		prog->sendUniform("u_DirLights[0].dir", glm::vec3(viewMatrix * lightDir));
		prog->sendUniform("u_DirLights[0].color", glm::vec3(0.5f, 0.5f, 0.5f));

	// render all entities
	_render(prog, viewMatrix, _toBeDrawn);

	_drawMenu(prog, viewMatrix);

	mDbgProgram->activate();
	mDbgProgram->sendUniform("u_ProjectionMatrix", mProjMatrix);

	mDbgProgram->sendUniform("u_ModelviewMatrix", viewMatrix * _player->getEntities()[0]->getWorldMatrix());

	CHECK_GL_ERRORS("drawing");
}

void MenuState::_render(ShaderProgram* prog, glm::mat4 viewMatrix, std::vector<Entity*> entities) {
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

bool MenuState::update(float dt) // GAME LOOP
{
	const Keyboard* kb = getKeyboard();
	const Mouse* mouse = getMouse();

	if (_asteroids.size() > 0) {
		for (int i = 0; i < _asteroids.size(); i++) {
			_asteroids[i]->update(dt);
		}
	}

	// Show Hitboxes
	if (kb->keyPressed(KC_TILDE))
		_visualHitboxes = !_visualHitboxes;

	
	mCamera->setPosition(0, -s.ROOM_SIZE / 2, 0);
	mCamera->lookAt(0, 0, 0);

	_mainMenu->interaction(mouse);

	// Menu Operations
	if (_mainMenu->getExitButton())
		return false;

	if (isFocused()) {
		glutSetCursor(GLUT_CURSOR_NONE);
		mCamera->update(dt);
	}
	else {
		// Display mouse cursor over screen when not focused
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}

	return true;
}

bool MenuState::isFocused() {
	if (GetActiveWindow()) {
		return true;
	}
	else {
		return false;
	}
}

void MenuState::_addEntities(std::vector<Entity*> entities) {
	for (int i = 0; i < entities.size(); i++) {
		mEntities.push_back(entities[i]);
	}

}

void MenuState::_drawEntities(std::vector<Entity*> entities) {
	for (int i = 0; i < entities.size(); i++) {
		_toBeDrawn.push_back(entities[i]);
	}
}

void MenuState::_createAsteroids() {
	int totalAsteroids = rand() % s.VARIANCE_ASTEROIDS + s.MIN_ASTEROIDS;
	int size = 0;
	glm::vec3 location, velocity;
	for (int i = 0; i < totalAsteroids; ++i) {
		location = glm::vec3(rand() % (int)(s.ROOM_SIZE + 1) - s.ROOM_SIZE / 2, rand() % (int)(s.ROOM_SIZE + 1) - s.ROOM_SIZE / 2, rand() % (int)(s.ROOM_SIZE + 1) - s.ROOM_SIZE / 2);
		velocity = glm::vec3(rand() % s.VARIANCE_XYZ + s.MIN_XYZ, rand() % s.VARIANCE_ASTEROIDS + s.MIN_XYZ, rand() % s.VARIANCE_XYZ + s.MIN_XYZ);
		size = rand() % s.ASTEROID_SCALES + 1;
		_asteroids.push_back(new Asteroid(location, velocity, size));
	}
}

bool MenuState::_timerCheck(clock_t timer, float elapsedTime) {
	timer = clock() - timer;
	if ((float)timer / CLOCKS_PER_SEC >= elapsedTime)
		return true;
	return false;
}

bool MenuState::_timerIntervalCheck(clock_t timer, int interval) {
	bool toggle = false;
	timer = clock() - timer;
	if ((timer / CLOCKS_PER_SEC) % interval == 0)
		toggle = !toggle;
	return toggle;
}

std::vector<Entity*> MenuState::_flattenAsteroids()
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

void MenuState::_drawMenu(ShaderProgram* prog, glm::mat4 viewMatrix) {
	glClear(GL_DEPTH_BUFFER_BIT);

	mCamera->orientationChange();
	glm::vec3 vP = mCamera->getPosition();
	_menuAligner->setPosition(vP);
	float radYaw = (3.14159265f / 180.0f) * mCamera->getYaw();
	float radPitch = (3.14159265f / 180.0f) * mCamera->getPitch();
	glm::quat viewOrientation(glm::vec3(radPitch, radYaw, 0.f));
	_menuAligner->setOrientation(viewOrientation);
	_menuAligner->translateLocal(0, 0, -10);
	vP = _menuAligner->getPosition();
	float scale = mCamera->getZoom();
	_mainMenu->update(viewOrientation, 10);

	std::vector<Entity*> entities;

	std::vector<Texture*> textures;
	textures.push_back(new Texture("textures/crosshair.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));
	textures.push_back(new Texture("textures/life.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));

	std::vector<Material*> materials;
	materials.push_back(new Material(textures[0]));
	materials.push_back(new Material(textures[1]));

	std::vector<Mesh*> meshes;
	meshes.push_back(CreateTexturedQuad(0.05f * scale, 0.025f * scale, 1.f, 1.f)); // Crosshair
	meshes.push_back(CreateTexturedQuad(0.05f * scale, 0.1f * scale, 1.f, 1.f)); // Life


	_menuAligner->translateLocal(-0.035 * scale, 0, 0);
	entities.push_back(new Entity(meshes[0], materials[0], Transform(_menuAligner->getPosition())));
	_menuAligner->setPosition(vP);

	_menuAligner->translateLocal(0.035 * scale, 0, 0);
	entities.push_back(new Entity(meshes[0], materials[0], Transform(_menuAligner->getPosition())));
	_menuAligner->setPosition(vP);


	for (int i = 0; i < entities.size(); ++i) {
		entities[i]->setOrientation(viewOrientation);
	}


	std::vector<Entity*> menuEntities = _mainMenu->getMenu();
	for (int i = 0; i < menuEntities.size(); ++i)
		entities.push_back(menuEntities[i]);

	glDepthMask(GL_FALSE);
	_render(prog, viewMatrix, entities);

	glBlendFunc(GL_DST_ALPHA, GL_ONE);
	std::vector<Entity*> cursorAdapter;
	cursorAdapter.push_back(_mainMenu->getCursor());
	_render(prog, viewMatrix, cursorAdapter);

	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
