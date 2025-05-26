#include "Camera.h"
#include "Controller3D.h"
#include "General.h"
#include "Global.h"

extern Vector2f WindowSize;
Camera::Camera() {
    pPosition = {4, 4, 2};
    pDelta = {-2, -2, 0};
    pVerticalAngle = 0;
    pWindowCenter.x = WindowSize.x/2;
    pWindowCenter.y = WindowSize.y/2;
    pDistance = 3;
    pDelta = pDelta/glm::length(pDelta)*pDistance;

    pDirection.setPrimitiveType(Lines);
    pDirection.resize(6);
    pDirection[0].position = pDirection[1].position = WindowSize/2.f;
    pDirection[2].position = pDirection[3].position = WindowSize/2.f;
    pDirection[4].position = pDirection[5].position = WindowSize/2.f;
    pDirection[0].color = pDirection[1].color = Color::Red;
    pDirection[2].color = pDirection[3].color = Color::Green;
    pDirection[4].color = pDirection[5].color = Color::Blue;

    glGenBuffers(1, &pCamera);
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, pCamera);

    pView = glm::lookAt(pPosition, pPosition + pDelta, glm::vec3(0, 0, 1));
    pProjection = glm::perspective(glm::radians(60.f), WindowSize.x/WindowSize.y, 0.1f, 100.f);

    update();
}
Camera::~Camera() {
    glDeleteBuffers(1, &pCamera);
}
Camera::operator GLuint() {
    return pCamera;
}
glm::vec3 Camera::getHorizontalVector() const {
    glm::vec3 ans = {-pDelta.y, pDelta.x, 0};
    ans /= glm::length(ans);
    return ans;
}
glm::vec3 Camera::getCenter() const {
    return pPosition + pDelta;
}
glm::vec3 Camera::getPosition() const {
    return pPosition;
}
glm::vec3 Camera::getDirection() const {
    return pDelta;
}
void Camera::setPosition(const glm::vec3& position) {
    pPosition = position;
    pView = glm::lookAt(pPosition, pPosition + pDelta, glm::vec3(0, 0, 1));
    update();
}
void Camera::setCameraDirection(const glm::vec3& position, const glm::vec3& center) {
    pDelta = center-position;
    pDelta = pDelta/glm::length(pDelta)*pDistance;
    pPosition = position;
    pView = glm::lookAt(pPosition, center, glm::vec3(0, 0, 1));
    update();
}

void Camera::setPerpective(const float& angle, const float& aspect, const float& near, const float& far) {
    pProjection = glm::perspective(glm::radians(angle), aspect, near, far);
    update();
}
void Camera::rotate(const float& vertical_angle, const float& horizontal_angle) {
    if (float tmp = pVerticalAngle + vertical_angle; tmp<M_PI_2-0.1 && tmp>-M_PI_2+0.1) {
        pVerticalAngle = tmp;
        glm::mat4 mat =  glm::rotate(glm::mat4(1), -horizontal_angle, glm::vec3(0, 0, 1));
        mat = glm::rotate(mat, vertical_angle, getHorizontalVector());
        pDelta = mat*glm::vec4(pDelta, 1);
        pView = glm::lookAt(pPosition, pPosition + pDelta, glm::vec3(0, 0, 1));
        update();
    }
}
void Camera::move(const float& x, const float& y, const float& z) {
    pPosition += glm::vec3(x, y, z);
    pView = glm::lookAt(pPosition, pPosition + pDelta, glm::vec3(0, 0, 1));
    update();
}
_handle_function(Camera, handle) {
    bool is_changed = Controller3D::handle(window, state);
    return is_changed;
}
void Camera::draw(RenderTarget& target, RenderStates state) const {
    Controller3D::draw(target, state);
    target.draw(pDirection);
}
void Camera::update() {
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, pCamera);
    pClipPlane = pProjection*pView*glm::mat4(1);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &pClipPlane[0][0]);

    glm::vec3 center = pPosition + pDelta;
    center.x += 0.05;
    pDirection[1].position = transfer(center);
    center.x -= 0.05;

    center.y += 0.05;
    pDirection[3].position = transfer(center);
    center.y -= 0.05;

    pDirection[5].position.y = WindowSize.y/2.f - 15*cos(pVerticalAngle);
}
Vector2f Camera::transfer(const glm::vec3& vector) const {
    glm::vec4 pos= pClipPlane*glm::vec4(vector,1);
    if (pos.w) pos /= pos.w;
    float x = (pos.x + 1)*pWindowCenter.x;
    float y = (1 - pos.y)*pWindowCenter.y;
    return {x, y};
}
Ray3f Camera::getSight() const {
    return Ray3f(pPosition, pPosition + pDelta);
}