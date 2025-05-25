#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "Controller3D.h"
class World: public Controller3D {
public:
    #define world_side 1
    World(const int& x, const int& y, const int& z);
    ~World();
    char&   at(const int& x, const int& y, const int& z);
protected:
    virtual void glDraw() const;
private:
    Chunk       pChunks[world_side*2 + 1][world_side*2 + 1][world_side*2 + 1];
    glm::vec3   pPosition;
};
#endif