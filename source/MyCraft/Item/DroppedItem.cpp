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

    void DropItemManage::add(const BlockCatogary& item, const glm::vec3& position) {
        __item.push_back(glm::vec4(position,item));
        __state.push_back(glm::mat4(1));
    }
    void DropItemManage::remove(const int& index) {

    }

    bool DropItemManage::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Controller3D::handle(window);
        if (__rotateClock.get() && __item.size()) {
            __rotateClock.restart();
            glm::mat4x3 mat;
            mat[1] = {0.3, 0,0};
            mat[2] = {0, 0.3, 0};
            mat[3] = {0, 0, 0.3};
            for (glm::mat4& i:__state) {
                i = glm::rotate(i, 0.05f, {0,0,1});
                i[2][3] = i[0][0]*0.05;
            }
            for (int i = 0; i<__item.size(); i++) {
                mat[0] = __item[i];
                __currentFall = i;
                send(new RequestFallMessage(mat, -0.03));
            }
        }
        return is_changed;
    }
    std::vector<std::pair<glm::vec3, RecipeSlot>> DropItemManage::getNearItem(const glm::vec3& position) {
        std::vector<std::pair<glm::vec3, RecipeSlot>> ans;
        for (int i = __item.size()-1; i>=0; i--) {
            float xy_distance = glm::length(glm::vec2(__item[i])-glm::vec2(position));
            float z_distance = abs(position.z-__item[i].z);
            if (xy_distance<2 && z_distance<3) {
                ans.push_back({__item[i], {1, (ItemType)__item[i].w}});
                __item.erase(__item.begin()+i);
                __state.erase(__state.begin()+i);
            }
        }
        return ans;
    }
    void DropItemManage::glDraw() const {
        if (__item.size()) {
            DrawingCenter::BindDroppedBlock(__blockTexture, glm::vec2(1, 0.3));
            DrawingCenter::DrawDroppedBlock((void*)__item.data(), (void*)__state.data(), __item.size());
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
        manage.add((BlockCatogary)package->type, package->position);
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
        __manage.__item[__manage.__currentFall].z += package->zVelocity;   
    }
}