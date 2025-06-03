#include "Camera.h"
#include "Controller3D.h"
#include "Global.h"
#include "InfoCenter.h"
#include "ShaderStorage.h"

namespace MyBase3D {

Camera::Camera():
    __position(4, 4, 2),
    __delta(-2, -2, 0),
    __verticalAngle(0),
    __windowCenter(MyCraft::InfoCenter::Default->getWindowHalf()) {

    __delta = __delta/glm::length(__delta)*CAMERA_DISTANCE;

    __direction[0] = __direction[1] = {0, 0};
    __direction[2] = __direction[3] = {0, 0};
    __direction[4] = __direction[5] = {0, 0};
    __direction_color[0] = __direction_color[1] = {1, 0, 0, 1};
    __direction_color[2] = __direction_color[3] = {0, 1, 0, 1};
    __direction_color[4] = __direction_color[5] = {0, 0, 1, 1};

    glGenBuffers(1, &__camera);
    glBindBuffer(GL_UNIFORM_BUFFER, __camera);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, __camera);

    __view = glm::lookAt(__position, __position + __delta, glm::vec3(0, 0, 1));
    __projection = glm::perspective(glm::radians(60.f), MyCraft::InfoCenter::Default->getWindowRatio(), 0.1f, 100.f);

    update();
}
Camera::~Camera() {
    glDeleteBuffers(1, &__camera);
}
Camera::operator GLuint() {
    return __camera;
}
glm::vec3 Camera::getHorizontalVector() const {
    glm::vec3 ans = {-__delta.y, __delta.x, 0};
    ans /= glm::length(ans);
    return ans;
}
glm::vec3 Camera::getCenter() const {
    return __position + __delta;
}
glm::vec3 Camera::getPosition() const {
    return __position;
}
glm::vec3 Camera::getDirection() const {
    return __delta;
}
void Camera::setPosition(const float& x, const float& y, const float& z) {
    setPosition({x, y, z});
}
void Camera::setPosition(const glm::vec3& position) {
    __position = position;
    __view = glm::lookAt(__position, __position + __delta, glm::vec3(0, 0, 1));
    update();
}
void Camera::setCameraDirection(const glm::vec3& position, const glm::vec3& center) {
    __delta = center-position;
    __delta = __delta/glm::length(__delta)*CAMERA_DISTANCE;
    __position = position;
    __view = glm::lookAt(__position, center, glm::vec3(0, 0, 1));
    update();
}

void Camera::setPerpective(const float& angle, const float& aspect, const float& near, const float& far) {
    __projection = glm::perspective(glm::radians(angle), aspect, near, far);
    update();
}
void Camera::rotate(const float& vertical_angle, const float& horizontal_angle) {
    if (float tmp = vertical_angle+__verticalAngle; tmp<M_PI_2-0.1 && tmp>-M_PI_2+0.1) {
        __verticalAngle = tmp;
        glm::mat4 mat =  glm::rotate(glm::mat4(1), -horizontal_angle, glm::vec3(0, 0, 1));
        mat = glm::rotate(mat, vertical_angle, getHorizontalVector());
        __delta = mat*glm::vec4(__delta,1);
        __view = glm::lookAt(__position, __position + __delta, glm::vec3(0, 0, 1));
        update();
    }
}
void Camera::move(const float& x, const float& y, const float& z) {
    __position += glm::vec3(x, y, z);
    __view = glm::lookAt(__position, __position + __delta, glm::vec3(0, 0, 1));
    update();
}
void Camera::update() {
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, __camera);
    __clipPlane= __projection*__view*glm::mat4(1);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &__clipPlane[0][0]);

    glm::vec3 center = __position+ __delta;
    center.x += 0.02;
    __direction[1] = transfer(center);
    center.x -= 0.02;

    center.y += 0.02;
    __direction[3] = transfer(center);
    center.y -= 0.02;

    __direction[5].y = 0.05*cos(__verticalAngle);
}
void Camera::glDraw() const {
    glUseProgram(MyBase3D::ShaderStorage::Default->getPoint2DShader());
    GLuint VAO, Positions, Colors;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
        
    glGenBuffers(1, &Positions);
    glBindBuffer(GL_ARRAY_BUFFER, Positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*6, &__direction[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &Colors);
    glBindBuffer(GL_ARRAY_BUFFER, Colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*6, &__direction_color[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
    glDeleteBuffers(1, &Positions);
    glDeleteBuffers(1, &Colors);
}
glm::vec2 Camera::transfer(const glm::vec3& vector) const {
    glm::vec4 pos= __clipPlane*glm::vec4(vector,1);
    return pos;
}
Ray3f Camera::getSight() const {
    return Ray3f(__position, __position + __delta);
}
}