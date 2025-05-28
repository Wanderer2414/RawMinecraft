#ifndef LINE_H
#define LINE_H
#include "Global.h"
namespace MyBase3D {
    class Line {
        public:
            Line(const glm::vec3& origin,  const glm::vec3& delta);
            ~Line();
        
            virtual bool    contains(const glm::vec3& position) const;
            virtual float   distance(const glm::vec3& position) const;
        
        protected:
            glm::vec3    sOrigin, sDelta;
        };
}
#endif