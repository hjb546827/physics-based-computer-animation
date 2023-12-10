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
// statements
void renderScene();
void clear();

int main(int argc, char* argv[]) {
	// Init
	last = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("สตั้าป");
	// loop
	glutDisplayFunc(renderScene);
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
	// set object color
	glBegin(GL_TRIANGLES);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();
	// swap the screen buffer and back buffer
	glFlush();

}
void clear() {
	glClearColor(0, 0, 0, 1);
	glColor3f(1, 0, 0);
}