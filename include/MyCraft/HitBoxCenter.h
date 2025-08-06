#ifndef HITBOX_H
#define HITBOX_H
#include "Message.h"
#include "Controller.h"
#include "ModelController.h"
namespace MyCraft {
    class HitBoxCenter: public MyBase3D::Controller3D {
        public:
            HitBoxCenter();
            ~HitBoxCenter();
            bool isBusyBlock(const glm::ivec3& position) const;
            bool isColistion(const glm::vec3& position) const;
            void insert(ModelController* model);
            void erase(ModelController* model);
        private:
            void glDraw() const             override;
            glm::vec3              __colors;
            std::vector<ModelController*> __models;
    };
}
#endif