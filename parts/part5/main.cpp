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
static GLfloat rotation, rotation_speed;
GLfloat x = 0, y = 0, rsize = 25;
GLfloat xstep = 1, ystep = 1;
GLfloat windowWidth, windowHeight;
GLfloat r = 0, g = 1, b = 1;
// statements
void renderScene();
void clear();
void resize(int width, int height);
void timeCount(int value);
void procIOEvents(unsigned char key, int x, int y);

int main(int argc, char* argv[]) {
	// Init
	last = 0;
	rotation = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("实验一");
	// loop
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
	glutTimerFunc(33, timeCount, 1);
	glutKeyboardFunc(procIOEvents);
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

	glClear(GL_COLOR_BUFFER_BIT);
	// set object color
	glColor3f(r, g, b);
	// IA set
	glRectf(x, y, x + rsize, y - rsize);

	// swap the screen buffer and back buffer
	glutSwapBuffers();
}
void clear() {
	glClearColor(0, 0, 0, 1);
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
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho(-100, 100, -windowHeight, windowHeight, 1, -1);
	}
	else
	{
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho(-windowWidth, windowWidth, -100, 100, 1, -1);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void timeCount(int value) {
	if (x > windowWidth - rsize || x < -windowWidth)
		xstep = -xstep;
	if (y > windowHeight || y < -windowHeight + rsize)
		ystep = -ystep;
	x += xstep;
	y += ystep;

	if (x > (windowWidth - rsize + xstep))
		x = windowWidth - rsize - 1;
	else if (x < -(windowWidth + xstep))
		x = -windowWidth - 1;
	if (y > (windowHeight + ystep))
		y = windowHeight - 1;
	else if (y < -(windowHeight - rsize + ystep))
		y = -windowHeight + rsize - 1;
	glutPostRedisplay();
	glutTimerFunc(33, timeCount, 1);
}
void procIOEvents(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case '0':
		r = 0;
		g = 0;
		b = 0;
		break;
	case '1':
		r = 1;
		g = 0;
		b = 0;
		break;
	case '2':
		r = 0;
		g = 1;
		b = 0;
		break;
	case '3':
		r = 0;
		g = 0;
		b = 1;
		break;
	case '4':
		r = 0;
		g = 1;
		b = 1;
		break;
	case '5':
		r = 1;
		g = 0;
		b = 1;
		break;
	case '6':
		r = 1;
		g = 1;
		b = 0;
		break;
	case '7':
		r = 1;
		g = 1;
		b = 1;
		break;
	case 'q':
		xstep += 1;
		break;
	case 'e':
		xstep -= 1;
		break;
	case 'a':
		ystep += 1;
		break;
	case 'd':
		ystep -= 1;
		break;
	}
}