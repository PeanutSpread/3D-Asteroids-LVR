#ifndef SCENE_RENDERER_APP_H_
#define SCENE_RENDERER_APP_H_

#include "Shaders.h"
#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "Projectile.h"
#include "Settings.h"

#include <vector>
#include <math.h>

enum LightingModel {
    PER_VERTEX_DIR_LIGHT,
    BLINN_PHONG_PER_FRAGMENT_DIR_LIGHT,
    BLINN_PHONG_PER_FRAGMENT_POINT_LIGHT,
    BLINN_PHONG_PER_FRAGMENT_MULTI_LIGHT,

    NUM_LIGHTING_MODELS
};


class BasicSceneRenderer : public GLApp {

    LightingModel               mLightingModel;

    // shaders used to render entities (one program per lighting model)
    std::vector<ShaderProgram*> mPrograms;

    // graphics resources
    std::vector<Texture*>       mTextures;
    std::vector<Mesh*>          mMeshes;
    std::vector<Material*>      mMaterials;

    // scene objects
    std::vector<Entity*>        mEntities;
	std::vector<Projectile*>	projectiles;
	Player*						player;

    Camera*                     mCamera;

    glm::mat4                   mProjMatrix;

    int                         mActiveEntityIndex;

    bool                        mVisualizePointLights;

	void						addEntities(std::vector<Entity*> entities);

	void						drawHUD(float scale);

	Settings					s;
	bool						startFix = true;

    //
    // debug visualization
    //

    // shader used to render active entity axes
    ShaderProgram*              mDbgProgram;

    // geometry of axes
    Mesh*                       mAxes;

public:
                        BasicSceneRenderer();

    void                initialize();
    void                shutdown();
    void                resize(int width, int height);
    void                draw();
    bool                update(float dt);
	bool				isFocused();
};

#endif
