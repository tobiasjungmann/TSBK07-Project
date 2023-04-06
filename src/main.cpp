// Lab 4, terrain generation

// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "scene/camera.hpp"
#include "scene/scene.hpp"
#include "scene/skybox.hpp"
#include "scene/shaders.hpp"
#include "resource_manager.hpp"


static scn::Scene mainScene;


void init(void)
{
	// GL inits
	glClearColor(0.2, 0.2, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	Model *fish, *ground;
	fish = ResourceManager::get().getModel("gorgonian", "gorgonian.obj");

}


void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




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