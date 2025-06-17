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
        unsigned char&          at(const int& x, const int& y, const int& z);
        unsigned char&          at(const glm::vec3& pos);
        void                    setHoverBlock(const glm::vec3& pos, const glm::vec3& placePosition),
                                unHoverBlock();
        friend class PlaceblockCommand;
    protected:
        bool handle(GLFWwindow* window) override;
        virtual void glDraw() const override;
    private:
        bool                    __isHoverBlock;
        glm::vec3               __hoverBlock, __placePosition;
        MyCraft::Chunk          pChunks[world_side*2 + 1][world_side*2 + 1][world_side*2 + 1];
        glm::vec3               pPosition;
        MyBase::Clock           pFrameAlarm;

    };

    class CheckEmptyCommand: public Command {
    public:
        CheckEmptyCommand(World* world);
        ~CheckEmptyCommand();

        void execute(Port& mine, Port& source, Message* message) override;
        MessageType getType() const override;
    private:
        World*  __world;
    };

    class CheckFallCommand: public Command {
    public:
        CheckFallCommand(World* world);
        ~CheckFallCommand();

        void execute(Port& mine, Port& des, Message* message) override;
        MessageType getType() const override;
    private:
        World*  __world;
    };

    class CheckHoverCommand: public Command {
    public:
        CheckHoverCommand(World* world);
        ~CheckHoverCommand();
        MessageType getType() const override;
        void execute(Port& mine, Port& des, Message* message) override;
    private:
        World* __world;
    };

    class PlaceblockCommand: public Command {
    public:
        PlaceblockCommand(World* world);
        ~PlaceblockCommand();
        MessageType getType() const override;
        void execute(Port& mine, Port& des, Message* message) override;
    private:
        World* __world;
    };
}
#endif