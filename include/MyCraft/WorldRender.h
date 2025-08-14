#ifndef WORLD_DATA_H
#define WORLD_DATA_H
#include "Block.h"
#include "Container3D.h"
#include "Controller3D.h"
#include "ChunkManage.h"
#include "Message.h"
#include "WaterManage.h"
namespace MyCraft {
    class WorldRender: public MyBase3D::Container3D, public MyBase::Port {
    public:
        WorldRender(const std::string& src);
        ~WorldRender();
        bool contains(const glm::ivec3& pos) const;
        bool isHover() const;
        BlockCatogary getHoverType() const;
        glm::ivec3 getHoverBlock() const;
        glm::ivec3 getPlaceBlock() const;
        bool isInWater(const glm::vec3& position) const;
        float getWaterHeight(const glm::vec3& position) const;
        glm::vec3 getWaterDirection(const glm::vec3& position) const;
        void place(const BlockCatogary& type);
        void unplace();
        void save();
        void playerAt(const glm::vec3& position);
        void setHoverBlock(const glm::vec3& pos, const glm::vec3& place);
        void unHover();
        bool isBusy(const glm::vec3& position) const;
        bool isHover(const glm::vec3& position) const;
        BlockCatogary getType(const glm::vec3& position) const;
    protected:
        void glDraw() const override;
        void glDrawTransparent() const override;
    private:
        bool        __isHover;
        ChunkManage __chunkLoader;
        unsigned char __hoverPlane;
        glm::vec3   __hoverBlock, __placePosition;
    };

    class RequestGotoMessage: public MyBase::Message {
    public:
        RequestGotoMessage(const glm::mat4x3& rectangleBox, const glm::vec2& direction);
        ~RequestGotoMessage();
        MyBase::MessageType     getType() const override;
        const glm::vec2       direction;
        const glm::mat4x3     rectangleBox;
    private:
    };

    class RequestFallMessage: public MyBase::Message {
    public:
        RequestFallMessage(const glm::mat4x3& rectangleBox, const float& zVelocity);
        ~RequestFallMessage();
        MyBase::MessageType     getType() const override;
        const glm::mat4x3 rectangleBox;
        float zVelocity;
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