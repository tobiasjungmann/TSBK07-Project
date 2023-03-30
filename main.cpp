// Lab 4, terrain generation

// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

mat4 projectionMatrix;
vec3 camera = vec3(0, 5, 8);
vec3 cameraFront = vec3(0, 0, -1);
vec3 cameraUp = vec3(0.0, 1.0, 0.0);

bool firstMouse = true;
int lastX = 450;
int lastY = 450;
float yaw = 0;
float pitch = 0;
float xoffset;
float yoffset;

int display_size = 900;

Model *GenerateTerrain()
{
	Model *model = LoadDataToModel(
		vertexArray,
		normalArray,
		texCoordArray,
		NULL,
		indexArray,
		vertexCount,
		triangleCount * 3);

	return model;
}


void init(void)
{
	// GL inits
	glClearColor(0.2, 0.2, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program_color = loadShaders("texture.vert", "texture.frag");
}

float toRadians(float degrees)
{
	return degrees * (M_1_PI / 180);
}


float computeMinMovement(float last, float offset)
{
	float noMovement = display_size / 10;
	float minMovementScaling = 0.05;

	if (last < (display_size / 2))
	{
		float minMovement = (last - (display_size / 2) + noMovement) * minMovementScaling;

	//	printf("last left:   minmovement: %f     offset: %f   lastX: %f\n", minMovement,offset,last);
		if (minMovement < offset && offset >= 0)
		{
			return minMovement;
		}
	}
	else
	{
		float minMovement = (last - (display_size / 2) - noMovement) * minMovementScaling;

	//	printf("last right: minmovement: %f     xoffset: %f   lastX: %f\n", minMovement,offset,last);
		if (minMovement > offset && offset >= 0)
		{
			return minMovement;
		}
	}
	return offset;
}

void updateCameraPosition()
{       // odo impreove movement range/ must feel more natural
	mat4 camMatrix;
	const float cameraSpeed = 0.05f; 
	if (glutKeyIsDown('w'))
		camera += cameraSpeed * cameraFront;
	if (glutKeyIsDown('s'))
		camera -= cameraSpeed * cameraFront;
	if (glutKeyIsDown('a'))
		camera -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glutKeyIsDown('d'))
		camera += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

	xoffset = computeMinMovement(lastX, xoffset);
	yoffset = -computeMinMovement(lastY, yoffset);

	float sensitivity = 0.7f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 direction;
	direction.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
	direction.y = sin(toRadians(pitch));
	direction.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
	cameraFront = normalize(direction);

	vec3 viewCenter = camera + cameraFront;
	camMatrix = lookAt(camera, viewCenter, cameraUp);

	// todo update glUniformMatrix4fv(glGetUniformLocation(program_color, "mdlMatrix"), 1, GL_TRUE, camMatrix.m);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	updateCameraPosition();

	// todo draw stuff here
	glutSwapBuffers();
}

// adapted from: https://learnopengl.com/Getting-started/Camera
void updateMousePosition(int x, int y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	xoffset = x - lastX;
	yoffset = lastY - y;
	lastX = x;
	lastY = y;
	glutPostRedisplay();
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