// Lab 4, terrain generation

// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "constants.hpp"
#include "LoadTGA.h"

#include "scene/scene.hpp"
#include "scene/skybox.hpp"
#include "scene/gameobj/light.hpp"
#include "modelv2.hpp"
#include "scene/shaders.hpp"
#include "scene/camera.hpp"
#include "scene/terrain.hpp"
#include "resource_manager.hpp"
#include "event.hpp"
#include "debugging.h"
#include "scene/gameobj/fish.hpp"
#include "scene/gameobj/fishCarp.hpp"

#define FRAME_GAP_MS 20

// const scn::Light redLight{{1.0f, 0.0f, 0.0f}, {10.0f, 5.0f, 0.0f}, false};
// const scn::Light greenLight{{0.0f, 1.0f, 0.0f}, {0.0f, 5.0f, 10.0f}, false};

// frustum
#define near 1.0
#define far 80.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

const mat4 projectionMatrix = {
    2.0 * near / (right - left),
    0.0,
    (right + left) / (right - left),
    0.0,
    0.0,
    2.0 * near / (top - bottom),
    (top + bottom) / (top - bottom),
    0.0,
    0.0,
    0.0,
    -(far + near) / (far - near),
    -2 * far *near / (far - near),
    0.0,
    0.0,
    -1.0,
    0.,
};

static scn::Scene mainScene;
static evt::Context context;
scn::Light whiteLight{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, false};

void keyControlCheck()
{
    context.setPressedKeys(
        glutKeyIsDown(constants::KEY_FORWARD),
        glutKeyIsDown(constants::KEY_LEFT),
        glutKeyIsDown(constants::KEY_BACK),
        glutKeyIsDown(constants::KEY_RIGHT));
}

/**
 * @brief
 * First iteration cannot use previous values -> camera would be moved to wrong position.
 * @param x position of the mouse
 * @param y poisiton of the mouse
 */
void mouseControlCallback(int x, int y)
{
    context.setMousePosition(x, y);
    glutPostRedisplay();
}

float rand1()
{
    return (float)(rand() / (float)RAND_MAX);
}

void init(void)
{
    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    auto skyboxShader = std::make_unique<scn::SkyboxShader>("src/shaders/skybox.vert", "src/shaders/skybox.frag", "projectionMatrix", "preProjTransform");
    skyboxShader->initTexturing("texUnit", "inTexCoord");
    rc::ResourceManager::config(skyboxShader->hndl, "in_Position", "", "inTexCoord"); // TODO write here name of texture coord variable
    Modelv2 skyboxModel{"skybox", "skyboxfull.obj", "skybox", 0, "skybox.tga"};
    scn::Skybox *skybox = new scn::Skybox(std::move(skyboxShader), skyboxModel);

    auto programShader = std::make_unique<scn::SceneShader>("src/shaders/light.vert", "src/shaders/light.frag", "projectionMatrix", "w2vMatrix", "m2wMatrix");
    rc::ResourceManager::config(programShader->hndl, "in_Position", "in_Normal", "inTexCoord"); // TODO write here name of texture coord variable
    const scn::Camera camera{{0.f, 0.2f, -20.f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.f, 0.0f}};

    // Create Scene
    mainScene = scn::Scene(std::move(programShader), camera, projectionMatrix, skybox);

    scn::Terrain terrain("terrain", "fft-terrain.tga");
    terrain.model().setLightProps(0.1, 1, 0.0, 1.0);
    terrain.model().texture({"terrainText", 1, "dirt.tga"});
    terrain.model().textureFactor(0.01);

    mainScene.addTerrain(std::move(terrain));
    mainScene.camera.position.x = mainScene.camera.position.z = -mainScene.terrain->width() / 2;

    srand(12);

    for (size_t i = 0; i < 10; i++)
    {
        float x = rand1();
        float z = rand1();
        Modelv2 fish_m{"green_reef", "green_reef.obj", "green_reef", 2, "fish.png"};
        fish_m.setLightProps(0.9, 0.8, 0.8, 100.0);
        obj::Fish *fish = new obj::Fish(fish_m,vec3(5*rand1(),4,3*rand1()),  normalize(vec3(0, -1,0)), 0.1f);
        mainScene.pushObject(fish, fish->orientationMtx());
    }

    // mainScene.shader->resetShaderDataLocation(scn::SceneShader::Matrices::preProj, "preProjTransform");

    mainScene.shader->initMaterialProps("materialLight");
    mainScene.shader->initLighting("lights");
    mainScene.shader->initTexturing("texUnit", "inTexCoord");

    scn::Light blueLight{{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, true};

    blueLight.setCoefficients(0.3, 0.4, 0);

    whiteLight.setCoefficients(0, 2.0, 0.6);
    whiteLight.setSpotlight(25, 35);
    whiteLight.setAttenuation(1.0, 0.027, 0.0015);

    whiteLight.attachToCamera(mainScene.camera);

    mainScene.addLightSource(blueLight);
    mainScene.addLightSource(whiteLight);

    glutRepeatingTimer(FRAME_GAP_MS);

    glutPassiveMotionFunc(mouseControlCallback);
}

void display(void)
{
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    keyControlCheck();

    mainScene.camera.updateCameraPosition(context, *mainScene.terrain);

    // FIXME move this out of the way
    // update all objects
    mainScene.update();

    mainScene.draw();

    // todo draw stuff here
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(constants::display_size, constants::display_size);
    glutCreateWindow("Deep Sea");
    glutDisplayFunc(display);
    init();

    glutRepeatingTimer(20);

    glutMainLoop();
    exit(0);
}