#ifndef COORDINATE_H
#define COORDINATE_H
#include "Controller3D.h"
class Coordinate: public Controller3D {
public:
    Coordinate();
    ~Coordinate();

protected:
    void draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
};
#endif