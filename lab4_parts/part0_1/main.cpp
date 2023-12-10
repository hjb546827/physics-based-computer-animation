/**************************************************************************

  File: Lesson40.cpp

  Based on Jeff Molofee's Basecode Example

  Modified by Erkin Tunca for nehe.gamedev.net

**************************************************************************/

#include <glut.h>														// Header File For NeHeGL
#include <stdio.h>
#include <stdlib.h>
#include "Physics2.h"														// Header File For Physics2.h


/*
  class RopeSimulation is derived from class Simulation (see Physics1.h). It simulates a rope with
  point-like particles binded with springs. The springs have inner friction and normal length. One tip of
  the rope is stabilized at a point in space called "Vector3D ropeConnectionPos". This point can be
  moved externally by a method "void setRopeConnectionVel(Vector3D ropeConnectionVel)". RopeSimulation
  creates air friction and a planer surface (or ground) with a normal in +y direction. RopeSimulation
  implements the force applied by this surface. In the code, the surface is refered as "ground".
*/
RopeSimulation* ropeSimulation = new RopeSimulation(
	80,						// 80 Particles (Masses)
	0.05f,					// Each Particle Has A Weight Of 50 Grams
	500.0f,				// springConstant In The Rope
	0.05f,					// Normal Length Of Springs In The Rope
	0.2f,					// Spring Inner Friction Constant
	Vector3D(0, -9.81f, 0)); // Gravitational Acceleration

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	ropeSimulation->getMass(ropeSimulation->numOfMasses - 1)->vel.z = 10.0f;

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);									// Black Background
	glClearDepth(1.0f);													// Depth Buffer Setup
	glShadeModel(GL_SMOOTH);												// Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);						// Set Perspective Calculations To Most Accurate

	return true;										// Initialization Went OK

}

void Deinitialize(void)													// Any User DeInitialization Goes Here
{
	ropeSimulation->release();												// Release The ropeSimulation
	delete(ropeSimulation);													// Delete The ropeSimulation
	ropeSimulation = NULL;
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

}

void Update()											// Perform Motion Updates Here
{
	ropeSimulation->operate(0.001);
}

void renderScene(void) {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();														// Reset The Modelview Matrix

	// Position Camera 40 Meters Up In Z-Direction.
	// Set The Up Vector In Y-Direction So That +X Directs To Right And +Y Directs To Up On The Window.
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					// Clear Screen And Depth Buffer

	//update 
	Update();

	// Start Drawing The Rope.
	glColor3ub(255, 255, 0);												// Set Color To Yellow
	for (int a = 0; a < ropeSimulation->numOfMasses - 1; ++a)
	{
		Mass* mass1 = ropeSimulation->getMass(a);
		Vector3D* pos1 = &mass1->pos;

		Mass* mass2 = ropeSimulation->getMass(a + 1);
		Vector3D* pos2 = &mass2->pos;

		glLineWidth(4);
		glBegin(GL_LINES);
		glVertex3f(pos1->x, pos1->y, pos1->z);
		glVertex3f(pos2->x, pos2->y, pos2->z);
		glEnd();
	}
	// Drawing The Rope Ends Here.

	glFlush();																// Flush The GL Rendering Pipeline

	glutSwapBuffers();

}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 's') {
		Vector3D ropeConnectionVel;
		ropeConnectionVel.x = 0.0;
		ropeConnectionVel.y = 0.0;
		ropeConnectionVel.z = 0.0;
		ropeSimulation->setRopeConnectionVel(ropeConnectionVel);
	}

	if (key == 27) {


		// Application Is Finished
		Deinitialize();											// User Defined DeInitialization

		exit(0);

	}
}

void processSpecialKeys(int key, int x, int y) {

	Vector3D ropeConnectionVel;												// Create A Temporary Vector3D
	switch (key) {
	case GLUT_KEY_RIGHT:                                              // Is The Right Arrow Being Pressed?
		ropeConnectionVel.x += 1.0f;							          // Add Velocity In +X Direction   
		break;
	case GLUT_KEY_LEFT:                                                    // Is The Left Arrow Being Pressed?
		ropeConnectionVel.x -= 1.0f;										// Add Velocity In -X Direction
		break;
	case GLUT_KEY_UP:
		ropeConnectionVel.y += 1.0f;										// Add Velocity In +Z Direction
		break;
	case GLUT_KEY_DOWN:
		ropeConnectionVel.y -= 1.0f;										// Add Velocity In -Z Direction
		break;
	case GLUT_KEY_PAGE_UP:
		ropeConnectionVel.z += 1.0f;										// Add Velocity In +Y Direction
		break;
	case GLUT_KEY_PAGE_DOWN:
		ropeConnectionVel.z -= 1.0f;										// Add Velocity In +Y Direction
		break;
	}

	ropeSimulation->setRopeConnectionVel(ropeConnectionVel);				// Set The Obtained ropeConnectionVel In The Simulation

}

int main(int argc, char** argv) {
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Rope Simulation");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	if (!InitGL()) {
		printf("Initialization Failed.");
		return false;
	}

	// process key
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;

}
