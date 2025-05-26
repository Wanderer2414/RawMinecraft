#ifndef WORLD_H
#define WORLD_H

#include "Alarm.h"
#include "Chunk.h"
#include "Controller3D.h"
#include "Global.h"
class World: public Controller3D {
public:
    #define world_side 1
    virtual bool setHover(const Ray3f& hover) override;
    World(const int& x, const int& y, const int& z);
    ~World();
    Block&   at(const int& x, const int& y, const int& z);
protected:
    handle_function(handle) override;
    virtual void glDraw() const override;
private:
    float       hX, hY, hZ;
    Chunk       pChunks[world_side*2 + 1][world_side*2 + 1][world_side*2 + 1];
    glm::vec3   pPosition;
    Alarm       pFrameAlarm;
};
#endif