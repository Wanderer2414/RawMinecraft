#include "PlayerModel.h"
#include "Message.h"
#include "GLFW/glfw3.h"
#include "Global.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelStorage.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include <algorithm>
namespace MyCraft {
    PlayerModel::PlayerModel(): __position(0), __direction(0, -1, 0), __runTime(0), __handTime(0),
        __isLeftAttack(0), __isRightAttack(0), __animation(ModelStorage::Default->getPlayerModel().getNodeCount(), 1), __eye_direction(0, -1, 0),
        __isCrouch(false) {
        ModelStorage::Default->getPlayerModel().apply(__animation, "walk", __runTime);
        __behaviourClock.setDuration(100);
        __animationClock.setDuration(30);
        __attack__cooldown.setDuration(250);
        __isRun = false;
        __speed = 0.2;
        __diagonal = {0.4, 0.2, 1.8};
        __zVelocity = 0;
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
                dir = __toAbsoluteCoordinate(dir);
                dir = glm::normalize(dir)*__speed;
                rotate(dir);
                send(new RequestGoto(getShape(), dir));
            }
            if (!__isFall) {
                if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                    //Jump here
                    __zVelocity = 0.35;
                    send(new RequestFall(getShape(), __zVelocity));
                }
            }
            else {
                //Auto fall
                send(new RequestFall(getShape(), __zVelocity));
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
        return __position;
    }
    glm::vec3 PlayerModel::getDirection() const {
        return __eye_direction;
    }
    glm::vec3 PlayerModel::__toAbsoluteCoordinate(const glm::vec3& delta) const {
        glm::vec3 d = delta.x*__eye_direction;
        d += delta.y*glm::normalize(glm::cross(__eye_direction, glm::vec3(0, 0, 1)));
        return d;
    }
    glm::mat4x3 PlayerModel::getShape() const {
        float angle = glm::angle(__direction, glm::vec3(0, -1, 0));
        if (__direction.x < 0) angle = -angle;

        glm::mat4x3 ans;
        ans[0] = __position;
        ans[1] = {0.6, 0, 0};
        ans[2] = {0, 0.4, 0};
        ans[3] = {0, 0, 1.8};

        ans[1] = glm::rotate(ans[1], angle, glm::vec3(0, 0, 1));
        ans[2] = glm::rotate(ans[2], angle, glm::vec3(0, 0, 1));
        ans[0] -= ans[1]/2.f+ans[2]/2.f;
        if (__isCrouch) {
            ans[0] -= ans[2];
            ans[2] = ans[2]*1.8f;
        } 
        return ans;
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
        __position += delta;
        if (delta.x || delta.y) {
            __behaviourClock.restart();
            __isRun = true;
        }
    }
    void PlayerModel::rotate(const glm::vec3& dir) {
        __direction = glm::normalize(dir);
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
        state.back() = glm::translate(state.back(), __position);
        float angle = glm::angle(__direction, glm::vec3(0, -1, 0));
        if (__direction.x<0) angle=-angle;
        state.back() = glm::rotate(state.back(), angle, glm::vec3(0, 0, 1));
        ModelStorage::Default->DrawModel(state, model);
        glDeleteVertexArrays(1, &VAO);
    }
    std::vector<MessageType> PlayerModel::getTypes() const {
        return {MessageType::MoveMessage, MessageType::FallMessage, MessageType::StopFallMessage};
    };
    void PlayerModel::update() {
    }
    void PlayerModel::receive(Port& port, Message* Message) {
        switch (Message->getType()) {
            case MyCraft::MessageType::MoveMessage: {
                Move* moveMessage = (Move*)Message;
                move(moveMessage->direction);
                if (!__isFall)
                    send(new RequestFall(getShape(), __zVelocity));
                delete moveMessage;
            }
            break;
            case MyCraft::MessageType::FallMessage: {
                Fall* fall = (Fall*)Message;
                __isFall = true;
                __zVelocity = fall->zVelocity;
                move(glm::vec3(0, 0, __zVelocity));
                delete fall;
            }
            break;
            case MyCraft::MessageType::StopFallMessage: {
                __isFall = false;
                __zVelocity = 0;
                delete (StopFall*)Message;
            };
            break;
            default:
            break;
        }
    }
}