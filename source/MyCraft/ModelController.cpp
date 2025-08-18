#include "ModelController.h"
#include "Message.h"
#include "PlayerModelController.h"
#include "WorldRender.h"

namespace MyCraft {
    ModelController::ModelController(): __zVelocity(0) {}
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

    
    MoveCommand::MoveCommand(ModelController* model): __model(model) {}
    MoveCommand::~MoveCommand() {}

     MyBase::MessageType MoveCommand::getType() const {
        return  MyBase::MessageType::Move;
    }
    void MoveCommand::execute(MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message)   {
        MoveMessage* moveMessage = (MoveMessage*)message;
        __model->__move(moveMessage->direction);
        mine.send(new RequestFallMessage(__model->getShape(), __model->getZVelocity()));
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
}