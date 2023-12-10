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
	const Vector2 gravity = Vector2(0, -9800);

public:
	Spring(const int n, const float t, const float ks, const float kd, const Vector2 origin) {
		this->n = n;
		this->t = t;
		this->ks = ks;
		this->kd = kd;
		this->l = 100.0f / (float)n;

		mass.resize(n);
		for (int i = 0; i < mass.size(); i++) {
			mass[i] = Mass(Vector2(origin.x + i * l, origin.y), 1, t);
		}
		//mass[n - 1].setVelocity(Vector2(0, 4000));
	}
	void update() {
		for (int i = 1; i < n; i++) {
			Vector2 x1 = mass[i - 1].getPos();
			Vector2 x2 = mass[i].getPos();
			Vector2 v1 = mass[i - 1].getVelocity();
			Vector2 v2 = mass[i].getVelocity();
			Vector2 f1 = ks * ((x2 - x1).magnitude() - l) * (x2 - x1).normalized() - kd * (v1 - v2);
			Vector2 f2 = -ks * ((x2 - x1).magnitude() - l) * (x2 - x1).normalized() - kd * (v2 - v1);
			mass[i - 1].addForce(f1);
			mass[i].addForce(f2);
			mass[i].addForce(gravity * mass[i].getMass());
		}
		for (int i = 0; i < n; i++) {
			mass[i].EulerSimulate();
		}
		for (int i = 0; i < n; i++)
			mass[i].clearForce();
	}
	Vector2 getPos(int i) {
		return mass[i].getPos();
	}
	void draw(float pointSize, float lineWidth) {
		glPointSize(pointSize);
		glBegin(GL_POINTS);
		for (int i = 0; i < n; i++) {
			glVertex2f(mass[i].getPos().x, mass[i].getPos().y);
		}
		glEnd();
		glLineWidth(lineWidth);
		glBegin(GL_LINES);
		for (int i = 0; i < n - 1; i++) {
			glVertex2f(mass[i].getPos().x, mass[i].getPos().y);
			glVertex2f(mass[i + 1].getPos().x, mass[i + 1].getPos().y);
		}
		glEnd();
	}
	void pin(int i) {
		mass[i].pin();
	}
};