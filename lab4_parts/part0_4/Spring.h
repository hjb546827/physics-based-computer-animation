#pragma once
#include "Mass.h"

Vector3 gravity = Vector3(0, -9.8, 0);

class Spring {
private:
	float t;
	float ks;
	float kd;
	float l;
public:
	int index1;
	int index2;

public:
	Spring(){
		this->t = 1;
		this->ks = 0;
		this->kd = 0;
		this->l = 0;
		this->index1 = 0;
		this->index2 = 0;
	}
	Spring(float t, float ks, float kd, float l, int index1, int index2) {
		this->t = t;
		this->ks = ks;
		this->kd = kd;
		this->l = l;
		this->index1 = index1;
		this->index2 = index2;
	}
	void calc_f(Mass& m1, Mass& m2) {
		Vector3 x1 = m1.x;
		Vector3 x2 = m2.x;
		Vector3 v1 = m1.v;
		Vector3 v2 = m2.v;
		Vector3 f1 = ks * ((x2 - x1).magnitude() - l) * (x2 - x1).normalized() - kd * (v1 - v2);
		Vector3 f2 = -ks * ((x2 - x1).magnitude() - l) * (x2 - x1).normalized() - kd * (v2 - v1);

		m1.addForce(f1);
		m2.addForce(f2);
	}
	static void calc_g(Mass& m) {
		m.addForce(gravity * m.m);
	}
	static void update(Mass& m) {
		m.EulerSimulate();
		m.clearForce();
	}
	void setPos(Vector3 x1, Vector3 x2, Mass& m1, Mass& m2) {
		m1.setPos(x1);
		m2.setPos(x2);
	}
};