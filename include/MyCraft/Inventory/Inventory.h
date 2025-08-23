#ifndef INVENTORY_H
#define INVENTORY_H
#include "Bag.h"
#include "Block.h"
#include "Controller.h"
#include "Controller2D.h"
#include "InventoryElement.h"
#include "Item.h"
#include "Message.h"
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
        Item* push(Item* item);
    private:
        Item* __items[4][10];
    };

    class Inventory: public MyBase::Wrapper, public MyBase::Port {
    public:
        Inventory();
        ~Inventory();
        Inventory(const Inventory&) = delete;
        Inventory& operator=(const Inventory&) const = delete; 
    protected:
    private:
    };


}
#endif