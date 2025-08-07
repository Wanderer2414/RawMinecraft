#include "Item.h"
#include "BlockItem.h"
#include "NonuseItem.h"
#include "Shape.h"
#include "ShapeManager.h"
#include "Texture.h"
#include "ToolItem.h"
namespace MyCraft {

    bool isAdaptive(const ItemType& tool, const BlockCatogary& block) {
        if (tool >= ItemType::WoodenShovel && tool <= ItemType::NetheriteShovel) return isShoverAdaptive(block);
        return false;
    }
    bool isShoverAdaptive(const BlockCatogary& type) {
        switch (type) {
            case Dirt: return true;
            case Grass: return true;
        default: return false;
        }
    }

    float getPowerness(const ItemType& tool) {
        if (tool >= ItemType::WoodenShovel && tool <= ItemType::NetheriteShovel)
            return 100;
        return 1;
    }
    BlockCatogary to_block(const ItemType& type) {
        return (BlockCatogary)type;
    }
    Item* Item::create(const ItemPacked& package, const unsigned char& count, const ItemType& type) {
        Item* item = 0;
        if (MyCraft::isBlock(type)) item = new BlockItem(package, type, count);
        else if (MyCraft::isNonuseItem(type)) item = new NonuseItem(package, type, count);
        else if (MyCraft::isToolItem(type)) item = new ToolItem(package, type);
        return item;
    }
    Item::Item(const ItemType& type, const unsigned char& count):  __type(type), __count(count) {}
    Item::~Item() {}

    Item::operator const ItemType&() const {
        return __type;
    }

    unsigned char Item::getCount() const {
        return __count;
    }
    
    void Item::setCount(const int& count) {
        __count = count;
        update();
    }
}