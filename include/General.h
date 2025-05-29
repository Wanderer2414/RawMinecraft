#ifndef GENERAL_H
#define GENERAL_H
#include "Ray.h"
#include "Rect3D.h"

bool isIntersect(const MyBase3D::Ray3f& ray, const MyBase3D::Rect3f& rectangle);

float abs(const sf::Vector2f& vector);

float operator*(const glm::vec3& a, const glm::vec3& b);

glm::vec3 det(const glm::vec3& a, const glm::vec3& b);

#endif