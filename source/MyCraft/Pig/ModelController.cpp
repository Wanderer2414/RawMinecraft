#include "Pig/ModelController.h"
#include "Color.h"
#include "ModelController.h"
#include "Pig/Model.h"
#include "WorldRender.h"
namespace MyCraft {
    namespace Pig {
        Controller::Controller(): MyCraft::ModelController(50), __isChanged(false), __speed(0.2), __isDamage(false) {
            add(new MoveCommand(this));
            add(new FallCommand(this));
            add(new JumpCommand(this));
            add( new FocusCommand(this));
            update();
            __damageDuration.setDuration(200);
            __fallCheckClock.setDuration(30);
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
            send(new RequestGotoMessage(getShape(), dir));
        }
        void Controller::rotate(const glm::vec3& angle) {

        };
        void Controller::__see(const glm::vec3& dir) {
            
        }

        void Controller::__move(const glm::vec3& dir) {
            Pig::Model::move(dir);
            update();
            __rotate(dir);
        }
        void Controller::__look(const glm::vec3& pos) {
            Pig::Model::look(pos);
        }

        void Controller::__rotate(const glm::vec3& dir) {
            Pig::Model::rotate(dir);
            update();
        }
        glm::vec3 Controller::getPosition() const {
            return Pig::Model::getPosition();
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
    }
}