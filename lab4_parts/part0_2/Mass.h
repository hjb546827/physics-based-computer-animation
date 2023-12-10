#pragma once
#pragma once
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>
#include <vector>
class Mass {
private:
	float x;
	float v;
	float a;
	float f;
	float m;
	float t;
	bool pinned = false;

public:
	Mass() {
		this->x = 0;
		this->v = 0;
		this->a = 0;
		this->f = 0;
		this->m = 1;
		this->t = 0.001f;
	}
	Mass(float x, float m, float t) {
		this->x = x;
		this->m = m;
		this->t = t;
		this->v = 0;
		this->a = 0;
		this->f = 0;
	}
	void clearForce() {
		f = 0;
	}
	void addForce(float force) {
		f += force;
	}
	void EulerSimulate() {
		if (!pinned) {
			a = f / m;
			v += a * t;
			x += v * t;
		}
		else
		{
			a = 0;
			v = 0;
		}
	}
	float getPos() {
		return x;
	}
	float getVelocity() {
		return v;
	}
	void setVelocity(float velocity) {
		v = velocity;
	}
	void pin() {
		pinned = true;
	}
};