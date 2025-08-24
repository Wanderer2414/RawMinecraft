#include "Zombie/ModelController.h"
#include "Color.h"
#include "HealthModule.h"
#include "ModelController.h"
#include "PathCreator.h"
#include "Zombie/Model.h"
#include "WorldRender.h"
#include "Path.h"
#include "glm/geometric.hpp"
namespace MyCraft {
    namespace Zombie {
        Controller::Controller(): MyCraft::ModelController(50), __isChanged(false), __speed(0.2), __isDamage(false) {
            add(new MoveCommand(this));
            add(new FallCommand(this));
            add(new JumpCommand(this));
            add( new FocusCommand(this));
            add(new RotateCommand(this));
            update();
            __damageDuration.setDuration(200);
            __fallCheckClock.setDuration(30);
            __freeTime.setDuration(3000);
        }
        Controller::~Controller() {}

        bool Controller::handle(GLFWwindow* window) {
            __isChanged = apply() || __isChanged;
            if (__damageDuration.get() && __isDamage) {
                __isDamage = false;
                __isChanged = true;
                setBaseColor(TRANSPARENCY);
            }
            if (__fallCheckClock.get()) {
                __fallCheckClock.restart();
                send(new RequestFallMessage(getShape(), getZVelocity()));
            }
            if (__folowController && !__path) {
                glm::vec3 direction = glm::normalize(__folowController->getPosition() - getPosition());
                float distance = glm::length(__folowController->getPosition()-getPosition());
                if (distance > 1) {
                    send(new CreateFollowPathMessage(this, __folowController->getPosition() - direction*0.5f));
                }
                else ModelController::attack();
            }
            if (__path) {
                __freeTime.restart();
                __isChanged = __path->handle(window) || __isChanged;
                if (__path->size()<=1) {
                    delete __path;
                    __path = 0;
                }
            }
            if (__freeTime.get()) send(new CreatePatrolPathMessage(this));
            if (__isChanged) return !(__isChanged = false);
            return false;
        }

        void Controller::damage(const unsigned int& damage) {
            __damageDuration.restart();
            __isDamage = true;
            __isChanged = true;
            setBaseColor({255, 0, 0, 100});
        }
        void Controller::heal(const unsigned int& health) {
            
        }

        void Controller::update() {
            setShape(Model::getShape());
        }
        void Controller::see(const glm::vec3& dir) {

        }
        void Controller::look(const glm::vec3& position) {
            __look(position);
        }
        void Controller::move(const glm::vec3& dir) {
            if (dir.z) send(new RequestJumpMessage(getShape(), dir.z));
            rotate(dir);
            send(new RequestGotoMessage(getShape(), dir));
        }
        void Controller::rotate(const glm::vec3& dir) {
            float angle = glm::angle(dir, glm::vec3(0,1,0));
            if (dir.x>0) angle   = -angle;
            float __angle = glm::angle(getDirection(), glm::vec3(0,1,0));
            if (dir.x>0) __angle = -__angle;
            send(new RequestRotateMessage(getShape(), angle-__angle, dir));
        };
        void Controller::__see(const glm::vec3& dir) {
            
        }

        void Controller::__move(const glm::vec3& dir) {
            Model::move(dir);
            update();
        }
        void Controller::__look(const glm::vec3& pos) {
            Model::look(pos);
        }

        void Controller::__rotate(const glm::vec3& dir) {
            Model::rotate(dir);
            update();
        }
        glm::vec3 Controller::getPosition() const {
            return Model::getPosition();
        }
        glm::mat4x3 Controller::getShape() const {
            return Model::getShape();
        }
        void Controller::glDraw() const {
            draw();
        }

        void Controller::__dead() {

        }
        void Controller::__damage() {

        }
        void Controller::__heal() {

        }
        void Controller::save(std::ostream& cout) {}
        void Controller::__load(std::istream& cin) {}

        FocusCommand::FocusCommand(MyCraft::ModelController* model): __model(model) {};
        FocusCommand::~FocusCommand() {};

        MyBase::MessageType FocusCommand::getType() const {
            return MyBase::Focus;
        }
        void FocusCommand::execute(MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message) {
            FocusMessage* package = (FocusMessage*)message;
            __model->look(package->host->getPosition());
            float distance = glm::length(package->host->getPosition()-__model->getPosition());
            if (distance < 10) {
                __model->folow(package->host);
            }
            else __model->folow(0);

        }
    }
}