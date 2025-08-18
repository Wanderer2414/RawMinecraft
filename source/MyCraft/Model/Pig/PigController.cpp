#include "Pig/PigController.h"
namespace MyCraft {
    namespace Pig {
        PigController::PigController() {}
        PigController::~PigController() {}

        void PigController::update() {}
        void PigController::see(const glm::vec3& dir) {
            
        }
        void PigController::move(const glm::vec3& dir) {
            
        }
        void PigController::rotate(const float& angle) {
            
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