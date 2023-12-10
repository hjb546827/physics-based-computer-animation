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
GLfloat rotation = 0;
// statements
void renderScene();
void timeCount(int value);
void clear();
void resize(int width, int height);

int main(int argc, char* argv[]) {
	// Init
	last = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("สตั้าป");
	// loop
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
	glutTimerFunc(33, timeCount, 1);
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

	glPushMatrix();
	glRotatef(rotation, 0, 0, 1);

	glColor3f(0.4f, 0.6f, 0.6f);

	// set object
	glBegin(GL_TRIANGLES);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();

	glPopMatrix();
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
void timeCount(int value) {
	rotation += 2.0f;
	// redraw
	glutPostRedisplay();
	glutTimerFunc(33, timeCount, 1);
}