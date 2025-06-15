#ifndef GENERAL_H
#define GENERAL_H
#include "Ray.h"
#include "Rect3D.h"
#include <ostream>

bool isIntersect(const MyBase3D::Ray3f& ray, const MyBase3D::Rect3f& rectangle);

float abs(const glm::vec2& vector);
float operator*(const glm::vec3& a, const glm::vec3& b);
glm::vec2 getMousePosition(GLFWwindow* window);
std::size_t GetTime();

glm::vec3 det(const glm::vec3& a, const glm::vec3& b);

std::queue<glm::vec3> rasterize(const glm::vec3& a, const glm::vec3& b);
std::queue<glm::vec3> rasterize(const glm::mat3& rec);

std::ostream& operator<<(std::ostream& cout, const glm::vec3& pos);
namespace MyBase {
    glm::vec2 getWindowSize();
}
namespace MyCraft {
    void DrawCube(const unsigned char& blockType, const glm::vec3& position);
}
#endif