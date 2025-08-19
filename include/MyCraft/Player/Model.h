#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H
#include "AttackAnimation.h"
#include "Clock.h"
#include "CrouchAnimation.h"
#include "GLTFAnimation.h"
#include "GLTFModel.h"
#include "SwimAnimation.h"
#include "Texture.h"
#include "WalkAnimation.h"
namespace MyCraft {
    namespace Player {
        class Model: public GLTFModel {
        public:
            Model();
            ~Model();
            bool isCrouch() const;
            bool isSwim() const;
            Model(const Model&) = delete;
            Model& operator=(const Model&) const = delete; 
            glm::vec3 getEyePosition() const;
            glm::vec3 getPosition() const override;
            void swim();
            void walk();
        protected:
            glm::mat4x3 getShape()              const   override;
            void crouch();
            void uncrouch();
            void look(const glm::vec3& position)        override;
            void see(const glm::vec3& dir)              override;
            bool attack()                               override;
            bool left_attack();
            void move(const glm::vec3& direction)       override;
            void rotate(const glm::vec3& direction)     override;
            void setPosition(const glm::vec3& position) override;
            bool apply()                                override;
            bool reset();
            void draw() const;
        private:
            bool            __isCrouch,__isRightAttack, __isLeftAttack, __isChanged, __isSwim;
            float           __moveTime;
            const glm::vec2 __diagonal;
            glm::vec3       __eye_direction, __position, __direction;
            size_t          __start_attack_time;
            MyBase::Texture __skin;
            WalkAnimation   __walk;
            AttackAnimation __left_attack, __right_attack;
            CrouchAnimation __crouch;
            SwimAnimation   __swim;
            MyBase::Clock   __move_clock, __attack_clock;
        };
    }
}
#endif