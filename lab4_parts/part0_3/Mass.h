#pragma once
#include "vec.h"

class Mass {
private:
	Vector2 x;
	Vector2 v;
	Vector2 a;
	Vector2 f;
	float m;
	float t;
	bool pinned = false;

public:
	Mass() {
		this->x = Vector2();
		this->v = Vector2();
		this->a = Vector2();
		this->f = Vector2();
		this->m = 1;
		this->t = 0.001f;
	}
	Mass(Vector2 x, float m, float t) {
		this->x = x;
		this->m = m;
		this->t = t;
		this->v = Vector2();
		this->a = Vector2();
		this->f = Vector2();
	}
	void clearForce() {
		f = Vector2();
	}
	void addForce(Vector2 force) {
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
			a = Vector2();
			v = Vector2();
		}
	}
	Vector2 getPos() {
		return x;
	}
	Vector2 getVelocity() {
		return v;
	}
	float getMass() {
		return m;
	}
	void setVelocity(Vector2 velocity) {
		v = velocity;
	}
	void pin() {
		pinned = true;
	}
};