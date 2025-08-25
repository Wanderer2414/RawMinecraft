#ifndef SKELETON_BOSS_MODEL_H
#define SKELETON_BOSS_MODEL_H
#include "Clock.h"
#include "GLTFModel.h"
#include "WalkAnimation.h"
#include "Texture.h"
namespace MyCraft {
    namespace SkeletonBoss {
        class Model: public GLTFModel {
        public:
            Model();
            ~Model();
        protected:

            glm::mat4x3 getShape()              const   override;
            glm::vec3   getPosition()           const   override;
            glm::vec3   getDirection()          const;
            void look(const glm::vec3& position)        override;
            bool attack()                               override;
            void see(const glm::vec3& direction)        override;
            void move(const glm::vec3& direction)       override;
            void rotate(const glm::vec3& direction)     override;
            void setPosition(const glm::vec3& position) override;
            bool apply()                                override;
            void draw() const;
        private:
            bool            __isChanged, __isFocus;
            float           __moveTime;
            MyBase::Texture __pigTexture;
            const glm::vec2 __diagonal;
            glm::vec3       __position;
            glm::vec3       __direction, __eye_focus;
            WalkAnimation __walk;
            MyBase::Clock __moveClock;
        };
    }
}
#endif