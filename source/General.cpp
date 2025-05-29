#include "General.h"
#include "Global.h"

float abs(const sf::Vector2f& v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

float operator*(const glm::vec3& a, const glm::vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

glm::vec3 det(const glm::vec3& a, const glm::vec3& b) {
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}