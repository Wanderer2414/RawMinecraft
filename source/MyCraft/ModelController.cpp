#include "ModelController.h"
#include "Message.h"
#include "PlayerModelController.h"

namespace MyCraft {
    ModelController::ModelController(): __isFall(false), __zVelocity(0) {}
    ModelController::~ModelController() {};

    bool ModelController::isFall() const {
        return __isFall;
    }
    float ModelController::getZVelocity() const {
        return __zVelocity;
    }
    void ModelController::setFall(const bool& isFall) {
        __isFall = isFall;
    }
    void ModelController::setZVelocity(const float& z) {
        __zVelocity = z;
    }
    MoveCommand::MoveCommand(ModelController* model): __model(model) {}
    MoveCommand::~MoveCommand() {}

     MyBase::MessageType MoveCommand::getType() const {
        return  MyBase::MessageType::Move;
    }
    void MoveCommand::execute(MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message)   {
        MoveMessage* moveMessage = (MoveMessage*)message;
        __model->move(moveMessage->direction);
        if (!__model->isFall())
            mine.send(new RequestFallMessage(__model->getShape(), __model->getZVelocity()));
    }
    
    FallCommand::FallCommand(ModelController* model): __model(model) {}
    FallCommand::~FallCommand() {}

     MyBase::MessageType FallCommand::getType() const {
        return  MyBase::MessageType::Fall;
    }
    void FallCommand::execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
        FallMessage* fall = (FallMessage*)message;
        __model->setFall(true);
        __model->setZVelocity(fall->zVelocity);
        __model->move(glm::vec3(0, 0, __model->getZVelocity()));
    }
    StopFallCommand::StopFallCommand(ModelController* model): __model(model) {}
    StopFallCommand::~StopFallCommand() {}

     MyBase::MessageType StopFallCommand::getType() const {
        return  MyBase::MessageType::StopFall;
    }
    void StopFallCommand::execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
        __model->setFall(false);
        __model->setZVelocity(0);
    }   
}