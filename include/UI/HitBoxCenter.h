#ifndef HITBOX_H
#define HITBOX_H
#include "Message.h"
#include "Controller3D.h"
#include "Model.h"
#include "World.h"
namespace MyCraft {
    class HitBoxCenter: public MyBase3D::Controller3D, public Network {
        public:
            HitBoxCenter();
            ~HitBoxCenter();

            void insert(Model* model);
            void erase(Model* model);
            void glDraw() const             override;
        private:
            GLuint              __colors;
            std::vector<Model*> __models;
    };
}
#endif