#ifndef GENERAL_H
#define GENERAL_H
#include "Global.h"
#include "Ray.h"
#include "Rect3D.h"

bool isIntersect(const Ray3f& ray, const Rect3f& rectangle);

float abs(const Vector2f& vector);
float abs(const Vector3f& vector);

float operator*(const Vector3f& a, const Vector3f& b);

Vector3f det(const Vector3f& a, const Vector3f& b);

#endif