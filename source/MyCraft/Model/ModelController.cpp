#include "ModelController.h"
#include "HealthModule.h"
#include "HitboxTree.h"
#include "Message.h"
#include "WorldRender.h"

namespace MyCraft {
    ModelController::ModelController(const unsigned int& max_heal): HealthModule(max_heal), HitboxNode(0), __zVelocity(0) {}
    ModelController::~ModelController() {};

    float ModelController::getZVelocity() const {
        return __zVelocity;
    }
    void ModelController::setZVelocity(const float& z) {
        __zVelocity = z;
    }

    MoveMessage::MoveMessage(const glm::vec3& d, const float& Depth): direction(d), depth(Depth) {}
    MoveMessage::~MoveMessage() {}
    MyBase::MessageType MoveMessage::getType() const {
        return MyBase::MessageType::Move;
    }

    FallMessage::FallMessage(const float& z): zVelocity(z) {}
    FallMessage::~FallMessage() {}
    MyBase::MessageType FallMessage::getType() const  {
        return MyBase::MessageType::Fall;
    }

    MyBase::MessageType StopFallMessage::getType() const {
        return MyBase::MessageType::StopFall;
    }
    StopFallMessage::StopFallMessage() {};
    StopFallMessage::~StopFallMessage() {};

    FocusMessage::FocusMessage(const glm::vec3& pos): position(pos) {}
    FocusMessage::~FocusMessage() {};
    MyBase::MessageType FocusMessage::getType() const {
        return MyBase::Focus;
    }

    MoveCommand::MoveCommand(ModelController* model): __model(model) {}
    MoveCommand::~MoveCommand() {}

     MyBase::MessageType MoveCommand::getType() const {
        return  MyBase::MessageType::Move;
    }
    void MoveCommand::execute(MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message)   {
        MoveMessage* moveMessage = (MoveMessage*)message;
        
        if (glm::length(moveMessage->direction) && moveMessage->depth<=1) {
            __model->__move(moveMessage->direction);
            mine.send(new RequestFallMessage(__model->getShape(), __model->getZVelocity()));
        }
    }
    
    FallCommand::FallCommand(ModelController* model): __model(model) {}
    FallCommand::~FallCommand() {}

     MyBase::MessageType FallCommand::getType() const {
        return  MyBase::MessageType::Fall;
    }
    void FallCommand::execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
        FallMessage* fall = (FallMessage*)message;
        __model->setZVelocity(fall->zVelocity);
        __model->__move(glm::vec3(0, 0, __model->getZVelocity()));
    }
    StopFallCommand::StopFallCommand(ModelController* model): __model(model) {}
    StopFallCommand::~StopFallCommand() {}

     MyBase::MessageType StopFallCommand::getType() const {
        return  MyBase::MessageType::StopFall;
    }
    void StopFallCommand::execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
        __model->setZVelocity(0);
    }   

    JumpCommand::JumpCommand(ModelController* model): __model(model) {}
    JumpCommand::~JumpCommand() {}

    MyBase::MessageType JumpCommand::getType() const {
        return MyBase::Jump;
    }
    void JumpCommand::execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
        JumpMessage* package = (JumpMessage*)message;
        __model->setZVelocity(package->zVelocity);
        __model->__move({0,0,package->zVelocity});
    }

    FocusCommand::FocusCommand(MyCraft::ModelController* model): __model(model) {};
    FocusCommand::~FocusCommand() {};

    MyBase::MessageType FocusCommand::getType() const {
        return MyBase::Focus;
    }
    void FocusCommand::execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
        FocusMessage* package = (FocusMessage*)message;
        __model->look(package->position);
    }

    DiveMessage::DiveMessage() {}
    DiveMessage::~DiveMessage() {}
    MyBase::MessageType DiveMessage::getType() const {
        return MyBase::Dive;;
    }
    OnGroundMessage::OnGroundMessage() {}
    OnGroundMessage::~OnGroundMessage() {}
    MyBase::MessageType OnGroundMessage::getType() const {
        return MyBase::OnGround;
    }

    AttackMessage::AttackMessage(const glm::vec3& pos, const glm::vec3& dir, const ItemType& left, const ItemType& right): position(pos), direction(dir), leftItem(left), rightItem(right) {}
    AttackMessage::~AttackMessage() {}
    MyBase::MessageType AttackMessage::getType() const{
        return MyBase::MessageType::Attack;
    }

    PlaceMessage::PlaceMessage(const glm::vec3& pos, const glm::vec3& dir, const ItemType& left, const ItemType& right): position(pos), direction(dir), rightItem(right), leftItem(left) {}
    PlaceMessage::~PlaceMessage() {}
    MyBase::MessageType PlaceMessage::getType() const {
        return MyBase::MessageType::Place;
    }
}