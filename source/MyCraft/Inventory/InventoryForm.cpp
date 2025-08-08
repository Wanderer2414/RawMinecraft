#include "InventoryForm.h"
#include "Block.h"
#include "ControlCenter.h"
#include "Controller2D.h"
#include "CraftingTableUI.h"
#include "GLFW/glfw3.h"
#include "InteractiveForm.h"
#include "Inventory.h"
#include "InventoryElement.h"
#include "MessageBox.h"
namespace MyCraft {
    InventoryForm::InventoryForm(Inventory& inventory): __defaultUI(0), __inventory(inventory) {
        setSize(glm::vec2(1610.f/940.f*1.2f/MyBase::ControlCenter::getInstance().GetWindowRatio(), 1.2), 0.01);
        setPosition(-getSize()/2.f);
        setFillColor(WHITE);

        insert(&__inventory);
        add(new OpenInventoryBlockCommand(*this));
    }
    InventoryForm::~InventoryForm() {}

    bool InventoryForm::catchEvent(GLFWwindow* window) {
        bool is_changed = MyBase::MessageBox::catchEvent(window);
        if (MyBase::ControlCenter::getInstance().IsKeyPressed() && glfwGetKey(window, GLFW_KEY_E)) {
            close();
            is_changed = true;
        };
        return is_changed;
    }
    void InventoryForm::setDefaultUI(InventoryUI* ui) {
        __defaultUI = ui;
    }
    void InventoryForm::__open(GLFWwindow* window) {
        MyBase::ControlCenter::EnableMouse(window);
        MyBase::ControlCenter::getInstance().BindSubScreen();
        __inventory.open(__defaultUI);
    }
    void InventoryForm::__close(GLFWwindow* window) {
        __inventory.close();
        MyBase::ControlCenter::DisableMouse(window);
        MyBase::ControlCenter::CenteringMouse(window);
    }
    ItemTable& InventoryForm::getItems() {
        return __inventory.getItems();
    }
    OpenInventoryBlockMessage::OpenInventoryBlockMessage(const glm::vec3& position, const BlockCatogary& type):
        blockPosition(position), blockType(type) {}

    OpenInventoryBlockMessage::~OpenInventoryBlockMessage() {}

    MyBase::MessageType OpenInventoryBlockMessage::getType() const {
        return MyBase::OpenInventoryBlock;
    }

    OpenInventoryBlockCommand::OpenInventoryBlockCommand(InventoryForm& inventory): __inventory(inventory) {}
    OpenInventoryBlockCommand::~OpenInventoryBlockCommand() {}

    MyBase::MessageType OpenInventoryBlockCommand::getType() const {
        return MyBase::OpenInventoryBlock;
    }
    void OpenInventoryBlockCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        OpenInventoryBlockMessage* package = (OpenInventoryBlockMessage*)message;
        if (package->blockType == CraftingTable) {
            __inventory.setDefaultUI(new CraftingTableUI(&__inventory.getItems()));
            __inventory.open(MyBase::ControlCenter::getInstance().getHomeScreeen());
            __inventory.setDefaultUI(0);
        }
    }
}