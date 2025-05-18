#include "General.h"
#include "Global.h"

float abs(const Vector2f& v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

float abs(const Vector3f& v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

float operator*(const Vector3f& a, const Vector3f& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3f det(const Vector3f& a, const Vector3f& b) {
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}
void glVertex3f(const Vector3f& position) {
    glVertex3f(position.x, position.y, position.z);
}