#define USE_IMGUI	/* 取消注释以启用ImGui */

#include <glut.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#ifdef USE_IMGUI
	#include "../../ImGui/imgui.h"
	#include "../../ImGui/imgui_impl_glut.h"
	#include "../../ImGui/imgui_impl_opengl2.h"
#endif // USE_IMGUI

using namespace std;

// global variables
static int frameTimer;
static float currentTime;
static float last;
char strFrameRate[50];		// fps count
static GLfloat rotation;
GLfloat x = 0, y = 0, rsize = 25;
GLfloat xstep = 1, ystep = 1;
GLfloat tx = 0, ty = 0, tsize = 25;
GLfloat t_left, t_right, t_top, t_bottom;
GLint dir[2] = { 1, 0 };
GLfloat txstep = 1, tystep = 1;
GLfloat windowWidth, windowHeight;
GLfloat r = 0, g = 1, b = 1;
GLfloat speed[2] = { 2, 2 };
// statements
void renderScene();
void clear();
void resize(int width, int height);
void timeCount(int value);
void procIOEvents(unsigned char key, int x, int y);
void procSIOEvents(int key, int x, int y);
#ifdef USE_IMGUI
	void initImGui();
	void setImGui();
	void renderImGui();
	void shutImGui();
#endif // USE_IMGUI

int main(int argc, char* argv[]) {
	// Init
	last = 0;
	rotation = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("实验一");

#ifdef USE_IMGUI
	initImGui();
#endif // USE_IMGUI

	// loop
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);

	glutTimerFunc(33, timeCount, 1);
	glutKeyboardFunc(procIOEvents);
	glutSpecialFunc(procSIOEvents);

	clear();

	glutMainLoop();

#ifdef USE_IMGUI
	shutImGui();
#endif // USE_IMGUI

	return 0;
}

void renderScene() {
#ifdef USE_IMGUI
	setImGui();
#endif // USE_IMGUI

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

	t_left = -windowWidth + tsize;
	t_right = windowWidth - tsize;
	t_top = windowHeight - tsize;
	t_bottom = -windowHeight + tsize;
	if ((tx < t_right && dir[0] > 0) || (tx <= t_left)) {
		dir[0] = 1;
	}
	else {
		dir[0] = -1;
	}
	if ((ty < t_top && dir[1] > 0) || (ty <= t_bottom)) {
		dir[1] = 1;
	}
	else {
		dir[1] = -1;
	}
	tx += dir[0] * speed[0];
	ty += dir[1] * speed[1];
	
	glPushMatrix();
	//glRotatef(rotation, 0, 0, 1);
	glTranslatef(tx, ty, 0);
	// set object color
	glColor3f(r, g, b);
	// IA set
	glBegin(GL_TRIANGLES);
	glVertex3f(-tsize, -tsize, 0);
	glVertex3f(tsize, 0, 0);
	glVertex3f(0, tsize, 0);
	glEnd();

	glPopMatrix();

#ifdef USE_IMGUI
	renderImGui();
#endif // USE_IMGUI

	// swap the screen buffer and back buffer
	glutSwapBuffers();
}
void clear() {
	glClearColor(0, 0, 0, 1);
}
void resize(int width, int height) {
#ifdef USE_IMGUI
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)width, (float)height);
#endif // USE_IMGUI

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
	}
}
void procSIOEvents(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_F1:
		dir[0] = -1;
		break;
	case GLUT_KEY_F2:
		dir[0] = 1;
		break;
	case GLUT_KEY_F3:
		dir[1] = -1;
		break;
	case GLUT_KEY_F4:
		dir[1] = 1;
		break;
	case GLUT_KEY_F5:
		speed[0] += 3;
		break;
	case GLUT_KEY_F6:
		speed[1] += 3;
		break;
	case GLUT_KEY_F7:
		speed[0] *= 0.5;
		break;
	case GLUT_KEY_F8:
		speed[1] *= 0.5;
		break;
	}
}

#ifdef USE_IMGUI
void initImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::StyleColorsDark();
	ImGui_ImplGLUT_Init();
	ImGui_ImplOpenGL2_Init();

	glutMotionFunc(ImGui_ImplGLUT_MotionFunc);
	glutPassiveMotionFunc(ImGui_ImplGLUT_MotionFunc);
	glutMouseFunc(ImGui_ImplGLUT_MouseFunc);
}
void renderImGui() {
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
void shutImGui() {
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}
void setImGui() {
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	ImGui::Begin("Settings");
	ImGui::Text("dir_x: ");
	ImGui::SameLine(80, 0);
	ImGui::SliderInt("##1", &dir[0], -1, 1, "%d");
	ImGui::Text("dir_y: ");
	ImGui::SameLine(80, 0);
	ImGui::SliderInt("##2", &dir[1], -1, 1, "%d");
	ImGui::Text("speed_x: ");
	ImGui::SameLine();
	ImGui::SliderFloat("##3", &speed[0], 0, 6, "%.2f");
	ImGui::Text("speed_y: ");
	ImGui::SameLine();
	ImGui::SliderFloat("##4", &speed[1], 0, 6, "%.2f");
	ImGui::End();
	ImGuiWindowFlags wf = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("Tips", (bool *)0, wf);
	ImGui::Text("Use 0-7 to change the color of the triangle");
	ImGui::Text("Use F1-F4 to change the direction of the triangle");
	ImGui::Text("Use F5-F8 to celerate or decelerate the triangle");
	ImGui::End();
}
#endif // USE_IMGUI