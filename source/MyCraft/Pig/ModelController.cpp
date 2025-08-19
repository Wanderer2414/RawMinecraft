#include "Pig/ModelController.h"
#include "ModelController.h"
#include "Pig/Model.h"
#include "WorldRender.h"
namespace MyCraft {
    namespace Pig {
        Controller::Controller(): __isChanged(false), __speed(0.2) {
            add(new MoveCommand(this));
            add(new FallCommand(this));
            add( new FocusCommand(this));
        }
        Controller::~Controller() {}

        bool Controller::handle(GLFWwindow* window) {
            __isChanged = apply() || __isChanged;
            if (__isChanged) return !(__isChanged = false);
            return false;
        }
        void Controller::update() {}
        void Controller::see(const glm::vec3& dir) {

        }
        void Controller::look(const glm::vec3& position) {
            __look(position);
        }
        void Controller::move(const glm::vec3& dir) {
            send(new RequestGotoMessage(getShape(), dir));
        }
        void Controller::rotate(const glm::vec3& angle) {

        };
        void Controller::__see(const glm::vec3& dir) {
            
        }

        void Controller::__move(const glm::vec3& dir) {
            Pig::Model::move(dir);
            __rotate(dir);
        }
        void Controller::__look(const glm::vec3& pos) {
            Pig::Model::look(pos);
        }

        void Controller::__rotate(const glm::vec3& dir) {
            Pig::Model::rotate(dir);
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
    }
}