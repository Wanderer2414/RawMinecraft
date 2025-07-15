#ifndef WORLD_H
#define WORLD_H

#include "Block.h"
#include "Clock.h"
#include "Controller3D.h"
#include "Message.h"
#include "WorldRender.h"
#include "glm/fwd.hpp"

namespace MyCraft {
class World: public MyBase3D::Container3D, public MyBase::Port {
    public:
        World(const int& x, const int& y, const int& z);
        ~World();
        const BlockCatogary::Catogary&    at(const glm::vec3& pos) const;
        void                    set(const int& x, const int& y, const int& z, const BlockCatogary::Catogary& type);
        void                    set(const glm::vec3& pos, const BlockCatogary::Catogary& type);
        void                    playerAt(const glm::vec3& pos);
        void                    setHoverBlock(const glm::vec3& pos, const glm::vec3& placePosition),
                                unHoverBlock();
        friend class PlaceblockCommand;
        friend class DestroyblockCommand;
        friend class CheckHoverCommand;
    protected:
        bool handle(GLFWwindow* window) override;
        virtual void glDraw() const override;
    private:
        WorldRender             __worldRender;
        bool                    __isHoverBlock;
        glm::ivec3              __hoverBlock, __placePosition;
        glm::vec3               __cameraPosition, __cameraDir;
        MyBase::Clock           pFrameAlarm;

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
    class PlaceBlockMessage: public MyBase::Message {
    public:
        PlaceBlockMessage(const glm::mat4x3& model, const BlockCatogary::Catogary& type);;
        ~PlaceBlockMessage();
        glm::mat4x3 shape;
        BlockCatogary::Catogary type;
        MyBase::MessageType getType() const override;
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
    class DestroyBlockMessage: public MyBase::Message {
    public:
        DestroyBlockMessage();
        ~DestroyBlockMessage();
        BlockCatogary::Catogary type;
        MyBase::MessageType getType() const override;
    };
    class DestroyblockCommand: public MyBase::Command {
    public:
        DestroyblockCommand(World* world);
        ~DestroyblockCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World* __world;
    };
    class WorldMoveMessage: public MyBase::Message {
    public:
        WorldMoveMessage(const glm::vec3& position);
        ~WorldMoveMessage();
        glm::vec3 position;
        MyBase::MessageType getType() const override;
    private:
    };
    class WorldMoveCommand: public MyBase::Command {
    public:
        WorldMoveCommand(MyCraft::World* world);
        ~WorldMoveCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World* __world;
    };
}
#endif