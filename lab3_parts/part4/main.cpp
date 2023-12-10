#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>
#include "../../inc/SOIL.h"
#pragma comment (lib, "../../lib/SOIL.lib")

#define USE_IMGUI	/* 取消注释以启用ImGui */
#ifdef USE_IMGUI
#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_impl_glut.h"
#include "../../ImGui/imgui_impl_opengl2.h"
#endif // USE_IMGUI


#define	MAX_PARTICLES	500        	// Number of particles to create
#define PI              3.14159f
#define RHO             50
#define TIME            0.01f

float windowHeight, windowWidth;
float life = 7.2f;

typedef struct						// Create a Structure for Particle
{
	float	life;					// Particle Life
	float   init_life;
	float	speed_aging;			// Aging speed

	float	r;						// Red value
	float	g;						// Green value
	float	b;						// Blue value

	float	x;						// X position
	float	y;						// Y position
	float	z;						// Z position

	float	v_x;					// X velocity
	float	v_y;					// Y velocity
	float	v_z;					// Z velocity

	float	a_x;					// X acceleration
	float	a_y;					// Y acceleration
	float	a_z;					// Z acceleration

	bool isNew;

}
particle;							// Particles structure

particle particles[MAX_PARTICLES];	// Particle array (Room for particle info)

GLint texture[1];

const float quad[8] = { -50, -50, 30, -50, 30, -100, -50, -100 };

float currentTime = 0, startTime = 0;

int frame = 0;

float blur = 0.2f, size = 0.2f;

#ifdef USE_IMGUI
void initImGui();
void setImGui();
void renderImGui();
void shutImGui();
#endif // USE_IMGUI

int LoadGLTextures() {
	texture[0] = SOIL_load_OGL_texture(
		"../../texture/1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y
	);
	if (texture[0] == 0)
		exit(2);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

//当窗口改变大小时由GLUT函数库调用
void ChangeSize(int w, int h)
{
#ifdef USE_IMGUI
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);
#endif // USE_IMGUI
	float aspectRatio;

	//防止被0所除
	if (h == 0) h = 1;

	//把视口设置为窗口的大小
	glViewport(0, 0, w, h);

	//重置坐标系统
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//建立剪裁区域（左、右、底、顶、近、远）
	aspectRatio = (float)w / (float)h;

	if (w <= h) {
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else {
		windowWidth = 100.0 * aspectRatio;
		windowHeight = 100;
		glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


int InitPaticleSystem(void)
{
	for (int i = 0; i < MAX_PARTICLES; i++)				         // Initials all particles
	{
		particles[i].init_life = life + rand() % 10 / 10.0;		    // Give All The Particles Full Life
		particles[i].life = particles[i].init_life;
		particles[i].speed_aging = TIME;

		particles[i].r = 1.0f;	                                // Set color for particle
		particles[i].g = 1.0f;
		particles[i].b = 1.0f;
		
		particles[i].x = 0.0f;								   // Set position for particle
		particles[i].y = -50.0f;
		particles[i].z = 0.0f;

		particles[i].v_x = 0;  // Set X Axis Speed And Direction for particle
		particles[i].v_y = 50;	  // Set Y Axis Speed And Direction for particle
		particles[i].v_z = 0;                // Set Z Axis Speed And Direction for particle

		particles[i].a_x = 0.0f;							// Set X Axis acceleration
		particles[i].a_y = -9.8f;							// Set Y Axis acceleration
		particles[i].a_z = 0.0f;							// Set Z Axis acceleration

		particles[i].isNew = true;
	}

	return true;

}

void RenderScene(void) {
#ifdef USE_IMGUI
	setImGui();
#endif // USE_IMGUI
	frame++;
	glClear(GL_DEPTH_BUFFER_BIT);		  // Clear screen and depth buffer

	for (int i = 0; i < MAX_PARTICLES; i++)					         // All particles
	{
		float x = particles[i].x;						        // Position of particle
		float y = particles[i].y;
		float z = particles[i].z;

		if (x >= quad[0] - 0.001f && x <= quad[2] + 0.001f && y >= quad[7] + 0.001f && y <= quad[1] - 0.001f) {
			particles[i].v_y *= -0.9f;
		}

		// Draw particle using RGB values, alpha value based on it's life
		glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].life);
		if (particles[i].isNew)
			glPointSize(4.0f);
		else
			glPointSize(size);
		glBegin(GL_POINTS);
		{
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	glColor4f(0, 0, 0, blur);
	glBegin(GL_QUADS);
	{
		glVertex3f(-100.0f, -100.0f, 0);
		glVertex3f(100.0f, -100.0f, 0);
		glVertex3f(100.0f, 100.0f, 0);
		glVertex3f(-100.0f, 100.0f, 0);
	}
	glEnd();

	glColor4f(0.3f, 0.7f, 0.9f, 1);
	glBegin(GL_QUADS);
	{
		glVertex3f(quad[0], quad[1], 0);
		glVertex3f(quad[2], quad[3], 0);
		glVertex3f(quad[4], quad[5], 0);
		glVertex3f(quad[6], quad[7], 0);
	}
	glEnd();
#ifdef USE_IMGUI
	renderImGui();
#endif // USE_IMGUI
	glutSwapBuffers();

}

void Update() {
	for (int i = 0; i < MAX_PARTICLES; i++)					// All The Particles
	{
		float theta, phi, rho;
		theta = (rand() % 360) * PI / 180;
		phi = 90 * PI / 180;
		rho = rand() % RHO;
		if (particles[i].v_y <= 1 && particles[i].v_y > 0 && particles[i].isNew) {
			particles[i].v_x = float(sinf(phi) * cosf(theta) * rho);
			particles[i].v_y = float(sinf(phi) * sin(theta) * rho);
			particles[i].v_z = float(cosf(phi) * rho);
			particles[i].r = (float)(rand() % 256) / 255.0f * 0.8f + 0.8f;
			particles[i].g = (float)(rand() % 256) / 255.0f;
			particles[i].b = (float)(rand() % 256) / 255.0f;
			particles[i].isNew = false;
		}
		particles[i].x += particles[i].v_x * TIME;         // update position of particle
		particles[i].y += particles[i].v_y * TIME;
		particles[i].z += particles[i].v_z * TIME;

		particles[i].v_x += particles[i].a_x * TIME;			// update velocity
		particles[i].v_y += particles[i].a_y * TIME;
		particles[i].v_z += particles[i].a_z * TIME;

		particles[i].life -= particles[i].speed_aging;		// reduce particles life

		currentTime = GetTickCount64() * 0.001f;
		if (particles[i].life < 0.0f && currentTime - startTime >= life + 2)					// if particle has reached end of life
		{
			InitPaticleSystem();
		}
	}
}

void TimerFunction(int value) {
	Update();
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

// Setup the rendering state
void SetupRC(void)
{
	if (!LoadGLTextures())
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
}

int main(int argc, char** argv) {
	startTime = GetTickCount64() * 0.001f;

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Particle system");

#ifdef USE_IMGUI
	initImGui();
#endif // USE_IMGUI

	// register callbacks
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(10, TimerFunction, 1);

	// Setup the rendering state
	SetupRC();
	InitPaticleSystem();

	// enter GLUT event processing loop
	glutMainLoop();

#ifdef USE_IMGUI
	shutImGui();
#endif // USE_IMGUI

	return 0;

}

#ifdef USE_IMGUI
void initImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(windowWidth, windowHeight);
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

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoBackground);
	ImGui::Text("blur factor: ");
	ImGui::SameLine(120, 0);
	ImGui::SliderFloat("##1", &blur, 0, 1.0f, "%.1f");
	ImGui::Text("sparkler size: ");
	ImGui::SameLine(120, 0);
	ImGui::SliderFloat("##2", &size, 0.1f, 5.0f, "%.1f");
	ImGui::Text("sparkler life: ");
	ImGui::SameLine(120, 0);
	ImGui::SliderFloat("##3", &life, 5.0f, 20.0f, "%.1f");
	ImGui::End();
}
#endif // USE_IMGUI