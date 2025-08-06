#ifndef WORLD_H
#define WORLD_H

#include "Block.h"
#include "Clock.h"
#include "Controller3D.h"
#include "CrackingManage.h"
#include "DroppedItem.h"
#include "HitBoxCenter.h"
#include "Item.h"
#include "Message.h"
#include "WorldRender.h"

namespace MyCraft {
class World: public MyBase3D::Container3D, public MyBase::Port {
    public:
        World(const int& x, const int& y, const int& z, const std::string& src);
        ~World();
        
        bool isBusyBlock(const glm::ivec3& position);
        void teleport(const glm::ivec3& position);
        friend class PlaceBlockCommand;
        friend class CrackBlockCommand;
    protected:
    private:
        HitBoxCenter            __hitbox;
        WorldRender             __worldRender;
        CrackingManage          __crackingManage;
        DropItemManage          __dropItemManage;
        glm::ivec3              __placePosition;
        glm::vec3               __cameraPosition, __cameraDir;

    };


    class PlaceBlockMessage: public MyBase::Message {
    public:
        PlaceBlockMessage(const glm::vec3& pos, const BlockCatogary& type);;
        ~PlaceBlockMessage();
        const glm::vec3 pos;
        const BlockCatogary type;
        MyBase::MessageType getType() const override;
    };
    class PlaceBlockCommand: public MyBase::Command {
    public:
        PlaceBlockCommand(MyCraft::World& world);
        ~PlaceBlockCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World& __world;
    };

    class CrackBlockMessage: public MyBase::Message {
    public:
        CrackBlockMessage(const ItemType& item);
        ~CrackBlockMessage();
        const ItemType type;
        MyBase::MessageType getType() const override;
    };
    class CrackBlockCommand: public MyBase::Command {
    public:
        CrackBlockCommand(World& world);
        ~CrackBlockCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World& __world;
    };
}
#endif