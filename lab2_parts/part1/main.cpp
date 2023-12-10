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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("สตั้ถþ");
	// loop
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
	glutIdleFunc(renderScene);
	glutSpecialFunc(procSKeys);

	glEnable(GL_DEPTH_TEST);
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(
		x,		1,		z,
		x + lx, 1, z + lz,
		0,		1,		0
	);

	glColor3f(0.9f, 0.9f, 0.9f);
	// set object color
	glBegin(GL_QUADS);
	glVertex3f(-100, 0, -100);
	glVertex3f(-100, 0,  100);
	glVertex3f( 100, 0,  100);
	glVertex3f( 100, 0, -100);
	glEnd();

	for(int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i * 10.0f, 0, j * 10.0f);
			drawModel();
			glPopMatrix();
		}
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
	
	gluPerspective(60, aspectRatio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void drawModel() {
	glColor3f(1, 1, 1);

	glTranslatef(0, 0.75f, 0);
	glutSolidSphere(0.75f, 20, 20);

	glTranslatef(0, 1, 0);
	glutSolidSphere(0.25f, 20, 20);

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.05f, 0.1f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0, 0);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	glColor3f(1, 0.5f, 0.5f);
	glRotatef(0, 1, 0, 0);
	glutSolidCone(0.08f, 0.5f, 10, 2);
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
	}
}