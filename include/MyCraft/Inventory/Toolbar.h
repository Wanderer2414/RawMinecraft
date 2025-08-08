#ifndef TOOLBAR_H
#define TOOLBAR_H
#include "Clock.h"
#include "Container2D.h"
#include "InventoryElement.h"
#include "Texture.h"
namespace MyCraft {
    class ItemTable;
    class ToolBar: public InventoryUI {
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
}
#endif