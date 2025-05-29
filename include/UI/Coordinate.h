#ifndef COORDINATE_H
#define COORDINATE_H
#include "Controller3D.h"
namespace MyCraft {
    class Coordinate: public MyBase3D::Controller3D {
        public:
            Coordinate();
            ~Coordinate();
        
        protected:
            void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
            void glDraw() const override;
        };
}
#endif