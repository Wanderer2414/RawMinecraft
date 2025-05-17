#include "Camera.h"
#include "Controller.h"
#include "General.h"
#include "Global.h"
#include "SFML/Window/Keyboard.hpp"

extern Vector2f WindowSize;
Camera::Camera() {
    pPosition = pCenter = pDelta = {0, 0, 0};
    pAngle = pNear = pFar = 0;
    pSpeed = 0.005;
    pUpward = false;
    pOnGround = true;
    pWindowCenter.x = WindowSize.x/2;
    pWindowCenter.y = WindowSize.y/2;
}
Camera::~Camera() {

}

Vector3f Camera::getHorizontalVector() const {
    Vector3f ans = {-pDelta.y, pDelta.x, 0};
    ans /= abs(pDelta);
    return ans;
}
Vector3f Camera::getCenter() const {
    return pCenter;
}
void Camera::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
    pDelta = pCenter - pPosition;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pAngle, WindowSize.x/WindowSize.y , pNear, pFar);
}
void Camera::rotate(const float& vertical_angle, const float& horizontal_angle) {
    float current_vertical_angle = atan(pDelta.z/sqrt(pDelta.x*pDelta.x + pDelta.y*pDelta.y));
    float current_horizontal_angle = atan(pDelta.y/pDelta.x);
    if (pDelta.x<0) current_horizontal_angle += M_PI;
    current_horizontal_angle -= horizontal_angle;
    Vector3f center = pCenter;
    float distance = abs(pDelta);
    center.z = pPosition.z + distance*sin(current_vertical_angle);
    center.x = pPosition.x + distance*cos(current_vertical_angle)*cos(current_horizontal_angle);
    center.y = pPosition.y + distance*cos(current_vertical_angle)*sin(current_horizontal_angle);
    setCenter(center.x, center.y, center.z);
}
void Camera::move(const float& x, const float& y, const float& z) {
    Vector3f delta = x*getHorizontalVector();
    delta += y*pDelta/abs(pCenter-pPosition);
    delta.z += z;
    pPosition += delta;
    pCenter += delta;
    pDelta = pCenter - pPosition;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pPosition.x, pPosition.y, pPosition.z, pCenter.x, pCenter.y, pCenter.z, 0, 0, 1);
}
void Camera::setCenter(const float& x, const float& y, const float& z) {
    pCenter = {x, y, z};
    pDelta = pCenter - pPosition;
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
_handle_function(Camera, handle) {
    bool is_changed = Controller::handle(window, state);
    if (Keyboard::isKeyPressed(Keyboard::Scancode::A)) {
        window.popGLStates();
            move(pSpeed, 0, 0);
        window.pushGLStates();
        is_changed = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Scancode::D)) {
        window.popGLStates();
            move(-pSpeed, 0, 0);
        window.pushGLStates();
        is_changed = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Scancode::W)) {
        window.popGLStates();
            move(0, pSpeed, 0);
        window.pushGLStates();
        is_changed = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Scancode::S)) {
        window.popGLStates();
            move(0, -pSpeed, 0);
        window.pushGLStates();
        is_changed = true;
    }

    if (pOnGround && Keyboard::isKeyPressed(Keyboard::Key::Space)) {
        pJumpHeight = 0;
        pUpward = true;
        pOnGround = false;
    }

    if (!pOnGround) {
        window.popGLStates();
        if (pUpward  && pJumpHeight < 1.2) {
            pJumpHeight += 0.005;
            move(0, 0, 0.005);
        }
        else if (pJumpHeight>0) {
            pJumpHeight -= 0.001;
            move(0, 0, -0.001);
        }
        if (pJumpHeight >= 1.2) {
            pUpward = false;
        }
        else if (pJumpHeight <= 0) {
            pOnGround = true;
            pJumpHeight = 0;
        }
        window.pushGLStates();
        is_changed = true;
    }

    Vector2i position = Mouse::getPosition(window);
    Vector2f delta;
    delta.x = position.x - pWindowCenter.x;
    delta.y = position.y - pWindowCenter.y;

    if (delta.x != 0 || delta.y != 0) {
        Mouse::setPosition(pWindowCenter, window);
        window.popGLStates();
        rotate(delta.y/1000/M_PI, delta.x/1000/M_PI);
        window.pushGLStates();
        is_changed = true;
    }
    return is_changed;
}