#ifndef MODEL_H
#define MODEL_H
#include "Clock.h"
#include "Controller3D.h"

namespace MyCraft {
    class HitBoxCenter;
    class Model: public MyBase3D::Controller3D {
        public:
            virtual void    see(const glm::vec3& dir) = 0,
                            move(const glm::vec3& dir) = 0,
                            rotate(const float& angle) = 0;
            virtual glm::vec3 getPosition() const = 0;
            virtual glm::mat4x3 getShape() const = 0;
        friend class HitBoxCenter;
        protected:
            std::queue<float> request;
            std::queue<float> post;

            virtual void update() = 0;
    };
}
#endif