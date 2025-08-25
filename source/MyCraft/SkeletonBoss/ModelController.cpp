#include "SkeletonBoss/ModelController.h"
#include "HitboxTree.h"
#include "Model/ModelController.h"
#include "WorldRender.h"
namespace MyCraft {
    namespace SkeletonBoss {
        ModelController::ModelController(): MyCraft::ModelController(500) {
            add(new MoveCommand(this));
            add(new FallCommand(this));
            add(new JumpCommand(this));
            add(new FocusCommand(this));
            update();
            __fallCheckClock.setDuration(30);
            __damageDuration.setDuration(200);

        }
        ModelController::~ModelController() {}

        void ModelController::see(const glm::vec3& dir) {
            
        }
        void ModelController::look(const glm::vec3& position) {
            __look(position);
        }
        void ModelController::move(const glm::vec3& dir) {
            if(dir.z) send(new RequestJumpMessage(getShape(), dir.z));
            send(new RequestGotoMessage(getShape(), dir));
        }
        void ModelController::rotate(const glm::vec3& angle) {
            
        }
        void ModelController::__see(const glm::vec3& dir) {
            
        }
        void ModelController::__look(const glm::vec3& position) {
            Model::look(position);
        }
        void ModelController::__move(const glm::vec3& dir) {
            ModelController::move(dir);
            update();
            __rotate(dir);
        }
        void ModelController::__rotate(const glm::vec3& angle) {
            
        }
        void ModelController::__dead() {

        }
        void ModelController::__damage() {

        }
        void ModelController::__heal() {
            
        }
        void ModelController::glDrawTransparent() const {
            Model::draw();
        }

        void ModelController::update() {
            setShape(Model::getShape());
            HitboxNode::update();
        }
        glm::vec3 ModelController::getPosition() const {
            return Model::getPosition();
        }

        glm::mat4x3 ModelController::getShape() const {
            return Model::getShape();
        }

        bool ModelController::handle(GLFWwindow* window) {
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

        void ModelController::damage(const unsigned int& damage) {
            __damageDuration.restart();
            __isDamage = true;
            __isChanged = true;
            setBaseColor({255, 0, 0, 100});
        }
        void ModelController::heal(const unsigned int& health) {
            
        }
        void ModelController::glDraw() const {
            Model::draw();
        }

    }
}