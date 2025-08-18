#include "PlayerModel.h"
#include "General.h"
#include "glm/geometric.hpp"

namespace MyCraft {
    namespace Player {
        Model::Model(): GLTFModel("assets/models/Player/Steve.gltf"), 
            __walk(Animations("walk")), 
            __left_attack(Animations("left_attack")),
            __right_attack(Animations("right_attack")),
            __crouch(Animations("crouch")),
            __swim(Animations("swim")),
            __isChanged(false), __position(0), __isSwim(false),
            __isCrouch(false),  __moveTime(0), __isRightAttack(false), __isLeftAttack(false) 
        {
            __skin.load("assets/models/Player/Steve.png", false);
            __move_clock.setDuration(40);
            __attack_clock.setDuration(250);
        }
        Model::~Model() {}

        bool Model::isCrouch() const {
            return __isCrouch;
        }
        bool Model::reset() {
            if (__move_clock.get() && __moveTime) {
                __moveTime = 0;
                __isChanged = true;
                return true;
            }
            return false;
        }
        bool Model::isSwim() const {
            return __isSwim;
        }
        bool Model::apply() {
            if (!__move_clock.get() || !__attack_clock.get()) __isChanged = true;
            if (__isChanged) {
                States().reset();
                if (__isSwim) __swim.apply(States(), 0.1 + __moveTime*0.8);
                else __walk.apply(States(), __moveTime);
                if (__isCrouch) __crouch.apply(States(), 1);

                if (!__attack_clock.get()) {
                    if (__isRightAttack) __right_attack.apply(States(), (GetTime()-__start_attack_time)/250.f);
                    else if (__isLeftAttack) __left_attack.apply(States(), (GetTime()-__start_attack_time)/250.f);
                }
                else __isLeftAttack = __isRightAttack = false;

                {
                    glm::vec3 xy(__eye_direction.x, __eye_direction.y, 0);
                    xy = glm::normalize(xy);
                    float angle = glm::angle(xy, glm::vec3(1,0,0)), __angle = glm::angle(__direction, glm::vec3(1,0,0));
                    if (xy.y>0) angle = -angle;
                    if (__direction.y>0) __angle = -__angle;

                    angle = __angle - angle;
                    if (angle<-M_PI*0.75) angle = -0.75*M_PI;
                    else if (angle>M_PI*0.75) angle = M_PI*0.75;
                    States()[5] *= glm::rotate(angle, glm::vec3(0,0,1));

                    angle = glm::angle(__eye_direction, xy);
                    if (__eye_direction.z<0) angle = -angle;

                    States()[5] *= glm::rotate(angle, glm::vec3(1, 0, 0));
                }
                {
                    States()[12] = glm::mat4(1);
                    States()[12][3] = glm::vec4(__position,1);
                }
                {
                    float angle = glm::angle(__direction, glm::vec3(0,1,0));
                    if (__direction.x > 0) angle = -angle;
                    States()[12] *= glm::rotate(angle, glm::vec3(0,0,1));
                }
                __isChanged = false;
                return true;   
            }
            return false;
        };
        glm::vec3 Model::getEyePosition() const {
            if (__isCrouch) return {__position.x, __position.y, __position.z + 1.3};
            else return {__position.x, __position.y, __position.z + 1.7};
        }
        glm::mat4x3 Model::getShape()              const {
            return glm::mat4x3(1);
        }
        glm::vec3 Model::getPosition() const {
            return __position;
        }
        void Model::swim() {
            __isSwim = true;
            __isChanged = true;
        }
        void Model::walk() {
            __isSwim = false;
            __isChanged = true;
        }
        void Model::rotate(const glm::vec3& direction) {
            __direction = direction;
            __isChanged = true;
        }
        void Model::crouch() {
            __isCrouch = true;
            __isChanged = true;
        }
        void Model::uncrouch() {
            __isCrouch = false;
            __isChanged = true;
        }
        bool Model::attack() {
            if (__attack_clock.get()) {
                __attack_clock.restart();
                __start_attack_time = GetTime();
                __isChanged = true;
                __isRightAttack = !(__isLeftAttack = false);
                return true;
            }
            return false;
        }
        bool Model::left_attack() {
            if (__attack_clock.get()) {
                __attack_clock.restart();
                __start_attack_time = GetTime();
                __isChanged = true;
                __isLeftAttack = !(__isRightAttack = false);
                return true;
            }
            return false;
        }
        void Model::look(const glm::vec3& position)         {
            
        }
        void Model::see(const glm::vec3& dir)                 {
            __eye_direction = dir;
            __isChanged = true;
        }
        void Model::move(const glm::vec3& dir)       {
            glm::vec2 direction = dir;
            __position += dir;
            __moveTime += glm::length(direction)/4;
            while (__moveTime>1) __moveTime--;
            __move_clock.restart();
        }
        void Model::setPosition(const glm::vec3& position) {
            __position = position;
            __isChanged = true;
        }
        void Model::draw() const {
            __skin.Bind();
            GLTFModel::draw();
        }
    }
}