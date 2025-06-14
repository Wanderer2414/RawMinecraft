#ifndef WORLD_H
#define WORLD_H

#include "Block.h"
#include "Clock.h"
#include "Chunk.h"
#include "Message.h"
#include "Controller.h"
#include "Global.h"
namespace MyCraft {
class World: public MyBase::Controller, public Port {
    #define world_side 1
    public:
        World(const int& x, const int& y, const int& z);
        ~World();
        // virtual bool            setHover(const MyBase3D::Ray3f& hover) override;
        MyCraft::Block&         at(const int& x, const int& y, const int& z);
        MyCraft::Block&         at(const glm::vec3& pos);
    protected:
        bool handle(GLFWwindow* window) override;
        virtual void glDraw() const override;
    private:
        float                   hX, hY, hZ;
        MyCraft::Chunk          pChunks[world_side*2 + 1][world_side*2 + 1][world_side*2 + 1];
        glm::vec3               pPosition;
        MyBase::Clock           pFrameAlarm;

    };

    class CheckEmpty: public Command {
    public:
        CheckEmpty(World* world);
        ~CheckEmpty();

        void execute(Port& mine, Port& source, Message* message) override;
        MessageType getType() const override;
    private:
        World*  __world;
    };

    class CheckFall: public Command {
    public:
        CheckFall(World* world);
        ~CheckFall();

        void execute(Port& mine, Port& des, Message* message) override;
        MessageType getType() const override;
    private:
        World*  __world;
    };
}
#endif