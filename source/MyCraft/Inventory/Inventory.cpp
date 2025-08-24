#include "Inventory.h"
#include "Controller2D.h"
#include "Item.h"
#include "Wrapper.h"
namespace MyCraft {

    ItemTable::ItemTable() {
        memset(__items[0], 0, sizeof(Item*)*40);
    }
    ItemTable::~ItemTable() {
        for (int i = 0; i<4; i++) {
            for (int j = 0; j<10; j++)
                if (__items[i][j]) delete __items[i][j];
        }
    }

    Item* ItemTable::getBags(const glm::ivec2& offset) const {
        if (offset.x>=0 && offset.x<=3 && offset.y>=0 && offset.y<=10)
            return __items[offset.x][offset.y];
        return 0;
    }
    Item* ItemTable::getToolBar(const int& n) const {
        if (n>=0 && n<=10) return __items[3][n];
        return 0;
    }

    Item* ItemTable::placeBags(const glm::ivec2& offset, Item* item) {
        if (offset.x<0 && offset.x>3 && offset.y<0 && offset.y>10) return 0;
        Item* out = 0;
        if (!item || __items[offset.x][offset.y]) out = __items[offset.x][offset.y];
        __items[offset.x][offset.y] = item;
        return out;
    }
    Item* ItemTable::placeToolbar(const int& n, Item* item) {
        if (n<0 && n>=10) return 0;
        Item* out = 0;
        if (!item || __items[3][n]) out = __items[3][n];
        __items[3][n] = item;
        return out;
    }
    Item* ItemTable::push(Item* item) {
        for (int i = 3; i>=0; i--) {
            for (int j = 0; j<10; j++) {
                if (__items[i][j] && *__items[i][j] == *item) {
                    item = __items[i][j]->merge(item);
                    if (!item) return 0;
                }
                else if (!__items[i][j]) {
                    __items[i][j] = item;
                    return 0;
                }
            }
        }
        return item;
    }

    Inventory::Inventory(): MyBase::Wrapper(0){}
    Inventory::~Inventory() { }

}