#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>
#include <vector>
#include "../../inc/SOIL.h"
#pragma comment (lib, "../../lib/SOIL.lib")

#define PI 3.141592653589
#define RESOLUTION 31

float amplitude = 0.01f;
float wavelength = 0.3f;
float speed = -0.2f;
float center[1][2] = { 0.0f, 0.0f };

static float surface[6 * RESOLUTION * (RESOLUTION + 1)];
static float normal[6 * RESOLUTION * (RESOLUTION + 1)];
bool wire_frame = false, normals = false;
int left_click = 0, right_click = 0, x_old, y_old;

GLfloat LightAmbient[] = { 0.9f, 0.9f, 0.9f, 1 };
GLfloat LightDiffuse[] = { 0.9f, 0.9f, 0.9f, 1 };
GLfloat LightPosition[] = { 1, 1, -0.5f, 0 };

struct Vector3 {
	float x, y, z;
};

static float	rotate_x = 30;
static float	rotate_y = 15;
static float	translate_z = 4;

float dot(float x, float y) {
	float cx = x - center[0][0];
	float cy = y - center[0][1];
	return sqrt(cx * cx + cy * cy);
}
float waveHeight(float x, float y, float time) {
	float frequency = 2.0f * PI / wavelength;
	float phase = speed * frequency;
	float theta = dot(x, y);

	return amplitude * sin(theta * frequency + time * phase);
}
float dWavedx(float x, float y, float time) {
	float frequency = 2.0f * PI / wavelength;
	float phase = speed * frequency;
	float theta = dot(x, y);
	float A = amplitude * x * frequency / theta;
	return A * cos(theta * frequency + time * phase);
}
float dWavedy(float x, float y, float time) {
	float frequency = 2.0f * PI / wavelength;
	float phase = speed * frequency;
	float theta = dot(x, y);
	float A = amplitude * y * frequency / theta;
	return A * cos(theta * frequency + time * phase);
}
Vector3 waveNormal(float x, float y, float time) {
	float dx = dWavedx(x, y, time);
	float dy = dWavedy(x, y, time);

	Vector3 n;
	n.x = -dx;
	n.y = 1.0f;
	n.z = -dy;

	float l = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
	if (l != 0) {
		n.x /= l;
		n.y /= l;
		n.z /= l;
	}
	else
	{
		n.x = 0;
		n.y = 1;
		n.z = 0;
	}

	return n;
}

void renderScene(void)
{
	const float t = glutGet(GLUT_ELAPSED_TIME) / 1000.;
	const float delta = 2. / RESOLUTION;
	const unsigned int length = 2 * (RESOLUTION + 1);
	const float xn = (RESOLUTION + 1) * delta + 1;
	unsigned int i;
	unsigned int j;
	float x;
	float y;
	unsigned int indice;
	unsigned int preindice;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0, 0, -translate_z);
	glRotatef(rotate_y, 1, 0, 0);
	glRotatef(rotate_x, 0, 1, 0);

	/* Vertices */
	for (j = 0; j < RESOLUTION; j++)
	{
		y = (j + 1) * delta - 1;
		for (i = 0; i <= RESOLUTION; i++)
		{
			indice = 6 * (i + j * (RESOLUTION + 1));

			x = i * delta - 1;
			surface[indice + 3] = x;
			surface[indice + 4] = waveHeight(x, y, t);
			surface[indice + 5] = y;
			if (j != 0)
			{
				/* Values were computed during the previous loop */
				preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
				surface[indice] = surface[preindice + 3];
				surface[indice + 1] = surface[preindice + 4];
				surface[indice + 2] = surface[preindice + 5];
			}
			else
			{
				surface[indice] = x;
				surface[indice + 1] = waveHeight(x, -1, t);
				surface[indice + 2] = -1;
			}
			/* Normals */
			Vector3 n = waveNormal(surface[indice], surface[indice + 2], t);
			normal[indice] = n.x;
			normal[indice + 1] = n.y;
			normal[indice + 2] = n.z;

			n = waveNormal(surface[indice + 3], surface[indice + 5], t);
			normal[indice + 3] = n.x;
			normal[indice + 4] = n.y;
			normal[indice + 5] = n.z;

		}
	}


	/* The ground */
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(1.0f, 0.9f, 0.7f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glEnd();

	glTranslatef(0, 0.2, 0);

	/* Render wireframe? */
	if (wire_frame != 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	/* The water */
	//glEnable(GL_TEXTURE_2D);
	glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normal);
	glVertexPointer(3, GL_FLOAT, 0, surface);
	for (i = 0; i < RESOLUTION; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i * length, length);

	/* Draw normals? */
	if (normals != 0)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		for (j = 0; j < RESOLUTION; j++)
			for (i = 0; i <= RESOLUTION; i++)
			{
				indice = 6 * (i + j * (RESOLUTION + 1));
				glVertex3fv(&(surface[indice]));
				glVertex3f(surface[indice] + normal[indice] / 50,
					surface[indice + 1] + normal[indice + 1] / 50,
					surface[indice + 2] + normal[indice + 2] / 50);
			}
		glEnd();
	}

	/* End */
	glutSwapBuffers();
	glutPostRedisplay();
}

void InitGL() {
	glClearColor(0, 0, 0, 0);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NEAREST);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}
void changeSize(int w, int h) {
	if (h == 0)	h = 1;
	float ratio = 1.0f * w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(20, ratio, 0.1f, 15);

	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}
void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q':
	case 27:
		exit(0);
		break;
	case 'l':
		wire_frame = !wire_frame;
		break;
	case 'n':
		normals = !normals;
		break;
	}
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON)
		left_click = state;
	if (button == GLUT_RIGHT_BUTTON)
		right_click = state;
	x_old = x;
	y_old = y;
}
void mouseMotion(int x, int y) {
	if (left_click == GLUT_DOWN) {
		rotate_y = rotate_y + (y - y_old) / 5.0f;
		rotate_x = rotate_x + (x - x_old) / 5.0f;
		if (rotate_y > 90)
			rotate_y = 90;
		if (rotate_y < -90)
			rotate_y = -90;
		glutPostRedisplay();
	}
	if (right_click == GLUT_DOWN) {
		translate_z += (y_old - y) / 50.0f;
		if (translate_z < 0.5f)
			translate_z = 0.5f;
		if (translate_z > 10)
			translate_z = 10;
		glutPostRedisplay();
	}
	x_old = x;
	y_old = y;
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Water");

	InitGL();

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	glutMainLoop();

	return 0;
}