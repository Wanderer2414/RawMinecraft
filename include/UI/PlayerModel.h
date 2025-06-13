#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H
#include "Camera.h"
#include "Clock.h"
#include "Message.h"
#include "GLFW/glfw3.h"
#include "Model.h"
namespace MyCraft {
    class PlayerModel: public Model, public Port {
    public: 
        PlayerModel();
        ~PlayerModel();
        bool        sensitiveHandle(GLFWwindow* window) override;
        bool        handle(GLFWwindow* window) override;
        glm::vec3   getModelPosition() const override,
                    getDirection() const;
        void        move(const glm::vec3& delta) override,
                    rotate(const float& angle) override,
                    rotate(const glm::vec3& dir),
                    rightAttack(),
                    leftAttack(),
                    see(const glm::vec3& dir) override,
                    seeRotate(const float& angle),
                    glDraw() const override;
        
        glm::mat4x3 getShape() const override;
        
    private:
        bool            __isRun,
                        __isLeftAttack, __isRightAttack,
                        __isCrouch;
        float           __runTime, __handTime, __speed;
        glm::vec3       __position, __diagonal;
        glm::vec3       __direction, __eye_direction;
        std::vector<glm::mat4> __animation;
        MyBase::Clock   __animationClock,
                        __runCooldown,
                        __attack__cooldown;
        glm::vec3       __toAbsoluteCoordinate(const glm::vec3& dir) const;
        void            update() override;
    };

    class ResetCameraCommand: public Command {
    public:
        ResetCameraCommand(PlayerModel* model);
        ~ResetCameraCommand();
        MessageType getType() const override;
        void execute(Port& mine, Port& source, Message* message) override;
    private:
        PlayerModel* __model;
    };
}
#endif