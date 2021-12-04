
#include "Shaders.h"
#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "PauseMenu.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "Settings.h"
#include "LightingModel.h"

#include <ctime>
#include <vector>
#include <math.h>

class MenuState : public GLApp {

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

	std::vector<Asteroid*>		_asteroids;
	std::vector<Entity*>		_boundries;

	Camera*                     mCamera;

	glm::mat4                   mProjMatrix;

	clock_t						_timer;

	bool                        mVisualizePointLights;

	void						_addEntities(std::vector<Entity*> entities);
	void						_drawEntities(std::vector<Entity*> entities);

	void						_createAsteroids();
	bool						_timerCheck(clock_t timer, float elapsedTime);
	bool						_timerIntervalCheck(clock_t timer, int interval);

	std::vector<Entity*>		_flattenAsteroids();

	void						_drawMenu(ShaderProgram* prog, glm::mat4 viewMatrix);
	Entity*						_menuAligner;
	PauseMenu*					_pauseMenu;


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
	MenuState();

	void                initialize();
	void                shutdown();
	void                resize(int width, int height);
	void                draw();
	bool                update(float dt);
	bool				isFocused();
};