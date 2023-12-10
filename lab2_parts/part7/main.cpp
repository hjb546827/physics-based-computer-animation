//#define USE_IMGUI	/* 取消注释以启用ImGui */

#include <glut.h>
#include <stdlib.h>
#include <SOIL.h>
#include <stdio.h>
#ifdef USE_IMGUI
#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_impl_glut.h"
#include "../../ImGui/imgui_impl_opengl2.h"
#endif // USE_IMGUI

GLfloat xrot;
GLfloat yrot;
GLfloat xspeed, speed_x = 0.01f;
GLfloat yspeed, speed_y = 0.01f;
GLfloat z = -5;
GLboolean light = true;
GLfloat LightAmbient[] = { 1, 1, 1, 1 };
GLfloat LightDiffuse[] = { 1, 1, 1, 1 };
GLfloat LightPosition[] = { 0, 0, 2, 1 };
GLfloat gray[] = { 0.9f, 0, 0, 1 };
float width = 800, height = 600;

void reSize(int w, int h);
int initGL(GLvoid);
void renderScene(void);
void procSKey(int key, int x, int y);
void procNKey(unsigned char key, int x, int y);
int LoadGLTextures();
bool useSolution1 = false;
GLuint filter;
GLuint texture[2];
#ifdef USE_IMGUI
void initImGui();
void setImGui();
void renderImGui();
void shutImGui();
#endif // USE_IMGUI

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("实验二");
#ifdef USE_IMGUI
	initImGui();
#endif // USE_IMGUI

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(reSize);
	glutSpecialFunc(procSKey);
	glutKeyboardFunc(procNKey);

	initGL();

	glutMainLoop();
#ifdef USE_IMGUI
	shutImGui();
#endif // USE_IMGUI

	return 0;
}

void reSize(int w, int h) {
#ifdef USE_IMGUI
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);
#endif // USE_IMGUI
	if (h == 0)	h = 1;
	float ratio = w * 1.0f / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
}
int initGL(GLvoid) {
	if (!LoadGLTextures())
		return false;
	glEnable(GL_TEXTURE_2D);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	if (useSolution1)
		glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
	else {
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	}

	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0.5f);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return true;
}
void renderScene(void) {
#ifdef USE_IMGUI
	setImGui();
#endif // USE_IMGUI
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, z);

	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, texture[filter]);

	if (useSolution1)
		glColor4f(0.9f, 0, 0, 1);
	else
		glColor4f(0, 0, 0.9f, 1);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(1, -1, 1);
	glTexCoord2f(1, 1); glVertex3f(1, 1, 1);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);

	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, -1);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
	glTexCoord2f(1, 0); glVertex3f(1, 1, 1);
	glTexCoord2f(1, 1); glVertex3f(1, 1, -1);

	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);

	glNormal3f(1, 0, 0);
	glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, 1, -1);
	glTexCoord2f(1, 0); glVertex3f(1, 1, 1);
	glTexCoord2f(1, 1); glVertex3f(1, -1, 1);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
	glEnd();

	//glutSolidCube(1);
#ifdef USE_IMGUI
	renderImGui();
#endif // USE_IMGUI
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
		xspeed -= speed_x;
		break;
	case GLUT_KEY_DOWN:
		xspeed += speed_x;
		break;
	case GLUT_KEY_LEFT:
		yspeed -= speed_y;
		break;
	case GLUT_KEY_RIGHT:
		yspeed += speed_y;
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
	case 'm':
		useSolution1 = !useSolution1;
		break;
	case 'f':
		filter += 1;
		if (filter > 1)	filter = 0;
		printf("%d", filter);
		break;
	}
}
int LoadGLTextures() {
	for (int i = 0; i < 2; i++) {
		texture[i] = SOIL_load_OGL_texture(
			"../tex/floor.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
		);
	}
	if (texture[0] == 0)
		return false;
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (texture[1] == 0)
		return false;
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return true;
}
#ifdef USE_IMGUI
void initImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);
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
	ImGui::Text("color:");
	ImGui::ColorEdit4("##1", )
	ImGui::End();
}
#endif // USE_IMGUI