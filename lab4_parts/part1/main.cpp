#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>
#include <vector>
#include "../../inc/SOIL.h"
#pragma comment (lib, "E:/OpenGL/Lab/Lab_c/lib/SOIL.lib")

#define USE_IMGUI
#ifdef USE_IMGUI
#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_impl_glut.h"
#include "../../ImGui/imgui_impl_opengl2.h"
#endif // USE_IMGUI

float windowHeight = 640, windowWidth = 640;

const float t = 0.0333f;
const float mass = 1;
const float rho = 0.995f;
const float spring_k = 8000;
const float gravity = 1.9f;
const int iter = 32;

float x_pos = 0;
float z_pos = 30.0f;
float y_pos = 0;
float lx = 0, lz = -1, ly = 0;
float angle = 0;
float rotate_y = 0;

GLboolean light = true;
GLfloat LightAmbient[] = { 1, 1, 1, 1 };
GLfloat LightDiffuse[] = { 1, 1, 1, 1 };
GLfloat LightPosition[] = { 0, 100, -100, 1 };
GLfloat LightPosition2[] = { 0, 100, 100, 1 };
GLfloat gray[] = { 0.9f, 0, 0, 1 };

struct Vector3 {
	float x, y, z;
	Vector3() { this->x = 0; this->y = 0; this->z = 0; }
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float magnitude() {
		return sqrtf(x * x + y * y + z * z);
	}
	Vector3 normalized() {
		return Vector3(x / magnitude(), y / magnitude(), z / magnitude());
	}
	Vector3 operator-() const { return Vector3(-this->x, -this->y, -this->z); }
};
struct Vector2
{
	float x, y;
	Vector2() { this->x = 0.0f; this->y = 0.0f; }
	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
};
const int n = 30;
const int n2 = n * n;
std::vector<Vector3>	E(n2);
std::vector<Vector3>	L(n2);
std::vector<Vector3> 	V(n2);
std::vector<Vector3>	velo(n2);
std::vector<Vector3>	deltaX(n2);
std::vector<Vector3>	last_dX(n2);
std::vector<Vector3>	old_dX(n2);
std::vector<Vector3>	X(n2);
std::vector<Vector2>	UV(n2);
const int triangle_length = 6 * (n - 1) * (n - 1);
int		triangles[triangle_length];

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
Sphere sphere(Vector3(0, -6, -2), 1.6f);

GLint texture[1];

#ifdef USE_IMGUI
void initImGui();
void setImGui();
void renderImGui();
void shutImGui();
#endif

inline Vector3 operator+(const Vector3& u, const Vector3& v) {
	return Vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}
inline Vector3 operator-(const Vector3& u, const Vector3& v) {
	return Vector3(u.x - v.x, u.y - v.y, u.z - v.z);
}
inline Vector3 operator*(double t, const Vector3& v) {
	return Vector3(t * v.x, t * v.y, t * v.z);
}

inline Vector3 operator*(const Vector3& v, double t) {
	return t * v;
}

inline Vector3 operator/(Vector3 v, double t) {
	return (1 / t) * v;
}
void Swap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}

int  Quick_Sort_Partition(int a[], int l, int r)
{
	int pivot_0, pivot_1, i, j;
	pivot_0 = a[l * 2 + 0];
	pivot_1 = a[l * 2 + 1];
	i = l;
	j = r + 1;
	while (true)
	{
		do ++i; while (i <= r && (a[i * 2] < pivot_0 || a[i * 2] == pivot_0 && a[i * 2 + 1] <= pivot_1));
		do --j; while (a[j * 2] > pivot_0 || a[j * 2] == pivot_0 && a[j * 2 + 1] > pivot_1);
		if (i >= j)	break;
		Swap(a[i * 2], a[j * 2]);
		Swap(a[i * 2 + 1], a[j * 2 + 1]);
	}
	Swap(a[l * 2 + 0], a[j * 2 + 0]);
	Swap(a[l * 2 + 1], a[j * 2 + 1]);
	return j;
}


void Quick_Sort(int a[], int l, int r)
{
	int j;
	if (l < r)
	{
		j = Quick_Sort_Partition(a, l, r);
		Quick_Sort(a, l, j - 1);
		Quick_Sort(a, j + 1, r);
	}
}
void Collision_Handling()
{
	Vector3 c = sphere.position;
	float r = sphere.radius;

	for (int i = 0; i < X.size(); i++)
	{
		if ((X[i] - c).magnitude() < r)
		{
			velo[i] = velo[i] + (c + r * (X[i] - c) / (X[i] - c).magnitude() - X[i]) / t;
			X[i] = c + r * (X[i] - c) / (X[i] - c).magnitude();
		}
	}
}
void Get_Gradient(std::vector<Vector3> &X, std::vector<Vector3> &X_hat, float t, std::vector<Vector3> &G)
{
	for (int i = 0; i < X.size(); i++)
	{
		G[i] = mass / t / t * (X[i] - X_hat[i]) - Vector3(0, -gravity * mass, 0); ;
	}
	for (int i = 0; i < E.size(); i += 2)
	{
		Vector3 g = spring_k * ((X[(E[i].x)] - X[(E[i + 1].x)]).magnitude() - L[i / 2].x) * (X[(E[i].x)] - X[(E[i + 1].x)]).normalized();
		G[(E[i].x)] = G[E[i].x] + g;
		G[(E[i + 1].x)] = G[E[i + 1].x] - g;
	}
}

void start(){
	for(int j = 0; j < n; j++)
		for (int i = 0; i < n; i++) {
			X[j * n + i] = Vector3(5.0f - 10.0f * (float)i / (float)(n - 1.0f), 0.0f, 0.0f - 10.0f * (float)j / (float)(n - 1.0f));
			UV[j * n + i].x = (float)i / (float)(n - 1.0f);
			UV[j * n + i].y = (float)j / (float)(n - 1.0f);
		}
	int t = 0;
	for (int j = 0; j < n - 1; j++)
		for (int i = 0; i < n - 1; i++)
		{
			triangles[t * 6 + 0] = j * n + i;
			triangles[t * 6 + 1] = j * n + i + 1;
			triangles[t * 6 + 2] = (j + 1) * n + i + 1;
			triangles[t * 6 + 3] = j * n + i;
			triangles[t * 6 + 4] = (j + 1) * n + i + 1;
			triangles[t * 6 + 5] = (j + 1) * n + i;
			t++;
		}

	int _E[triangle_length * 2];
	for (int i = 0; i < triangle_length; i += 3)
	{
		_E[i * 2 + 0] = triangles[i + 0];
		_E[i * 2 + 1] = triangles[i + 1];
		_E[i * 2 + 2] = triangles[i + 1];
		_E[i * 2 + 3] = triangles[i + 2];
		_E[i * 2 + 4] = triangles[i + 2];
		_E[i * 2 + 5] = triangles[i + 0];
	}
	for (int i = 0; i < triangle_length * 2; i += 2)
		if (_E[i] > _E[i + 1])
			Swap(_E[i], _E[i + 1]);
	Quick_Sort(_E, 0, triangle_length - 1);

	int e_number = 0;
	for (int i = 0; i < triangle_length * 2; i += 2)
		if (i == 0 || _E[i + 0] != _E[i - 2] || _E[i + 1] != _E[i - 1])
			e_number++;

	E.resize(e_number * 2);
	for (int i = 0, e = 0; i < triangle_length * 2; i += 2)
		if (i == 0 || _E[i + 0] != _E[i - 2] || _E[i + 1] != _E[i - 1])
		{
			E[e * 2 + 0].x = _E[i + 0];
			E[e * 2 + 1].x = _E[i + 1];
			e++;
		}

	L.resize(E.size() / 2);
	for (int e = 0; e < E.size() / 2; e++)
	{
		int v0 = E[e * 2 + 0].x;
		int v1 = E[e * 2 + 1].x;
		Vector3 tmp(X[v0].x - X[v1].x, X[v0].y - X[v1].y, X[v0].z - X[v1].z);
		L[e].x = tmp.magnitude();
	}

	V.resize(X.size());
	for (int i = 0; i < V.size(); i++)
		V[i] = Vector3(0.0f, 0.0f, 0.0f);

	velo.resize(X.size());
	for (int i = 0; i < V.size(); i++)
		velo[i] = Vector3(0.0f, 0.0f, 0.0f);
	deltaX.resize(X.size());
	last_dX.resize(X.size());
	old_dX.resize(X.size());
}
void update()
{
	std::vector<Vector3> last_X(X.size());
	std::vector<Vector3> X_hat(X.size());
	std::vector<Vector3> G(X.size());

	for (int i = 0; i < X.size(); i++)
	{
		last_X[i] = X[i];
		X_hat[i] = X[i];
		X_hat[i] = X_hat[i] + t * velo[i];
		X[i] = X_hat[i];

		deltaX[i] = Vector3(0.0f, 0.0f, 0.0f);
		last_dX[i] = Vector3(0.0f, 0.0f, 0.0f);
	}

	for (int k = 0; k < iter; k++)
	{
		Get_Gradient(X, X_hat, t, G);

		for (int i = 0; i < X.size(); i++)
		{
			if (i == 0 || i == n - 1) continue;
			float H = mass / (t * t) + 4 * spring_k;
			float omega = 0;
			old_dX[i] = deltaX[i];
			deltaX[i] = deltaX[i] - deltaX[i] - G[i] / H;
			if (k == 0) omega = 1;
			else if (k == 1) omega = 2 / (2 - rho * rho);
			else omega = 4 / (4 - rho * rho * omega);
			deltaX[i] = omega * deltaX[i] + (1 - omega) * last_dX[i];
			last_dX[i] = old_dX[i];
			X[i] = X[i] + deltaX[i];
		}
	}

	for (int i = 0; i < X.size(); i++)
		velo[i] = velo[i] + 1.0f / t * (X[i] - X_hat[i]);

	Collision_Handling();
}

int LoadGLTextures() {
	texture[0] = SOIL_load_OGL_texture(
		"../../texture/images.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y
	);

	if (texture[0] == 0)
		return false;

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	return true;
}

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
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);		  // Clear screen and depth buffer
	
	glLoadIdentity();
	gluLookAt(
		x_pos, y_pos, z_pos,
		x_pos + lx, y_pos + ly, z_pos + lz,
		0, 1, 0
	);
	glRotatef(rotate_y, 0, 1, 0);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < X.size(); i++) {
		float x = X[i].x;
		float y = X[i].y;
		float z = X[i].z;
		glPointSize(2);
		glBegin(GL_POINTS);
		{
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(sphere.position.x, sphere.position.y, sphere.position.z);
	glColor4f(0.5f, 0.3f, 0.6f, 1);
	glutSolidSphere(sphere.radius - 0.1f, 50, 50);
	glPopMatrix();
#ifdef USE_IMGUI
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	renderImGui();
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
#endif
	glutSwapBuffers();

}

void TimerFunction(int value) {
	update();
	glutPostRedisplay();
	glutTimerFunc(30, TimerFunction, 1);
}

void SetupRC(void)
{
	if (!LoadGLTextures())
		return;
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
	glEnable(GL_TEXTURE_2D);
}
void procNKeys(unsigned char key, int x, int y);
int main(int argc, char** argv) {
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Elastic Mesh");

#ifdef USE_IMGUI
	initImGui();
#endif

	start();
	// register callbacks
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(10, TimerFunction, 1);
	glutKeyboardFunc(procNKeys);

	// Setup the rendering state
	SetupRC();

	// enter GLUT event processing loop
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
	ImGui::DragFloat("##1", &sphere.position.x, 0.01f, -5, 5, "%.2f");
	ImGui::Text("     y:");
	ImGui::SameLine();
	ImGui::DragFloat("##2", &sphere.position.y, 0.01f, -5, 15, "%.2f");
	ImGui::Text("     z:");
	ImGui::SameLine();
	ImGui::DragFloat("##3", &sphere.position.z, 0.01f, -15, 15, "%.2f");
	ImGui::Text("radius:");
	ImGui::SameLine();
	ImGui::DragFloat("##4", &sphere.radius, 0.01f, 0.2f, 5, "%.2f");
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
		z_pos += 2;
		break;
	case 'W':
	case 'w':
		z_pos -= 2;
		break;
	case 'Q':
	case 'q':
		rotate_y += 10;
		break;
	case 'E':
	case 'e':
		rotate_y -= 10;
		break;
	}
}