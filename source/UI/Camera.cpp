#include "Camera.h"
#include "General.h"

extern Vector2f WindowSize;
Camera::Camera() {
    pPosition = pCenter = {0, 0, 0};
    pAngle = pNear = pFar = 0;
}
Camera::~Camera() {

}

void Camera::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pAngle, WindowSize.x/WindowSize.y , pNear, pFar);
}

void Camera::move(const float& x, const float& y, const float& z) {
    Vector3f delta = x*getHorizontalVector();
    delta += y*(pCenter-pPosition)/abs(pCenter-pPosition);
    delta.z += z;
    pPosition += delta;
    pCenter += delta;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pPosition.x, pPosition.y, pPosition.z, pCenter.x, pCenter.y, pCenter.z, 0, 0, 1);
}
void Camera::setCenter(const float& x, const float& y, const float& z) {
    pCenter = {x, y, z};
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pPosition.x, pPosition.y, pPosition.z, pCenter.x, pCenter.y, pCenter.z, 0, 0, 1);
}
void Camera::setWide(const float& angle) {
    pAngle = angle;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pAngle, WindowSize.x/WindowSize.y , pNear, pFar);
}
void Camera::setNearProjection(const float& near) {
    pNear = near;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pAngle, WindowSize.x/WindowSize.y , pNear, pFar);
}
void Camera::setFarProjection(const float& far) {
    pFar = far;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pAngle, WindowSize.x/WindowSize.y , pNear, pFar);
}
Vector3f Camera::getHorizontalVector() const {
    Vector3f delta = pCenter - pPosition;
    Vector3f ans = {-delta.y, delta.x, 0};
    ans /= abs(delta);
    return ans;
}
Vector3f Camera::getCenter() const {
    return pCenter;
}