#include "Camera.h"
#include "Controller3D.h"
#include "Global.h"

extern sf::Vector2f WindowSize;
namespace MyBase3D {

Camera::Camera():
    __position(4, 4, 2),
    __delta(-2, -2, 0),
    __verticalAngle(0),
    __windowCenter(WindowSize.x/2, WindowSize.y/2) {

    __delta = __delta/glm::length(__delta)*CAMERA_DISTANCE;

    __direction.setPrimitiveType(sf::Lines);
    __direction.resize(6);
    __direction[0].position = __direction[1].position = WindowSize/2.f;
    __direction[2].position = __direction[3].position = WindowSize/2.f;
    __direction[4].position = __direction[5].position = WindowSize/2.f;
    __direction[0].color = __direction[1].color = sf::Color::Red;
    __direction[2].color = __direction[3].color = sf::Color::Green;
    __direction[4].color = __direction[5].color = sf::Color::Blue;

    glGenBuffers(1, &__camera);
    glBindBuffer(GL_UNIFORM_BUFFER, __camera);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, __camera);

    __view = glm::lookAt(__position, __position + __delta, glm::vec3(0, 0, 1));
    __projection = glm::perspective(glm::radians(60.f), WindowSize.x/WindowSize.y, 0.1f, 100.f);

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
    if (float tmp = __verticalAngle + vertical_angle; tmp<M_PI_2-0.1 && tmp>-M_PI_2+0.1) {
        __verticalAngle = tmp;
        glm::mat4 mat =  glm::rotate(glm::mat4(1), -horizontal_angle, glm::vec3(0, 0, 1));
        mat = glm::rotate(mat, vertical_angle, getHorizontalVector());
        __delta = mat*glm::vec4(__delta, 1);
        __view = glm::lookAt(__position, __position + __delta, glm::vec3(0, 0, 1));
        update();
    }
}
void Camera::move(const float& x, const float& y, const float& z) {
    __position += glm::vec3(x, y, z);
    __view = glm::lookAt(__position, __position + __delta, glm::vec3(0, 0, 1));
    update();
}
_handle_function(Camera, handle) {
    bool is_changed = Controller3D::handle(window, state);
    return is_changed;
}
void Camera::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    Controller3D::draw(target, state);
    target.draw(__direction);
}
void Camera::update() {
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, __camera);
    __clipPlane= __projection*__view*glm::mat4(1);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &__clipPlane[0][0]);

    glm::vec3 center = __position+ __delta;
    center.x += 0.05;
    __direction[1].position = transfer(center);
    center.x -= 0.05;

    center.y += 0.05;
    __direction[3].position = transfer(center);
    center.y -= 0.05;

    __direction[5].position.y = WindowSize.y/2.f - 15*cos(__verticalAngle);
}
sf::Vector2f Camera::transfer(const glm::vec3& vector) const {
    glm::vec4 pos= __clipPlane*glm::vec4(vector,1);
    if (pos.w) pos /= pos.w;
    float x = (pos.x + 1)*__windowCenter.x;
    float y = (1 - pos.y)*__windowCenter.y;
    return {x, y};
}
Ray3f Camera::getSight() const {
    return Ray3f(__position, __position + __delta);
}
}