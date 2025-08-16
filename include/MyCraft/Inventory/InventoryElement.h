#ifndef INVENTORY_ELEMENT_H
#define INVENTORY_ELEMENT_H
#include "Container2D.h"
namespace MyCraft {
    class InventoryUI: public MyBase::Container2D {
    public:
        InventoryUI() = default;
        virtual ~InventoryUI();
        InventoryUI(const InventoryUI&) = delete;
        InventoryUI& operator=(const InventoryUI&) const = delete; 

        virtual void close() = 0;
        virtual void open() = 0;
    protected:
    private:
    };
}
#endif