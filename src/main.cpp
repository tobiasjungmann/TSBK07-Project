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

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "scene/scene.hpp"
#include "scene/skybox.hpp"
#include "scene/light.hpp"
#include "scene/modelv2.hpp"
#include "scene/shaders.hpp"
#include "scene/camera.hpp"
#include "scene/terrain.hpp"
#include "resource_manager.hpp"

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

void keyControlCheck()
{
  mainScene.camera.setPressedKeys(glutKeyIsDown(constants::KEY_FORWARD), glutKeyIsDown(constants::KEY_LEFT), glutKeyIsDown(constants::KEY_BACK), glutKeyIsDown(constants::KEY_RIGHT));
}

/**
 * @brief
 * First iteration cannot use previous values -> camera would be moved to wrong position.
 * @param x position of the mouse
 * @param y poisiton of the mouse
 */
void mouseControlCallback(int x, int y)
{
  mainScene.camera.setMousePosition(x, y);
  glutPostRedisplay();
}

void init(void)
{
  // GL inits
  glClearColor(0.2, 0.2, 0.5, 0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  auto programShader = std::make_unique<scn::SceneShader>("src/shaders/light.vert", "src/shaders/light.frag", "projectionMatrix", "w2vMatrix", "m2wMatrix");
  const scn::Camera camera{{0.f, 0.2f, -20.f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.f, 0.0f}};

  scn::Terrain terrain("fft-terrain.tga");
  terrain.model().init(programShader->hndl, "in_Position", "in_Normal", NULL);
  terrain.model().setLightProps(1.0, 1.0, 1.0, 100.0);

  Modelv2 green_reef{"green_reef", "green_reef.obj"};
  green_reef.init(programShader->hndl, "in_Position", "in_Normal", NULL);
  green_reef.setLightProps(1.0, 0.8, 0.5, 100.0);

  mainScene = scn::Scene(std::move(programShader), camera, projectionMatrix);
  // mainScene.shader->resetShaderDataLocation(scn::SceneShader::Matrices::preProj, "preProjTransform");

  mainScene.shader->initMaterialProps("materialLight");
  mainScene.shader->initLighting("lights");

  scn::Light blueLight{{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, true};
  scn::Light whiteLight{{1.0f, 1.0f, 1.0f}, {10.0f, 5.0f, 0.0f}, false};

  blueLight.setCoefficients(0.3, 0.7, 0);

  whiteLight.setCoefficients(0, 0.8, 0.4);
  whiteLight.setSpotlight(cos(25), cos(35));
  whiteLight.setAttenuation(1.0, 0.027, 0.0028);

  mainScene.addLightSource(blueLight);
  mainScene.addLightSource(whiteLight);

  mainScene.pushModel(terrain.model());
  mainScene.pushModel(green_reef);
  glutRepeatingTimer(FRAME_GAP_MS);

  glutPassiveMotionFunc(mouseControlCallback);
}

void display(void)
{
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  keyControlCheck();

  mainScene.camera.updateCameraPosition();
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