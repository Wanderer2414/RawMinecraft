#ifndef WORLD_H
#define WORLD_H

#include "Block.h"
#include "Clock.h"
#include "Chunk.h"
#include "Message.h"
#include "Controller.h"
namespace MyCraft {
class World: public MyBase::Controller, public Port {
    #define world_side 2
    public:
        World(const int& x, const int& y, const int& z);
        ~World();
        const BlockCatogary::Catogary&    at(const int& x, const int& y, const int& z) const;
        const BlockCatogary::Catogary&    at(const glm::vec3& pos) const;
        void                    set(const int& x, const int& y, const int& z, const BlockCatogary::Catogary& type);
        void                    set(const glm::vec3& pos, const BlockCatogary::Catogary& type);
        void                    setHoverBlock(const glm::vec3& pos, const glm::vec3& placePosition),
                                unHoverBlock();
        friend class PlaceblockCommand;
        friend class CheckHoverCommand;
    protected:
        bool handle(GLFWwindow* window) override;
        virtual void glDraw() const override;
    private:
        bool                    __isHoverBlock;
        glm::vec3               __hoverBlock, __placePosition, __cameraPosition, __cameraDir;
        MyCraft::Chunk          pChunks[world_side*2 + 1][world_side*2 + 1][world_side*2 + 1];
        glm::vec3               pPosition;
        MyBase::Clock           pFrameAlarm;
        std::vector<glm::vec4>  __chunkPositions;
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