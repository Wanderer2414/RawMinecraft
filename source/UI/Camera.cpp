#include "Camera.h"
#include "Controller.h"
#include "Controller3D.h"
#include "General.h"
#include "Global.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <cmath>

extern Vector2f WindowSize;
Camera::Camera() {
    pPosition = {0, 0, 0};
    pDelta = {0, 0, 2};
    pVerticalAngle = pHorizontalAngle = 0;
    pAngle = pNear = pFar = 0;
    pSpeed = 0.1;
    pUpward = false;
    pOnGround = true;
    pWindowCenter.x = WindowSize.x/2;
    pWindowCenter.y = WindowSize.y/2;
    pDistance = 2;

    pDirection.setPrimitiveType(Lines);
    pDirection.resize(6);
    pDirection[0].position = pDirection[1].position = WindowSize/2.f;
    pDirection[2].position = pDirection[3].position = WindowSize/2.f;
    pDirection[4].position = pDirection[5].position = WindowSize/2.f;
    pDirection[0].color = pDirection[1].color = Color::Red;
    pDirection[2].color = pDirection[3].color = Color::Green;
    pDirection[4].color = pDirection[5].color = Color::Blue;

    pFrameAlarm.setDuration(1.f/60);
}
Camera::~Camera() {

}

Vector3f Camera::getHorizontalVector() const {
    Vector3f ans = {-pDelta.y, pDelta.x, 0};
    ans /= abs(ans);
    return ans;
}
Vector3f Camera::getCenter() const {
    return pPosition + pDelta;
}

void Camera::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pAngle, WindowSize.x/WindowSize.y , pNear, pFar);
}
void Camera::rotate(const float& vertical_angle, const float& horizontal_angle) {
    pVerticalAngle -= vertical_angle;
    if (pVerticalAngle < -M_PI_2*0.99) pVerticalAngle = -M_PI_2*0.99;
    if (pVerticalAngle > M_PI_2*0.99) pVerticalAngle = M_PI_2*0.99;
    pHorizontalAngle -= horizontal_angle;
    pDelta.z = pDistance*sin(pVerticalAngle);
    pDelta.x = pDistance*cos(pVerticalAngle)*cos(pHorizontalAngle);
    pDelta.y = pDistance*cos(pVerticalAngle)*sin(pHorizontalAngle);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pPosition.x, pPosition.y, pPosition.z, pPosition.x + pDelta.x, pPosition.y+pDelta.y, pPosition.z + pDelta.z, 0, 0, 1);
    update();
}
void Camera::move(const float& x, const float& y, const float& z) {
    pPosition += x*getHorizontalVector();
    
    pPosition.x += y*cos(pHorizontalAngle);
    pPosition.y += y*sin(pHorizontalAngle);
    
    pPosition.z += z;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pPosition.x, pPosition.y, pPosition.z, pPosition.x + pDelta.x, pPosition.y+pDelta.y, pPosition.z + pDelta.z, 0, 0, 1);
}
void Camera::setCenter(const float& x, const float& y, const float& z) {
    pDelta = Vector3f(x, y, z) - pPosition;
    
    pVerticalAngle = atan(pDelta.z/sqrt(pDelta.x*pDelta.x + pDelta.y*pDelta.y));
    pHorizontalAngle = atan(pDelta.y/pDelta.x);
    if (pDelta.x<0) pHorizontalAngle += M_PI;

    pDelta.z = pDistance*sin(pVerticalAngle);
    pDelta.x = pDistance*cos(pVerticalAngle)*cos(pHorizontalAngle);
    pDelta.y = pDistance*cos(pVerticalAngle)*sin(pHorizontalAngle);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pPosition.x, pPosition.y, pPosition.z, pPosition.x + pDelta.x, pPosition.y+pDelta.y, pPosition.z + pDelta.z, 0, 0, 1);
    update();
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
    bool is_changed = Controller3D::handle(window, state);
    if (pFrameAlarm.get()) {
        if (Keyboard::isKeyPressed(Keyboard::Scancode::A)) {
                move(pSpeed, 0, 0);
            is_changed = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Scancode::D)) {
                move(-pSpeed, 0, 0);
            is_changed = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Scancode::W)) {
                move(0, pSpeed, 0);
            is_changed = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Scancode::S)) {
                move(0, -pSpeed, 0);
            is_changed = true;
        }

        if (pOnGround && Keyboard::isKeyPressed(Keyboard::Key::Space)) {
            pJumpHeight = 0;
            pUpward = true;
            pOnGround = false;
        }

        if (!pOnGround) {
            if (pUpward  && pJumpHeight < 1.4) {
                pJumpHeight += 0.1;
                move(0, 0, 0.1);
            }
            else if (pJumpHeight>0) {
                float delta = min(0.1f, pJumpHeight);
                pJumpHeight -= delta;
                move(0, 0, -delta);
            }
            if (pJumpHeight >= 1.2) {
                pUpward = false;
            }
            else if (pJumpHeight <= 0) {
                pOnGround = true;
                pJumpHeight = 0;
            }
            is_changed = true;
        }
    }

    Vector2i position = Mouse::getPosition(window);
    Vector2f delta;
    delta.x = position.x - pWindowCenter.x;
    delta.y = position.y - pWindowCenter.y;

    if (delta.x != 0 || delta.y != 0) {
        Mouse::setPosition(pWindowCenter, window);
        rotate(delta.y/1000, delta.x/1000);
        is_changed = true;
    }
    return is_changed;
}
void Camera::draw(RenderTarget& target, RenderStates state) const {
    Controller3D::draw(target, state);
    target.draw(pDirection);
}
void Camera::update() {
    Vector3f center = pPosition + pDelta;
    center.x += 1;
    Vector2f axis = transfer(center) - WindowSize/2.f;
    pDirection[1].position = WindowSize/2.f + axis/abs(axis)*10.f;
    center.x -= 1;

    center.y += 1;
    axis = transfer(center) - WindowSize/2.f;
    pDirection[3].position = WindowSize/2.f + axis/abs(axis)*10.f;
    center.y -= 1;

    pDirection[5].position.y = WindowSize.y/2.f - 10*cos(pVerticalAngle);
}
Vector2f Camera::transfer(const Vector3f& vector) const {
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    double projectionView[16], modelView[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projectionView);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    double winX, winY, winZ;
    gluProject(vector.x, vector.y, vector.z, modelView, projectionView, viewport, &winX, &winY, &winZ);
    Vector2f ans;
    ans.x = winX;
    ans.y = WindowSize.y - winY;
    return ans;
}
Ray3f Camera::getSight() const {
    return Ray3f(pPosition, pPosition + pDelta);
}