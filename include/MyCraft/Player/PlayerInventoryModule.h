#ifndef PLAYER_INVENTORY_H
#define PLAYER_INVENTORY_H
#include "Inventory.h"
#include "Item.h"
#include "Message.h"
namespace MyCraft {
    class PlayerInventoryModule {
    public:
        PlayerInventoryModule();
        ~PlayerInventoryModule();
        PlayerInventoryModule(PlayerInventoryModule&&) = delete;
        PlayerInventoryModule(const PlayerInventoryModule&) = delete;
        PlayerInventoryModule& operator=(const PlayerInventoryModule&) const = delete; 

        ItemTable& getItems();
        void setLeftHandItem(Item* item);
        void setRightHandItem(Item* item);
    protected:
        ItemType        getItemTypeLeftHand() const;
        ItemType        getItemTypeRightHand() const;
    private:
        ItemTable       __items;
        Item            *__leftHandItem, *__rightHandItem;
    };

    class PrepareOpenInventoryMessage: public MyBase::Message {
    public:
        PrepareOpenInventoryMessage(const glm::ivec3& position, const BlockCatogary& type);
        ~PrepareOpenInventoryMessage();
        MyBase::MessageType     getType() const override;
        const glm::ivec3 position;
        const BlockCatogary type;
    };

    class PrepareOpenInventoryCommand: public MyBase::Command {
    public:
        PrepareOpenInventoryCommand(PlayerInventoryModule* model);
        ~PrepareOpenInventoryCommand();

        MyBase::MessageType getType()                               const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
    private:
        MyCraft::PlayerInventoryModule* __model;
    };

    class ReceiveItemMessage: public MyBase::Message {
    public:
        ReceiveItemMessage(const glm::vec3& position, const ItemType& type, const unsigned int& count);
        ~ReceiveItemMessage();
        MyBase::MessageType     getType() const override;
        const glm::vec3 position;
        const ItemType type;
        const int count;
    };

    class ReceiveItemCommand: public MyBase::Command {
    public:
        ReceiveItemCommand(PlayerInventoryModule* model);
        ~ReceiveItemCommand();

        MyBase::MessageType getType()                               const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
    private:
        MyCraft::PlayerInventoryModule* __model;
    };

    class HoldItemMessage: public MyBase::Message {
    public:
        HoldItemMessage(Item* item, const bool& isRightHand);
        ~HoldItemMessage();

        MyBase::MessageType getType() const override;
        Item* item;
        bool isRightHand;
    private:
    };
    class HoldItemCommand: public MyBase::Command {
    public:
        HoldItemCommand(PlayerInventoryModule* module);
        ~HoldItemCommand();

        MyBase::MessageType getType()                               const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
    private:
        PlayerInventoryModule* module;
    };
}
#endif