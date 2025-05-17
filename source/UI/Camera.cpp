#include "Camera.h"
#include <GL/gl.h>
#include <GL/glu.h>

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
    pPosition += {x, y, z};
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, z);
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