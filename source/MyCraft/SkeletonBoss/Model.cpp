#include "SkeletonBoss/Model.h"
#include "GLTFModel.h"
namespace MyCraft {
    namespace SkeletonBoss {
        
        Model::Model(): GLTFModel("assets/models/SkeletonBoss/SkeletonBoss.gltf"), 
        __texture("assets/models/SkeletonBoss/SkeletonBoss.png", false), 
        __diagonal(1.5, 1.5, 5), __position(0,0,0), __direction(0, 1, 0) {

        }
        Model::~Model() {}

        glm::mat4x3 Model::getShape() const   {
            glm::mat4x3 shape(0);
            shape[0] = __position;
            shape[1] = {__diagonal.x, 0, 0};
            shape[2] = {0, __diagonal.y, 0};
            shape[3] = {0,0, __diagonal.z};

            {
                float angle = glm::angle(glm::normalize(__direction), {0, 1, 0});
                if (__direction.x>0) angle = -angle;
                shape[1] = glm::rotate(angle, glm::vec3(0,0,1))*glm::vec4(shape[1],1);
                shape[2] = glm::rotate(angle, glm::vec3(0,0,1))*glm::vec4(shape[2],1);
            }
            
            shape[0] += shape[1]/2.f;
            shape[0] += shape[2]/2.f;
            shape[1] = -shape[1];
            shape[2] = -shape[2];
            return shape;
        };
        glm::vec3 Model::getPosition() const   {
            return __position;
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