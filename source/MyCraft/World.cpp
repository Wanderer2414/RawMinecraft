#include "World.h"
#include "Block.h"
#include "BlockItem.h"
#include "InteractiveForm.h"
#include "Item.h"
#include "Message.h"
#include "ModelController.h"
#include "Zombie/ModelController.h"
#include "Player/ModelController.h"

namespace MyCraft {
    World::World(const std::string& src): 
        __worldRender(src), __pathCreator(__worldRender) 
    {
        insert(&__worldRender);
        insert(&__dropItemManage);
        insert(&__crackingManage);
        insert(&__hitbox);
        MyBase::Network::match(&__crackingManage);
        MyBase::Network::match(&__worldRender);
        MyBase::Network::match(&__dropItemManage);
        MyBase::Network::match(&__pathCreator);
        add(new HealthWorldCommand(*this));
        add(new AttackWorldCommand(*this));
        add(new CheckHoverCommand(*this));
        add(new TeleportCommand(*this));
        add(new EraseMobCommand(*this));
        add(new SpawnMobCommand(*this));
    }
    World::~World() {}

    bool World::isBusyBlock(const glm::ivec3& position) {
        return false;
    }
    void World::addPlayerModel(ModelController* controller) {
        return __hitbox.pushPlayerModel(controller);
    }
    void World::teleport(const glm::ivec3& position) {
        __worldRender.playerAt(position);
    }

    void World::pushMob(ModelController* model) {
        __hitbox.insert(model);
        __worldRender.pushMob(model);
    }
    void World::eraseMob(ModelController* model) {
        __hitbox.erase(model);
        __worldRender.eraseMob(model);
    }


    HealthWorldCommand::HealthWorldCommand(World& world): __world(world) {}
    HealthWorldCommand::~HealthWorldCommand() {}
    MyBase::MessageType HealthWorldCommand::getType() const {
        return MyBase::MessageType::Place;
    }
    void HealthWorldCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        PlaceMessage* package = (PlaceMessage*)message;
        // bool below_result = true, above_result = true;
        // std::queue<glm::ivec3> q = rasterize(package->shape[0], package->shape[0]+package->shape[1]);
        // while (q.size() && below_result) {
        //     if (q.front() == __world.getPlaceBlock()) below_result = false;
        //     q.pop();
        // }
        // if (below_result) {
        //     q = rasterize(package->shape[0]+package->shape[2], package->shape[0]+package->shape[1] + package->shape[2]);
        //     while (q.size() && below_result) {
        //         if (q.front() == __world.getPlaceBlock()) below_result = false;
        //         q.pop();
        //     }
        // }
        // if (below_result) {
        //     q = rasterize(package->shape[0]+package->shape[3], package->shape[0]+package->shape[1]+package->shape[3]);
        //     while (q.size() && above_result) {
        //         if (q.front() == __world.getPlaceBlock()) above_result = false;
        //         q.pop();
        //     }
        //     if (above_result) {
        //         q = rasterize(package->shape[0]+package->shape[2] + package->shape[3], package->shape[0]+package->shape[1]+ package->shape[2] + package->shape[3]);
        //         while (q.size() && above_result) {
        //             if (q.front() == __world.getPlaceBlock()) above_result = false;
        //             q.pop();
        //         }
        //     }
        // }
        if (__world.__worldRender.isHover()) {            
            BlockCatogary hoverType = __world.__worldRender.getHoverType();
            if (isInteractive(hoverType)) {
                mine.send(new PrepareOpenInventoryMessage(__world.__worldRender.getHoverBlock(), hoverType));
            }
            else if (!__world.isBusyBlock(__world.__worldRender.getPlaceBlock())) {
                if (isBlock(package->rightItem)) {
                    BlockCatogary type = BlockCatogary(package->rightItem);
                    __world.__worldRender.place(type);
                    mine.send(new AcceptPlaceMessage(type));
                }
            }
        }
    }

    AttackWorldCommand::AttackWorldCommand(World& w): __world(w) {}
    AttackWorldCommand::~AttackWorldCommand() {}
    MyBase::MessageType AttackWorldCommand::getType() const {
        return MyBase::Attack;
    }
    void AttackWorldCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        AttackMessage* package = (AttackMessage*)message;
        if (__world.__worldRender.isHover()) {
            BlockCatogary type =__world.__worldRender.getHoverType();
            __world.__crackingManage.setCrackBlock(__world.__worldRender.getHoverBlock(), type);
            float percent = getPowerness(package->rightItem)/getHardness(type);
            if (!isAdaptive(package->rightItem, type)) percent /= 1.5;
            __world.__crackingManage.crack(percent);
            if (!__world.__crackingManage.getPercent()) {
                glm::vec3 position = __world.__crackingManage.getCrackingBlock();
                position += glm::vec3(0.5);
                __world.__worldRender.unplace();
                __world.__crackingManage.uncrack();
                if (isAdaptive(package->rightItem, type)) __world.__dropItemManage.add(getBrokenResult(type), 1, position);
                mine.send(des, new AcceptDestroyMessage(1/percent, type, position));
            }
        }
        else if (__world.__hitbox.isHover()) {
            __world.__hitbox.attackEntity(getSharpness(package->rightItem), package->direction);
        }
    }


    CheckHoverMessage::CheckHoverMessage(const glm::vec3& pos, const glm::vec3& dir): position(pos), direction(dir) {}
    CheckHoverMessage::~CheckHoverMessage() {}
    MyBase::MessageType CheckHoverMessage::getType() const {
        return MyBase::MessageType::CheckHover;
    }

    CheckHoverCommand::CheckHoverCommand(World& world): __world(world) {}
    CheckHoverCommand::~CheckHoverCommand() {}
    MyBase::MessageType CheckHoverCommand::getType() const {
        return MyBase::MessageType::CheckHover;
    };
    void CheckHoverCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        CheckHoverMessage* package = (CheckHoverMessage*)message;
        glm::vec3 position = package->position;
        glm::vec3 direction = package->direction;

        auto* model = __world.__hitbox.isColistion(position+direction, direction*4.f);
        if (model) {
            __world.__hitbox.setHoverEntity(model);
            __world.__worldRender.unHover();
        }
        else {
            __world.__hitbox.setHoverEntity(0);
            glm::vec3 delta = direction*4.f/20.f;
            glm::ivec3 hoverPosition, placePosition;
            bool hover = false;
            
            for (int i = 0; i<=20 && !hover; i++) {
                glm::vec3 cur = position+i*1.0f*delta;
                if (__world.__worldRender.isHover(cur)) {
                    hover = true;
                    hoverPosition = glm::ivec3(floor(cur.x), floor(cur.y), floor(cur.z));
                }
                else {
                    placePosition = glm::ivec3(floor(cur.x), floor(cur.y), floor(cur.z));
                }
            }
            if (hover) {
                __world.__worldRender.setHoverBlock(hoverPosition, placePosition);
            }
            else __world.__worldRender.unHover();
        }
    }

    TeleportMessage::TeleportMessage(const glm::vec3& pos): position(pos) {}
    TeleportMessage::~TeleportMessage() {}
    MyBase::MessageType TeleportMessage::getType() const {
        return MyBase::Teleport;
    }

    TeleportCommand::TeleportCommand(MyCraft::World& world): __world(world) {}
    TeleportCommand::~TeleportCommand() {}
    MyBase::MessageType TeleportCommand::getType() const {
        return MyBase::Teleport;
    };
    void TeleportCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        TeleportMessage* package = (TeleportMessage*)message;
        __world.teleport(package->position);
    }


    SpawnMobMessage::SpawnMobMessage(ModelController* controller): model(controller) {}
    SpawnMobMessage::~SpawnMobMessage() {}

    MyBase::MessageType SpawnMobMessage::getType() const {
        return MyBase::SpawnMob;
    }
    SpawnMobCommand::SpawnMobCommand(World& world): __world(world) {}
    SpawnMobCommand::~SpawnMobCommand() {}
    MyBase::MessageType SpawnMobCommand::getType() const {
        return MyBase::SpawnMob;
    }
    void SpawnMobCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        SpawnMobMessage* package = (SpawnMobMessage*)message;
        __world.pushMob(package->model);
    }



    EraseMobMessage::EraseMobMessage(ModelController* controller): model(controller) {}
    EraseMobMessage::~EraseMobMessage() {}

    MyBase::MessageType EraseMobMessage::getType() const {
        return MyBase::EraseMob;
    }
    EraseMobCommand::EraseMobCommand(World& world): __world(world) {}
    EraseMobCommand::~EraseMobCommand() {}
    MyBase::MessageType EraseMobCommand::getType() const {
        return MyBase::EraseMob;
    }
    void EraseMobCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        SpawnMobMessage* package = (SpawnMobMessage*)message;
        __world.eraseMob(package->model);
    }

}