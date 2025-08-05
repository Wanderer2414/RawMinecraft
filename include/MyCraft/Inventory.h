#ifndef INVENTORY_H
#define INVENTORY_H
#include "Block.h"
#include "Clock.h"
#include "Container2D.h"
#include "Controller2D.h"
#include "GLFW/glfw3.h"
#include "Item.h"
#include "Message.h"
#include "Texture.h"
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
    class Bags: public MyBase::Container2D {
    public:
        Bags(ItemTable& table);
        ~Bags();
        bool setHover(const glm::vec2& position)    override;
        void glDraw() const                         override;
        glm::vec2 getPosition() const               override;
        glm::vec2 getSize() const                   override;
        void update()                               override;
    private:
        bool contains(const glm::vec2& position) const override;
        glm::vec2 getHelmetPosition() const;
        glm::vec2 getChestplatePosition() const;
        glm::vec2 getLeggingsPosition() const;
        glm::vec2 getBootsPosition() const;
        glm::vec2 getLeftHandPosition() const;
        glm::vec2 getRecipiPosition(const glm::ivec2& offfset) const;
        glm::vec2 getStoragePosition(const glm::ivec2& offset) const;
        glm::vec2 getToolbarPosition(const int& n) const;
        glm::vec2 getOutputItemPosition() const;

        ItemTable& __items;
        MyBase::TextureViewer __chosenTexture, __inventoryTexture;
    };
    class ToolBar: public MyBase::Container2D {
    public:
        ToolBar(ItemTable& table);
        ~ToolBar();
        bool setHover(const glm::vec2& position)    override;
        glm::vec2 getPosition() const               override;
        glm::vec2 getSize() const                   override;
        void update()                               override;
        friend class RightAttackCommand;
        friend class LeftAttackCommand;
        friend class AcceptPlaceCommand;
        friend class AcceptDestroyCommand;
    private:
        bool contains(const glm::vec2& position) const override;
        bool catchEvent(GLFWwindow*)                override;
        void glDraw() const                         override;
        int __chosenIndex;
        ItemTable& __items;
        MyBase::TextureViewer __toolBarTexture, __chosenTexture;
        MyBase::Clock           __mouseEllapse;
        glm::vec2 getToolbarPosition(const int& n) const;
        glm::vec2 getToolbarChosenPosition(const int& n) const;
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
        Bags                    *__bags;
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
        AcceptDestroyMessage(const float& dec);
        ~AcceptDestroyMessage();

        float percent;
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