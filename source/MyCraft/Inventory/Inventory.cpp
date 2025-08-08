#include "Inventory.h"
#include "Block.h"
#include "ControlCenter.h"
#include "Controller2D.h"
#include "Font.h"
#include "Item.h"
#include "Message.h"
#include "NonuseItem.h"
#include "PlayerModelController.h"
#include "Texture.h"
#include "World.h"
#include "Wrapper.h"
#include "BlockItem.h"
#include "ToolItem.h"
namespace MyCraft {

    ItemTable::ItemTable() {
        memset(__items[0], 0, sizeof(Item*)*40);
    }
    ItemTable::~ItemTable() {
        for (int i = 0; i<4; i++) {
            for (int j = 0; j<10; j++)
                if (__items[i][j]) delete __items[i][j];
        }
    }

    Item* ItemTable::getBags(const glm::ivec2& offset) const {
        if (offset.x>=0 && offset.x<=3 && offset.y>=0 && offset.y<=10)
            return __items[offset.x][offset.y];
        return 0;
    }
    Item* ItemTable::getToolBar(const int& n) const {
        if (n>=0 && n<=10) return __items[3][n];
        return 0;
    }

    Item* ItemTable::placeBags(const glm::ivec2& offset, Item* item) {
        if (offset.x<0 && offset.x>3 && offset.y<0 && offset.y>10) return 0;
        Item* out = 0;
        if (!item || __items[offset.x][offset.y]) out = __items[offset.x][offset.y];
        __items[offset.x][offset.y] = item;
        return out;
    }
    Item* ItemTable::placeToolbar(const int& n, Item* item) {
        if (n<0 && n>=10) return 0;
        Item* out = 0;
        if (!item || __items[3][n]) out = __items[3][n];
        __items[3][n] = item;
        return out;
    }
    Item* ItemTable::push(Item* item) {
        for (int i = 3; i>=0; i--) {
            for (int j = 0; j<10; j++) {
                if (__items[i][j] && *__items[i][j] == *item) {
                    item = __items[i][j]->merge(item);
                    if (!item) return 0;
                }
                else if (!__items[i][j]) {
                    __items[i][j] = item;
                    return 0;
                }
            }
        }
        return item;
    }

    Inventory::Inventory(): MyBase::Wrapper(0), __currentUI(0) {
        __items.package.texture = MyBase::Texture("assets/images/blockItem.png");
        __items.package.size = glm::vec2(102.f/940*1.2f/MyBase::ControlCenter::getInstance().GetWindowRatio(), 102.f/940*1.2)*0.8f;
        __items.package.font = MyBase::Font("assets/fonts/SyneMono-Regular.ttf");

        __items.placeToolbar(0, new BlockItem(__items.package, ItemType::OakLog, 48));

        __items.placeToolbar(1,new BlockItem(__items.package, ItemType::OakLog, 32));

        __items.placeToolbar(2,new ToolItem(__items.package, ItemType::WoodenShovel));
        __items.placeToolbar(3,new NonuseItem(__items.package, ItemType::Stick, 64));

        __toolBar = new ToolBar(__items);
        __toolBar->open();
        add(new LeftAttackCommand(__toolBar));
        add(new AcceptPlaceCommand(__toolBar));
        add(new AcceptDestroyCommand(__toolBar));
        add(new RightAttackCommand(__toolBar));
        changeState(__toolBar);
    }
    Inventory::~Inventory() {
        delete __toolBar;
    }
    ItemTable& Inventory::getItems() {
        return __items;
    }
    void Inventory::open(InventoryUI* ui) {
        if (!ui) return ;
        if (__currentUI) {
            __currentUI->close();
            delete __currentUI;
        } else __toolBar->close();
        __currentUI = ui;
        changeState(ui);
        __currentUI->open();
    }
    void Inventory::close() {
        if (__currentUI) {
            __currentUI->close();
            delete __currentUI;
            __currentUI = 0;
        }
        __toolBar->open();
        changeState(__toolBar);
    }

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
}