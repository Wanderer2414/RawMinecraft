#include "Pig/PigModel.h"
#include "PlayerModel.h"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/geometric.hpp"
namespace MyCraft {
    namespace Pig {
        Model::Model(): GLTFModel("assets/models/Pig/Pig.gltf"), 
            __pigTexture("assets/models/Pig/Pig.png",false), 
            __walk(Animations("Walk")),
            __diagonal(0.5, 1.4), __position(0), __moveTime(0), __isChanged(false)
        {
            __moveClock.setDuration(250);
        }
        Model::~Model() {}
        glm::mat4x3 Model::getShape() const {
            glm::mat4x3 shape(0);
            shape[0] = __position;
            shape[1] = {__diagonal.x, 0, 0};
            shape[2] = {0, __diagonal.y, 0};
            shape[3] = {0,0,0.8};

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
        }
        glm::vec3   Model::getPosition()           const {
            return __position;
        }
        void Model::look(const glm::vec3& position) {

        }
        bool Model::attack() {
            return false;
        }
        void Model::see(const glm::vec3& direction) {

        }
        void Model::move(const glm::vec3& direction) {
            __position += direction;
            __moveTime += glm::length(direction)/2;
            if (__moveTime>1) __moveTime--;
            __isChanged = true;
            __moveClock.restart();
        }
        void Model::rotate(const glm::vec3& direction) {
            if (direction.x || direction.y) {
                __direction = direction;
                __direction.z = 0;
                __direction = glm::normalize(__direction);
                __isChanged = true;
            }
        }
        void Model::setPosition(const glm::vec3& position) {

        }
        bool Model::apply() {
            if (__moveClock.get()) __isChanged = true;

            if (__isChanged) {
                States().reset();
                __walk.apply(States(), __moveTime);
                States()[13][3] = glm::vec4(__position, 1);

                {
                    float angle = glm::angle(glm::normalize(__direction), {0, 1, 0});
                    if (__direction.x>0) angle = -angle;
                    States()[13] *= glm::rotate(angle, glm::vec3(0,0,1));
                }
                __isChanged = false;
                return true;
            }
            return false;
        }
        void Model::draw() const {
            __pigTexture.Bind();
            GLTFModel::draw();
        }
    }
}