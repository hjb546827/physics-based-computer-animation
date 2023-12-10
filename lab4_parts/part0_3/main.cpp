#include "Spring.h"

using namespace std;

GLfloat windowWidth, windowHeight;

Spring spring = Spring(24, 0.002f, 80000, 50, Vector2(-50, 50));

void renderScene();
void resize(int width, int height);
void timeCount(int value);
void procIOEvents(unsigned char key, int x, int y);

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(640, 640);
	glutCreateWindow("µ¯ÐÔÍøÂç");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);

	glutTimerFunc(30, timeCount, 1);
	glutKeyboardFunc(procIOEvents);

	glClearColor(0, 0, 0, 1);

	glutMainLoop();

	return 0;
}
void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 0, 0);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);

	spring.pin(0);
	spring.pin(9);
	spring.update();

	spring.draw(10, 4);

	glutSwapBuffers();
}
void resize(int width, int height) {
	if (height == 0)
		height = 1;
	float aspectRatio = 1.0f * width / height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
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
	glutPostRedisplay();
	glutTimerFunc(30, timeCount, 1);
}
void procIOEvents(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	}
}
