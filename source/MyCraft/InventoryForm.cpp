#include "InventoryForm.h"
#include "MessageBox.h"
namespace MyCraft {
    InventoryForm::InventoryForm() {
        setSize(__inventory.getSize(), 0.01);
        setPosition(__inventory.getPosition());
        setFillColor(WHITE);
        insert(&__inventory);
    }
    InventoryForm::~InventoryForm() {}
}