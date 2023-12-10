#include <glut.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

using namespace std;

// global variables
static int frameTimer;
static float currentTime;
static float last;
char strFrameRate[50];		// fps count
float windowWidth = 800;
float windowHeight = 600;
float angle = 0;
float lx = 0, lz = -1;
float x = 0, z = 5;
bool useSmooth = true;
// statements
void renderScene();
void clear();
void resize(int width, int height);
void drawModel();
void procSKeys(int key, int xx, int yy);

int main(int argc, char* argv[]) {
	// Init
	last = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("สตั้ถþ");
	// loop
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
	glutIdleFunc(renderScene);
	glutSpecialFunc(procSKeys);

	// 
	clear();
	//
	glutMainLoop();

	return 0;

}

void renderScene() {
	// fps count
	float currentTime = GetTickCount64() * 0.001f;
	frameTimer++;
	if (currentTime - last >= 1.0f) {
		last = currentTime;
		sprintf_s(strFrameRate, "Frames Per Second: %d", frameTimer);
		glutSetWindowTitle(strFrameRate);
		frameTimer = 0;
	}
	//

	glClear(GL_COLOR_BUFFER_BIT);

	if (useSmooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);

	// set object color
	glBegin(GL_TRIANGLES);
	glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);
	glVertex3f(0, 0.4f, 0);
	glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)0);
	glVertex3f(0.4f, -0.7f, 0);
	glColor3ub((GLubyte)0, (GLubyte)0, (GLubyte)255);
	glVertex3f(-0.4f, -0.7f, 0);
	glEnd();

	// swap the screen buffer and back buffer
	glutSwapBuffers();

}
void clear() {
	glClearColor(0, 0, 0, 1);
	glColor3f(1, 0, 0);
}
void resize(int width, int height) {
	// get aspectRadio
	if (height == 0)
		height = 1;
	float aspectRatio = 1.0f * width / height;
	// init
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// set viewport
	glViewport(0, 0, width, height);
	if (width <= height) {
		windowWidth = 1;
		windowHeight = 1 / aspectRatio;
		glOrtho(-1, 1, -windowHeight, windowHeight, 1, -1);
	}
	else
	{
		windowWidth = 1 * aspectRatio;
		windowHeight = 1;
		glOrtho(-windowWidth, windowWidth, -1, 1, 1, -1);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void procSKeys(int key, int xx, int yy) {
	float fraction = 0.1f;

	switch (key)
	{
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	case GLUT_KEY_F1:
		useSmooth = !useSmooth;
		break;
	}
}