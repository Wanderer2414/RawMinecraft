#include "World.h"
#include "Block.h"
#include "InteractiveForm.h"
#include "Inventory.h"
#include "InventoryForm.h"
#include "Item.h"
#include "Message.h"

namespace MyCraft {
    World::World(const int& x, const int& y, const int& z, const std::string& src): __worldRender(src) {
        insert(&__crackingManage);
        insert(&__worldRender);
        insert(&__dropItemManage);
        MyBase::Network::match(&__crackingManage);
        MyBase::Network::match(&__worldRender);
        MyBase::Network::match(&__dropItemManage);
        add(new PlaceBlockCommand(*this));
        add(new CrackBlockCommand(*this));
    }
    World::~World() {}

    bool World::isBusyBlock(const glm::ivec3& position) {
        return false;
    }
    void World::teleport(const glm::ivec3& position) {
        __worldRender.playerAt(position);
    }

    PlaceBlockMessage::PlaceBlockMessage(const glm::vec3& p, const BlockCatogary& t): pos(p), type(t) {}
    PlaceBlockMessage::~PlaceBlockMessage() {}
    MyBase::MessageType PlaceBlockMessage::getType() const {
        return MyBase::PlaceBlock;
    }

    PlaceBlockCommand::PlaceBlockCommand(World& world): __world(world) {}
    PlaceBlockCommand::~PlaceBlockCommand() {}
    MyBase::MessageType PlaceBlockCommand::getType() const {
        return MyBase::MessageType::PlaceBlock;
    }
    void PlaceBlockCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        PlaceBlockMessage* package = (PlaceBlockMessage*)message;
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
            BlockCatogary hoverType = __world.__worldRender.getType(__world.__worldRender.getHoverBlock());
            if (isInteractive(hoverType)) {
                mine.send(new OpenInventoryBlockMessage(__world.__worldRender.getHoverBlock(), hoverType));
            }
            else if (!__world.isBusyBlock(__world.__worldRender.getPlaceBlock())) {
                __world.__worldRender.setType(__world.__worldRender.getPlaceBlock(), package->type);
                mine.send(des, new AcceptPlaceMessage(package->type));
            }
        }
    }

    CrackBlockMessage::CrackBlockMessage(const ItemType& t): type(t) {}
    CrackBlockMessage::~CrackBlockMessage() {}
    MyBase::MessageType CrackBlockMessage::getType() const {
        return MyBase::DestroyBlock;
    }

    CrackBlockCommand::CrackBlockCommand(World& w): __world(w) {}
    CrackBlockCommand::~CrackBlockCommand() {}
    MyBase::MessageType CrackBlockCommand::getType() const {
        return MyBase::DestroyBlock;
    }
    void CrackBlockCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        CrackBlockMessage* package = (CrackBlockMessage*)message;
        if (__world.__worldRender.isHover()) {
            BlockCatogary type = __world.__worldRender.getType(__world.__worldRender.getHoverBlock());
            __world.__crackingManage.setCrackBlock(__world.__worldRender.getHoverBlock(), type);
            float percent = 0.03;
            if (isAdaptive(package->type, type)) {
                percent = getPowerness(package->type)/getHardness(type);
            }
            __world.__crackingManage.crack(percent);
            if (!__world.__crackingManage.getPercent()) {
                glm::vec3 position = __world.__crackingManage.getCrackingBlock();
                position += glm::vec3(0.5);
                __world.__worldRender.setType(__world.__crackingManage.getCrackingBlock(), Air);
                __world.__crackingManage.uncrack();
                if (isAdaptive(package->type, type)) __world.__dropItemManage.add(type, position);
                mine.send(des, new AcceptDestroyMessage(1/percent, type, position));
            }
        }
    }

}