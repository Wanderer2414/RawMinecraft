#ifndef ITEM_H
#define ITEM_H
#include "Block.h"
#include "Controller2D.h"
#include "Font.h"
#include "Text.h"
#include "Texture.h"
namespace MyCraft {
    enum class ItemType {
        Air, Dirt, Grass, CobbleStone, Stone, OakWood, OakLeaf,
        BedRock, Sand, SandStone, Obsidian, DeepStale, Ice
    };
    bool isBlock(const ItemType& type);
    struct ItemPacked {
        glm::vec2 size;
        MyBase::Font font;
        MyBase::Texture texture;
    };
    class Item: private MyBase::TextureContainer, private MyBase::Text {
    public:
        Item(const ItemPacked& package, const ItemType& type, const int& count);
        ~Item();
        Item(const Item&) = delete;
        Item& operator=(const Item&) const = delete;
        operator const ItemType&() const;
        operator BlockCatogary() const;
        int getCount() const;
        void setPosition(const glm::vec2& position);
        void setCount(const int& count);
        void draw() const;
    protected:
    private:
        int __count;
        ItemType __type;
    };
}
#endif