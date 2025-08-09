#ifndef TOOL_ITEM_H
#define TOOL_ITEM_H
#include "Item.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Texture.h"
#include "BlockItem.h"
namespace MyCraft {
    bool isToolItem(const ItemType& type);
    class ToolItem: private MyBase::TextureContainer, private MyBase::ShapeContainer, public Item {
    public:
        ToolItem(const ItemPacked& package, const ItemType& type);
        ~ToolItem();
        ToolItem(const BlockItem&) = delete;
        ToolItem& operator=(const BlockItem&) const = delete;
        virtual glm::vec2 getPosition() const       override;
        virtual Item* merge(Item*& item)            override;
        void setPosition(const glm::vec2& position) override;
        void draw() const                           override;
        void update()                               override;
        Item*split(const int& count)                override;
    protected:
    private:
        glm::vec2         __size;
        unsigned char     __currentCount;
        MyBase::Rectangle __durabilityBackground;
    };
}
#endif