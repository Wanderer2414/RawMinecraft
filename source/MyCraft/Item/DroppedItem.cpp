#include "DroppedItem.h"
#include "Block.h"
#include "Controller3D.h"
#include "DrawingCenter.h"
#include "Inventory.h"
#include "Item.h"
#include "Message.h"
#include "PlayerModelController.h"
#include "Recipe.h"
#include "Toolbar.h"
namespace MyCraft {
    DropItemManage::DropItemManage() {
        __rotateClock.setDuration(20);
        __blockTexture.load("assets/images/blockCatogary.png", false);
        add(Grass, glm::vec3(0,0, 1));
        Port::add(new LootItemCommand(*this));
        Port::add(new DropItemCommand(*this));
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
            for (glm::mat4& i:__state) {
                i = glm::rotate(i, 0.05f, {0,0,1});
                i[2][3] = i[0][0]*0.05;
            }
        }
        return is_changed;
    }
    std::vector<std::pair<glm::vec3, RecipeSlot>> DropItemManage::getNearItem(const glm::vec3& position) {
        std::vector<std::pair<glm::vec3, RecipeSlot>> ans;
        for (int i = __item.size()-1; i>=0; i--) {
            float distance = glm::length(glm::vec3(__item[i])-position);
            if (distance<2) {
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

    LootItemCommand::LootItemCommand(DropItemManage& m): manage(m) {};
    LootItemCommand::~LootItemCommand() {};
    MyBase::MessageType LootItemCommand::getType() const {
        return MyBase::RequestGoto;
    }
    void LootItemCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        RequestGotoMessage* package = (RequestGotoMessage*)message;
        auto items = manage.getNearItem(package->rectangleBox[0]);
        for (auto item: items) mine.send(source, new ReceiveItemMessage(item.first, item.second.item, item.second.count));
        if (items.size()) mine.send(new AddItemMessage());
    }
}