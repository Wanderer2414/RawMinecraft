#ifndef HITBOX_H
#define HITBOX_H
#include "Message.h"
#include "Controller.h"
#include "ModelController.h"
#include "ModelTest/gltf_mesh.h"
#include "ModelTest/GLTFModel.h"
namespace MyCraft {
    class HitBoxCenter: public MyBase3D::Controller3D {
        public:
            HitBoxCenter();
            ~HitBoxCenter();
            //Check whether one position of block is collision with any model?
            bool isBusyBlock(const glm::ivec3& position) const;
            //Check whether one point is in any model?
            bool isColistion(const glm::vec3& position) const;
            void insert(ModelController* model);
            void erase(ModelController* model);
        private:
            void glDraw() const             override;
            glm::vec3              __colors;
            std::vector<ModelController*> __models;
            // GLTFModel * modelTest;
    };
}
#endif