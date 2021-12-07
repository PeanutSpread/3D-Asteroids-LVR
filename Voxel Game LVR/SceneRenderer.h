#ifndef SCENE_RENDERER_APP_H_
#define SCENE_RENDERER_APP_H_

#include "Shaders.h"
#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "GameEndMenu.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "Settings.h"
#include "LightingModel.h"
#include "ProgramState.h"

#include <ctime>
#include <vector>
#include <math.h>

class SceneRenderer : public GLApp {

	LightingModel               mLightingModel;

	// shaders used to render entities (one program per lighting model)
	std::vector<ShaderProgram*> mPrograms;

	// graphics resources
	std::vector<Texture*>       mTextures;
	std::vector<Mesh*>          mMeshes;
	std::vector<Material*>      mMaterials;

	// scene objects
	std::vector<Entity*>        mEntities;
	std::vector<Entity*>		_toBeDrawn;
	std::vector<Projectile*>	_projectiles;
	std::vector<Asteroid*>		_asteroids;
	std::vector<Entity*>		_boundries;
	std::vector<int>			_projectileIndexBin;

	Player*						_player;
	Camera*                     mCamera;

	glm::mat4                   mProjMatrix;

	clock_t						_respawnTimer;
	clock_t						_projectileTimer;

	ProgramState				_state;

	bool                        mVisualizePointLights;
	bool						_pause = false;
	bool						_controls = false;
	bool						_startFix = true;
	bool						_dieing = false;
	bool						_projectileReady = true;
	bool						_spawnSafety = false;
	bool						_gameOver = false;
	int							_lives = 5;
	unsigned int				_score = 0;

	void						_addEntities(std::vector<Entity*> entities);
	void						_drawEntities(std::vector<Entity*> entities);
	void						_cleanUpProjectiles();
	void						_playerDeath(float dt);
	void						_projectileCheck(int index);
	void						_asteroidCheck(int index);
	void						_destroyAsteroid(Projectile* projectile);
	void						_createAsteroids();
	void						_addScore(int stage);
	bool						_timerCheck(clock_t timer, float elapsedTime);
	bool						_timerIntervalCheck(clock_t timer, int interval);

	std::vector<Entity*>		_flattenProjectiles();
	std::vector<Entity*>		_flattenAsteroids();
	std::vector<Entity*>		_getDangersTo(glm::vec3 point, std::vector<Asteroid*> entities);

	void						_renderScore(glm::vec3 position, glm::quat orientation, int scale, std::vector<Entity*> &entities);
	void						_showControls(glm::vec3 position, glm::quat orientation, int scale, std::vector<Entity*> &entities);
	void						_drawHUD(ShaderProgram* prog, glm::mat4 viewMatrix);
	void						_drawMenu(ShaderProgram* prog, glm::mat4 viewMatrix);
	void						_drawEnd(ShaderProgram* prog, glm::mat4 viewMatrix);
	Entity*						_menuAligner;
	PauseMenu*					_pauseMenu;
	MainMenu*					_mainMenu;
	GameEndMenu*				_endMenu;

	// State Management
	bool						_menuUpdate(const Keyboard* kb, const Mouse* mouse, float dt);
	void						_menuDraw(ShaderProgram* prog, glm::mat4 viewMatrix);
	bool						_gameUpdate(const Keyboard* kb, const Mouse* mouse, float dt);
	void						_gameDraw(ShaderProgram* prog, glm::mat4 viewMatrix);
	bool						_endUpdate(const Keyboard* kb, const Mouse* mouse, float dt);
	void						_endDraw(ShaderProgram* prog, glm::mat4 viewMatrix);
	void						_switchToGame();
	void						_switchToMenu();
	void						_switchToEnd();

	Settings					s;

	// debug visualization
	bool						_visualHitboxes = false;

	// shader used to render active entity axes
	ShaderProgram*              mDbgProgram;

	// geometry of axes
	Mesh*                       mAxes;

	// Rendering
	void						_render(ShaderProgram* prog, glm::mat4 viewMatrix, std::vector<Entity*> entitie);

public:
	SceneRenderer();

	void                initialize();
	void                shutdown();
	void                resize(int width, int height);
	void                draw();
	bool                update(float dt);
	bool				isFocused();
};

#endif
