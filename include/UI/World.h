#ifndef WORLD_H
#define WORLD_H

#include "Clock.h"
#include "Chunk.h"
#include "Controller3D.h"
#include "Global.h"
namespace MyCraft {
class World: public MyBase3D::Controller3D {
    #define world_side 1
    public:
        World(const int& x, const int& y, const int& z);
        ~World();
        virtual bool            setHover(const MyBase3D::Ray3f& hover) override;
        MyCraft::Block&         at(const int& x, const int& y, const int& z);
    protected:
        bool handle(GLFWwindow* window) override;
        virtual void glDraw() const override;
    private:
        float                   hX, hY, hZ;
        MyCraft::Chunk          pChunks[world_side*2 + 1][world_side*2 + 1][world_side*2 + 1];
        glm::vec3               pPosition;
        MyBase::Clock           pFrameAlarm;
    };
}
#endif