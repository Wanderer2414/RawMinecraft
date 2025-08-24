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

}
#endif