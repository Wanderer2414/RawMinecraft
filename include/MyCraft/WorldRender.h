#ifndef WORLD_DATA_H
#define WORLD_DATA_H
#include "ChunkBase.h"
#include "Container3D.h"
#include "Controller3D.h"
#include "ChunkManage.h"
#include "Message.h"
namespace MyCraft {
    class WorldRender: public MyBase3D::Container3D, public ChunkLoader, public MyBase::Port {
    public:
        WorldRender(const std::string& src);
        ~WorldRender();
        bool isHover() const;
        glm::ivec3 getHoverBlock() const;
        glm::ivec3 getPlaceBlock() const;
        void save();
        void playerAt(const glm::vec3& position);
        void setHoverBlock(const glm::vec3& pos, const glm::vec3& place);
        void unHover();
        const BlockCatogary&    getType(const glm::vec3&) const;
        const BlockCatogary&    getType(const glm::vec3&)      ;
    protected:
        void glDraw() const override;
        void glDrawTransparent() const override;
        Chunk& getChunk(const glm::ivec3& position) override;
        const Chunk& getChunk(const glm::ivec3& position) const override;
        const glm::ivec3& getPosition() const override;
    private:
        bool        __isHover;
        ChunkManage __chunkLoader;
        glm::vec3   __hoverBlock, __placePosition;
    };

    class CheckEmptyCommand: public MyBase::Command {
    public:
        CheckEmptyCommand(MyCraft::WorldRender& world);
        ~CheckEmptyCommand();

        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
        MyBase::MessageType getType() const override;
    private:
        WorldRender&  __world;
    };

    class CheckFallCommand: public MyBase::Command {
    public:
        CheckFallCommand(MyCraft::WorldRender& world);
        ~CheckFallCommand();

        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
        MyBase::MessageType getType() const override;
    private:
        WorldRender&  __world;
    };

    class CheckHoverMessage: public MyBase::Message {
    public:
        CheckHoverMessage(const glm::vec3& pos, const glm::vec3& dir);
        ~CheckHoverMessage();
        MyBase::MessageType     getType() const override;
        const glm::vec3 position, direction;
    };

    class CheckHoverCommand: public MyBase::Command {
    public:
        CheckHoverCommand(MyCraft::WorldRender& world);
        ~CheckHoverCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        WorldRender& __world;
    };
    class DestroyBlockCommand: public MyBase::Command {
    public:
        DestroyBlockCommand(WorldRender& world);
        ~DestroyBlockCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        WorldRender& __world;
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
        WorldMoveCommand(MyCraft::WorldRender& world);
        ~WorldMoveCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        WorldRender& __world;
    };
}
#endif