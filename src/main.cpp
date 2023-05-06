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
#include "scene/shaders.hpp"
#include "scene/camera.hpp"
#include "scene/terrain.hpp"
#include "resource_manager.hpp"
#include "gameobj/fish.hpp"

#define FRAME_GAP_MS 20

const scn::Light redLight{{1.0f, 0.0f, 0.0f}, {10.0f, 5.0f, 0.0f}, false};
const scn::Light greenLight{{0.0f, 1.0f, 0.0f}, {0.0f, 5.0f, 10.0f}, false};
const scn::Light blueLight{{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, true};
const scn::Light whiteLight{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, true};

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

void setMaterial(Model *m, GLfloat Ka, GLfloat Kd, GLfloat Kspec, GLfloat alpha)
{
	if (!m->material)
	{
		m->material = (MtlPtr)malloc(sizeof(Mtl));
	}
	m->material->alpha = alpha;
	m->material->specularity = Kspec;
	m->material->diffuseness = Kd;
	m->material->Ka = Ka;
}

void init(void)
{
	// GL inits
	glClearColor(0.2, 0.2, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	auto programShader = std::make_unique<scn::SceneShader>("src/shaders/light.vert", "src/shaders/light.frag", "projectionMatrix", "w2vMatrix", "m2wMatrix");
	const scn::Camera camera{{0.f, 0.2f, -20.f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.f, 0.0f}};

	scn::Terrain terrain("rc/models/fft-terrain.tga");
	InitModel(terrain.getModel(), programShader->hndl, "in_Position", "in_Normal", NULL);

	Model *green_reef;
	green_reef = ResourceManager::get().getModel("green_reef", "green_reef.obj");
	InitModel(green_reef, programShader->hndl, "in_Position", "in_Normal", NULL);
	// obj::Fish fish(green_reef, vec3(0),vec3(1,0,0));

	mainScene = scn::Scene(std::move(programShader), camera, projectionMatrix);
	setMaterial(green_reef, 1.0, 1.0, 1.0, 100.0);
	setMaterial(terrain.getModel(), 1.0, 1.0, 1.0, 100.0);

	mainScene.shader->initLighting("lightSourcesDirPosArr", "lightSourcesColorArr", "isDirectional", "specularExponent");
	mainScene.addLightSource(redLight);
	mainScene.addLightSource(greenLight);
	mainScene.addLightSource(blueLight);
	mainScene.addLightSource(whiteLight);

	mainScene.pushModel(terrain.getModel());
	
	auto fish =std::make_unique<obj::Fish>(green_reef, vec3(0), vec3(1, 0, 0));
	mainScene.pushMoveableObject(std::move(fish));
	
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
	glutCreateWindow("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init();

	glutRepeatingTimer(20);

	glutMainLoop();
	exit(0);
}