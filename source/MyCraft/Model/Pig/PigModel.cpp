#include "Pig/PigModel.h"
#include "PlayerModel.h"
namespace MyCraft {
    namespace Pig {
        Model::Model(): GLTFModel("assets/models/Pig/Pig.gltf"), __pigTexture("assets/models/Pig/Pig.png",false) {
        }
        Model::~Model() {}
        glm::mat4x3 Model::getShape()              const {
            return glm::mat4x3(1);
        }
        glm::vec3   Model::getPosition()           const {
            return glm::vec3(0);
        }
        void Model::look(const glm::vec3& position) {

        }
        bool Model::attack() {
            return false;
        }
        void Model::see(const glm::vec3&) {

        }
        void Model::move(const glm::vec3& direction) {

        }
        void Model::rotate(const glm::vec3& direction) {

        }
        void Model::setPosition(const glm::vec3& position) {

        }
        bool Model::apply() {
            return false;
        }
        void Model::draw() const {
            __pigTexture.Bind();
            GLTFModel::draw();
        }
    }
}