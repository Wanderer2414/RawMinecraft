#include "Model.h"
#include "Message.h"

namespace MyCraft {
    Model::Model(): __isFall(false), __zVelocity(0) {}
    Model::~Model() {};

    bool Model::isFall() const {
        return __isFall;
    }
    float Model::getZVelocity() const {
        return __zVelocity;
    }
    void Model::setFall(const bool& isFall) {
        __isFall = isFall;
    }
    void Model::setZVelocity(const float& z) {
        __zVelocity = z;
    }
    MoveCommand::MoveCommand(Model* model): __model(model) {}
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
    
    FallCommand::FallCommand(Model* model): __model(model) {}
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
    StopFallCommand::StopFallCommand(Model* model): __model(model) {}
    StopFallCommand::~StopFallCommand() {}

    MessageType StopFallCommand::getType() const {
        return MessageType::StopFallMessage;
    }
    void StopFallCommand::execute(Port& mine, Port& source, Message* message) {
        __model->setFall(false);
        __model->setZVelocity(0);
    }   
}