// Lab 1-6.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

#include "camera.hpp"
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "globals.h"
#include <cmath>
#include <iostream>
// uses framework OpenGL
// uses framework Cocoa

#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "scene.hpp"
#include "skybox.hpp"
#include "light.hpp"
#include "shaders.hpp"


using namespace scn;
static Scene mainScene;

const GLfloat specularExponent[] = {100.0, 200.0, 60.0};

const Light redLight {{1.0f, 0.0f, 0.0f}, {10.0f, 5.0f, 0.0f}, false};
const Light greenLight {{0.0f, 1.0f, 0.0f}, {0.0f, 5.0f, 10.0f}, false};
const Light blueLight {{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, true};
const Light whiteLight {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, true};

// camera
GLfloat camera_angle = M_PI_2;
GLfloat camera_depth = 10.0f;
vec3 cameraPositionOffset = {0.0f, 0.0f, 0.0f};



void setMaterial(Model *m, GLfloat Ka, GLfloat Kd, GLfloat Kspec, GLfloat alpha) {
	if (m->material) free(m->material);
	m->material = (MtlPtr) malloc(sizeof(Mtl));
	m->material->alpha = alpha;
	m->material->specularity = Kspec;
	m->material->diffuseness = Kd;
	m->material->Ka = Ka;
}

void mouseControlCallback(int x, int y) {	
	mainScene.camera.at.x = ((float) x/ WINDOW_WIDTH - 0.5f) * 4;
	mainScene.camera.at.y = ((float) y/WINDOW_HEIGHT - 0.5f ) * 4;
}

void keyControlCheck() {
	if (glutKeyIsDown('d'))
		camera_angle += 0.05f;
	else if (glutKeyIsDown('q'))
		camera_angle -= 0.05f;
	else if (glutKeyIsDown('s'))
		camera_depth += 0.2f;
	else if (glutKeyIsDown('z'))
		camera_depth -= 0.2f;

	else if (glutKeyIsDown('h'))
		cameraPositionOffset.x -= 0.05f;
	else if (glutKeyIsDown('l'))
		cameraPositionOffset.x += 0.05f;
	else if (glutKeyIsDown('j'))
		mainScene.camera.translate({0.0f, -0.05f, 0.0f});
	else if (glutKeyIsDown('k'))
		mainScene.camera.translate({0.0f, +0.05f, 0.0f});
	else if (glutKeyIsDown('u'))
		cameraPositionOffset.z = 0.05f;
	else if (glutKeyIsDown('n'))
		cameraPositionOffset.z = -0.05f;

}


void init(void)
{
	// vertex buffer object, used for uploading the geometry
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// load model object
	Model *walls, *roof, *balcony, *blade, *teapot, *ground;
	walls = LoadModel("windmill/windmill-walls2.obj");
	roof = LoadModel("windmill/windmill-roof.obj");
	balcony = LoadModel("windmill/windmill-balcony.obj");
	blade = LoadModel("windmill/blade.obj");
	teapot = LoadModel("models/various/teapot.obj");
	ground = LoadDataToModel(ground_vertices, ground_vertex_normals, ground_tex_coords, NULL, ground_indices, 4, 6);
	Model *skyboxModel = LoadModel("skybox/skyboxfull.obj");

	// load texture
	GLuint skyboxTexRefID;
	LoadTGATextureSimple("skybox/SkyBoxFull.tga", &skyboxTexRefID);

	// Load and compile shader
	auto programShader = std::make_unique<SceneShader>("lab3-3.raw.vert", "lab3-3.raw.frag", "projectionMatrix", "w2vMatrix", "m2wMatrix");
	auto skyboxShader = std::make_unique<SkyboxShader>("lab3-3.skybox.vert", "lab3-3.skybox.frag", "projectionMatrix", "preProjTransform");
	printError("init shader");

	// skkybox
	Skybox* skybox = new Skybox(std::move(skyboxShader), skyboxModel);
	InitModel(skybox->model, skybox->shader->hndl, "in_Position", NULL, "inTexCoord");
	skybox->shader->initTexturing("texUnit", "inTexCoord");
	skybox->shader->uploadTexture(skybox->model, skyboxTexRefID, TEXTURE_UNIT);
	
	// Upload geometry to the GPU
	InitModel(ground, programShader->hndl, "in_Position", "in_Normal", NULL);
	InitModel(walls, programShader->hndl, "in_Position", "in_Normal", NULL);
	InitModel(roof, programShader->hndl, "in_Position", "in_Normal", NULL);
	InitModel(balcony, programShader->hndl, "in_Position", "in_Normal", NULL);
	InitModel(blade, programShader->hndl, "in_Position", "in_Normal", NULL);

	const Camera camera {{0.f, 0.2f, -20.f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.f, 0.0f}};
	mainScene = Scene(std::move(programShader), camera, std::make_shared<mat4>(projectionMatrix), std::move(skybox));
	mainScene.shader->resetShaderDataLocation(SceneShader::Matrices::preProj, "preProjTransform");
	

	// set Materials

	//floor
	setMaterial(ground, 1.0, 1.0, 1.0, specularExponent[0]);
	// windmill
	setMaterial(walls, 1.0, 1.0, 1.0, specularExponent[1]);
	setMaterial(roof, 1.0, 1.0, 1.0, specularExponent[1]);
	setMaterial(balcony, 1.0, 1.0, 1.0, specularExponent[1]);
	setMaterial(blade, 1.0, 1.0, 1.0, specularExponent[1]);
	// teapot
	setMaterial(teapot, 1.0, 1.0, 1.0, specularExponent[2]);

	// define scene
	mainScene.pushModel(ground, std::make_shared<mat4>(T(0, -5.f, 0)));
	mainScene.pushModel(walls, std::make_shared<mat4>(T(0, 0, 0)));
	mainScene.pushModel(roof);
	mainScene.pushModel(balcony);
	mainScene.pushModel(blade);
	mainScene.pushModel(blade);
	mainScene.pushModel(blade);
	mainScene.pushModel(blade);
	mainScene.pushModel(teapot);


	mainScene.shader->initLighting("lightSourcesDirPosArr", "lightSourcesColorArr", "isDirectional", "specularExponent");
	mainScene.addLightSource(redLight);
	mainScene.addLightSource(greenLight);
	mainScene.addLightSource(blueLight);
	mainScene.addLightSource(whiteLight);

	// set up animation timer
	glutRepeatingTimer(FRAME_GAP_MS);

	glutPassiveMotionFunc(mouseControlCallback);

	printError("init arrays");
}


void display(void)
{
	printError("pre display");
	static GLfloat blade_angle = 0.01;

	keyControlCheck();
	mainScene.camera.rotateAround(Axis::Y, camera_angle, camera_depth);
	mainScene.camera.translate(cameraPositionOffset);

	// MOVING THE CAMERA ALSO MOVES THE FRUSTUM !!!!

	// clear the screen ( and Z-buffer ?)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	blade_angle += 0.01f;
	const mat4 m2w_blades = T(4.2f, 9.2f, -0.5f) * Rx(blade_angle) * Rx(M_PI_2);
	mainScene.updateModelM2W(-5, std::make_shared<mat4>(m2w_blades));
	mainScene.updateModelM2W(-4, std::make_shared<mat4>(m2w_blades * Rx(M_PI_2)));
	mainScene.updateModelM2W(-3, std::make_shared<mat4>(m2w_blades * Rx(M_PI)));
	mainScene.updateModelM2W(-2, std::make_shared<mat4>(m2w_blades * Rx(-M_PI_2)));

	mainScene.draw();

	

	printError("display");	
	// swap front and back buffers
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("GL3 walls and roof 3D scene example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
