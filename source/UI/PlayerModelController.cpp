#include "PlayerModelController.h"
#include "Camera.h"
#include "Message.h"
#include "Global.h"
#include "ModelController.h"
#include "ModelLoader.h"
#include "ModelStorage.h"
namespace MyCraft {
    PlayerModelController::PlayerModelController(): __position(0), __direction(0, -1, 0), __runTime(0), __handTime(0),
        __isLeftAttack(0), __isRightAttack(0), __animation(ModelStorage::Default->getPlayerModel().getNodeCount(), 1), __eye_direction(0, -1, 0),
        __isCrouch(false), __isDrawable(true) {
        ModelStorage::Default->getPlayerModel().apply(__animation, "walk", __runTime);
        __animationClock.setDuration(30);
        __attack__cooldown.setDuration(250);
        __isRun = false;
        __speed = 0.2;
        __diagonal = {0.6, 0.4, 1.9};
        __runCooldown.setDuration(30);

        add(new MoveCommand(this));
        add(new FallCommand(this));
        add(new StopFallCommand(this));
        add (new ResetCameraCommand(this));
}
    PlayerModelController::~PlayerModelController() {

    }
    bool PlayerModelController::sensitiveHandle(GLFWwindow* window) {
        bool is_changed = ModelController::handle(window);
        glm::vec3 dir(0);
        if (glfwGetKey(window, GLFW_KEY_A)) {
            dir.y -= __speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            dir.y += __speed;
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            dir.x += __speed;
        } else if (!__isCrouch && __speed!=0.2f) {
            __speed = 0.2;
            __animationClock.setDuration(30);
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            dir.x -= __speed;
        }
        if (__speed!=0.3f && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && !__isCrouch) {
            __speed = 0.3;
            __animationClock.setDuration(10);
        }
        if (glm::length(dir)) {
            dir = __toAbsoluteCoordinate(dir);
            dir.z = 0;
            dir = glm::normalize(dir)*__speed;
            rotate(dir);
            send(new RequestGoto(getShape(), dir));
        }
        if (!isFall()) {
            if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                //Jump here
                setZVelocity(0.35);
                send(new RequestFall(getShape(), getZVelocity()));
            }
        }
        else {
            //Auto fall
            send(new RequestFall(getShape(), getZVelocity()));
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
            if (!__isCrouch) {
                __animationClock.setDuration(40);
                __isCrouch = true;
            }
        } else __isCrouch = false;

        return is_changed;
    }
    bool PlayerModelController::handle(GLFWwindow* window) {
        bool is_changed = ModelController::handle(window);
        if (__animationClock.get()) {
            __animationClock.restart();
            std::fill(__animation.begin(), __animation.end(), glm::mat4(1));
            if (__isCrouch) {
                ModelStorage::Default->getPlayerModel().apply(__animation, "crouch", 0);
                __speed = 0.05;
                is_changed = true;
            }
            if (__isRun) {
                if (__runCooldown.get()) {
                    __runCooldown.restart();
                    __isRun = false;
                    __runTime = 0;
                }
                else __runTime+=0.1;
                if (__runTime>=1) __runTime -= 1;
                ModelStorage::Default->getPlayerModel().apply(__animation, "walk", __runTime);
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
        }
        return is_changed;
    }
    glm::vec3 PlayerModelController::getModelPosition() const {
        return __position;
    }
    glm::vec3 PlayerModelController::getDirection() const {
        return __eye_direction;
    }
    glm::vec3 PlayerModelController::__toAbsoluteCoordinate(const glm::vec3& delta) const {
        glm::vec3 d = delta.x*__eye_direction;
        d += delta.y*glm::normalize(glm::cross(__eye_direction, glm::vec3(0, 0, 1)));
        return d;
    }
    glm::mat4x3 PlayerModelController::getShape() const {
        float angle = glm::angle(__direction, glm::vec3(0, -1, 0));
        if (__direction.x < 0) angle = -angle;

        glm::mat4x3 ans;
        ans[0] = __position;
        ans[1] = {__diagonal.x, 0, 0};
        ans[2] = {0, __diagonal.y, 0};
        ans[3] = {0, 0, __diagonal.z};

        ans[1] = glm::rotate(ans[1], angle, glm::vec3(0, 0, 1));
        ans[2] = glm::rotate(ans[2], angle, glm::vec3(0, 0, 1));
        ans[0] -= ans[1]/2.f+ans[2]/2.f;
        if (__isCrouch) {
            ans[0] -= ans[2];
            ans[2] = ans[2]*1.8f;
        } 
        return ans;
    }
    void PlayerModelController::leftAttack() {
        if (__attack__cooldown.get()) {
            __attack__cooldown.restart();
            __handTime = 0;
            __isRightAttack = false;
            __isLeftAttack = true;
        } 
    }
    void PlayerModelController::rightAttack() {
        if (__attack__cooldown.get()) {
            __attack__cooldown.restart();
            __handTime = 0;
            __isRightAttack = false;
            __isRightAttack = true;
        }
    }
    void PlayerModelController::move(const glm::vec3& delta) {
        send(new MoveCameraMessage(delta));
        __position += delta;
        if (delta.x || delta.y) {
            __runCooldown.restart();
            __isRun = true;
        }
    }
    void PlayerModelController::rotate(const glm::vec3& dir) {
        __direction = glm::normalize(dir);
    }
    void PlayerModelController::rotate(const float& angle) {
        __direction = glm::rotate(__direction, angle, glm::vec3(0,0, 1));
    }
    void PlayerModelController::see(const glm::vec3& dir) {
        float angle = glm::angle(glm::vec3(0, 1, 0), glm::normalize(dir));
        __animation[8] = glm::rotate(glm::mat4(1), angle, glm::vec3(1, 0, 0));
    }
    void PlayerModelController::seeRotate(const float& horizontal, const float& vertical) {
        __eye_direction = glm::rotate(__eye_direction, horizontal, glm::vec3(0, 0, 1));
        glm::vec3 axis = glm::cross(__eye_direction, glm::vec3(0,0,1));
        __eye_direction = glm::rotate(__eye_direction, vertical, axis);
        send(new RotateCameraMessage(__position, __eye_direction));
    }
    void PlayerModelController::setDrawAble(const bool& drawable) {
        __isDrawable = drawable;
    }
    void PlayerModelController::glDraw() const {
        if (__isDrawable) {

            GLuint VAO;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            const ModelLoader& model = ModelStorage::Default->getPlayerModel();
            auto state = __animation;
            state.back() = glm::translate(state.back(), __position);
            float angle = glm::angle(__direction, glm::vec3(0, -1, 0));
            if (__direction.x<0) angle=-angle;
            state.back() = glm::rotate(state.back(), angle, glm::vec3(0, 0, 1));
    
            glm::vec3 tmp;
            auto& head = state[ModelStorage::Default->getPlayerModel().getHead()];
            // tmp = __eye_direction;
            // tmp.z = 0;
            // tmp = glm::normalize(tmp);
            // angle = glm::angle( tmp, __eye_direction);
            // if (angle) {
            //     glm::vec3 axis = glm::cross(__eye_direction, tmp);
            //     head = glm::rotate(head,angle, axis);
            // }
            tmp = __eye_direction;
            tmp.z = 0;
            tmp = glm::normalize(tmp);
            angle = glm::angle( tmp, __direction);
            if (angle) {
                glm::vec3 axis = glm::cross(__direction, tmp);
                head = glm::rotate(head,angle, axis);
            }
    
            ModelStorage::Default->DrawModel(state, model);
            glDeleteVertexArrays(1, &VAO);
        }
    }
    void PlayerModelController::update() {
    }

    ResetCameraCommand::ResetCameraCommand(PlayerModelController* model): __model(model) {};
    MessageType ResetCameraCommand::getType() const {
        return MessageType::ResetCamera;
    }
    void ResetCameraCommand::execute(Port& mine, Port& source, Message* message) {
        ResetCameraMessage* package = (ResetCameraMessage*)message;
        if (package->isFirstCamera) __model->setDrawAble(false);
        else __model->setDrawAble(true);
        __model->send(new RotateCameraMessage(__model->getModelPosition(), __model->getDirection()));
    }
}