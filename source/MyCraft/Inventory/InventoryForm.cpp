#include "InventoryForm.h"
#include "Block.h"
#include "ControlCenter.h"
#include "Controller2D.h"
#include "CraftingTableUI.h"
#include "InteractiveForm.h"
#include "Inventory.h"
#include "InventoryElement.h"
#include "Message.h"
#include "MessageBox.h"
namespace MyCraft {
    InventoryForm::InventoryForm(Inventory& inventory, ItemTable& table): __inventory(inventory), __toolBar(table) {
        setSize(glm::vec2(1610.f/940.f*1.2f/MyBase::ControlCenter::getInstance().GetWindowRatio(), 1.2), 0.01);
        setPosition(-getSize()/2.f);
        setFillColor(WHITE);

        inventory.changeState(&__toolBar);
        insert(&__inventory);

        MyBase::Network::match(&__toolBar);

        add(new OpenInventoryBlockCommand(*this));

        __toolBar.open();
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
        if (!ui) ui = &__toolBar;
        ui->open();
        __inventory.changeState(ui);
    }
    void InventoryForm::__open(GLFWwindow* window) {
        MyBase::ControlCenter::EnableMouse(window);
        MyBase::ControlCenter::getInstance().BindSubScreen();
    }
    void InventoryForm::__close(GLFWwindow* window) {
        MyBase::ControlCenter::DisableMouse(window);
        MyBase::ControlCenter::CenteringMouse(window);
    }
    OpenInventoryBlockMessage::OpenInventoryBlockMessage(const glm::vec3& position, const BlockCatogary& type, ItemTable& t):
        table(t), blockPosition(position), blockType(type) {}

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
            InventoryUI* ui = new CraftingTableUI(&package->table);
            __inventory.setDefaultUI(ui);
            __inventory.open(MyBase::ControlCenter::getInstance().getHomeScreeen());
            ui->close();
            __inventory.setDefaultUI(0);
            delete ui;
        }
    }
}