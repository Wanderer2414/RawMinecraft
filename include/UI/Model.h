#ifndef MODEL_H
#define MODEL_H
#include "Controller3D.h"
namespace MyCraft {
    class Model: public MyBase3D::Controller3D {
        public:
            Model();
            ~Model();
            void glDraw() const;
    };
}
#endif