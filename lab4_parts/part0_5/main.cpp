#include "Mesh.h"
#include "../../inc/SOIL.h"
#pragma comment (lib, "../../lib/SOIL.lib")

#define USE_IMGUI
#ifdef USE_IMGUI
#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_impl_glut.h"
#include "../../ImGui/imgui_impl_opengl2.h"
#endif

float windowHeight = 640, windowWidth = 640;

float x_pos = 0;
float z_pos = 30.0f;
float y_pos = 0;
float lx = 0, lz = -1, ly = 0;
float angle = 0;
float rotate_y = 0;

GLfloat LightAmbient[] = { 1, 1, 1, 1 };
GLfloat LightDiffuse[] = { 1, 1, 1, 1 };
GLfloat LightPosition[] = { -50, 100, -100, 1 };
GLfloat LightPosition2[] = { 50, 100, 100, 1 };
GLfloat gray[] = { 0.9f, 0, 0, 1 };

struct Sphere
{
	Vector3 position;
	float radius;
	Sphere() { this->position = Vector3(0.0f, 0.0f, 0.0f); this->radius = 1.0f; }
	Sphere(Vector3 position, float radius) {
		this->position = position;
		this->radius = radius;
	}
};
Sphere sphere(Vector3(0, -2, -2), 1.6f);

Mesh mesh = Mesh(5, 0.002f, 3000, 50, Vector3(0, 5, 0));

#ifdef USE_IMGUI
void initImGui();
void setImGui();
void renderImGui();
void shutImGui();
#endif

void ChangeSize(int w, int h)
{
#ifdef USE_IMGUI
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);
#endif
	if (h == 0)	h = 1;
	float ratio = w * 1.0f / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void RenderScene(void) {
#ifdef USE_IMGUI
	setImGui();
#endif
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(
		x_pos, y_pos, z_pos,
		x_pos + lx, y_pos + ly, z_pos + lz,
		0, 1, 0
	);
	glRotatef(rotate_y, 0, 1, 0);
	glColor4f(0.3, 0.5, 1, 1);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);


	//mesh.pin(0, 0);
	//mesh.pin(0, mesh.quads[0].n - 1);
	//mesh.pin(0, mesh.quads[0].masses.size() - mesh.quads[0].n);
	//mesh.pin(0, mesh.quads[0].masses.size() - 1);
	for (int i = 0; i < mesh.quads[0].masses.size(); i++) {
		mesh.pin(0, i);
	}
	mesh.update();
	mesh.sphereCollision(sphere.radius, sphere.position);
	mesh.draw();

	glPushMatrix();
	glTranslatef(sphere.position.x, sphere.position.y, sphere.position.z);
	glColor4f(0.5f, 0.3f, 0.6f, 1);
	glutSolidSphere(sphere.radius - 0.1f, 50, 50);
	glPopMatrix();
#ifdef USE_IMGUI
	renderImGui();
#endif
	glutSwapBuffers();
}

void TimerFunction(int value) {
	glutPostRedisplay();
	glutTimerFunc(1, TimerFunction, 1);
}

void SetupRC(void)
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NEAREST);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, LightPosition2);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void procNKeys(unsigned char key, int x, int y);
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Elastic Mesh");
#ifdef USE_IMGUI
	initImGui();
#endif
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(30, TimerFunction, 1);
	glutKeyboardFunc(procNKeys);

	SetupRC();

	glutMainLoop();

#ifdef USE_IMGUI
	shutImGui();
#endif

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
	ImGuiIO& io = ImGui::GetIO();
	if (!ImGui::IsAnyItemActive())
	{
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
			x_pos -= 0.05f * io.MouseDelta.x;
			y_pos += 0.05f * io.MouseDelta.y;
		}
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
			lx += 0.02f * io.MouseDelta.x;
			ly -= 0.02f * io.MouseDelta.y;
		}
	}
	ImGui::Begin("Sphere", nullptr, ImGuiWindowFlags_NoBackground);
	ImGui::Text("position:");
	ImGui::Text("     x:");
	ImGui::SameLine();
	ImGui::DragFloat("##1", &sphere.position.x, 0.01f, -10, 10, "%.2f");
	ImGui::Text("     y:");
	ImGui::SameLine();
	ImGui::DragFloat("##2", &sphere.position.y, 0.01f, -15, 15, "%.2f");
	ImGui::Text("     z:");
	ImGui::SameLine();
	ImGui::DragFloat("##3", &sphere.position.z, 0.01f, -15, 15, "%.2f");
	ImGui::Text("radius:");
	ImGui::SameLine();
	ImGui::DragFloat("##4", &sphere.radius, 0.01f, 0.1f, 10, "%.2f");
	ImGui::End();
}
#endif
void procNKeys(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'S':
	case 's':
		z_pos += 1;
		break;
	case 'W':
	case 'w':
		z_pos -= 1;
		break;
	case 'Q':
	case 'q':
		rotate_y += 5;
		break;
	case 'E':
	case 'e':
		rotate_y -= 5;
		break;
	}
}