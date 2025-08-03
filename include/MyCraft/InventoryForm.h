#ifndef INVENTORY_FORM_H
#define INVENTORY_FORM_H
#include "Inventory.h"
#include "MessageBox.h"
namespace MyCraft {
    class InventoryForm: public MyBase::MessageBox {
    public:
        InventoryForm();
        InventoryForm(const InventoryForm&) = delete;
        ~InventoryForm();
        InventoryForm& operator=(const InventoryForm&) const = delete; 
    protected:
    private:
        Inventory       __inventory;
    };
}
#endif