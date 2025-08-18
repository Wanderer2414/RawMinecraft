#include "Pig/PigController.h"
#include "ModelController.h"
#include "PigModel.h"
#include "WorldRender.h"
namespace MyCraft {
    namespace Pig {
        PigController::PigController(): __isChanged(false), __speed(0.2) {
            add(new MoveCommand(this));
            add(new FallCommand(this));
        }
        PigController::~PigController() {}

        bool PigController::handle(GLFWwindow* window) {
            __isChanged = apply() || __isChanged;
            if (__isChanged) return !(__isChanged = false);
            return false;
        }
        void PigController::update() {}
        void PigController::see(const glm::vec3& dir) {

        }
        void PigController::move(const glm::vec3& dir) {
            send(new RequestGotoMessage(getShape(), dir));
        }
        void PigController::rotate(const glm::vec3& angle) {

        };
        void PigController::__see(const glm::vec3& dir) {
            
        }

        void PigController::__move(const glm::vec3& dir) {
            Pig::Model::move(dir);
            __rotate(dir);
        }

        void PigController::__rotate(const glm::vec3& dir) {
            Pig::Model::rotate(dir);
        }
        glm::vec3 PigController::getPosition() const {
            
        }
        glm::mat4x3 PigController::getShape() const {
            return Model::getShape();
        }
        void PigController::glDraw() const {
            draw();
        }
    }
}