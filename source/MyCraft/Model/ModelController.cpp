#include "ModelController.h"
#include "HealthModule.h"
#include "HitboxTree.h"
#include "Message.h"
#include "Sun.h"
#include "WorldRender.h"
#include "Path.h"
namespace MyCraft {
    ModelController::ModelController(const unsigned int& max_heal): HealthModule(max_heal), HitboxNode(0), 
        __zVelocity(0), __path(0), __folowController(0) {
        __attackCooldown.setDuration(500);
    }
    ModelController::~ModelController() {
        if (__path) delete __path;
    };
    bool ModelController::canSaved() const {
        return false;
    }
    float ModelController::Powerness() const {
        return 0;
    }
    bool ModelController::hasPath() const {
        return __path;
    }

    float ModelController::getZVelocity() const {
        return __zVelocity;
    }
    void ModelController::setZVelocity(const float& z) {
        __zVelocity = z;
    }
    void ModelController::setPath(Path* path) {
        __path = path;
        if (__path) __path->setModelHost(this);
    }

    void ModelController::clearPath() {
        delete __path;
        __path = 0;
    }
    void ModelController::attack() {
        if (__attackCooldown.get() && __folowController) {
            __attackCooldown.restart();
            glm::vec3 dir = __folowController->getPosition() - getPosition();
            dir.z = 0;
            if (glm::length(dir)) dir = glm::normalize(dir)*2.f;
            dir.z = 0.2;
            __folowController->move(dir);
            __folowController->damage(Powerness());
        }
    }
    void ModelController::folow(ModelController* model) {
        __folowController = model;
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

    RotateMessage::RotateMessage(const glm::vec3& dir): direction(dir) {}
    RotateMessage::~RotateMessage() {}
    MyBase::MessageType RotateMessage::getType() const {
        return MyBase::Rotate;
    }

    MyBase::MessageType StopFallMessage::getType() const {
        return MyBase::MessageType::StopFall;
    }
    StopFallMessage::StopFallMessage() {};
    StopFallMessage::~StopFallMessage() {};

    FocusMessage::FocusMessage(ModelController* h): host(h) {}
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
        __model->__move(moveMessage->direction);
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

    RotateCommand::RotateCommand(MyCraft::ModelController* model): __model(model) {}
    RotateCommand::~RotateCommand() {}

    MyBase::MessageType RotateCommand::getType() const {
        return MyBase::Rotate;
    }
    void RotateCommand::execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
        RotateMessage* package = (RotateMessage*)message;
        __model->__rotate(package->direction);
    }

    FocusCommand::FocusCommand(MyCraft::ModelController* model): __model(model) {};
    FocusCommand::~FocusCommand() {};

    MyBase::MessageType FocusCommand::getType() const {
        return MyBase::Focus;
    }
    void FocusCommand::execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
        FocusMessage* package = (FocusMessage*)message;
        __model->look(package->host->getPosition());
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

    TimeDarknessCommand::TimeDarknessCommand(ModelController& darknessModel): __model(darknessModel) {}
    TimeDarknessCommand::~TimeDarknessCommand() {}

    MyBase::MessageType TimeDarknessCommand::getType() const {
        return MyBase::TimeNotice;
    }
    void TimeDarknessCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        TimeMessage* package = (TimeMessage*)message;
        if (package->time>0.25 && package->time<=0.75) {
            __model.damage(10);
        }
    }

    TimeLightnessCommand::TimeLightnessCommand(ModelController& darknessModel): __model(darknessModel) {}
    TimeLightnessCommand::~TimeLightnessCommand() {}

    MyBase::MessageType TimeLightnessCommand::getType() const {
        return MyBase::TimeNotice;
    }
    void TimeLightnessCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        TimeMessage* package = (TimeMessage*)message;
        if (package->time<0.25 || package->time>0.75) {
            __model.damage(10);
        }
    }
}