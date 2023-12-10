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
GLfloat rotation = 0, rotation_speed = 2;
GLfloat r = 0, g = 1, b = 1;
// statements
void renderScene();
void timeCount(int value);
void clear();
void resize(int width, int height);
void procIOEvents(unsigned char key, int x, int y);

int main(int argc, char* argv[]) {
	// Init
	last = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
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
	//
	cout << "z轴旋转角:" << (int)rotation % 360 << endl;

	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glRotatef(rotation, 0, 0, 1);

	glColor3f(r, g, b);

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
	rotation += rotation_speed;
	// redraw
	glutPostRedisplay();
	glutTimerFunc(33, timeCount, 1);
}
void procIOEvents(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
	if (key == '0') {
		r = 0;
		g = 0;
		b = 0;
	}
	if (key == '1') {
		r = 1;
		g = 0;
		b = 0;
	}
	if (key == '2') {
		r = 0;
		g = 1;
		b = 0;
	}
	if (key == '3') {
		r = 0;
		g = 0;
		b = 1;
	}
	if (key == '4') {
		r = 0;
		g = 1;
		b = 1;
	}
	if (key == '5') {
		r = 1;
		g = 0;
		b = 1;
	}
	if (key == '6') {
		r = 1;
		g = 1;
		b = 0;
	}
	if (key == '7') {
		r = 1;
		g = 1;
		b = 1;
	}
	if (key == 'q') {
		rotation_speed += 1;
	}
	if (key == 'e') {
		rotation_speed -= 1;
	}
}