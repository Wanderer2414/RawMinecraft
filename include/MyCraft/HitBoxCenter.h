#ifndef HITBOX_H
#define HITBOX_H
#include "Message.h"
#include "Controller.h"
#include "ModelController.h"
namespace MyCraft {
    class HitBoxCenter: public MyBase3D::Controller3D, public MyBase::Network {
        public:
            HitBoxCenter();
            ~HitBoxCenter();

            void insert(ModelController* model);
            void erase(ModelController* model);
            void glDraw() const             override;
        private:
            glm::vec3              __colors;
            std::vector<ModelController*> __models;
    };
}
#endif