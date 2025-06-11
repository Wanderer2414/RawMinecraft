#ifndef HITBOX_H
#define HITBOX_H
#include "Controller3D.h"
#include "Model.h"
#include "World.h"
namespace MyCraft {
    class HitBoxCenter: public MyBase3D::Controller3D {
        public:
            HitBoxCenter();
            ~HitBoxCenter();

            void setWorld(World* world);

            void insert(Model* model);
            void erase(Model* model);
            void glDraw() const             override;
            bool handle(GLFWwindow* window) override;
        private:
            World               *__world;
            GLuint              __colors;
            std::vector<Model*> __models;
    };
}
#endif