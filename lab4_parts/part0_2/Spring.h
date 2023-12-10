#pragma once
#pragma once
#include "Mass.h"
class Spring {
private:
	std::vector<Mass> mass;
	int n;
	float t;
	float ks;
	float kd;
	float l;

public:
	Spring(const int n, const float t, const float ks, const float kd, const float origin) {
		this->n = n;
		this->t = t;
		this->ks = ks;
		this->kd = kd;
		this->l = 100.0f / (float)n;

		mass.resize(n);
		for (int i = 0; i < mass.size(); i++) {
			mass[i] = Mass(origin + i * l, 1, t);
		}
		mass[n - 1].setVelocity(4000);
	}
	void update() {
		for (int i = 1; i < n; i++) {
			float f1 = ks * (mass[i].getPos() - mass[i - 1].getPos() - l) - kd * (mass[i - 1].getVelocity() - mass[i].getVelocity());
			float f2 = -ks * (mass[i].getPos() - mass[i - 1].getPos() - l) - kd * (mass[i].getVelocity() - mass[i - 1].getVelocity());
			mass[i - 1].addForce(f1);
			mass[i].addForce(f2);
		}
		for (int i = 0; i < n; i++) {
			mass[i].EulerSimulate();
		}
		for (int i = 0; i < n; i++)
			mass[i].clearForce();
	}
	float getPos(int i) {
		return mass[i].getPos();
	}
	void draw(float pointSize, float lineWidth) {
		glPointSize(pointSize);
		glBegin(GL_POINTS);
		for (int i = 0; i < n; i++) {
			glVertex2f(mass[i].getPos(), 0);
		}
		glEnd();
		glLineWidth(lineWidth);
		glBegin(GL_LINES);
		for (int i = 0; i < n - 1; i++) {
			glVertex2f(mass[i].getPos(), 0);
			glVertex2f(mass[i + 1].getPos(), 0);
		}
		glEnd();
	}
	void pin(int i) {
		mass[i].pin();
	}
};