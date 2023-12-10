#include <glut.h>
#include <stdlib.h>

GLfloat xrot;
GLfloat yrot;
GLfloat xspeed;
GLfloat yspeed;
GLfloat z = -5;
GLboolean light = true;
GLfloat LightAmbient[] = { 1, 1, 1, 1 };
GLfloat LightDiffuse[] = { 1, 1, 1, 1 };
GLfloat LightPosition[] = { 0, 0, 2, 1 };

void reSize(int w, int h);
int initGL(GLvoid);
void renderScene(void);
void procSKey(int key, int x, int y);
void procNKey(unsigned char key, int x, int y);

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 640);
	glutCreateWindow("สตั้ถþ");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(reSize);
	glutSpecialFunc(procSKey);
	glutKeyboardFunc(procNKey);

	initGL();

	glutMainLoop();

	return 0;
}

void reSize(int w, int h) {
	if (h == 0)	h = 1;
	float ratio = w * 1.0f / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
}
int initGL(GLvoid) {
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0.5f);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	return true;
}
void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, z);

	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);

	glColor4f(1, 0, 0, 1);
	glutSolidCube(1);

	glutSwapBuffers();

	xrot += xspeed;
	yrot += yspeed;
}
void procSKey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		z -= 0.02f;
		break;
	case GLUT_KEY_PAGE_DOWN:
		z += 0.02f;
		break;
	case GLUT_KEY_UP:
		xspeed -= 0.01f;
		break;
	case GLUT_KEY_DOWN:
		xspeed += 0.01f;
		break;
	case GLUT_KEY_LEFT:
		yspeed -= 0.01f;
		break;
	case GLUT_KEY_RIGHT:
		yspeed += 0.01f;
		break;
	}
}
void procNKey(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'l':
		light = !light;
		light ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		break;
	}
}