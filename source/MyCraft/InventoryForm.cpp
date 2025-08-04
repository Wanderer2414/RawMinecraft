#include "InventoryForm.h"
#include "ControlCenter.h"
#include "Inventory.h"
namespace MyCraft {
    InventoryForm::InventoryForm(Inventory& inventory): __inventory(inventory) {
        setSize(glm::vec2(1610.f/940.f*1.2f/MyBase::ControlCenter::getInstance().GetWindowRatio(), 1.2), 0.01);
        setPosition(-getSize()/2.f);
        setFillColor(WHITE);
        insert(&__inventory);
    }
    InventoryForm::~InventoryForm() {}
    void InventoryForm::__open(GLFWwindow* window) {
        __inventory.open();
    }
    void InventoryForm::__close(GLFWwindow*) {
        __inventory.close();
    }
}