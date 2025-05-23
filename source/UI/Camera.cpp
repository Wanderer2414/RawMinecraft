#include "Camera.h"
#include "Controller.h"
#include "Controller3D.h"
#include "General.h"
#include "Global.h"

extern Vector2f WindowSize;
Camera::Camera(GLuint& camera): pCamera(camera) {
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

glm::vec3 Camera::getHorizontalVector() const {
    glm::vec3 ans = {-pDelta.y, pDelta.x, 0};
    ans /= abs(ans);
    return ans;
}
glm::vec3 Camera::getCenter() const {
    return pPosition + pDelta;
}

void Camera::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
    pView = glm::lookAt(pPosition, pPosition + pDelta, glm::vec3(0, 0, 1));
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &pView[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Camera::rotate(const float& vertical_angle, const float& horizontal_angle) {
    // pView = glm::rotate(pView, vertical_angle, getHorizontalVector());
    pView = glm::rotate(pView, horizontal_angle, glm::vec3(0, 0, 1));
    
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &pView[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Camera::move(const float& x, const float& y, const float& z) {
    glm::vec3 delta = {0, 0, 0};
    delta -= x*getHorizontalVector();
    delta.x -= y*pDelta.x;
    delta.y -= y*pDelta.y;
    delta.z -= z;
    pView = glm::translate(pView, delta);
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &pView[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Camera::setCenter(const float& x, const float& y, const float& z) {
    pDelta = glm::vec3(x, y, z) - pPosition;
    pPosition = {x, y, z};
    pView = glm::lookAt(pPosition, pPosition + pDelta, glm::vec3(0, 0, 1));
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &pView[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    update();
}
void Camera::setWide(const float& angle) {
    pAngle = angle;
    glm::mat4 proj = glm::perspective(glm::radians(angle), WindowSize.x/WindowSize.y, pNear, pFar);
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &proj[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Camera::setNearProjection(const float& bnear) {
    pNear = bnear;
    glm::mat4 proj = glm::perspective(glm::radians(pAngle), WindowSize.x/WindowSize.y, pNear, pFar);
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &proj[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Camera::setFarProjection(const float& bfar) {
    pFar = bfar;
    glm::mat4 proj = glm::perspective(glm::radians(pAngle), WindowSize.x/WindowSize.y, pNear, pFar);
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &proj[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
    glm::vec3 center = pPosition + pDelta;
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
Vector2f Camera::transfer(const glm::vec3& vector) const {
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    double projectionView[16], modelView[16];
    // glGetDoublev(GL_PROJECTION_MATRIX, projectionView);
    // glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    double winX, winY, winZ;
    // gluProject(vector.x, vector.y, vector.z, modelView, projectionView, viewport, &winX, &winY, &winZ);
    Vector2f ans;
    ans.x = winX;
    ans.y = WindowSize.y - winY;
    return ans;
}
Ray3f Camera::getSight() const {
    return Ray3f(pPosition, pPosition + pDelta);
}