#include "SkeletonBoss/ModelController.h"
#include "Model/ModelController.h"
namespace MyCraft {
    namespace SkeletonBoss {
        ModelController::ModelController(): MyCraft::ModelController(500) {}
        ModelController::~ModelController() {}

        void ModelController::see(const glm::vec3& dir) {
            
        }
        void ModelController::look(const glm::vec3& position) {
            
        }
        void ModelController::move(const glm::vec3& dir) {
            
        }
        void ModelController::rotate(const glm::vec3& angle) {
            
        }
        void ModelController::__see(const glm::vec3& dir) {
            
        }
        void ModelController::__look(const glm::vec3& position) {
            
        }
        void ModelController::__move(const glm::vec3& dir) {
            
        }
        void ModelController::__rotate(const glm::vec3& angle) {
            
        }
        void ModelController::__dead() {}
        void ModelController::__damage() {}
        void ModelController::__heal() {}
        void ModelController::glDrawTransparent() const {
            Model::draw();
        }

        void ModelController::update() {}
        glm::vec3 ModelController::getPosition() const {}

    }
}