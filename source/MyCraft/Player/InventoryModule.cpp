#include "InventoryModule.h"
#include "ControlCenter.h"
#include "DroppedItem.h"
#include "Inventory.h"
#include "InventoryForm.h"
#include "Item.h"
#include "Message.h"

namespace MyCraft {
    namespace Player {
        InventoryModule::InventoryModule(): __leftHandItem(0), __rightHandItem(0) {
            __items.package.texture = MyBase::Texture("assets/images/blockItem.png");
            __items.package.size = glm::vec2(102.f/940*1.2f/MyBase::ControlCenter::getInstance().GetWindowRatio(), 102.f/940*1.2)*0.8f;
            __items.package.font = MyBase::Font("assets/fonts/SyneMono-Regular.ttf");
        }

        InventoryModule::~InventoryModule() {}

        ItemTable& InventoryModule::getItems() {
            return __items;
        }
        ItemType InventoryModule::getItemTypeLeftHand() const {
            if (__leftHandItem) return *__leftHandItem;
            return ItemType::Air;
        }
        ItemType InventoryModule::getItemTypeRightHand() const {
            if (__rightHandItem) return *__rightHandItem;
            return ItemType::Air;
        }

        void InventoryModule::setRightHandItem(Item* item) {
            __rightHandItem = item;
        }

        void InventoryModule::setLeftHandItem(Item* item) {
            __leftHandItem = item;
        }
    }

    PrepareOpenInventoryMessage::PrepareOpenInventoryMessage(const glm::ivec3& p, const BlockCatogary& t): position(p), type(t) {}
    PrepareOpenInventoryMessage::~PrepareOpenInventoryMessage() {}
    MyBase::MessageType PrepareOpenInventoryMessage::getType() const {
        return MyBase::PrepareOpenInventory;
    }

    PrepareOpenInventoryCommand::PrepareOpenInventoryCommand(Player::InventoryModule* model): __model(model) {}
    PrepareOpenInventoryCommand::~PrepareOpenInventoryCommand() {}

    MyBase::MessageType PrepareOpenInventoryCommand::getType() const {
        return MyBase::PrepareOpenInventory;
    }
    void PrepareOpenInventoryCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)  {
        PrepareOpenInventoryMessage* package = (PrepareOpenInventoryMessage*)message;
        mine.send(new OpenInventoryBlockMessage(package->position, package->type, __model->getItems()));
    }

    ReceiveItemMessage::ReceiveItemMessage(const glm::vec3& p, const ItemType& t, const unsigned int& c): type(t), count(c), position(p) {};
    ReceiveItemMessage::~ReceiveItemMessage() {};
    MyBase::MessageType ReceiveItemMessage::getType() const {
        return MyBase::ReceiveItem;
    }

    ReceiveItemCommand::ReceiveItemCommand(Player::InventoryModule* model): __model(model) {}
    ReceiveItemCommand::~ReceiveItemCommand() {}

    MyBase::MessageType ReceiveItemCommand::getType() const {
        return MyBase::ReceiveItem;
    }
    void ReceiveItemCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        ReceiveItemMessage* package = (ReceiveItemMessage*)message;
        Item* item = Item::create(__model->getItems().package, package->count, package->type);
        item = __model->getItems().push(item);
        if (item) {
            int count = item->getCount();
            delete item;
            mine.send(source, new DropItemMessage(package->type, count, package->position));
        }
    }

    HoldItemMessage::HoldItemMessage(Item* i, const bool& b): item(i), isRightHand(b) {}
    HoldItemMessage::~HoldItemMessage() {}

    MyBase::MessageType HoldItemMessage::getType() const {
        return MyBase::HoldItem;
    }
    
    HoldItemCommand::HoldItemCommand(Player::InventoryModule* m): module(m) {}
    HoldItemCommand::~HoldItemCommand() {}

    MyBase::MessageType HoldItemCommand::getType() const {
        return MyBase::HoldItem;
    }
    void HoldItemCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        HoldItemMessage* package = (HoldItemMessage*)message;
        if (package->isRightHand) module->setRightHandItem(package->item);
        else module->setLeftHandItem(package->item);
    };
}