#include "PlayerModel.h"
#include "GLFW/glfw3.h"
#include "Global.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelStorage.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include <algorithm>
namespace MyCraft {
    PlayerModel::PlayerModel(): __postition(1), __direction(0, -1, 0), __runTime(0), __handTime(0),
        __isLeftAttack(0), __isRightAttack(0), __animation(ModelStorage::Default->getPlayerModel().getNodeCount(), 1), __eye_direction(0, -1, 0),
        __isCrouch(false) {
        ModelStorage::Default->getPlayerModel().apply(__animation, "walk", __runTime);
        __behaviourClock.setDuration(100);
        __animationClock.setDuration(30);
        __attack__cooldown.setDuration(250);
        __isRun = false;
        __speed = 0.2;
        _bottomRec[0] = {-0.4, 0.2, 0};
        _bottomRec[1] = {-0.4, -0.2, 0};
        _bottomRec[2] = {0.4, -0.2, 0};
        _bottomRec[3] = {0.4, 0.2, 0};
        _minZ = 0;
        _maxZ = 3;
    }
    PlayerModel::~PlayerModel() {

    }
    bool PlayerModel::handle(GLFWwindow* window) {
        bool is_changed = Model::handle(window);
        if (__animationClock.get()) {
            __animationClock.restart();
            std::fill(__animation.begin(), __animation.end(), glm::mat4(1));
            if (__isRun) {
                if (__behaviourClock.get()) {
                    __behaviourClock.restart();
                    __isRun = false;
                    __runTime = 0;
                }
                else __runTime+=0.1;
                if (__runTime>=1) __runTime -= 1;
                ModelStorage::Default->getPlayerModel().apply(__animation, "walk", __runTime);
                is_changed = true;
            }
            if (__isCrouch) {
                ModelStorage::Default->getPlayerModel().apply(__animation, "crouch", 0);
                __speed = 0.05;
                is_changed = true;
            }
            if (__isRightAttack) {
                if (__attack__cooldown.get()) {
                    __isRightAttack = false;
                    __handTime = 0;
                }
                else __handTime += 0.03;
                ModelStorage::Default->getPlayerModel().apply(__animation, "right_attack", __handTime);
                is_changed = true;
            }
            if (__isLeftAttack) {
                if (__attack__cooldown.get()) {
                    __isLeftAttack = false;
                    __handTime = 0;
                }
                else __handTime += 0.03;
                ModelStorage::Default->getPlayerModel().apply(__animation, "left_attack", __handTime);
                is_changed = true;
            }
            glm::vec3 dir(0);
            if (glfwGetKey(window, GLFW_KEY_A)) {
                dir.y -= __speed;
            }
            if (glfwGetKey(window, GLFW_KEY_D)) {
                dir.y += __speed;
            }
            if (glfwGetKey(window, GLFW_KEY_W)) {
                dir.x += __speed;
            } else if (!__isCrouch) __speed = 0.2;
            if (glfwGetKey(window, GLFW_KEY_S)) {
                dir.x -= __speed;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && !__isCrouch) __speed = 0.3;
            if (glm::length(dir)) {
                move(dir);
                is_changed = true;
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
                rightAttack();
                is_changed = true;
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
                leftAttack();
                is_changed = true;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                __isCrouch = true;
            } else __isCrouch = false;
        }
        return is_changed;
    }
    glm::vec3 PlayerModel::getPosition() const {
        return __postition;
    }
    glm::vec3 PlayerModel::getDirection() const {
        return __eye_direction;
    }
    const glm::mat4x3& PlayerModel::getBottomRec() const {
        return _bottomRec;
    }
    glm::vec2 PlayerModel::getZRange() const {
        return {_minZ, _maxZ};
    }
    void PlayerModel::leftAttack() {
        if (__attack__cooldown.get()) {
            __attack__cooldown.restart();
            __handTime = 0;
            __isLeftAttack = true;
        } 
    }
    void PlayerModel::rightAttack() {
        if (__attack__cooldown.get()) {
            __attack__cooldown.restart();
            __handTime = 0;
            __isRightAttack = true;
        }
    }
    void PlayerModel::move(const glm::vec3& delta) {
        rotate(delta);
        glm::vec3 d = delta.x*__eye_direction;
        d += delta.y*glm::normalize(glm::cross(__eye_direction, glm::vec3(0, 0, 1)));
        __postition += d;
        __behaviourClock.restart();
        __isRun = true;
    }
    void PlayerModel::rotate(const glm::vec3& dir) {
        // std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
        // glm::vec3 direction = dir;
        // if (direction.z) direction.z = 0;
        // direction = glm::normalize(direction);
        // float angle = glm::angle(direction, __direction);
        glm::vec3 d;
        d = dir.x*__eye_direction;
        d += dir.y*glm::normalize(glm::cross(__eye_direction, glm::vec3(0, 0, 1)));
        __direction = glm::normalize(d);
    }
    void PlayerModel::rotate(const float& angle) {
        __direction = glm::rotate(__direction, angle, glm::vec3(0,0, 1));
    }
    void PlayerModel::see(const glm::vec3& dir) {
        float angle = glm::angle(glm::vec3(0, 1, 0), glm::normalize(dir));
        __animation[8] = glm::rotate(glm::mat4(1), angle, glm::vec3(1, 0, 0));
    }
    void PlayerModel::seeRotate(const float& angle) {
        __eye_direction = glm::rotate(__eye_direction, angle, glm::vec3(0, 0, 1));
    }
    void PlayerModel::glDraw() const {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        const ModelLoader& model = ModelStorage::Default->getPlayerModel();
        auto state = __animation;
        state.back() = glm::translate(state.back(), __postition);
        float angle = glm::angle(__direction, glm::vec3(0, -1, 0));
        if (__direction.x<0) angle=-angle;
        state.back() = glm::rotate(state.back(), angle, glm::vec3(0, 0, 1));
        ModelStorage::Default->DrawModel(state, model);
        glDeleteVertexArrays(1, &VAO);
    }
}