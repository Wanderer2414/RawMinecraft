#ifndef ITEM_H
#define ITEM_H
#include "Font.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Text.h"
#include "Texture.h"
namespace MyCraft {
    enum class ItemType {
        Air, Dirt, Grass, CobbleStone, Stone, OakWood, OakLeaf,
        BedRock, Sand, SandStone, Obsidian, DeepStale, Ice, Shovel
    };
    struct ItemPacked {
        glm::vec2 size;
        MyBase::Font font;
        MyBase::Texture texture;
    };
    class Item {
    public:
        Item(const ItemType& type, const unsigned char& count);
        virtual ~Item();
        unsigned char   getCount() const;
        void            setCount(const int& count);
        virtual void    setPosition(const glm::vec2& position) = 0;
        virtual void    draw() const = 0;
        virtual void    update() = 0;
        virtual bool    isBlock() const;
        virtual bool    isTool() const;
        operator const ItemType&() const;
    private:
        unsigned char   __count;
        ItemType        __type;
    };
    class BlockItem: private MyBase::TextureContainer, private MyBase::Text, public Item {
    public:
        BlockItem(const ItemPacked& package, const ItemType& type, const unsigned char& count);
        ~BlockItem();
        BlockItem(const BlockItem&) = delete;
        BlockItem& operator=(const BlockItem&) const = delete;
        virtual bool isBlock() const                override;
        void setPosition(const glm::vec2& position) override;
        void draw() const                           override;
        void update()                               override;
    protected:
    private:
    };

    class ToolItem: private MyBase::TextureContainer, private MyBase::ShapeContainer, public Item {
    public:
        ToolItem(const ItemPacked& package, const ItemType& type);
        ~ToolItem();
        ToolItem(const BlockItem&) = delete;
        ToolItem& operator=(const BlockItem&) const = delete;
        bool isTool() const                         override;
        void setPosition(const glm::vec2& position) override;
        void draw() const                           override;
        void update()                               override;
    protected:
    private:
        unsigned char     __currentCount;
        MyBase::Rectangle __durabilityBackground;
    };
    
}
#endif