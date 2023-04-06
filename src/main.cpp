// Lab 4, terrain generation

// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"


// TODO adapt in case the windowsize is changed
int display_size = 900; // size of the window - used for mouse movement

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "scene/scene.hpp"
#include "scene/skybox.hpp"
#include "scene/light.hpp"
#include "scene/shaders.hpp"
#include "scene/camera.hpp"
#include "resource_manager.hpp"


static scn::Scene mainScene;
static Scene mainScene;

void keyControlCheck()
{
	vec4 pressedKeys = vec4(false, false, false, false);
	if (glutKeyIsDown('w'))
		pressedKeys.a = true;
	if (glutKeyIsDown('a'))
		pressedKeys.b = true;
	if (glutKeyIsDown('s'))
		pressedKeys.g = true;
	if (glutKeyIsDown('d'))
		pressedKeys.h = true;
	mainScene.camera.forwardPressedKeys(pressedKeys);
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
	if (m->material)
		free(m->material);
	m->material = (MtlPtr)malloc(sizeof(Mtl));
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
	Model *teapot;
	teapot = LoadModel("models/various/teapot.obj");
	auto programShader = std::make_unique<SceneShader>("lab3-3.raw.vert", "lab3-3.raw.frag", "projectionMatrix", "w2vMatrix", "m2wMatrix");
	InitModel(teapot, programShader->hndl, "in_Position", "in_Normal", NULL);

	const Camera camera{{0.f, 0.2f, -20.f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.f, 0.0f}};
	mainScene = Scene(std::move(programShader), camera, std::make_shared<mat4>(projectionMatrix), std::move(skybox));
	mainScene.shader->resetShaderDataLocation(SceneShader::Matrices::preProj, "preProjTransform");


	setMaterial(teapot, 1.0, 1.0, 1.0, 100.0);
	mainScene.pushModel(teapot);
		glutRepeatingTimer(FRAME_GAP_MS);

	glutPassiveMotionFunc(mouseControlCallback);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	keyControlCheck();


	// todo draw stuff here
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(display_size, display_size);
	glutCreateWindow("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init();
	glutPassiveMotionFunc(updateMousePosition);
	glutRepeatingTimer(20);

	glutMainLoop();
	exit(0);
}