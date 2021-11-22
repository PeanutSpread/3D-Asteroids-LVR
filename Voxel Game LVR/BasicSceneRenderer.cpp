#include "BasicSceneRenderer.h"
#include "Image.h"
#include "Prefabs.h"
#include "Projectile.h"

#include <iostream>

BasicSceneRenderer::BasicSceneRenderer()
	: mLightingModel(PER_VERTEX_DIR_LIGHT)
	, mCamera(NULL)
	, mProjMatrix(1.0f)
	, mDbgProgram(NULL)
	, mAxes(NULL)
	, mVisualizePointLights(true)
	, player(NULL)
{
}

void BasicSceneRenderer::initialize()
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
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    float roomWidth = 500;
    float roomHeight = 500;
    float roomDepth = 500;
    float roomTilesPerUnit = 100.0f;

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
	texNames.push_back("textures/rocky.tga");

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
	mMaterials[2]->specular = glm::vec3(0.5f, 0.0f, 0.5f);  // orange hightlights
	mMaterials[2]->shininess = 32;

    //
    // Create entities
    //

	// create Player
	player = new Player(glm::vec3(0, 0, 0));
	addEntities(player->getEntities());

    //
    // Create room
    //

    // back wall
    boundries.push_back(new Entity(fbMesh, mMaterials[2], Transform(0, 0, -0.5f * roomDepth)));
    // front wall
    boundries.push_back(new Entity(fbMesh, mMaterials[2], Transform(0, 0, 0.5f * roomDepth, glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)))));
    // left wall
    boundries.push_back(new Entity(lrMesh, mMaterials[2], Transform(-0.5f * roomWidth, 0, 0, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)))));
    // right wall
    boundries.push_back(new Entity(lrMesh, mMaterials[2], Transform(0.5f * roomWidth, 0, 0, glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)))));
    // floor
    boundries.push_back(new Entity(cfMesh, mMaterials[2], Transform(0, -0.5f * roomHeight, 0, glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))));
    // ceiling
    boundries.push_back(new Entity(cfMesh, mMaterials[2], Transform(0, 0.5f * roomHeight, 0, glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))));

	addEntities(boundries);

    //
    // create the camera
    //

    mCamera = new Camera(this, player);
    mCamera->setSpeed(2);

	glutWarpPointer(s.SCREEN_WIDTH / 2, s.SCREEN_HEIGHT / 2);

    // create shader program for debug geometry
    mDbgProgram = new ShaderProgram("shaders/vpc-vs.glsl",
                                    "shaders/vcolor-fs.glsl");

    // create geometry for axes
    mAxes = CreateAxes(2);

    CHECK_GL_ERRORS("initialization");
}

void BasicSceneRenderer::shutdown()
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

void BasicSceneRenderer::resize(int width, int height)
{
    glViewport(0, 0, width, height);

    // compute new projection matrix
    mProjMatrix = glm::perspective(glm::radians(50.f), width / (float)height, 0.1f, 1000.0f);
}

void BasicSceneRenderer::draw()
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

    //
    // light setup depends on lighting model
    //

    if (mLightingModel == PER_VERTEX_DIR_LIGHT) {

        //----------------------------------------------------------------------------------//
        //                                                                                  //
        // Basic directional light (no ambient, specular, or emissive contributions)        //
        //                                                                                  //
        //----------------------------------------------------------------------------------//

        // direction to light
        glm::vec4 lightDir = glm::normalize(glm::vec4(1, 3, 2, 0));

        // send light direction in eye space
        prog->sendUniform("u_LightDir", glm::vec3(viewMatrix * lightDir));

        // send light color/intensity
        prog->sendUniform("u_LightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    } else if (mLightingModel == BLINN_PHONG_PER_FRAGMENT_DIR_LIGHT) {

        //----------------------------------------------------------------------------------//
        //                                                                                  //
        // Directional light with ambient, specular, and emissive contributions             //
        //                                                                                  //
        //----------------------------------------------------------------------------------//

        prog->sendUniform("u_AmbientLightColor", glm::vec3(0.2f, 0.2f, 0.2f));

        // direction to light
        glm::vec4 lightDir = glm::normalize(glm::vec4(1, 3, 2, 0));

        // send light direction in eye space
        prog->sendUniform("u_LightDir", glm::vec3(viewMatrix * lightDir));

        // send light color/intensity
        prog->sendUniform("u_LightColor", glm::vec3(0.8f, 0.8f, 0.8f));

    } else if (mLightingModel == BLINN_PHONG_PER_FRAGMENT_POINT_LIGHT) {

        //----------------------------------------------------------------------------------//
        //                                                                                  //
        // Point light with ambient, specular, and emissive contributions, and attenuation  //
        //                                                                                  //
        //----------------------------------------------------------------------------------//

        prog->sendUniform("u_AmbientLightColor", glm::vec3(0.1f, 0.1f, 0.1f));

        // point light position
        glm::vec3 lightPos = glm::vec3(0, 7, 0);
        glm::vec3 lightColor = glm::vec3(1.0f, 0.9f, 0.8f);

        // send light position in eye space
        prog->sendUniform("u_LightPos", glm::vec3(viewMatrix * glm::vec4(lightPos, 1)));

        // send light color/intensity
        prog->sendUniform("u_LightColor", lightColor);

        prog->sendUniform("u_AttQuat", 0.005f);
        prog->sendUniform("u_AttLin", 0.05f);
        prog->sendUniform("u_AttConst", 1.0f);

        // render the light as an emissive cube, if desired
        if (mVisualizePointLights) {
            const Mesh* lightMesh = mMeshes[0];
            lightMesh->activate();
            glBindTexture(GL_TEXTURE_2D, mTextures[1]->id());  // use black texture
            prog->sendUniform("u_MatEmissiveColor", lightColor);
            prog->sendUniform("u_ModelviewMatrix", glm::translate(viewMatrix, glm::vec3(lightPos)));
            prog->sendUniform("u_NormalMatrix", glm::mat3(1.0f));
            lightMesh->draw();
        }

    } else if (mLightingModel == BLINN_PHONG_PER_FRAGMENT_MULTI_LIGHT) {

        //----------------------------------------------------------------------------------//
        //                                                                                  //
        // Multiple directional/point lights                                                //
        //                                                                                  //
        //----------------------------------------------------------------------------------//

        prog->sendUniform("u_AmbientLightColor", glm::vec3(0.1f, 0.1f, 0.1f));

        prog->sendUniformInt("u_NumDirLights", 1);
        prog->sendUniformInt("u_NumPointLights", 3);

        // directional light
        glm::vec4 lightDir = glm::normalize(glm::vec4(1, 3, 2, 0));
        prog->sendUniform("u_DirLights[0].dir", glm::vec3(viewMatrix * lightDir));
        prog->sendUniform("u_DirLights[0].color", glm::vec3(0.3f, 0.3f, 0.3f));

        // point light
        glm::vec3 lightPos1 = glm::vec3(-7, 5, -12);
        glm::vec3 lightColor1 = glm::vec3(1.0f, 0.0f, 0.0f);
        prog->sendUniform("u_PointLights[0].pos", glm::vec3(viewMatrix * glm::vec4(lightPos1, 1)));
        prog->sendUniform("u_PointLights[0].color", lightColor1);
        prog->sendUniform("u_PointLights[0].attQuat", 0.01f);
        prog->sendUniform("u_PointLights[0].attLin", 0.1f);
        prog->sendUniform("u_PointLights[0].attConst", 1.0f);

        // point light
        glm::vec3 lightPos2 = glm::vec3(7, 5, -12);
        glm::vec3 lightColor2 = glm::vec3(0.0f, 0.0f, 1.0f);
        prog->sendUniform("u_PointLights[1].pos", glm::vec3(viewMatrix * glm::vec4(lightPos2, 1)));
        prog->sendUniform("u_PointLights[1].color", lightColor2);
        prog->sendUniform("u_PointLights[1].attQuat", 0.01f);
        prog->sendUniform("u_PointLights[1].attLin", 0.1f);
        prog->sendUniform("u_PointLights[1].attConst", 1.0f);

        // point light
        glm::vec3 lightPos3 = glm::vec3(-7, -5, 15);
        glm::vec3 lightColor3 = glm::vec3(0.0f, 1.0f, 0.0f);
        prog->sendUniform("u_PointLights[2].pos", glm::vec3(viewMatrix * glm::vec4(lightPos3, 1)));
        prog->sendUniform("u_PointLights[2].color", lightColor3);
        prog->sendUniform("u_PointLights[2].attQuat", 0.01f);
        prog->sendUniform("u_PointLights[2].attLin", 0.1f);
        prog->sendUniform("u_PointLights[2].attConst", 1.0f);

        // render the point lights as emissive cubes, if desirable
        if (mVisualizePointLights) {
            glBindTexture(GL_TEXTURE_2D, mTextures[1]->id());  // use black texture
            prog->sendUniform("u_NormalMatrix", glm::mat3(1.0f));
            const Mesh* lightMesh = mMeshes[0];
            lightMesh->activate();
            prog->sendUniform("u_MatEmissiveColor", lightColor1);
            prog->sendUniform("u_ModelviewMatrix", glm::translate(viewMatrix, glm::vec3(lightPos1)));
            lightMesh->draw();
            prog->sendUniform("u_MatEmissiveColor", lightColor2);
            prog->sendUniform("u_ModelviewMatrix", glm::translate(viewMatrix, glm::vec3(lightPos2)));
            lightMesh->draw();
            prog->sendUniform("u_MatEmissiveColor", lightColor3);
            prog->sendUniform("u_ModelviewMatrix", glm::translate(viewMatrix, glm::vec3(lightPos3)));
            lightMesh->draw();
        }
    }

    // render all entities
    for (unsigned i = 0; i < mEntities.size(); i++) {

        Entity* ent = mEntities[i];

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
    mDbgProgram->activate();
    mDbgProgram->sendUniform("u_ProjectionMatrix", mProjMatrix);

    //Entity* activeEntity = mEntities[mActiveEntityIndex];
    mDbgProgram->sendUniform("u_ModelviewMatrix", viewMatrix * player->getEntities()[0]->getWorldMatrix());
    //mAxes->activate();
    //mAxes->draw();
	
	//2D Drawing
	drawHUD(mCamera->getZoom()/10);

    CHECK_GL_ERRORS("drawing");
}

static float t;
void SplineInit()
{
	t = 0.0f;
}

glm::vec3 SplinePointOnCurve(float dt, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	//
	//	The spline passes through all of the control points.
	//	The spline is C1 continuous, meaning that there are no discontinuities in the tangent direction and magnitude.
	//	The spline is not C2 continuous.  The second derivative is linearly interpolated within each segment, causing the curvature to vary linearly over the length of the segment.
	//	Points on a segment may lie outside of the domain of P1 -> P2.
	glm::vec3 vOut = glm::vec3(0.0f, 0.0f, 0.0f);

	// update state
	t += dt;
	if (t > 1.0f)
		t = 1.0f;
	float t2 = t * t;
	float t3 = t2 * t;

	vOut = 0.5f * ((2.0f * p1) + (-p0 + p2) * t + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 + (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);

	return vOut;
}

bool BasicSceneRenderer::update(float dt) // GAME LOOP
{
	const Keyboard* kb = getKeyboard();
	const Mouse* mouse = getMouse();

	if (!mCamera->getFreeLook()) {
		player->headLook(mCamera->getYaw(), mCamera->getPitch(), dt);
		player->bodyMove(kb, dt);
		if (mouse->buttonPressed(MOUSE_BUTTON_LEFT)) {
			projectiles.push_back(player->shoot()[0]);
			addEntities(projectiles[projectiles.size() - 1]->getEntities());
		}
	}

	if (projectiles.size() > 0) {
		for (int i = 0; i < projectiles.size(); i++) {
			projectiles[i]->update(dt);
		}
	}

    if (kb->keyPressed(KC_ESCAPE))
        return false;

    // change lighting models
    if (kb->keyPressed(KC_1))
        mLightingModel = PER_VERTEX_DIR_LIGHT;
    if (kb->keyPressed(KC_2))
        mLightingModel = BLINN_PHONG_PER_FRAGMENT_DIR_LIGHT;
    if (kb->keyPressed(KC_3))
        mLightingModel = BLINN_PHONG_PER_FRAGMENT_POINT_LIGHT;
    if (kb->keyPressed(KC_4))
        mLightingModel = BLINN_PHONG_PER_FRAGMENT_MULTI_LIGHT;

    // toggle visualization of point lights
    if (kb->keyPressed(KC_TAB))
        mVisualizePointLights = !mVisualizePointLights;

	/*
	//
	// start spline
	//
	static bool bSplineStart = false;
	if (kb->keyPressed(KC_S))
	{
		SplineInit();
		bSplineStart = true;
	}
	if (bSplineStart)
	{
		glm::vec3 vPos = SplinePointOnCurve(dt, mEntities[0]->getPosition(),
			mEntities[1]->getPosition(),
			mEntities[2]->getPosition(),
			mEntities[3]->getPosition());

		mEntities[4]->setPosition(vPos);
	}
	*/

    // update the camera
	if (kb->keyPressed(KC_P)) {
		mCamera->toggleFreelook();
	}
	if (isFocused()) {
		glutSetCursor(GLUT_CURSOR_NONE);
		glutWarpPointer(s.SCREEN_WIDTH/2 , s.SCREEN_HEIGHT/2 );
		mCamera->update(dt);
	}
	else {
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}

    return true;
}

bool BasicSceneRenderer::isFocused() {
	if (GetActiveWindow()) {
		return true;
	} else {
		return false;
	}
}

void BasicSceneRenderer::addEntities(std::vector<Entity*> entities) {
	for (int i = 0; i < entities.size(); i++) {
		mEntities.push_back(entities[i]);
    }

}

void BasicSceneRenderer::drawHUD(float scale) {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, s.SCREEN_WIDTH, s.SCREEN_HEIGHT, 0.0, -10.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.025 * scale, 0.0, 3.0);
	glVertex3f(0.5 * scale, 0.0, 3.0 - (0.075 * scale));
	glVertex3f(0.5 * scale, 0.0, 3.0 + (0.075 * scale));
	//--------------------------
	glVertex3f(-0.025 * scale, 0.0, 3.0);
	glVertex3f(-0.5 * scale, 0.0, 3.0 - (0.075 * scale));
	glVertex3f(-0.5 * scale, 0.0, 3.0 + (0.075 * scale));
	glEnd();



	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
