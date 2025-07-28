#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H
#include "Camera.h"
#include "Clock.h"
#include "Message.h"
#include "ModelController.h"
namespace MyCraft {
    class PlayerModelController: public ModelController, public MyBase::Port {
    public: 
        PlayerModelController();
        ~PlayerModelController();
        bool        isCrounch() const;
        bool        isRun() const;
        glm::vec3   getModelPosition() const override,
                    getDirection() const;
        void        move(const glm::vec3& delta) override,
                    rotate(const float& angle) override,
                    teleport(const glm::vec3& position),
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
        MyBase::Clock   __animationClock, __speedControl,
                        __runCooldown,
                        __attack__cooldown;
        glm::vec3       __toAbsoluteCoordinate(const glm::vec3& dir) const;
        bool            __moveManage(GLFWwindow* window);
        bool            handle(GLFWwindow* window) override;
        bool            catchEvent(GLFWwindow* window) override;
        void            update() override;
    };
    class ResetCameraCommand: public MyBase::Command {
    public:
        ResetCameraCommand(MyCraft::PlayerModelController* model);
        ~ResetCameraCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        MyCraft::PlayerModelController* __model;
    };
    class MoveMessage: public MyBase::Message {
    public:
        MoveMessage(const glm::vec3& direction);
        ~MoveMessage();
        MyBase::MessageType     getType() const override;
        const glm::vec3       direction;
    };
    class FallMessage: public MyBase::Message {
    public:
        FallMessage(const float& zVelocity);
        ~FallMessage();
        MyBase::MessageType     getType() const override;

        const float zVelocity;
    };
    class StopFallMessage: public MyBase::Message {
    public:
        StopFallMessage();
        ~StopFallMessage();
        MyBase::MessageType     getType() const override;
    };
    
    class RequestGotoMessage: public MyBase::Message {
    public:
        RequestGotoMessage(const glm::mat4x3& rectangleBox, const glm::vec2& direction);
        ~RequestGotoMessage();
        MyBase::MessageType     getType() const override;
        const glm::vec2       direction;
        const glm::mat4x3     rectangleBox;
    private:
    };

    class RequestFallMessage: public MyBase::Message {
    public:
        RequestFallMessage(const glm::mat4x3& rectangleBox, const float& zVelocity);
        ~RequestFallMessage();
        MyBase::MessageType     getType() const override;
        const glm::mat4x3 rectangleBox;
        float zVelocity;
    };

    class RightAttackMessage: public MyBase::Message {
    public:
        RightAttackMessage(const glm::vec3& pos, const glm::vec3& dir);
        ~RightAttackMessage();
        MyBase::MessageType getType() const override;
        const glm::vec3 posistion, direction;
    };

    class LeftAttackMessage: public MyBase::Message {
    public:
        LeftAttackMessage();
        ~LeftAttackMessage();
        MyBase::MessageType     getType() const override;
    };

    class CheckHoverMessage: public MyBase::Message {
    public:
        CheckHoverMessage(const glm::vec3& pos, const glm::vec3& dir);
        ~CheckHoverMessage();
        MyBase::MessageType     getType() const override;
        const glm::vec3 position, direction;
    };

    class PlayerMoveCommand: public MyBase::Command {
    public:
        PlayerMoveCommand(MyCraft::PlayerModelController* model);
        ~PlayerMoveCommand();

        MyBase::MessageType getType()                               const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
    private:
        MyCraft::PlayerModelController*      __model;
    };
}
#endif