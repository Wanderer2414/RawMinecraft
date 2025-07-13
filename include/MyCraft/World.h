#ifndef WORLD_H
#define WORLD_H

#include "Block.h"
#include "Clock.h"
#include "Message.h"
#include "Controller.h"

namespace MyCraft {
class World: public MyBase::Controller, public MyBase::Port {
    #define world_side 5
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
        std::map<BlockCatogary::Catogary,std::vector<glm::vec4>>  __list;
        BlockCatogary::Catogary                 ***__blockTypes;
        std::bitset<16*world_side>              **__bits;
        glm::vec3               __position;
        MyBase::Clock           pFrameAlarm;
        std::vector<glm::vec4>  __chunkPositions;

        void __enableBit(const int& x, const int& y, const int& z);
        void __disableBit(const int& x, const int& y, const int& z);
    };
    class CheckEmptyCommand: public MyBase::Command {
    public:
        CheckEmptyCommand(MyCraft::World* world);
        ~CheckEmptyCommand();

        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
        MyBase::MessageType getType() const override;
    private:
        World*  __world;
    };

    class CheckFallCommand: public MyBase::Command {
    public:
        CheckFallCommand(MyCraft::World* world);
        ~CheckFallCommand();

        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
        MyBase::MessageType getType() const override;
    private:
        World*  __world;
    };

    class CheckHoverCommand: public MyBase::Command {
    public:
        CheckHoverCommand(MyCraft::World* world);
        ~CheckHoverCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World* __world;
    };

    class PlaceblockCommand: public MyBase::Command {
    public:
        PlaceblockCommand(MyCraft::World* world);
        ~PlaceblockCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World* __world;
    };

    
}
#endif