#include "Toolbar.h"
#include "ControlCenter.h"
#include "DroppedItem.h"
#include "GLFW/glfw3.h"
#include "Inventory.h"
#include "InventoryElement.h"
#include "Message.h"
#include "PlayerModelController.h"
#include "ToolItem.h"
#include "World.h"
namespace MyCraft {
    ToolBar::ToolBar(ItemTable& table): __items(table), __chosenIndex(0) {
        MyBase::Texture texture = MyBase::Texture("assets/images/Inventory.png");
        __toolBarTexture.setTexture(texture);
        
        __toolBarTexture.setTextureImportPosition({0, 0});
        __toolBarTexture.setTextureImportSize({1110.f/1610, 120.f/1060});
        glm::vec2 size(0.15f/MyBase::ControlCenter::getInstance().GetWindowRatio()*1110.f/120.f, 0.15);
        __toolBarTexture.setTextureExportPosition({-size.x/2, -1});
        __toolBarTexture.setTextureExportSize(size);

        __chosenTexture.setTexture(texture);
        __chosenTexture.setTextureImportPosition({1233.f/1610, 3.f/1060});
        __chosenTexture.setTextureImportSize({114.f/1610, 114.f/1060});
        __chosenTexture.setTextureExportSize(glm::vec2(114.f/940*1.2/MyBase::ControlCenter::getInstance().GetWindowRatio(), 114.f/940*1.2));
        __chosenTexture.setTextureExportPosition(getToolbarChosenPosition(0));

        insert(&__chosenTexture);
        insert(&__toolBarTexture);
        __mouseEllapse.setDuration(100);

        add(new LeftAttackCommand(this));
        add(new AcceptPlaceCommand(this));
        add(new AcceptDestroyCommand(this));
        add(new RightAttackCommand(this));
        add(new AddItemCommand(this));
    }

    ToolBar::~ToolBar() {}
    bool ToolBar::setHover(const glm::vec2& position) {
        return false;
    }
    bool ToolBar::catchEvent(GLFWwindow* window) {
        bool is_changed = Controller2D::catchEvent(window);
        if (__mouseEllapse.get()) {
            float x = MyBase::ControlCenter::getInstance().getScroll().y;
            if (x<0) {
                __mouseEllapse.restart();
                __chosenIndex = (__chosenIndex+1)%10;
                __chosenTexture.setTextureExportPosition(getToolbarChosenPosition(__chosenIndex));
                is_changed = true;
            }
            else if (x>0) {
                __mouseEllapse.restart();
                __chosenIndex = (__chosenIndex+9)%10;
                __chosenTexture.setTextureExportPosition(getToolbarChosenPosition(__chosenIndex));
                is_changed = true;
            }
        }
        if (MyBase::ControlCenter::getInstance().IsKeyPressed()) {
            char c = MyBase::ControlCenter::getInstance().GetCharInput() - '0';
            if (c>=0 && c<=9) {
                __chosenIndex = (c+9)%10;
                __chosenTexture.setTextureExportPosition(getToolbarChosenPosition(__chosenIndex));
                is_changed = true;
            }
        }
        return is_changed;
    }
    void ToolBar::glDraw() const {
        MyBase::Container2D::glDraw();
        for (int i = 0; i<10; i++)
            if (__items.getToolBar(i)) __items.getToolBar(i)->draw();
    }
    glm::vec2 ToolBar::getPosition() const {
        return __toolBarTexture.getPosition();
    };
    glm::vec2 ToolBar::getSize() const {
        return __toolBarTexture.getSize();
    };
    glm::vec2 ToolBar::getToolbarPosition(const int& n) const {
        return getPosition() + glm::vec2((9.f + 110.f*n)/1110, 9.f/120)*getSize();
    }
    glm::vec2 ToolBar::getToolbarChosenPosition(const int& n) const {
        return getPosition() + glm::vec2((3.f + 110.f*n)/1110, 3.f/120)*getSize();
    }

    bool ToolBar::contains(const glm::vec2& position) const {
        return false;
    }

    void ToolBar::open() {
        for (int i = 0; i<10; i++) if (__items.getToolBar(i)) 
            __items.getToolBar(i)->setPosition(getToolbarPosition(i) + __items.package.size*0.125f);
    }
    void ToolBar::close() {}
    
    LeftAttackCommand::LeftAttackCommand(ToolBar* toolBar): __toolBar(toolBar) {}
    LeftAttackCommand::~LeftAttackCommand() {}

    MyBase::MessageType LeftAttackCommand::getType() const {
        return MyBase::LeftAttack;
    }
    void LeftAttackCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        LeftAttackMessage* package = (LeftAttackMessage*)message;
        Item* item = __toolBar->__items.getToolBar(__toolBar->__chosenIndex);
        if (item) {
            if (isBlock(*item)) {
                BlockCatogary type = (BlockCatogary)ItemType(*item);
                mine.send(new PlaceBlockMessage(package->position, type));
            }
        }
        else {
            mine.send(new PlaceBlockMessage(package->position, Air));
        }
    }

    RightAttackCommand::RightAttackCommand(ToolBar* toolbar): __toolBar(toolbar) {}
    RightAttackCommand::~RightAttackCommand() {}
    MyBase::MessageType RightAttackCommand::getType() const {
        return MyBase::RightAttack;
    } 
    void RightAttackCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        Item* item = __toolBar->__items.getToolBar(__toolBar->__chosenIndex);
        if (item && isToolItem(*item)) mine.send(new CrackBlockMessage(*item));
        else mine.send(new CrackBlockMessage(ItemType::Air));
    }

    AcceptPlaceMessage::AcceptPlaceMessage(const BlockCatogary& t): type(t) {}
    AcceptPlaceMessage::~AcceptPlaceMessage() {}

    MyBase::MessageType AcceptPlaceMessage::getType() const {
        return MyBase::AcceptPlace;
    }

    AcceptPlaceCommand::AcceptPlaceCommand(ToolBar* toolbar): __toolbar(toolbar) {}
    AcceptPlaceCommand::~AcceptPlaceCommand() {}

    MyBase::MessageType AcceptPlaceCommand::getType() const {
        return MyBase::AcceptPlace;
    }
    void AcceptPlaceCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        AcceptPlaceMessage* package = (AcceptPlaceMessage*)message;
        int count = __toolbar->__items.getToolBar(__toolbar->__chosenIndex)->getCount();
        if (count) {
            if (count-1==0) delete __toolbar->__items.placeToolbar(__toolbar->__chosenIndex, 0);
            else __toolbar->__items.getToolBar(__toolbar->__chosenIndex)->setCount(count-1);
        }
    }

    AcceptDestroyMessage::AcceptDestroyMessage(const float& dec, const BlockCatogary& t, const glm::vec3& p): percent(dec), type(t), position(p) {}
    AcceptDestroyMessage::~AcceptDestroyMessage() {}

    MyBase::MessageType AcceptDestroyMessage::getType() const {
        return MyBase::AcceptDestroy;
    }

    AcceptDestroyCommand::AcceptDestroyCommand(ToolBar* toolbar): __toolbar(toolbar) {}
    AcceptDestroyCommand::~AcceptDestroyCommand() {}

    MyBase::MessageType AcceptDestroyCommand::getType() const {
        return MyBase::AcceptDestroy;
    }
    void AcceptDestroyCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        AcceptDestroyMessage* package = (AcceptDestroyMessage*)message;
        Item* item = __toolbar->__items.getToolBar(__toolbar->__chosenIndex);
        if (item) {
            int count = item->getCount() - package->percent;
            if (count<=0) delete __toolbar->__items.placeToolbar(__toolbar->__chosenIndex, 0);
            else __toolbar->__items.getToolBar(__toolbar->__chosenIndex)->setCount(count);
        }
    }

    AddItemMessage::AddItemMessage() {}
    AddItemMessage::~AddItemMessage() {}
    MyBase::MessageType AddItemMessage::getType() const {
        return MyBase::AddItem;  
    };
    AddItemCommand::AddItemCommand(InventoryUI* u): ui(u) {}
    AddItemCommand::~AddItemCommand() {}
    MyBase::MessageType AddItemCommand::getType() const {
        return MyBase::AddItem;
    }
    void AddItemCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        ui->open();
    }
}