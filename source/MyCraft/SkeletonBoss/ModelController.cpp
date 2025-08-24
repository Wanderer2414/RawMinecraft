#include "SkeletonBoss/ModelController.h"
#include "HitboxTree.h"
#include "Model/ModelController.h"
namespace MyCraft {
    namespace SkeletonBoss {
        Controller::Controller(): MyCraft::ModelController(500) {
            update();
        }
        Controller::~Controller() {}

        void Controller::see(const glm::vec3& dir) {
            
        }
        void Controller::look(const glm::vec3& position) {
            
        }
        void Controller::move(const glm::vec3& dir) {
            
        }
        void Controller::rotate(const glm::vec3& angle) {
            
        }
        void Controller::__see(const glm::vec3& dir) {
            
        }
        void Controller::__look(const glm::vec3& position) {
            
        }
        void Controller::__move(const glm::vec3& dir) {
            
        }
        void Controller::__rotate(const glm::vec3& angle) {
            
        }
        void Controller::__dead() {}
        void Controller::__damage() {}
        void Controller::__heal() {}
        void Controller::glDrawTransparent() const {
            Model::draw();
        }

        void Controller::update() {
            setShape(Model::getShape());
            HitboxNode::update();
        }
        glm::vec3 Controller::getPosition() const {
            return glm::vec3(0,0,0);
        }
        void Controller::save(std::ostream& cout) {}
        void Controller::__load(std::istream& cin) {}
    }
}