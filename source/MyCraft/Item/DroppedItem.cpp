#include "DroppedItem.h"
#include "Block.h"
#include "Controller3D.h"
#include "DrawingCenter.h"
#include "Item.h"
#include "Message.h"
#include "PlayerInventoryModule.h"
#include "PlayerModelController.h"
#include "WorldRender.h"
#include "Recipe.h"
#include "Toolbar.h"

namespace MyCraft {
    DropItemManage::DropItemManage() {
        __rotateClock.setDuration(20);
        __blockTexture.load("assets/images/blockCatogary.png", false);
        Port::add(new LootItemByMoveCommand(*this));
        Port::add(new LootItemByJumpCommand(*this));
        Port::add(new DropItemCommand(*this));
        Port::add(new FallItemCommand(*this));
    }
    DropItemManage::~DropItemManage() {}

    void DropItemManage::add(const BlockCatogary& item, const unsigned char& count, const glm::vec3& position) {
        bool isMerge = false;
        for (int i = 0; i<__normal.size() && !isMerge; i++) {
            float distance = glm::length(position-__normal.getPosition(i));
            if (distance<3 && BlockCatogary(__normal.getType(i)) == item) {
                __count[i] += count;
                isMerge = true;
            }
        }
        if (!isMerge) {
            glm::mat4 state = getSpecialState(item);
            state = glm::scale(state, glm::vec3(0.4));
            state[3] = glm::vec4(position.x - state[0].x/2, position.y-state[1].y/2, position.z,1);
            __normal.push(position, state, glm::vec4(0,0,0,item));
            __count.push_back(count);
        }
    }
    void DropItemManage::remove(const int& index) {

    }

    bool DropItemManage::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Controller3D::handle(window);
        if (__rotateClock.get() && __normal.size()) {
            __rotateClock.restart();
            for (int i = 0; i<__normal.size(); i++) {
                __normal.getState(i) *= glm::translate(glm::mat4(1), glm::vec3(0.5))*
                    glm::rotate(glm::mat4(1), 0.05f, {0,0,1})*
                    glm::translate(glm::mat4(1), glm::vec3(-0.5));
            }
            glm::mat4x3 mat;
            mat[1] = {0.3, 0,0};
            mat[2] = {0, 0.3, 0};
            mat[3] = {0, 0, 0.3};
            for (int i = 0; i<__normal.size(); i++) {
                mat[0] = __normal.getPosition(i) - glm::vec3(0.15,0.15,0);
                __currentFall = i;
                send(new RequestFallMessage(mat, -0.03));
            }
        }
        return is_changed;
    }
    std::vector<std::pair<glm::vec3, RecipeSlot>> DropItemManage::getNearItem(const glm::vec3& position) {
        std::vector<std::pair<glm::vec3, RecipeSlot>> ans;
        for (int i = __normal.size()-1; i>=0; i--) {
            float xy_distance = glm::length(glm::vec2(__normal.getPosition(i))-glm::vec2(position));
            float z_distance = abs(position.z-__normal.getPosition(i).z);
            if (xy_distance<2 && z_distance<3) {
                ans.push_back({__normal.getPosition(i), {__count[i], __normal.getType(i)}});
                __normal.remove(i);
                std::swap(__count[i], __count.back());
                __count.pop_back();
            }
        }
        return ans;
    }
    void DropItemManage::glDraw() const {
        if (__normal.size()) {
            DrawingCenter::BindCube(__blockTexture, glm::vec2(1, 0.3));
            DrawingCenter::DrawCubes(__normal);
        }
    }

    DropItemMessage::DropItemMessage(const ItemType& t, const int& c, const glm::vec3& p): type(t), count(c), position(p) {}
    DropItemMessage::~DropItemMessage() {}

    MyBase::MessageType DropItemMessage::getType() const {
        return MyBase::DropItem;
    }
    DropItemCommand::DropItemCommand(DropItemManage& m): manage(m) {}
    DropItemCommand::~DropItemCommand() {}

    MyBase::MessageType DropItemCommand::getType() const {
        return MyBase::DropItem;
    }
    void DropItemCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        DropItemMessage* package = (DropItemMessage*)message;
        manage.add((BlockCatogary)package->type, package->count, package->position);
    }

    LootItemByMoveCommand::LootItemByMoveCommand(DropItemManage& m): manage(m) {};
    LootItemByMoveCommand::~LootItemByMoveCommand() {};
    MyBase::MessageType LootItemByMoveCommand::getType() const {
        return MyBase::RequestGoto;
    }
    void LootItemByMoveCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        RequestGotoMessage* package = (RequestGotoMessage*)message;
        auto items = manage.getNearItem(package->rectangleBox[0]);
        for (auto item: items) mine.send(source, new ReceiveItemMessage(item.first, item.second.item, item.second.count));
        if (items.size()) mine.send(new AddItemMessage());
    }

    LootItemByJumpCommand::LootItemByJumpCommand(DropItemManage& m): manage(m) {};
    LootItemByJumpCommand::~LootItemByJumpCommand() {}
    MyBase::MessageType LootItemByJumpCommand::getType() const {
        return MyBase::RequestFall;
    }
    void LootItemByJumpCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        if (&source == &manage) return ;
        RequestFallMessage* package = (RequestFallMessage*)message;
        auto items = manage.getNearItem(package->rectangleBox[0] + package->rectangleBox[1]/2.f + package->rectangleBox[2]/2.f + package->rectangleBox[3]/2.f);
        for (auto item: items) mine.send(source, new ReceiveItemMessage(item.first, item.second.item, item.second.count));
        if (items.size()) mine.send(new AddItemMessage());
    }

    FallItemCommand::FallItemCommand(DropItemManage& m): __manage(m) {}
    FallItemCommand::~FallItemCommand() {}
    MyBase::MessageType FallItemCommand::getType() const {
        return MyBase::Fall;
    }
    void FallItemCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        FallMessage* package = (FallMessage*)message;
        __manage.__normal.getState(__manage.__currentFall)[3].z += package->zVelocity;   
    }
}