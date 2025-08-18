#ifndef PLAYER_MODEL_CONTROLLER_H
#define PLAYER_MODEL_CONTROLLER_H
#include "Block.h"
#include "Clock.h"
#include "HealthModule.h"
#include "Item.h"
#include "Message.h"
#include "ModelController.h"
#include "PlayerInventoryModule.h"
#include "PlayerModel.h"
namespace MyCraft {
    class PlayerModelController: public ModelController, public MyBase::Port, public PlayerInventoryModule, public HealthModule, public Player::Model {
    public: 
        PlayerModelController();
        ~PlayerModelController();
        glm::vec3   getPosition() const    override,
                    getDirection() const;
        void        move(const glm::vec3& delta) override,
                    rotate(const float& angle)  override,
                    teleport(const glm::vec3& position),
                    rotate(const glm::vec3& dir) override,
                    rightAttack(),
                    leftAttack(),
                    see(const glm::vec3& dir) override,
                    seeRotate(const float& horizontal, const float& vertical),
                    setDrawAble(const bool& drawable),
                    glDraw() const override,
                    
                    setHoverBlock(const glm::vec3& hover, const glm::vec3& place, const BlockCatogary& type);
        glm::mat4x3 getShape() const override;
    private:
        bool            __isDrawable, __isChanged;
        float           __speed;
        glm::vec3       __diagonal;
        glm::vec3       __direction, __eye_direction;
        MyBase::Clock   __speedControl;
        BlockCatogary   __type;
        glm::vec3       __toAbsoluteCoordinate(const glm::vec3& dir) const;
        bool            __moveManage(GLFWwindow* window);
        void            __damage() override;
        void            __dead() override;
        void            __heal() override;
        
        bool            handle(GLFWwindow* window) override;
        bool            catchEvent(GLFWwindow* window) override;
        void            reset() override;
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
    

    class PlaceMessage: public MyBase::Message {
    public:
        PlaceMessage(const glm::vec3& position, const glm::vec3& direction, const ItemType& left, const ItemType& right);
        ~PlaceMessage();
        MyBase::MessageType getType() const override;
        const glm::vec3 position, direction;
        const ItemType rightItem, leftItem;
    };

    class AttackMessage: public MyBase::Message {
    public:
        AttackMessage(const glm::vec3& position, const glm::vec3& direction, const ItemType& left, const ItemType& right);
        ~AttackMessage();
        MyBase::MessageType     getType() const override;
        const glm::vec3 position, direction;
        const ItemType rightItem, leftItem;
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
    class DiveMessage: public MyBase::Message {
    public:
        DiveMessage();
        ~DiveMessage();
        MyBase::MessageType getType()                               const override;
    };
    class DiveCommand: public MyBase::Command {
    public:
        DiveCommand(PlayerModelController& model);
        ~DiveCommand();
        MyBase::MessageType getType()                               const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
    private:
        PlayerModelController& __model;
    };

    class OnGroundMessage: public MyBase::Message {
    public:
        OnGroundMessage();
        ~OnGroundMessage();
        MyBase::MessageType getType()                               const override;
    };
    class OnGroundCommand: public MyBase::Command {
    public:
        OnGroundCommand(PlayerModelController& model);
        ~OnGroundCommand();
        MyBase::MessageType getType()                               const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
    private:
        PlayerModelController& __model;
    };
}
#endif