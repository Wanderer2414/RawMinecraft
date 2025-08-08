#ifndef INVENTORY_FORM_H
#define INVENTORY_FORM_H
#include "Inventory.h"
#include "InventoryElement.h"
#include "Message.h"
#include "MessageBox.h"
namespace MyCraft {
    class InventoryForm: public MyBase::MessageBox, public MyBase::Port {
    public:
        InventoryForm(Inventory& inventory);
        InventoryForm(const InventoryForm&) = delete;
        ~InventoryForm();
        InventoryForm& operator=(const InventoryForm&) const = delete; 
        void setDefaultUI(InventoryUI* ui);
        ItemTable& getItems();
    protected:
        bool catchEvent(GLFWwindow* window) override;
        void __open(GLFWwindow*) override;
        void __close(GLFWwindow*) override;
    private:
        InventoryUI*      __defaultUI;
        Inventory&        __inventory;
    };

    class OpenInventoryBlockMessage: public MyBase::Message {
    public:
        OpenInventoryBlockMessage(const glm::vec3& blockPosition, const BlockCatogary& type);
        ~OpenInventoryBlockMessage();
        
        const glm::vec3 blockPosition;
        const BlockCatogary blockType;

        MyBase::MessageType getType() const override;
    };

    class OpenInventoryBlockCommand: public MyBase::Command {
    public:
        OpenInventoryBlockCommand(InventoryForm& inventory);
        ~OpenInventoryBlockCommand();

        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;;
    private:
        InventoryForm& __inventory;
    };
}
#endif