#pragma once
#include "Quad.h"

class Mesh {
public:
	int n;
	float t;
	float kd;
	float ks;
	Vector3 origin;
	std::vector<Quad> quads;
	std::vector<Spring> springs;
public:
	Mesh(const int n, const float t, const float kd, const float ks, const Vector3 origin) {
		this->n = n;
		this->t = t;
		this->kd = kd;
		this->ks = ks;
		this->origin = origin;
		for(int i = 0; i < n; i++)
			quads.push_back(Quad(20, t, kd, ks, Vector3(0, 5 - i * 0.5f, 0)));

		springs.resize(quads[0].masses.size() * (n - 1));
		for (int i = 0; i < quads[0].masses.size(); i++) {
			springs[i] = Spring(t, kd, ks, quads[0].origin.y - quads[1].origin.y, i, i);
		}
	}
	void update() {
		for (int i = 0; i < quads.size(); i++) {
			quads[i].update(1);
		}
		for (int i = 0; i < springs.size(); i++) {
			for(int j = 0; j < quads.size() - 1; j++)
				springs[i].calc_f(quads[j].masses[springs[i].index1], quads[j + 1].masses[springs[i].index2]);
		}
		for (int i = 0; i < quads.size(); i++) {
			quads[i].update(2);
		}
		for (int i = 0; i < quads.size(); i++) {
			quads[i].update(3);
		}
	}
	void draw() {
		quads[0].draw();
		if (n > 1)
			quads[quads.size() - 1].draw();
		for (int i = 0; i < springs.size(); i++) {
			glLineWidth(0.1);
			glBegin(GL_LINES);
			for (int j = 0; j < quads.size() - 1; j++) {
				Vector3 x1 = quads[j].masses[springs[i].index1].x;
				Vector3 x2 = quads[j + 1].masses[springs[i].index1].x;
				glVertex3f(x1.x, x1.y, x1.z);
				glVertex3f(x2.x, x2.y, x2.z);
			}
			glEnd();
		}
	}
	void sphereCollision(float r, Vector3 c) {
		for (int i = 0; i < quads.size(); i++) {
			quads[i].sphereCollision(r, c);
		}
	}
	void pin(int i, int j) {
		quads[i].pin(j);
	}
};