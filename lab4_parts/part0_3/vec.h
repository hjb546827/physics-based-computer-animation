#pragma once
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>
#include <vector>

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
	float magnitude() {
		return sqrtf(x * x + y * y);
	}
	Vector2 normalized() {
		return Vector2(x / magnitude(), y / magnitude());
	}
	Vector2 operator-() const { return Vector2(-this->x, -this->y); }
};

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
inline Vector2 operator+(const Vector2& u, const Vector2& v) {
	return Vector2(u.x + v.x, u.y + v.y);
}
inline Vector2 operator-(const Vector2& u, const Vector2& v) {
	return Vector2(u.x - v.x, u.y - v.y);
}
inline Vector2 operator*(double t, const Vector2& v) {
	return Vector2(t * v.x, t * v.y);
}
inline Vector2 operator*(const Vector2& v, double t) {
	return t * v;
}
inline Vector2 operator/(Vector2 v, double t) {
	return (1 / t) * v;
}