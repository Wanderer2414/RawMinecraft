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
#include "ModelController.h"
#include "PathContainer.h"
#include "PathCreator.h"
#include "WorldRender.h"

namespace MyCraft {
class World: public MyBase3D::Container3D, public MyBase::Port {
    public:
        World(const int& x, const int& y, const int& z, const std::string& src);
        ~World();
        
        bool isBusyBlock(const glm::ivec3& position);
        
        void teleport(const glm::ivec3& position);
        void addPlayerModel(ModelController* controller);
        void pushMob(ModelController* model);
        void eraseMob(ModelController* model);
        friend class HealthWorldCommand;
        friend class AttackWorldCommand;
        friend class CheckHoverCommand;
    protected:
    private:
        PathCreator             __pathCreator;
        HitBoxCenter            __hitbox;
        WorldRender             __worldRender;
        CrackingManage          __crackingManage;
        DropItemManage          __dropItemManage;

    };


    class HealthWorldCommand: public MyBase::Command {
    public:
        HealthWorldCommand(MyCraft::World& world);
        ~HealthWorldCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World& __world;
    };

    class AttackWorldCommand: public MyBase::Command {
    public:
        AttackWorldCommand(World& world);
        ~AttackWorldCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World& __world;
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
        CheckHoverCommand(MyCraft::World& world);
        ~CheckHoverCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World& __world;
    };


    class TeleportMessage: public MyBase::Message {
    public:
        TeleportMessage(const glm::vec3& pos);
        ~TeleportMessage();
        MyBase::MessageType     getType() const override;
        const glm::vec3 position;
    };

    class TeleportCommand: public MyBase::Command {
    public:
        TeleportCommand(MyCraft::World& world);
        ~TeleportCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) override;
    private:
        World& __world;
    };


    class SpawnMobMessage: public MyBase::Message {
    public:
        SpawnMobMessage(ModelController* controller);
        ~SpawnMobMessage();

        ModelController* model;
        MyBase::MessageType getType() const override;
    };
    class SpawnMobCommand: public MyBase::Command {
    public:
        SpawnMobCommand(World& world);
        ~SpawnMobCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        World& __world;
    };


    class EraseMobMessage: public MyBase::Message {
    public:
        EraseMobMessage(ModelController* controller);
        ~EraseMobMessage();

        ModelController* model;
        MyBase::MessageType getType() const override;
    };
    class EraseMobCommand: public MyBase::Command {
    public:
        EraseMobCommand(World& world);
        ~EraseMobCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        World& __world;
    };
}
#endif