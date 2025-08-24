#ifndef HITBOX_H
#define HITBOX_H
#include "Container3D.h"
#include "HitboxTree.h"
#include "ModelController.h"
namespace MyCraft {
    class HitBoxCenter: public MyBase3D::Container3D {
        public:
            HitBoxCenter();
            ~HitBoxCenter();
            bool isHover() const;
            //Check whether one position of block is collision with any model?
            bool isBusyBlock(const glm::ivec3& position) const;
            //Check whether one point is in any model?
            bool isColistion(const glm::vec3& position) const;
            void setHoverEntity(ModelController* controller);
            void attackEntity(const unsigned int& damage, const glm::vec3& direction);
            void feedEntity(const unsigned int& health, const glm::vec3& direction);
            ModelController* isColistion(const glm::vec3& position, const glm::vec3& direction) const;
            void pushPlayerModel(ModelController* model);
            void insert(ModelController* model);
            void erase(ModelController* model);
        private:
            bool handle(GLFWwindow* window) override;
            void glDraw() const             override;
            glm::vec3                       __colors;
            HitboxTree                      __tree;
            ModelController                 *__hoverEntity;
    };
}
#endif