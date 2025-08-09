#ifndef TOOLBAR_H
#define TOOLBAR_H
#include "Block.h"
#include "Clock.h"
#include "InventoryElement.h"
#include "Message.h"
#include "Texture.h"
namespace MyCraft {
    class ItemTable;
    class ToolBar: public InventoryUI, public MyBase::Port {
    public:
        ToolBar(ItemTable& table);
        ~ToolBar();
        bool setHover(const glm::vec2& position)    override;
        glm::vec2 getPosition() const               override;
        glm::vec2 getSize() const                   override;
        void open()                                 override;
        void close()                                override;
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
    class AddItemMessage: public MyBase::Message {
    public:
        AddItemMessage();
        ~AddItemMessage();
        MyBase::MessageType getType()      const override;;
    };
    class AddItemCommand: public MyBase::Command {
    public:
        AddItemCommand(InventoryUI* ui);
        ~AddItemCommand();
        MyBase::MessageType getType()      const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;;
    private:
        InventoryUI* ui;
    };
}
#endif