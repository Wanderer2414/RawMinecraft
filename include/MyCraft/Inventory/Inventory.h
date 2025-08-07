#ifndef INVENTORY_H
#define INVENTORY_H
#include "Bag.h"
#include "Block.h"
#include "Clock.h"
#include "Container2D.h"
#include "Controller2D.h"
#include "Item.h"
#include "Message.h"
#include "Texture.h"
#include "Toolbar.h"
#include "Wrapper.h"
namespace MyCraft {
    class ItemTable {
    public:
        ItemTable();
        ~ItemTable();
        ItemPacked package;

        Item* getBags(const glm::ivec2& offset) const;
        Item* getToolBar(const int& n) const;

        Item* placeBags(const glm::ivec2& offset, Item* item);
        Item* placeToolbar(const int& n, Item* item);
    private:
        Item* __items[4][10];
    };
    class Inventory: public MyBase::Wrapper, public MyBase::Port {
    public:
        Inventory();
        ~Inventory();
        Inventory(const Inventory&) = delete;
        Inventory& operator=(const Inventory&) const = delete; 

        void open();
        void close();
    protected:
    private:
        char __indices[4][10];
        Bag                    *__bags;
        ToolBar                 *__toolBar;
        ItemTable               __items;
    };

    class LeftAttackCommand: public MyBase::Command {
    public:
        LeftAttackCommand(ToolBar* toolbar);
        ~LeftAttackCommand();
        MyBase::MessageType getType()      const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        ToolBar* __toolBar;
    };

    class RightAttackCommand: public MyBase::Command {
    public:
        RightAttackCommand(ToolBar* toolbar);
        ~RightAttackCommand();
        MyBase::MessageType getType()      const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        ToolBar* __toolBar;
    };

    class AcceptPlaceMessage: public MyBase::Message {
    public:
        AcceptPlaceMessage(const BlockCatogary& type);
        ~AcceptPlaceMessage();

        const BlockCatogary type;
        MyBase::MessageType getType() const override;
    };

    class AcceptPlaceCommand: public MyBase::Command {
    public:
        AcceptPlaceCommand(ToolBar* toolbar);
        ~AcceptPlaceCommand();

        MyBase::MessageType getType()      const override;;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;;
    private:
        ToolBar* __toolbar;
    };
    class AcceptDestroyMessage: public MyBase::Message {
    public:
        AcceptDestroyMessage(const float& dec, const BlockCatogary& type, const glm::vec3& position);
        ~AcceptDestroyMessage();
        const BlockCatogary type;
        const float percent;
        const glm::vec3 position;
        MyBase::MessageType getType() const override;
    };
    class AcceptDestroyCommand: public MyBase::Command {
    public:
        AcceptDestroyCommand(ToolBar* toolbar);
        ~AcceptDestroyCommand();

        MyBase::MessageType getType()      const override;;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;;
    private:
        ToolBar* __toolbar;
    };
}
#endif