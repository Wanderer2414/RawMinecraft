#include "DroppedItem.h"
#include "Block.h"
#include "Controller3D.h"
#include "DrawingCenter.h"
#include "Inventory.h"
#include "Item.h"
#include "Message.h"
namespace MyCraft {
    DropItemManage::DropItemManage() {
        __rotateClock.setDuration(20);
        __blockTexture.load("assets/images/blockCatogary.png", false);
        add(Grass, glm::vec3(0,0, 1));
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
    void DropItemManage::glDraw() const {
        if (__item.size()) {
            DrawingCenter::BindDroppedBlock(__blockTexture, glm::vec2(1, 0.3));
            DrawingCenter::DrawDroppedBlock((void*)__item.data(), (void*)__state.data(), __item.size());
        }
    }

    DropItemMessage::DropItemMessage(const ItemType& t): type(t) {}
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
        // manage.add(, );
    }
}