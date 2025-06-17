#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H
#include "Camera.h"
#include "Clock.h"
#include "Message.h"
#include "ModelController.h"
namespace MyCraft {
    class PlayerModelController: public ModelController, public Port {
    public: 
        PlayerModelController();
        ~PlayerModelController();
        bool        isCrounch() const;
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
                    seeRotate(const float& horizontal, const float& vertical),
                    setDrawAble(const bool& drawable),
                    glDraw() const override;
        
        glm::mat4x3 getShape() const override;
        
    private:
        bool            __isRun, __isDrawable,
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
        ResetCameraCommand(PlayerModelController* model);
        ~ResetCameraCommand();
        MessageType getType() const override;
        void execute(Port& mine, Port& source, Message* message) override;
    private:
        PlayerModelController* __model;
    };
    class MoveMessage: public Message {
    public:
        MoveMessage(const glm::vec3& direction);
        ~MoveMessage();
        MessageType     getType() const override;
        const glm::vec3       direction;
    };
    class FallMessage: public Message {
    public:
        FallMessage(const float& zVelocity);
        ~FallMessage();
        MessageType     getType() const override;

        const float zVelocity;
    };
    class StopFallMessage: public Message {
    public:
        StopFallMessage();
        ~StopFallMessage();
        MessageType     getType() const override;
    };
    
    class RequestGotoMessage: public Message {
    public:
        RequestGotoMessage(const glm::mat4x3& rectangleBox, const glm::vec2& direction);
        ~RequestGotoMessage();
        MessageType     getType() const override;
        const glm::vec2       direction;
        const glm::mat4x3     rectangleBox;
    private:
    };

    class RequestFallMessage: public Message {
    public:
        RequestFallMessage(const glm::mat4x3& rectangleBox, const float& zVelocity);
        ~RequestFallMessage();
        MessageType     getType() const override;
        const glm::mat4x3 rectangleBox;
        float zVelocity;
    };

    class RightAttackMessage: public Message {
    public:
        RightAttackMessage(const glm::vec3& pos, const glm::vec3& dir);
        ~RightAttackMessage();
        MessageType getType() const override;
        const glm::vec3 posistion, direction;
    };

    class LeftAttackMessage: public Message {
    public:
        LeftAttackMessage();
        ~LeftAttackMessage();
        MessageType     getType() const override;
    };

    class CheckHoverMessage: public Message {
    public:
        CheckHoverMessage(const glm::vec3& pos, const glm::vec3& dir);
        ~CheckHoverMessage();
        MessageType     getType() const override;
        const glm::vec3 position, direction;
    };

    class PlayerMoveCommand: public Command {
    public:
        PlayerMoveCommand(PlayerModelController* model);
        ~PlayerMoveCommand();

        MessageType getType()                               const override;
        void execute(Port& mine, Port& source, Message* message)   override;
    private:
        PlayerModelController*      __model;
    };
}
#endif