#ifndef SKELETON_BOSS_H
#define SKELETON_BOSS_H
#include "GLTFModel.h"
#include "Texture.h"
namespace MyCraft {
    namespace SkeletonBoss {
        class Model: public GLTFModel {
        public:
            Model();
            ~Model();
            Model(const Model&) = delete;
            Model& operator=(const Model&) const = delete; 
        protected:
            glm::mat4x3 getShape()              const   override;
            glm::vec3   getPosition()           const   override;
            void look(const glm::vec3& position)        override;
            bool attack()                               override;
            void see(const glm::vec3& direction)        override;
            void move(const glm::vec3& direction)       override;
            void rotate(const glm::vec3& direction)     override;
            void setPosition(const glm::vec3& position) override;
            bool apply()                                override;

            void draw() const;
        private:
            MyBase::Texture   __texture;
            const glm::vec3 __diagonal;
            glm::vec3       __position, __direction;
        };
    };
}
#endif