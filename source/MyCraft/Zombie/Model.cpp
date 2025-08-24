#include "Zombie/Model.h"
#include "Global.h"
namespace MyCraft {
    namespace Zombie {
        Model::Model(): GLTFModel("assets/models/Zombie/Zombie.gltf"), 
            __pigTexture("assets/models/Zombie/Zombie.png",false), __direction(0, 1, 0),
            __walk(Animations("walk")), 
            __eye_focus(0), __isFocus(false),
            __diagonal(0.8, 0.4), __position(0), __moveTime(0), __isChanged(false)
        {
            __moveClock.setDuration(250);
        }
        Model::~Model() {}
        glm::mat4x3 Model::getShape() const {
            glm::mat4x3 shape(0);
            shape[0] = __position;
            shape[1] = {__diagonal.x, 0, 0};
            shape[2] = {0, __diagonal.y, 0};
            shape[3] = {0,0,2.15};

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

        glm::vec3   Model::getDirection()          const {
            return __direction;
        }
        void Model::look(const glm::vec3& position) {
            glm::vec3 direction = position - getPosition();
            if (glm::length(direction)<10 && glm::length(direction)>0.1) {
                __eye_focus = position;
                __isFocus = true;
            }
            else __isFocus = false;
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
            __position = position;
            __isChanged = true;
        }
        bool Model::apply() {
            if (__moveClock.get() && __moveTime) {
                __isChanged = true;
                __moveTime = 0;
            }

            if (__isChanged) {
                States().reset();
                __walk.apply(States(), __moveTime);
                States()[12][3] = glm::vec4(__position, 1);

                {
                    float angle = glm::angle(glm::normalize(__direction), {0, 1, 0});
                    if (__direction.x>0) angle = -angle;
                    States()[12] *= glm::rotate(angle, glm::vec3(0,0,1));
                }

                if (__isFocus) {
                    glm::vec3 eye_direction = __eye_focus-__position;
                    glm::vec3 xy(eye_direction.x, eye_direction.y, 0);
                    xy = glm::normalize(xy);
                    float angle = glm::angle(xy, glm::vec3(1,0,0)), __angle = glm::angle(__direction, glm::vec3(1,0,0));
                    if (xy.y>0) angle = -angle;
                    if (__direction.y>0) __angle = -__angle;

                    angle = __angle - angle;
                    if (angle<-M_PI*0.6) angle = -0.6*M_PI;
                    else if (angle>M_PI*0.6) angle = M_PI*0.6;
                    States()[1] *= glm::rotate(angle, glm::vec3(0,0,1));

                    angle = glm::angle(eye_direction, xy);
                    if (eye_direction.z<0) angle = -angle;

                    States()[1] *= glm::rotate(angle, glm::vec3(1, 0, 0));
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