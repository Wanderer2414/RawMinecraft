#include "General.h"
#include "Global.h"

float abs(const Vector2f& v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

float abs(const Vector3f& v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}