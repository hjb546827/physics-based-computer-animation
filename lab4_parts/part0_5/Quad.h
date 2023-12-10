#pragma once
#include "Spring.h"

class Quad {
public:
	int n;
	float t;
	float kd;
	float ks;
	Vector3 origin;
	std::vector<Spring> springs;
	std::vector<Mass> masses;
private:
	int nxn;
	std::vector<int> triangles;
	std::vector<int> edges;
public:
	Quad(const int n, const float t, const float kd, const float ks, const Vector3 origin) {
		this->n = n;
		this->t = t;
		this->kd = kd;
		this->ks = ks;
		this->origin = origin;
		this->nxn = n * n;
		triangles.resize(6 * (n - 1) * (n - 1));
		edges.resize(nxn);
		masses.resize(nxn);
		for (int i = 0; i < masses.size(); i++)
			masses[i] = Mass(Vector3(), 1, t);

		int tmp = 0;
		for (int j = 0; j < n - 1; j++)
			for (int i = 0; i < n - 1; i++)
			{
				triangles[tmp * 6 + 0] = j * n + i;
				triangles[tmp * 6 + 1] = j * n + i + 1;
				triangles[tmp * 6 + 2] = (j + 1) * n + i + 1;
				triangles[tmp * 6 + 3] = j * n + i;
				triangles[tmp * 6 + 4] = (j + 1) * n + i + 1;
				triangles[tmp * 6 + 5] = (j + 1) * n + i;
				tmp++;
			}

		std::vector<int> _edges;
		_edges.resize(triangles.size() * 2);
		for (int i = 0; i < triangles.size(); i += 3)
		{
			_edges[i * 2 + 0] = triangles[i + 0];
			_edges[i * 2 + 1] = triangles[i + 1];
			_edges[i * 2 + 2] = triangles[i + 1];
			_edges[i * 2 + 3] = triangles[i + 2];
			_edges[i * 2 + 4] = triangles[i + 2];
			_edges[i * 2 + 5] = triangles[i + 0];
		}
		for (int i = 0; i < _edges.size(); i += 2)
			if (_edges[i] > _edges[i + 1])
				std::swap(_edges[i], _edges[i + 1]);
		Quick_Sort(_edges, 0, triangles.size() - 1);

		int e_number = 0;
		for (int i = 0; i < _edges.size(); i += 2)
			if (i == 0 || _edges[i + 0] != _edges[i - 2] || _edges[i + 1] != _edges[i - 1])
				e_number++;

		edges.resize(e_number * 2);
		for (int i = 0, e = 0; i < _edges.size(); i += 2)
			if (i == 0 || _edges[i + 0] != _edges[i - 2] || _edges[i + 1] != _edges[i - 1])
			{
				edges[e * 2 + 0] = _edges[i + 0];
				edges[e * 2 + 1] = _edges[i + 1];
				e++;
			}

		std::vector<Vector3> x_tmp;
		x_tmp.resize(nxn);
		for (int j = 0; j < n; j++)
			for (int i = 0; i < n; i++) {
				x_tmp[j * n + i] = Vector3(origin.x + 5.0f - 10.0f * (float)i / (float)(n - 1.0f), origin.y, origin.z - 10.0f * (float)j / (float)(n - 1.0f));
			}

		springs.resize(edges.size() / 2);
		for (int i = 0; i < springs.size(); i++) {
			int index1 = edges[i * 2];
			int index2 = edges[i * 2 + 1];
			Vector3 tmp(x_tmp[index1].x - x_tmp[index2].x, x_tmp[index1].y - x_tmp[index2].y, x_tmp[index1].z - x_tmp[index2].z);
			float l = tmp.magnitude();
			springs[i] = Spring(this->t, kd, ks, l, index1, index2);
		}

		for (int i = 0; i < springs.size(); i++) {
			springs[i].setPos(x_tmp[springs[i].index1], x_tmp[springs[i].index2], masses[springs[i].index1], masses[springs[i].index2]);
		}
	}
	void update(int step) {
		if (step == 1) {
			for (int i = 0; i < springs.size(); i++) {
				springs[i].calc_f(masses[springs[i].index1], masses[springs[i].index2]);
			}
		}
		if (step == 2) {
			for (int i = 0; i < masses.size(); i++) {
				Spring::calc_g(masses[i]);
			}
		}
		if (step == 3) {
			for (int i = 0; i < masses.size(); i++) {
				Spring::update(masses[i]);
			}
		}
	}
	void draw() {
		for (int i = 0; i < masses.size(); i++) {
			glPointSize(2);
			glBegin(GL_POINTS);
			{
				Vector3 p1 = masses[i].x;
				glVertex3f(p1.x, p1.y, p1.z);
			}
			glEnd();
		}
		for (int i = 0; i < springs.size(); i++) {
			glLineWidth(0.1);
			glBegin(GL_LINES);
			Vector3 x1 = masses[springs[i].index1].x;
			Vector3 x2 = masses[springs[i].index2].x;
			glVertex3f(x1.x, x1.y, x1.z);
			glVertex3f(x2.x, x2.y, x2.z);
			glEnd();
		}
	}
	void sphereCollision(float r, Vector3 c) {
		for (int i = 0; i < masses.size(); i++) {
			masses[i].sphereCollision(r, c);
		}
	}
	void pin(int i) {
		masses[i].pin();
	}

private:
	int  Quick_Sort_Partition(std::vector<int> a, int l, int r)
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
			std::swap(a[i * 2], a[j * 2]);
			std::swap(a[i * 2 + 1], a[j * 2 + 1]);
		}
		std::swap(a[l * 2 + 0], a[j * 2 + 0]);
		std::swap(a[l * 2 + 1], a[j * 2 + 1]);
		return j;
	}
	void Quick_Sort(std::vector<int> a, int l, int r)
	{
		int j;
		if (l < r)
		{
			j = Quick_Sort_Partition(a, l, r);
			Quick_Sort(a, l, j - 1);
			Quick_Sort(a, j + 1, r);
		}
	}
};