#include "SkeletonBoss/Model.h"
#include "GLTFModel.h"
namespace MyCraft {
    namespace SkeletonBoss {
        
        Model::Model(): GLTFModel("assets/models/SkeletonBoss/SkeletonBoss.gltf"), __texture("assets/models/SkeletonBoss/SkeletonBoss.png", false) {}
        Model::~Model() {}

        glm::mat4x3 Model::getShape()              const   {
            return glm::mat4x3(0);
        };
        glm::vec3   Model::getPosition()           const   {
            return glm::vec3(1);
        };
        void Model::look(const glm::vec3& position)        {

        };
        bool Model::attack()                               {

        };
        void Model::see(const glm::vec3& direction)        {

        };
        void Model::move(const glm::vec3& direction)       {

        };
        void Model::rotate(const glm::vec3& direction)     {

        };
        void Model::setPosition(const glm::vec3& position) {

        };
        bool Model::apply()                                {

        };

        void Model::draw() const {
            __texture.Bind();
            GLTFModel::draw();
        }
    };
}