#ifndef GENERAL_H
#define GENERAL_H
#include "Global.h"
#include "Ray.h"
#include "Rect3D.h"

bool isIntersect(const Ray3f& ray, const Rect3f& rectangle);

float abs(const Vector2f& vector);
float abs(const glm::vec3& vector);

float operator*(const glm::vec3& a, const glm::vec3& b);

glm::vec3 det(const glm::vec3& a, const glm::vec3& b);

#endif