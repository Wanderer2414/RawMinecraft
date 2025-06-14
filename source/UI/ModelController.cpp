#include "ModelController.h"
#include "Message.h"

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

    MessageType MoveCommand::getType() const {
        return MessageType::MoveMessage;
    }
    void MoveCommand::execute(Port& mine, Port& source, Message* message)   {
        Move* moveMessage = (Move*)message;
        __model->move(moveMessage->direction);
        if (!__model->isFall())
            mine.send(new RequestFall(__model->getShape(), __model->getZVelocity()));
    }
    
    FallCommand::FallCommand(ModelController* model): __model(model) {}
    FallCommand::~FallCommand() {}

    MessageType FallCommand::getType() const {
        return MessageType::FallMessage;
    }
    void FallCommand::execute(Port& mine, Port& source, Message* message) {
        Fall* fall = (Fall*)message;
        __model->setFall(true);
        __model->setZVelocity(fall->zVelocity);
        __model->move(glm::vec3(0, 0, __model->getZVelocity()));
    }
    StopFallCommand::StopFallCommand(ModelController* model): __model(model) {}
    StopFallCommand::~StopFallCommand() {}

    MessageType StopFallCommand::getType() const {
        return MessageType::StopFallMessage;
    }
    void StopFallCommand::execute(Port& mine, Port& source, Message* message) {
        __model->setFall(false);
        __model->setZVelocity(0);
    }   
}