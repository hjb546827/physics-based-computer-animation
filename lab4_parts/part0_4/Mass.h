#pragma once
#include "vec.h"

class Mass {
public:
	Vector3 x;
	Vector3 v;
	float m;
private:
	Vector3 a;
	Vector3 f;
	float t;
	bool pinned = false;

public:
	Mass() {
		this->x = Vector3();
		this->v = Vector3();
		this->a = Vector3();
		this->f = Vector3();
		this->m = 1;
		this->t = 0.001f;
	}
	Mass(Vector3 x, float m, float t) {
		this->x = x;
		this->m = m;
		this->t = t;
		this->v = Vector3();
		this->a = Vector3();
		this->f = Vector3();
	}
	void clearForce() {
		f = Vector3();
	}
	void addForce(Vector3 force) {
		f = f + force;
	}
	void EulerSimulate() {
		if (!pinned) {
			a = f / m;
			v = v + a * t;
			x = x + v * t;
		}
		else
		{
			a = Vector3();
			v = Vector3();
		}
	}
	void sphereCollision(float r, Vector3 c) {
		if ((x - c).magnitude() < r) {
			v = v + (c + r * (x - c) / (x - c).magnitude() - x) / t;
			x = c + r * (x - c) / (x - c).magnitude();
		}
	}
	void setPos(Vector3 pos) {
		x = pos;
	}
	void setVelocity(Vector3 velocity) {
		v = velocity;
	}
	void pin() {
		pinned = true;
	}
};