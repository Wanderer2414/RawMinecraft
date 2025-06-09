#ifndef HITBOX_H
#define HITBOX_H
#include "Controller3D.h"
#include "Model.h"
namespace MyCraft {
    class HitBoxCenter: public MyBase3D::Controller3D {
        public:
            HitBoxCenter();
            ~HitBoxCenter();
        
            void insert(Model* model);
            void erase(Model* model);
            void glDraw() const;
        private:
            std::vector<Model*> __models;
    };
}
#endif