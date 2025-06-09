#ifndef MODEL_H
#define MODEL_H
#include "Clock.h"
#include "Controller3D.h"

namespace MyCraft {
    class Model: public MyBase3D::Controller3D {
        public:
            virtual void    see(const glm::vec3& dir) = 0,
                            move(const glm::vec3& dir) = 0,
                            rotate(const float& angle) = 0;
            virtual glm::vec3 getPosition() const = 0;
            virtual const glm::mat4x3& getBottomRec() const = 0; 
            virtual glm::vec2 getZRange() const = 0;
        protected:
            glm::mat4x3     _bottomRec;
            float           _minZ,
                            _maxZ;
    };
}
#endif